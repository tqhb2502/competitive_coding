#!/usr/bin/env python3
"""Validate cp_library metadata and build deterministic catalog indexes."""

import argparse
import json
import os
import re
import sys
from pathlib import Path, PurePosixPath
from typing import Any, Dict, List, Mapping, Optional, Sequence, Tuple
from urllib.parse import quote


SCHEMA_VERSION = 1
STATUSES = ("draft", "reviewed", "verified", "stress-tested")
ALLOWED_CATEGORIES = (
    "01_foundations",
    "02_data_structures",
    "03_graphs",
    "04_trees",
    "05_dynamic_programming",
    "06_strings",
    "07_mathematics",
    "08_geometry",
    "09_offline_algorithms",
    "10_advanced_techniques",
)
REQUIRED_FIELDS = (
    "schema_version",
    "id",
    "title",
    "category",
    "summary",
    "aliases",
    "tags",
    "status",
    "source",
    "complexities",
    "tests",
    "practice",
)

CATEGORY_PATTERN = re.compile(r"^[0-9]{2}_[a-z0-9]+(?:[_-][a-z0-9]+)*$")
SLUG_PATTERN = re.compile(r"^[a-z0-9]+(?:-[a-z0-9]+)*$")


class CatalogError(Exception):
    """Raised when metadata or generated catalog data is invalid."""


def _object_without_duplicate_keys(pairs: Sequence[Tuple[str, Any]]) -> Dict[str, Any]:
    result: Dict[str, Any] = {}
    for key, value in pairs:
        if key in result:
            raise CatalogError("duplicate JSON key {!r}".format(key))
        result[key] = value
    return result


def _read_json(path: Path) -> Dict[str, Any]:
    try:
        with path.open("r", encoding="utf-8") as input_file:
            value = json.load(
                input_file, object_pairs_hook=_object_without_duplicate_keys
            )
    except CatalogError:
        raise
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise CatalogError("{}: cannot read JSON: {}".format(path, error))
    if not isinstance(value, dict):
        raise CatalogError("{}: top-level JSON value must be an object".format(path))
    return value


def _require_nonempty_string(value: Any, field: str, meta_path: Path) -> str:
    if not isinstance(value, str) or not value.strip():
        raise CatalogError(
            "{}: {!r} must be a non-empty string".format(meta_path, field)
        )
    if value != value.strip():
        raise CatalogError(
            "{}: {!r} must not have surrounding whitespace".format(meta_path, field)
        )
    if any(ord(character) < 32 or ord(character) == 127 for character in value):
        raise CatalogError(
            "{}: {!r} must not contain control characters".format(meta_path, field)
        )
    return value


def _require_string_list(value: Any, field: str, meta_path: Path) -> List[str]:
    if not isinstance(value, list):
        raise CatalogError("{}: {!r} must be a list".format(meta_path, field))
    result: List[str] = []
    seen = set()
    for index, item in enumerate(value):
        item_value = _require_nonempty_string(
            item, "{}[{}]".format(field, index), meta_path
        )
        normalized = item_value.casefold()
        if normalized in seen:
            raise CatalogError(
                "{}: duplicate value {!r} in {!r}".format(
                    meta_path, item_value, field
                )
            )
        seen.add(normalized)
        result.append(item_value)
    return result


def _is_within(path: Path, root: Path) -> bool:
    try:
        path.relative_to(root)
        return True
    except ValueError:
        return False


def _validate_relative_path(
    raw_path: Any,
    *,
    base: Path,
    containment_root: Path,
    field: str,
    meta_path: Path,
    require_file: bool,
) -> Tuple[str, Path]:
    path_value = _require_nonempty_string(raw_path, field, meta_path)
    if "\\" in path_value:
        raise CatalogError(
            "{}: {!r} must use forward slashes".format(meta_path, field)
        )

    pure_path = PurePosixPath(path_value)
    if pure_path.is_absolute() or ".." in pure_path.parts:
        raise CatalogError(
            "{}: {!r} must be a safe relative path".format(meta_path, field)
        )

    resolved_root = containment_root.resolve()
    resolved_path = (base / Path(*pure_path.parts)).resolve()
    if not _is_within(resolved_path, resolved_root):
        raise CatalogError(
            "{}: {!r} escapes {}".format(meta_path, field, containment_root)
        )
    if not resolved_path.exists():
        raise CatalogError(
            "{}: {!r} does not exist: {}".format(meta_path, field, path_value)
        )
    if require_file and not resolved_path.is_file():
        raise CatalogError(
            "{}: {!r} must refer to a file: {}".format(
                meta_path, field, path_value
            )
        )
    return pure_path.as_posix(), resolved_path


