# AGENTS

This repository is a C++ codebase built with Bazel.

- Prefer small, focused changes and keep diffs minimal.
- Follow existing style and naming conventions in nearby files.
- Add or update tests when changing behavior.
- Use `rg` for searching the codebase.
- When adding source files, create or update Bazel targets near those files (add BUILD files as needed).
- For Bazel targets, keep the main source filename aligned with the target name unless there is a strong reason not to.

## About project

- This is a simple discord bot used for playing Call of Cthulhu 7th edition 
- It implements basic operations on characters like rolling for statistics etc.
- There is not yet working //:cli target that implements local version of the client for testing.

### Running

Example character sheet repository is located under cthulhu/test_repo
You can use //:cli target to check whether everything is working
