#!/usr/bin/env python3
"""Regression tests for cp_library's dependency-free tooling."""

import json
import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path
from typing import Any, Dict, List, Optional


TOOLS_DIRECTORY = Path(__file__).resolve().parents[1]
BUILD_INDEX = TOOLS_DIRECTORY / "build_index.py"
SEARCH = TOOLS_DIRECTORY / "search.py"
BUNDLE = TOOLS_DIRECTORY / "bundle.py"
CHECK_CSES_TAGS = TOOLS_DIRECTORY / "check_cses_tags.py"


class ToolTestCase(unittest.TestCase):
    def setUp(self) -> None:
        self.temporary_directory = tempfile.TemporaryDirectory()
        self.repository_root = Path(self.temporary_directory.name) / "repository"
        self.library_root = self.repository_root / "cp_library"
        self.library_root.mkdir(parents=True)

    def tearDown(self) -> None:
        self.temporary_directory.cleanup()

    def run_tool(
        self, script: Path, *arguments: str, expected_returncode: int = 0
    ) -> subprocess.CompletedProcess:
        result = subprocess.run(
            [sys.executable, str(script)] + list(arguments),
            check=False,
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
        self.assertEqual(
            result.returncode,
            expected_returncode,
            msg="stdout:\n{}\nstderr:\n{}".format(result.stdout, result.stderr),
        )
        return result

    def create_practice(self, relative_path: str) -> None:
        path = self.repository_root / relative_path
        path.mkdir(parents=True, exist_ok=True)
        (path / "solution.cpp").write_text("int main() {}\n", encoding="utf-8")

    def create_entry(
        self,
        category: str,
        entry_id: str,
        *,
        title: Optional[str] = None,
        aliases: Optional[List[str]] = None,
        tags: Optional[List[str]] = None,
        status: str = "reviewed",
        tests: Optional[List[str]] = None,
        practice_path: str = "cses/01_introductory/example-task",
        extra_metadata: Optional[Dict[str, Any]] = None,
    ) -> Path:
        entry_directory = self.library_root / category / entry_id
        entry_directory.mkdir(parents=True)
        (entry_directory / "README.md").write_text(
            "# {}\n".format(title or entry_id), encoding="utf-8"
        )
        (entry_directory / "algorithm.hpp").write_text(
            "#ifndef TEST_ALGORITHM_HPP\n"
            "#define TEST_ALGORITHM_HPP\n"
            "inline int answer() { return 42; }\n"
            "#endif\n",
            encoding="utf-8",
        )
        (entry_directory / "example.cpp").write_text(
            '#include "algorithm.hpp"\n'
            "int main() { return answer() == 42 ? 0 : 1; }\n",
            encoding="utf-8",
        )

        test_paths = ["test.cpp"] if tests is None else tests
        for test_path in test_paths:
            target = entry_directory / test_path
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_text("int main() { return 0; }\n", encoding="utf-8")

        self.create_practice(practice_path)
        metadata: Dict[str, Any] = {
            "schema_version": 1,
            "id": entry_id,
            "title": title or entry_id.replace("-", " ").title(),
            "category": category,
            "summary": "Cấu trúc mẫu phục vụ kiểm thử tooling.",
            "aliases": aliases or [],
            "tags": tags or ["example"],
            "status": status,
            "source": "algorithm.hpp",
            "complexities": {"query": "O(1)", "memory": "O(1)"},
            "tests": test_paths,
            "practice": [{"title": "Example Task", "path": practice_path}],
        }
        if extra_metadata:
            metadata.update(extra_metadata)
        (entry_directory / "meta.json").write_text(
            json.dumps(metadata, ensure_ascii=False, indent=2) + "\n",
            encoding="utf-8",
        )
        return entry_directory


class BuildIndexTests(ToolTestCase):
    def test_generation_is_deterministic_and_check_detects_stale_files(self) -> None:
        self.create_entry(
            "06_strings",
            "z-function",
            title="Z Function",
            aliases=["z algorithm"],
            tags=["string", "pattern-matching"],
            status="reviewed",
            practice_path="cses/06_string_algorithms/2107_string-matching",
        )
        self.create_entry(
            "02_data_structures",
            "disjoint-set-union",
            title="Disjoint Set Union",
            aliases=["union find"],
            tags=["dsu", "graph"],
            status="verified",
            practice_path="cses/04_graph_algorithms/1676_road-construction",
        )

        self.run_tool(BUILD_INDEX, "--root", str(self.library_root))
        indexes = self.library_root / "indexes"
        expected_files = {
            "catalog.md",
            "by-tag.md",
            "by-status.md",
            "by-practice.md",
            "catalog.json",
        }
        self.assertEqual(
            {path.name for path in indexes.iterdir() if path.is_file()},
            expected_files,
        )

        catalog = json.loads((indexes / "catalog.json").read_text(encoding="utf-8"))
        self.assertEqual(catalog["schema_version"], 1)
        self.assertEqual(
            [entry["id"] for entry in catalog["entries"]],
            ["disjoint-set-union", "z-function"],
        )
        self.assertEqual(
            catalog["entries"][0]["source"],
            "02_data_structures/disjoint-set-union/algorithm.hpp",
        )
        practice_index = (indexes / "by-practice.md").read_text(encoding="utf-8")
        self.assertIn("../../cses/04_graph_algorithms/1676_road-construction", practice_index)

        self.run_tool(
            BUILD_INDEX, "--root", str(self.library_root), "--check"
        )
        catalog_markdown = indexes / "catalog.md"
        catalog_markdown.write_text(
            catalog_markdown.read_text(encoding="utf-8") + "stale\n",
            encoding="utf-8",
        )
        result = self.run_tool(
            BUILD_INDEX,
            "--root",
            str(self.library_root),
            "--check",
            expected_returncode=1,
        )
        self.assertIn("stale:", result.stderr)
        self.assertTrue(
            catalog_markdown.read_text(encoding="utf-8").endswith("stale\n"),
            "--check must not rewrite stale files",
        )

    def test_validation_rejects_unknown_fields_and_missing_verified_tests(self) -> None:
        self.create_entry(
            "01_foundations",
            "bad-entry",
            tests=[],
            status="verified",
            extra_metadata={"titel": "typo"},
        )
        result = self.run_tool(
            BUILD_INDEX,
            "--root",
            str(self.library_root),
            expected_returncode=2,
        )
        self.assertIn("unknown fields", result.stderr)

        meta_path = (
            self.library_root / "01_foundations" / "bad-entry" / "meta.json"
        )
        metadata = json.loads(meta_path.read_text(encoding="utf-8"))
        del metadata["titel"]
        meta_path.write_text(json.dumps(metadata), encoding="utf-8")
        (meta_path.parent / "test.cpp").write_text(
            "int main() { return 0; }\n", encoding="utf-8"
        )
        result = self.run_tool(
            BUILD_INDEX,
            "--root",
            str(self.library_root),
            expected_returncode=2,
        )
        self.assertIn("requires at least one test", result.stderr)

    def test_validation_rejects_duplicate_id_or_alias(self) -> None:
        self.create_entry(
            "01_foundations",
            "first-entry",
            aliases=["shared name"],
            practice_path="cses/01_introductory/first-task",
        )
        self.create_entry(
            "02_data_structures",
            "second-entry",
            aliases=["Shared Name"],
            practice_path="cses/01_introductory/second-task",
        )
        result = self.run_tool(
            BUILD_INDEX,
            "--root",
            str(self.library_root),
            expected_returncode=2,
        )
        self.assertIn("duplicate id/alias", result.stderr)

    def test_validation_rejects_entry_without_metadata(self) -> None:
        entry_directory = (
            self.library_root / "01_foundations" / "missing-metadata"
        )
        entry_directory.mkdir(parents=True)
        (entry_directory / "README.md").write_text(
            "# Missing Metadata\n", encoding="utf-8"
        )

        result = self.run_tool(
            BUILD_INDEX,
            "--root",
            str(self.library_root),
            expected_returncode=2,
        )
        self.assertIn("missing meta.json", result.stderr)

    def test_validation_requires_complete_entry_skeleton(self) -> None:
        entry_directory = self.create_entry(
            "01_foundations",
            "incomplete-entry",
        )
        (entry_directory / "example.cpp").unlink()

        result = self.run_tool(
            BUILD_INDEX,
            "--root",
            str(self.library_root),
            expected_returncode=2,
        )
        self.assertIn("sibling example.cpp is required", result.stderr)

        (entry_directory / "example.cpp").write_text(
            '#include "algorithm.hpp"\n'
            "int main() { return answer() == 42 ? 0 : 1; }\n",
            encoding="utf-8",
        )
        (entry_directory / "test.cpp").unlink()
        result = self.run_tool(
            BUILD_INDEX,
            "--root",
            str(self.library_root),
            expected_returncode=2,
        )
        self.assertIn("sibling test.cpp is required", result.stderr)

    def test_validation_requires_the_main_test_in_metadata(self) -> None:
        entry_directory = self.create_entry(
            "01_foundations",
            "unlisted-main-test",
            tests=["extra_test.cpp"],
        )
        (entry_directory / "test.cpp").write_text(
            "int main() { return 0; }\n", encoding="utf-8"
        )

        result = self.run_tool(
            BUILD_INDEX,
            "--root",
            str(self.library_root),
            expected_returncode=2,
        )
        self.assertIn("tests must include the sibling test.cpp", result.stderr)


class SearchTests(ToolTestCase):
    def test_free_text_and_filters(self) -> None:
        self.create_entry(
            "02_data_structures",
            "disjoint-set-union",
            title="Disjoint Set Union",
            aliases=["union find"],
            tags=["dsu", "graph"],
            status="verified",
            practice_path="cses/04_graph_algorithms/1676_road-construction",
        )
        self.create_entry(
            "06_strings",
            "z-function",
            title="Z Function",
            aliases=["z algorithm"],
            tags=["string"],
            status="reviewed",
            practice_path="cses/06_string_algorithms/2107_string-matching",
        )
        self.run_tool(BUILD_INDEX, "--root", str(self.library_root))
        catalog_path = self.library_root / "indexes" / "catalog.json"

        result = self.run_tool(
            SEARCH,
            "--catalog",
            str(catalog_path),
            "--tag",
            "dsu",
            "--status",
            "verified",
            "union",
        )
        self.assertIn("disjoint-set-union", result.stdout)
        self.assertNotIn("z-function", result.stdout)

        result = self.run_tool(
            SEARCH,
            "--catalog",
            str(catalog_path),
            "--category",
            "06_strings",
            "pattern-that-does-not-exist",
        )
        self.assertEqual(result.stdout.strip(), "No matches.")


class CsesTagTests(ToolTestCase):
    def prepare_catalog_and_idea(self, tags_line: str) -> Path:
        practice_path = "cses/05_range_queries/example-task"
        self.create_entry(
            "01_foundations",
            "prefix-sum",
            tags=["prefix-sum", "range-query"],
            practice_path=practice_path,
        )
        self.run_tool(BUILD_INDEX, "--root", str(self.library_root))

        idea_path = self.repository_root / practice_path / "idea.txt"
        idea_path.write_text(
            "Example Task - CSES 1\n"
            "https://cses.fi/problemset/task/1\n"
            "\n"
            "{}\n"
            "\n"
            "## Đề bài\n"
            "Nội dung mẫu.\n".format(tags_line),
            encoding="utf-8",
        )
        return idea_path

    def run_tag_checker(self, expected_returncode: int = 0) -> subprocess.CompletedProcess:
        return self.run_tool(
            CHECK_CSES_TAGS,
            "--repository-root",
            str(self.repository_root),
            "--catalog",
            str(self.library_root / "indexes" / "catalog.json"),
            expected_returncode=expected_returncode,
        )

    def test_valid_canonical_tags(self) -> None:
        self.prepare_catalog_and_idea("Tags: prefix-sum, range-query")
        result = self.run_tag_checker()
        self.assertIn("1 tagged, 2 assignments", result.stdout)
        self.assertIn(
            "Semantic correctness and coverage are not checked.",
            result.stdout,
        )

    def test_allows_untagged_ideas_during_staged_rollout(self) -> None:
        idea_path = self.prepare_catalog_and_idea(
            "Tags: prefix-sum, range-query"
        )
        idea_path.write_text(
            "Example Task - CSES 1\n"
            "https://cses.fi/problemset/task/1\n"
            "\n"
            "## Đề bài\n"
            "Nội dung mẫu.\n",
            encoding="utf-8",
        )

        result = self.run_tag_checker()
        self.assertIn("0 tagged, 0 assignments", result.stdout)
        self.assertIn(
            "Semantic correctness and coverage are not checked.",
            result.stdout,
        )

    def test_rejects_malformed_tag_prefix(self) -> None:
        self.prepare_catalog_and_idea(" Tags : prefix-sum")
        result = self.run_tag_checker(expected_returncode=2)
        self.assertIn("use the exact prefix 'Tags: '", result.stderr)

    def test_requires_a_canonical_entry_id(self) -> None:
        self.prepare_catalog_and_idea("Tags: range-query")
        result = self.run_tag_checker(expected_returncode=2)
        self.assertIn("canonical catalog entry id", result.stderr)

    def test_rejects_noncanonical_placement(self) -> None:
        idea_path = self.prepare_catalog_and_idea("Tags: prefix-sum")
        idea_path.write_text(
            "Example Task - CSES 1\n"
            "https://cses.fi/problemset/task/1\n"
            "\n"
            "\n"
            "Tags: prefix-sum\n"
            "\n"
            "## Đề bài\n"
            "Nội dung mẫu.\n",
            encoding="utf-8",
        )
        result = self.run_tag_checker(expected_returncode=2)
        self.assertIn("one blank line after the problem URL", result.stderr)

    def test_rejects_empty_or_unexpected_cses_inventory(self) -> None:
        self.create_entry(
            "01_foundations",
            "prefix-sum",
            tags=["prefix-sum", "range-query"],
        )
        self.run_tool(BUILD_INDEX, "--root", str(self.library_root))
        result = self.run_tag_checker(expected_returncode=2)
        self.assertIn("contains no idea.txt files", result.stderr)

        nested_idea = (
            self.repository_root
            / "cses"
            / "01_introductory"
            / "nested"
            / "example-task"
            / "idea.txt"
        )
        nested_idea.parent.mkdir(parents=True)
        nested_idea.write_text(
            "Nested Task - CSES 2\n"
            "https://cses.fi/problemset/task/2\n"
            "\n"
            "## Đề bài\n",
            encoding="utf-8",
        )
        result = self.run_tag_checker(expected_returncode=2)
        self.assertIn("unexpected CSES idea.txt layout", result.stderr)

    def test_rejects_unsorted_duplicate_and_unknown_tags(self) -> None:
        idea_path = self.prepare_catalog_and_idea(
            "Tags: range-query, prefix-sum, prefix-sum"
        )
        result = self.run_tag_checker(expected_returncode=2)
        self.assertIn("unique, sorted", result.stderr)

        idea_path.write_text(
            "Example Task - CSES 1\n"
            "https://cses.fi/problemset/task/1\n"
            "\n"
            "Tags: prefix-sum, typo-tag\n"
            "\n"
            "## Đề bài\n"
            "Nội dung mẫu.\n",
            encoding="utf-8",
        )
        result = self.run_tag_checker(expected_returncode=2)
        self.assertIn("not present in the CP catalog", result.stderr)


class BundleTests(ToolTestCase):
    def test_recursive_inline_guards_duplicates_and_terminates_cycles(self) -> None:
        source_directory = self.repository_root / "bundle"
        source_directory.mkdir()
        (source_directory / "common.hpp").write_text(
            "#pragma once\ninline int common() { return 7; }\n",
            encoding="utf-8",
        )
        (source_directory / "a.hpp").write_text(
            '#pragma once\n#include "b.hpp"\n'
            "inline int a() { return b(); }\n",
            encoding="utf-8",
        )
        (source_directory / "b.hpp").write_text(
            '#pragma once\n#include "a.hpp"\n#include "common.hpp"\n'
            "inline int b() { return common(); }\n",
            encoding="utf-8",
        )
        source = source_directory / "main.cpp"
        source.write_text(
            '#include <iostream>\n#include "a.hpp"\n#include "common.hpp"\n'
            "int main() { std::cout << a() << '\\n'; }\n",
            encoding="utf-8",
        )

        result = self.run_tool(BUNDLE, str(source))
        bundled = result.stdout
        self.assertNotIn("#pragma once", bundled)
        self.assertNotIn('#include "a.hpp"', bundled)
        self.assertEqual(bundled.count("inline int common()"), 2)
        self.assertEqual(bundled.count("inline int a()"), 1)
        self.assertEqual(bundled.count("inline int b()"), 1)
        self.assertIn("CP_LIBRARY_BUNDLE_INCLUDED_", bundled)
        self.assertIn("#include <iostream>", bundled)

        output_path = source_directory / "submission.cpp"
        self.run_tool(BUNDLE, str(source), "-o", str(output_path))
        self.assertEqual(output_path.read_text(encoding="utf-8"), bundled)

    def test_conditional_include_keeps_the_first_active_expansion(self) -> None:
        source_directory = self.repository_root / "conditional-bundle"
        source_directory.mkdir()
        (source_directory / "value.hpp").write_text(
            "#pragma once\ninline int value() { return 7; }\n",
            encoding="utf-8",
        )
        source = source_directory / "main.cpp"
        source.write_text(
            '#if 0\n#include "value.hpp"\n#endif\n'
            '#include "value.hpp"\n'
            "int main() { return value() == 7 ? 0 : 1; }\n",
            encoding="utf-8",
        )

        output_path = source_directory / "submission.cpp"
        self.run_tool(BUNDLE, str(source), "-o", str(output_path))
        bundled = output_path.read_text(encoding="utf-8")
        self.assertNotIn('#include "value.hpp"', bundled)
        self.assertEqual(bundled.count("inline int value()"), 2)

        executable = source_directory / "submission"
        compile_result = subprocess.run(
            [
                os.environ.get("CXX", "c++"),
                "-std=c++17",
                str(output_path),
                "-o",
                str(executable),
            ],
            check=False,
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
        self.assertEqual(
            compile_result.returncode,
            0,
            msg="stdout:\n{}\nstderr:\n{}".format(
                compile_result.stdout, compile_result.stderr
            ),
        )
        run_result = subprocess.run(
            [str(executable)],
            check=False,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
        self.assertEqual(run_result.returncode, 0)

    def test_include_cannot_escape_allowed_roots(self) -> None:
        source_directory = self.repository_root / "safe"
        source_directory.mkdir()
        (self.repository_root / "outside.hpp").write_text(
            "inline int outside() { return 0; }\n", encoding="utf-8"
        )
        source = source_directory / "main.cpp"
        source.write_text('#include "../outside.hpp"\n', encoding="utf-8")
        result = self.run_tool(BUNDLE, str(source), expected_returncode=2)
        self.assertIn("escapes allowed roots", result.stderr)


if __name__ == "__main__":
    unittest.main()