def _validate_metadata(
    meta_path: Path, library_root: Path, repository_root: Path
) -> Dict[str, Any]:
    metadata = _read_json(meta_path)
    missing = [field for field in REQUIRED_FIELDS if field not in metadata]
    if missing:
        raise CatalogError(
            "{}: missing required fields: {}".format(meta_path, ", ".join(missing))
        )
    unknown = sorted(set(metadata) - set(REQUIRED_FIELDS))
    if unknown:
        raise CatalogError(
            "{}: unknown fields: {}".format(meta_path, ", ".join(unknown))
        )

    entry_directory = meta_path.parent
    category_directory = entry_directory.parent
    category_name = category_directory.name
    entry_name = entry_directory.name

    schema_version = metadata["schema_version"]
    if (
        isinstance(schema_version, bool)
        or not isinstance(schema_version, int)
        or schema_version != SCHEMA_VERSION
    ):
        raise CatalogError(
            "{}: schema_version must be {}".format(meta_path, SCHEMA_VERSION)
        )

    entry_id = _require_nonempty_string(metadata["id"], "id", meta_path)
    if not SLUG_PATTERN.fullmatch(entry_id):
        raise CatalogError("{}: id must be a lowercase slug".format(meta_path))
    if entry_id != entry_name:
        raise CatalogError(
            "{}: id {!r} must match entry directory {!r}".format(
                meta_path, entry_id, entry_name
            )
        )

    category = _require_nonempty_string(metadata["category"], "category", meta_path)
    if category not in ALLOWED_CATEGORIES:
        raise CatalogError(
            "{}: category must be one of: {}".format(
                meta_path, ", ".join(ALLOWED_CATEGORIES)
            )
        )
    if category != category_name:
        raise CatalogError(
            "{}: category {!r} must match parent directory {!r}".format(
                meta_path, category, category_name
            )
        )

    title = _require_nonempty_string(metadata["title"], "title", meta_path)
    summary = _require_nonempty_string(metadata["summary"], "summary", meta_path)

    aliases = _require_string_list(metadata["aliases"], "aliases", meta_path)
    tags = _require_string_list(metadata["tags"], "tags", meta_path)
    for tag in tags:
        if not SLUG_PATTERN.fullmatch(tag):
            raise CatalogError(
                "{}: tag {!r} must be a lowercase slug".format(meta_path, tag)
            )

    status = _require_nonempty_string(metadata["status"], "status", meta_path)
    if status not in STATUSES:
        raise CatalogError(
            "{}: status must be one of: {}".format(meta_path, ", ".join(STATUSES))
        )

    readme_path = entry_directory / "README.md"
    if not readme_path.is_file():
        raise CatalogError("{}: sibling README.md is required".format(meta_path))
    example_path = entry_directory / "example.cpp"
    if not example_path.is_file():
        raise CatalogError("{}: sibling example.cpp is required".format(meta_path))
    test_path = entry_directory / "test.cpp"
    if not test_path.is_file():
        raise CatalogError("{}: sibling test.cpp is required".format(meta_path))

    source, _ = _validate_relative_path(
        metadata["source"],
        base=entry_directory,
        containment_root=entry_directory,
        field="source",
        meta_path=meta_path,
        require_file=True,
    )
    if PurePosixPath(source).suffix != ".hpp":
        raise CatalogError("{}: source must have extension .hpp".format(meta_path))

    complexities_value = metadata["complexities"]
    if not isinstance(complexities_value, dict) or not complexities_value:
        raise CatalogError(
            "{}: complexities must be a non-empty object".format(meta_path)
        )
    complexities: Dict[str, str] = {}
    for key, value in complexities_value.items():
        key_value = _require_nonempty_string(key, "complexities key", meta_path)
        value_string = _require_nonempty_string(
            value, "complexities[{!r}]".format(key_value), meta_path
        )
        complexities[key_value] = value_string

    tests_value = metadata["tests"]
    if not isinstance(tests_value, list):
        raise CatalogError("{}: tests must be a list".format(meta_path))
    tests: List[str] = []
    seen_tests = set()
    for index, test_path in enumerate(tests_value):
        normalized_path, _ = _validate_relative_path(
            test_path,
            base=entry_directory,
            containment_root=entry_directory,
            field="tests[{}]".format(index),
            meta_path=meta_path,
            require_file=True,
        )
        if PurePosixPath(normalized_path).suffix != ".cpp":
            raise CatalogError(
                "{}: tests[{}] must have extension .cpp".format(meta_path, index)
            )
        if normalized_path in seen_tests:
            raise CatalogError(
                "{}: duplicate test path {!r}".format(meta_path, normalized_path)
            )
        seen_tests.add(normalized_path)
        tests.append(normalized_path)
    if status in ("verified", "stress-tested") and not tests:
        raise CatalogError(
            "{}: status {!r} requires at least one test".format(meta_path, status)
        )
    if "test.cpp" not in seen_tests:
        raise CatalogError(
            "{}: tests must include the sibling test.cpp".format(meta_path)
        )

    practice_value = metadata["practice"]
    if not isinstance(practice_value, list):
        raise CatalogError("{}: practice must be a list".format(meta_path))
    practice: List[Dict[str, str]] = []
    seen_practice = set()
    for index, item in enumerate(practice_value):
        if not isinstance(item, dict) or set(item) != {"title", "path"}:
            raise CatalogError(
                "{}: practice[{}] must contain exactly title and path".format(
                    meta_path, index
                )
            )
        practice_title = _require_nonempty_string(
            item["title"], "practice[{}].title".format(index), meta_path
        )
        practice_path, _ = _validate_relative_path(
            item["path"],
            base=repository_root,
            containment_root=repository_root,
            field="practice[{}].path".format(index),
            meta_path=meta_path,
            require_file=False,
        )
        practice_key = (practice_title.casefold(), practice_path)
        if practice_key in seen_practice:
            raise CatalogError(
                "{}: duplicate practice item {!r}".format(meta_path, practice_title)
            )
        seen_practice.add(practice_key)
        practice.append({"title": practice_title, "path": practice_path})

    entry_path = entry_directory.relative_to(library_root).as_posix()
    return {
        "schema_version": SCHEMA_VERSION,
        "id": entry_id,
        "title": title,
        "category": category,
        "summary": summary,
        "aliases": sorted(aliases, key=str.casefold),
        "tags": sorted(tags),
        "status": status,
        "source": "{}/{}".format(entry_path, source),
        "complexities": dict(sorted(complexities.items())),
        "tests": ["{}/{}".format(entry_path, path) for path in sorted(tests)],
        "example": "{}/example.cpp".format(entry_path),
        "practice": sorted(
            practice, key=lambda item: (item["title"].casefold(), item["path"])
        ),
        "entry_path": entry_path,
        "readme": "{}/README.md".format(entry_path),
    }


