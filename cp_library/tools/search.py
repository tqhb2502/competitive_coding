#!/usr/bin/env python3
"""Search the generated cp_library catalog."""

import argparse
import json
import sys
from pathlib import Path
from typing import Any, Dict, List, Mapping, Optional, Sequence


STATUSES = ("draft", "reviewed", "verified", "stress-tested")


class SearchError(Exception):
    """Raised when catalog input is missing or malformed."""


def _read_catalog(path: Path) -> List[Dict[str, Any]]:
    try:
        with path.open("r", encoding="utf-8") as input_file:
            catalog = json.load(input_file)
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise SearchError("cannot read {}: {}".format(path, error))

    if not isinstance(catalog, dict) or catalog.get("schema_version") != 1:
        raise SearchError("{} is not a schema v1 catalog".format(path))
    entries = catalog.get("entries")
    if not isinstance(entries, list) or not all(
        isinstance(entry, dict) for entry in entries
    ):
        raise SearchError("{}: entries must be a list of objects".format(path))
    return entries


def _search_text(entry: Mapping[str, Any]) -> str:
    values: List[str] = [
        str(entry.get("id", "")),
        str(entry.get("title", "")),
        str(entry.get("category", "")),
        str(entry.get("summary", "")),
        str(entry.get("status", "")),
    ]
    values.extend(str(alias) for alias in entry.get("aliases", []))
    values.extend(str(tag) for tag in entry.get("tags", []))

    complexities = entry.get("complexities", {})
    if isinstance(complexities, dict):
        for key, value in complexities.items():
            values.extend((str(key), str(value)))

    practice = entry.get("practice", [])
    if isinstance(practice, list):
        for item in practice:
            if isinstance(item, dict):
                values.extend((str(item.get("title", "")), str(item.get("path", ""))))
    return "\n".join(values).casefold()


def search_entries(
    entries: Sequence[Mapping[str, Any]],
    terms: Sequence[str],
    categories: Sequence[str],
    tags: Sequence[str],
    statuses: Sequence[str],
) -> List[Mapping[str, Any]]:
    normalized_terms = [term.casefold() for term in terms if term]
    category_filter = {category.casefold() for category in categories}
    tag_filter = {tag.casefold() for tag in tags}
    status_filter = {status.casefold() for status in statuses}

    matches: List[Mapping[str, Any]] = []
    for entry in entries:
        category = str(entry.get("category", "")).casefold()
        status = str(entry.get("status", "")).casefold()
        entry_tags = {str(tag).casefold() for tag in entry.get("tags", [])}

        if category_filter and category not in category_filter:
            continue
        if status_filter and status not in status_filter:
            continue
        if tag_filter and not tag_filter.issubset(entry_tags):
            continue

        haystack = _search_text(entry)
        if not all(term in haystack for term in normalized_terms):
            continue
        matches.append(entry)

    return sorted(
        matches,
        key=lambda entry: (str(entry.get("category", "")), str(entry.get("id", ""))),
    )


def _parse_arguments(arguments: Optional[Sequence[str]]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Search cp_library catalog.json.")
    parser.add_argument("query", nargs="*", help="free-text terms (all must match)")
    parser.add_argument(
        "--catalog",
        type=Path,
        default=Path(__file__).resolve().parents[1] / "indexes" / "catalog.json",
        help="path to generated catalog.json",
    )
    parser.add_argument(
        "--category",
        action="append",
        default=[],
        help="exact category filter; repeat for OR",
    )
    parser.add_argument(
        "--tag",
        action="append",
        default=[],
        help="exact tag filter; repeated tags must all be present",
    )
    parser.add_argument(
        "--status",
        action="append",
        choices=STATUSES,
        default=[],
        help="status filter; repeat for OR",
    )
    return parser.parse_args(arguments)


def main(arguments: Optional[Sequence[str]] = None) -> int:
    options = _parse_arguments(arguments)
    try:
        entries = _read_catalog(options.catalog.resolve())
        matches = search_entries(
            entries,
            options.query,
            options.category,
            options.tag,
            options.status,
        )
    except SearchError as error:
        print("error: {}".format(error), file=sys.stderr)
        return 2

    if not matches:
        print("No matches.")
        return 0

    for entry in matches:
        tags = ",".join(str(tag) for tag in entry.get("tags", [])) or "-"
        print(
            "{}\t{}\t{}\t{}\t{}".format(
                entry.get("id", ""),
                entry.get("title", ""),
                entry.get("category", ""),
                entry.get("status", ""),
                tags,
            )
        )
    return 0


if __name__ == "__main__":
    sys.exit(main())
