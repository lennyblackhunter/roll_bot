from __future__ import annotations

import argparse

from sheet_render import render_html


def main() -> None:
    parser = argparse.ArgumentParser(description="Render a character sheet HTML file.")
    parser.add_argument("sheet_json", help="Path to character sheet JSON file.")
    parser.add_argument("output_html", help="Path to output HTML file.")
    args = parser.parse_args()

    render_html(args.sheet_json, args.output_html)


if __name__ == "__main__":
    main()