def load_entries(library_root: Path) -> List[Dict[str, Any]]:
    """Load, validate, and deterministically sort all exact-depth entries."""
    library_root = library_root.resolve()
    if not library_root.is_dir():
        raise CatalogError(
            "library root does not exist or is not a directory: {}".format(
                library_root
            )
        )
    repository_root = library_root.parent.resolve()

    meta_paths: List[Path] = []
    for category_directory in sorted(library_root.iterdir()):
        if (
            not category_directory.is_dir()
            or not CATEGORY_PATTERN.fullmatch(category_directory.name)
        ):
            continue
        if category_directory.is_symlink():
            raise CatalogError(
                "category directory must not be a symbolic link: {}".format(
                    category_directory
                )
            )
        for entry_directory in sorted(category_directory.iterdir()):
            if not entry_directory.is_dir():
                continue
            if entry_directory.is_symlink():
                raise CatalogError(
                    "entry directory must not be a symbolic link: {}".format(
                        entry_directory
                    )
                )
            meta_path = entry_directory / "meta.json"
            if not meta_path.is_file():
                raise CatalogError(
                    "entry directory is missing meta.json: {}".format(
                        entry_directory
                    )
                )
            meta_paths.append(meta_path)

    entries = [
        _validate_metadata(path, library_root, repository_root)
        for path in meta_paths
    ]

    names: Dict[str, str] = {}
    sources: Dict[str, str] = {}
    for entry in entries:
        entry_id = str(entry["id"])
        for lookup_name in [entry_id] + list(entry["aliases"]):
            normalized = lookup_name.casefold()
            previous = names.get(normalized)
            if previous is not None:
                raise CatalogError(
                    "duplicate id/alias {!r} in entries {!r} and {!r}".format(
                        lookup_name, previous, entry_id
                    )
                )
            names[normalized] = entry_id

        source = str(entry["source"])
        previous_source = sources.get(source)
        if previous_source is not None:
            raise CatalogError(
                "duplicate source {!r} in entries {!r} and {!r}".format(
                    source, previous_source, entry_id
                )
            )
        sources[source] = entry_id

    return sorted(entries, key=lambda item: (item["category"], item["id"]))


