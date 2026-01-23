from __future__ import annotations

import json
import re
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Dict, Iterable, Tuple

from jinja2 import Environment, FileSystemLoader


@dataclass(frozen=True)
class RenderOptions:
    title: str = "Character Sheet"


def _load_template() -> Environment:
    template_dir = Path(__file__).resolve().parent
    return Environment(loader=FileSystemLoader(template_dir), autoescape=True)


def _group_stats(stats: Dict[str, Dict[str, Any]]) -> Tuple[Iterable[Tuple[str, int]], Iterable[Tuple[str, int]], Iterable[Tuple[str, int]]]:
    attributes = []
    abilities = []
    resources = []
    for name, stat in stats.items():
        stat_type = stat.get("stat_type", 0)
        value = stat.get("value", 0)
        if stat_type == 1:
            attributes.append((name, value))
        elif stat_type == 2:
            resources.append((name, value))
        else:
            abilities.append((_format_ability_name(name), value))
    return attributes, abilities, resources


def _format_ability_name(name: str) -> str:
    formatted = name.replace("_", " ")
    formatted = re.sub(r"(?<!\s)\(", " (", formatted)
    return formatted


def render_html(sheet_json_path: str, output_html_path: str, options: RenderOptions | None = None) -> None:
    opts = options or RenderOptions()
    sheet_path = Path(sheet_json_path)
    with sheet_path.open("r", encoding="utf-8") as handle:
        payload = json.load(handle)

    stats = payload.get("stats", {})
    attributes, abilities, resources = _group_stats(stats)

    env = _load_template()
    template = env.get_template("template.j2")
    html = template.render(
        title=opts.title,
        character_name=payload.get("name", "Unknown"),
        attributes=sorted(attributes),
        abilities=sorted(abilities),
        resources=sorted(resources),
    )

    output_path = Path(output_html_path)
    output_path.write_text(html, encoding="utf-8")
