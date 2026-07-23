#!/usr/bin/env python3
"""Validate the format and catalog vocabulary of CSES Tags lines."""

import argparse
import json
import re
import sys
from pathlib import Path
from typing import Any, Dict, List, Optional, Sequence, Set


TAG_PATTERN = re.compile(r"^[a-z0-9]+(?:-[a-z0-9]+)*$")
TAG_LIKE_PATTERN = re.compile(r"^\s*tags\s*:", re.IGNORECASE)
CSES_URL_PATTERN = re.compile(
    r"^https://cses\.fi/problemset/task/[0-9]+/?$"
)


class TagError(Exception):
    """Raised when the catalog or a CSES Tags line is invalid."""


def _read_catalog(path: Path) -> List[Dict[str, Any]]:
    try:
        with path.open("r", encoding="utf-8") as input_file:
            catalog = json.load(input_file)
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise TagError("cannot read {}: {}".format(path, error))

    if not isinstance(catalog, dict) or catalog.get("schema_version") != 1:
        raise TagError("{} is not a schema v1 catalog".format(path))
    entries = catalog.get("entries")
    if not isinstance(entries, list) or not all(
        isinstance(entry, dict) for entry in entries
    ):
        raise TagError("{}: entries must be a list of objects".format(path))
    return entries


def _tag_vocabulary(
    entries: Sequence[Dict[str, Any]]
) -> tuple[Set[str], Set[str]]:
    allowed: Set[str] = set()
    entry_ids: Set[str] = set()
    for entry in entries:
        entry_id = entry.get("id")
        tags = entry.get("tags")
        if not isinstance(entry_id, str) or not TAG_PATTERN.fullmatch(entry_id):
            raise TagError("catalog contains an invalid entry id")
        if not isinstance(tags, list) or not all(
            isinstance(tag, str) and TAG_PATTERN.fullmatch(tag) for tag in tags
        ):
            raise TagError(
                "catalog entry {!r} contains invalid tags".format(entry_id)
            )
        allowed.add(entry_id)
        entry_ids.add(entry_id)
        allowed.update(tags)
    return allowed, entry_ids


def _validate_idea(
    path: Path, allowed: Set[str], entry_ids: Set[str]
) -> int:
    try:
        lines = path.read_text(encoding="utf-8").splitlines()
    except (OSError, UnicodeError) as error:
        raise TagError("{}: cannot read file: {}".format(path, error))

    tag_indices = [
        index for index, line in enumerate(lines)
        if TAG_LIKE_PATTERN.match(line)
    ]
    if not tag_indices:
        return 0
    if len(tag_indices) != 1:
        raise TagError("{}: expected at most one Tags line".format(path))

    index = tag_indices[0]
    prefix = "Tags: "
    if not lines[index].startswith(prefix):
        raise TagError(
            "{}: use the exact prefix 'Tags: '".format(path)
        )
    url_indices = [
        line_index for line_index, line in enumerate(lines)
        if CSES_URL_PATTERN.fullmatch(line)
    ]
    if len(url_indices) != 1:
        raise TagError("{}: Tags line must appear after the problem URL".format(path))
    url_index = url_indices[0]
    if (
        index != url_index + 2
        or lines[url_index + 1] != ""
        or index + 2 >= len(lines)
        or lines[index + 1] != ""
        or not lines[index + 2].startswith("## ")
    ):
        raise TagError(
            "{}: Tags must be one blank line after the problem URL and one "
            "blank line before the first section".format(path)
        )
    raw_tags = lines[index][len(prefix):]
    if not raw_tags:
        raise TagError("{}: Tags list must not be empty".format(path))

    tags = raw_tags.split(", ")
    canonical = "Tags: {}".format(", ".join(sorted(set(tags))))
    if lines[index] != canonical:
        raise TagError(
            "{}: Tags must be unique, sorted, and separated by ', '; expected {!r}"
            .format(path, canonical)
        )

    invalid = [tag for tag in tags if not TAG_PATTERN.fullmatch(tag)]
    if invalid:
        raise TagError(
            "{}: invalid tag syntax: {}".format(path, ", ".join(invalid))
        )
    unknown = [tag for tag in tags if tag not in allowed]
    if unknown:
        raise TagError(
            "{}: tags are not present in the CP catalog: {}".format(
                path, ", ".join(unknown)
            )
        )
    if not any(tag in entry_ids for tag in tags):
        raise TagError(
            "{}: include at least one canonical catalog entry id".format(path)
        )
    return len(tags)


def _parse_arguments(arguments: Optional[Sequence[str]]) -> argparse.Namespace:
    default_library = Path(__file__).resolve().parents[1]
    parser = argparse.ArgumentParser(
        description=(
            "Validate the format and catalog vocabulary of Tags lines in "
            "CSES idea.txt files."
        )
    )
    parser.add_argument(
        "--repository-root",
        type=Path,
        default=default_library.parent,
        help="repository root containing cses/ and cp_library/",
    )
    parser.add_argument(
        "--catalog",
        type=Path,
        default=default_library / "indexes" / "catalog.json",
        help="generated CP library catalog",
    )
    return parser.parse_args(arguments)


def main(arguments: Optional[Sequence[str]] = None) -> int:
    options = _parse_arguments(arguments)
    repository_root = options.repository_root.resolve()
    cses_root = repository_root / "cses"
    if not cses_root.is_dir():
        print("error: CSES root is not a directory: {}".format(cses_root),
              file=sys.stderr)
        return 2

    try:
        entries = _read_catalog(options.catalog.resolve())
        allowed, entry_ids = _tag_vocabulary(entries)
        idea_paths = sorted(cses_root.rglob("idea.txt"))
        if not idea_paths:
            raise TagError("{} contains no idea.txt files".format(cses_root))
        unexpected_paths = [
            path for path in idea_paths
            if len(path.relative_to(cses_root).parts) != 3
        ]
        if unexpected_paths:
            raise TagError(
                "unexpected CSES idea.txt layout: {}".format(
                    ", ".join(str(path) for path in unexpected_paths)
                )
            )
        tagged_files = 0
        assignment_count = 0
        errors: List[str] = []
        for idea_path in idea_paths:
            try:
                count = _validate_idea(idea_path, allowed, entry_ids)
                if count > 0:
                    tagged_files += 1
                    assignment_count += count
            except TagError as error:
                errors.append(str(error))
        if errors:
            raise TagError("\n".join(errors))
    except TagError as error:
        print("error: {}".format(error), file=sys.stderr)
        return 2

    print(
        "CSES tag format/vocabulary valid: {} idea files, {} tagged, "
        "{} assignments. Semantic correctness and coverage are not "
        "checked.".format(
            len(idea_paths), tagged_files, assignment_count
        )
    )
    return 0


if __name__ == "__main__":
    sys.exit(main())