def _markdown_text(value: Any) -> str:
    return (
        str(value)
        .replace("\\", "\\\\")
        .replace("|", "\\|")
        .replace("[", "\\[")
        .replace("]", "\\]")
    )


def _link_target(path: str) -> str:
    return quote(path, safe="/._-")


def _entry_link(entry: Mapping[str, Any]) -> str:
    target = "../{}".format(entry["readme"])
    return "[{}]({})".format(
        _markdown_text(entry["title"]), _link_target(target)
    )


def _render_catalog_markdown(entries: Sequence[Mapping[str, Any]]) -> str:
    lines = [
        "# Danh mục",
        "",
        "_Được sinh bởi `tools/build_index.py`; không chỉnh sửa thủ công._",
        "",
        "| Mã | Tên | Nhóm | Trạng thái | Thẻ | Độ phức tạp |",
        "| --- | --- | --- | --- | --- | --- |",
    ]
    for entry in entries:
        complexities = "; ".join(
            "{}: {}".format(key, value)
            for key, value in entry["complexities"].items()
        )
        lines.append(
            "| `{}` | {} | `{}` | `{}` | {} | {} |".format(
                _markdown_text(entry["id"]),
                _entry_link(entry),
                _markdown_text(entry["category"]),
                _markdown_text(entry["status"]),
                ", ".join("`{}`".format(_markdown_text(tag)) for tag in entry["tags"])
                or "—",
                _markdown_text(complexities),
            )
        )
    return "\n".join(lines) + "\n"


def _render_by_tag(entries: Sequence[Mapping[str, Any]]) -> str:
    grouped: Dict[str, List[Mapping[str, Any]]] = {}
    for entry in entries:
        for tag in entry["tags"]:
            grouped.setdefault(tag, []).append(entry)

    lines = [
        "# Danh mục theo thẻ",
        "",
        "_Được sinh bởi `tools/build_index.py`; không chỉnh sửa thủ công._",
    ]
    if not grouped:
        lines.extend(["", "_Chưa có entry nào có thẻ._"])
    for tag in sorted(grouped):
        lines.extend(["", "## `{}`".format(_markdown_text(tag)), ""])
        for entry in grouped[tag]:
            lines.append(
                "- {} — `{}` · `{}`".format(
                    _entry_link(entry), entry["id"], entry["status"]
                )
            )
    return "\n".join(lines) + "\n"


def _render_by_status(entries: Sequence[Mapping[str, Any]]) -> str:
    grouped: Dict[str, List[Mapping[str, Any]]] = {
        status: [] for status in STATUSES
    }
    for entry in entries:
        grouped[str(entry["status"])].append(entry)

    lines = [
        "# Danh mục theo trạng thái",
        "",
        "_Được sinh bởi `tools/build_index.py`; không chỉnh sửa thủ công._",
    ]
    for status in STATUSES:
        lines.extend(["", "## `{}`".format(status), ""])
        if not grouped[status]:
            lines.append("_Chưa có entry._")
        else:
            for entry in grouped[status]:
                lines.append(
                    "- {} — `{}` · `{}`".format(
                        _entry_link(entry), entry["id"], entry["category"]
                    )
                )
    return "\n".join(lines) + "\n"


