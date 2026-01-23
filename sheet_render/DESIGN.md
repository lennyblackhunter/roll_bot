Character Sheet Image Generator - Design Sketch

Goal
Generate a nice-looking character sheet using an HTML template (Jinja2) in a Python library.
The output is deterministic HTML that can later be rendered to PDF/PNG.

Why HTML first
- Rich layout with imagery and ornamentation is easy in CSS.
- We can add background art, frames, and section dividers without custom graphics code.
- Later we can render HTML -> PDF/PNG with a headless browser.

High-Level Flow
1) Load CharacterSheet data from existing model (JSON input).
2) Normalize to a render-friendly layout model (rows, groups, labels, values).
3) Render HTML via Jinja2 templates + CSS assets.
4) Optionally render HTML -> PDF/PNG in a separate step.

Layout Sketch (A4, portrait)
- Header band: character name (large), player name (optional), era/campaign (optional)
- Left column: Attributes and Resources
- Right column: Abilities (skill list)
- Footer: Notes + "Last updated" timestamp

Typography and Style
- One serif for headings, one clean sans for body.
- Strong grid; 2 columns with fixed gutters.
- Decorative rules, soft textures, and panel frames (no literal glow).

Proposed API (Python)
sheet_render.render_html(sheet_json_path, output_html_path, options={...})

Directory Structure
sheet_render/
  DESIGN.md
  sheet_render.hh
  sheet_render.cc
  assets/  (optional fonts or template data)
  BUILD

Dependencies
- Python: Jinja2 for templating.
- Optional: headless browser for HTML -> PDF/PNG (not required initially).

Immediate Next Step
Create a Python library + Bazel py_library target and a starter HTML template.
