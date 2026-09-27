# New Project Creator

A small Tkinter app that creates a new C++ game project from a template.
Enter a game name, a C++ namespace, and a destination folder, and it copies
the template there and swaps in your namespace.

The template lives in `project-template/` (raylib + nlohmann/json + Clay,
fetched by CMake on the first build).

## Running

```sh
python3 -m venv .venv
.venv/bin/python main.py
```

## macOS app

`./make_app.sh` builds `New Project Creator.app`, a double-clickable wrapper
that runs `main.py` with this folder's `.venv`. It also turns `icon.png`
(1024×1024 or larger) into the app icon. Re-run it after moving this folder
or changing the icon.