def _render_by_practice(
    entries: Sequence[Mapping[str, Any]], library_root: Path
) -> str:
    grouped: Dict[Tuple[str, str], List[Mapping[str, Any]]] = {}
    for entry in entries:
        for item in entry["practice"]:
            key = (item["title"], item["path"])
            grouped.setdefault(key, []).append(entry)

    lines = [
        "# Danh mục theo bài luyện tập",
        "",
        "_Được sinh bởi `tools/build_index.py`; không chỉnh sửa thủ công._",
    ]
    if not grouped:
        lines.extend(["", "_Chưa có bài luyện tập được tham chiếu._"])

    index_directory = library_root / "indexes"
    repository_root = library_root.parent
    for title, practice_path in sorted(
        grouped, key=lambda item: (item[0].casefold(), item[1])
    ):
        target = os.path.relpath(
            repository_root / Path(*PurePosixPath(practice_path).parts),
            index_directory,
        )
        target = Path(target).as_posix()
        lines.extend(
            [
                "",
                "## [{}]({})".format(
                    _markdown_text(title), _link_target(target)
                ),
                "",
            ]
        )
        for entry in grouped[(title, practice_path)]:
            lines.append(
                "- {} — `{}` · `{}`".format(
                    _entry_link(entry), entry["id"], entry["status"]
                )
            )
    return "\n".join(lines) + "\n"


def render_outputs(
    entries: Sequence[Mapping[str, Any]], library_root: Path
) -> Dict[str, str]:
    catalog = {
        "schema_version": SCHEMA_VERSION,
        "entries": list(entries),
    }
    return {
        "catalog.md": _render_catalog_markdown(entries),
        "by-tag.md": _render_by_tag(entries),
        "by-status.md": _render_by_status(entries),
        "by-practice.md": _render_by_practice(entries, library_root),
        "catalog.json": json.dumps(
            catalog, ensure_ascii=False, indent=2, sort_keys=True
        )
        + "\n",
    }


def _check_outputs(output_directory: Path, outputs: Mapping[str, str]) -> bool:
    current = True
    for filename in sorted(outputs):
        path = output_directory / filename
        try:
            existing = path.read_text(encoding="utf-8")
        except (OSError, UnicodeError):
            existing = None
        if existing != outputs[filename]:
            print("stale: {}".format(path), file=sys.stderr)
            current = False
    return current


def _write_outputs(output_directory: Path, outputs: Mapping[str, str]) -> None:
    output_directory.mkdir(parents=True, exist_ok=True)
    for filename in sorted(outputs):
        (output_directory / filename).write_text(
            outputs[filename], encoding="utf-8"
        )


def _parse_arguments(arguments: Optional[Sequence[str]]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Validate metadata and generate cp_library indexes."
    )
    parser.add_argument(
        "library_root",
        nargs="?",
        type=Path,
        help="cp_library root (defaults to the parent of tools/)",
    )
    parser.add_argument(
        "--root",
        dest="root_option",
        type=Path,
        help="explicit cp_library root",
    )
    parser.add_argument(
        "--check",
        action="store_true",
        help="do not write; fail if generated files are missing or stale",
    )
    namespace = parser.parse_args(arguments)
    if namespace.library_root is not None and namespace.root_option is not None:
        parser.error("pass the library root either positionally or with --root")
    return namespace


def main(arguments: Optional[Sequence[str]] = None) -> int:
    options = _parse_arguments(arguments)
    default_root = Path(__file__).resolve().parents[1]
    library_root = (
        options.root_option or options.library_root or default_root
    ).resolve()
    try:
        entries = load_entries(library_root)
        outputs = render_outputs(entries, library_root)
        output_directory = library_root / "indexes"
        if options.check:
            return 0 if _check_outputs(output_directory, outputs) else 1
        _write_outputs(output_directory, outputs)
        return 0
    except CatalogError as error:
        print("error: {}".format(error), file=sys.stderr)
        return 2
    except OSError as error:
        print("error: {}".format(error), file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main())
