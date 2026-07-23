#!/usr/bin/env python3
"""Inline quoted local C++ headers into one submission file."""

import argparse
import hashlib
import re
import sys
from pathlib import Path, PurePosixPath
from typing import List, Optional, Sequence, Set


INCLUDE_PATTERN = re.compile(
    r'^\s*#\s*include\s*"([^"]+)"'
    r"(?:\s*(?://.*|/\*.*\*/))?\s*$"
)
PRAGMA_ONCE_PATTERN = re.compile(
    r"^\s*#\s*pragma\s+once(?:\s*(?://.*|/\*.*\*/))?\s*$"
)


class BundleError(Exception):
    """Raised when an include cannot be expanded safely."""


def _is_within(path: Path, root: Path) -> bool:
    try:
        path.relative_to(root)
        return True
    except ValueError:
        return False


class Bundler:
    def __init__(self, source: Path, include_roots: Sequence[Path]) -> None:
        self.source = source.resolve()
        if not self.source.is_file():
            raise BundleError("source is not a file: {}".format(source))

        roots = [self.source.parent]
        for root in include_roots:
            resolved = root.resolve()
            if not resolved.is_dir():
                raise BundleError("include root is not a directory: {}".format(root))
            roots.append(resolved)

        self.include_roots: List[Path] = []
        for root in roots:
            if root not in self.include_roots:
                self.include_roots.append(root)
        self.seen: Set[Path] = set()
        self.active: List[Path] = []

    @staticmethod
    def _include_guard(path: Path) -> str:
        digest = hashlib.sha256(path.as_posix().encode("utf-8")).hexdigest()
        return "CP_LIBRARY_BUNDLE_INCLUDED_{}".format(digest.upper())

    def _resolve_include(self, including_file: Path, include_name: str) -> Path:
        if "\\" in include_name:
            raise BundleError(
                "{}: include path must use forward slashes: {!r}".format(
                    including_file, include_name
                )
            )
        pure_name = PurePosixPath(include_name)
        if pure_name.is_absolute():
            raise BundleError(
                "{}: absolute quoted include is not allowed: {!r}".format(
                    including_file, include_name
                )
            )

        search_directories = [including_file.parent] + self.include_roots
        checked: Set[Path] = set()
        unsafe_match: Optional[Path] = None
        for directory in search_directories:
            candidate = (directory / Path(*pure_name.parts)).resolve()
            if candidate in checked:
                continue
            checked.add(candidate)
            if not any(
                _is_within(candidate, allowed_root)
                for allowed_root in self.include_roots
            ):
                if candidate.exists():
                    unsafe_match = candidate
                continue
            if candidate.is_file():
                return candidate

        if unsafe_match is not None:
            raise BundleError(
                "{}: include escapes allowed roots: {!r}".format(
                    including_file, include_name
                )
            )
        raise BundleError(
            "{}: quoted include not found: {!r}".format(
                including_file, include_name
            )
        )

    def _expand(self, path: Path) -> List[str]:
        resolved = path.resolve()
        if resolved in self.active:
            # Mỗi header được bọc bằng guard tổng hợp trước phần nội dung, nên
            # bỏ lần include ngược về một header đang mở là tương đương với
            # hành vi của #pragma once. Root source không có lớp bọc đó.
            if resolved == self.source:
                raise BundleError(
                    "include cycle reaches the root source: {}".format(resolved)
                )
            return []

        self.seen.add(resolved)
        self.active.append(resolved)
        try:
            try:
                text = resolved.read_text(encoding="utf-8")
            except (OSError, UnicodeError) as error:
                raise BundleError("cannot read {}: {}".format(resolved, error))

            output: List[str] = []
            for line_number, line in enumerate(text.splitlines(), start=1):
                if PRAGMA_ONCE_PATTERN.fullmatch(line):
                    continue
                include_match = INCLUDE_PATTERN.fullmatch(line)
                if include_match is None:
                    output.append(line + "\n")
                    continue

                include_name = include_match.group(1)
                try:
                    included_path = self._resolve_include(resolved, include_name)
                    output.extend(self._expand(included_path))
                except BundleError as error:
                    chain = " -> ".join(str(item) for item in self.active)
                    raise BundleError(
                        "{}:{}: {}\ninclude chain: {}".format(
                            resolved, line_number, error, chain
                        )
                    )
        finally:
            self.active.pop()

        if resolved == self.source:
            return output

        # Không thể global-deduplicate text trước khi preprocessor chạy: lần
        # include đầu có thể nằm trong một nhánh #if không hoạt động. Lặp lớp
        # bọc này tại từng include giữ đúng ngữ nghĩa điều kiện; guard tổng hợp
        # vẫn khiến nội dung header chỉ được preprocessor nhận một lần.
        guard = self._include_guard(resolved)
        return [
            "#ifndef {}\n".format(guard),
            "#define {}\n".format(guard),
        ] + output + ["#endif  // {}\n".format(guard)]

    def bundle(self) -> str:
        return "".join(self._expand(self.source))


def bundle_source(source: Path, include_roots: Sequence[Path]) -> str:
    """Return a bundled translation unit for source."""
    return Bundler(source, include_roots).bundle()


def _parse_arguments(arguments: Optional[Sequence[str]]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Recursively inline quoted local C++ headers."
    )
    parser.add_argument("source", type=Path, help="root C++ source file")
    parser.add_argument(
        "-I",
        "--include-root",
        action="append",
        default=[],
        type=Path,
        help="allowed include search root; repeat as needed",
    )
    parser.add_argument("-o", "--output", type=Path, help="output file (stdout by default)")
    return parser.parse_args(arguments)


def main(arguments: Optional[Sequence[str]] = None) -> int:
    options = _parse_arguments(arguments)
    try:
        bundler = Bundler(options.source, options.include_root)
        result = bundler.bundle()
        if options.output is None:
            sys.stdout.write(result)
            return 0

        output_path = options.output.resolve()
        if output_path in bundler.seen:
            raise BundleError(
                "output must not overwrite the source or an included file: {}".format(
                    options.output
                )
            )
        if not output_path.parent.is_dir():
            raise BundleError(
                "output directory does not exist: {}".format(output_path.parent)
            )
        output_path.write_text(result, encoding="utf-8")
        return 0
    except (BundleError, OSError) as error:
        print("error: {}".format(error), file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main())
