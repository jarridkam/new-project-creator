from pathlib import Path
import tkinter as tk
from tkinter import ttk, filedialog, messagebox

import project_creator


ICON_PATH = Path(__file__).resolve().parent / "icon.png"


def build_window():
    window = tk.Tk()
    window.title("New Project Creator")
    window.geometry("560x320")

    if ICON_PATH.is_file():
        window.iconphoto(True, tk.PhotoImage(file=ICON_PATH))

    panel = ttk.Frame(window, padding=24)
    panel.pack(fill="both", expand=True)

    game_name = tk.StringVar()
    namespace = tk.StringVar()
    destination = tk.StringVar(
        value=str(project_creator.DEFAULT_DESTINATION)
    )

    def browse_folder():
        folder = filedialog.askdirectory(
            parent=window,
            title="Choose where to create your game",
            initialdir=destination.get() or project_creator.DEFAULT_DESTINATION,
        )

        if folder:
            destination.set(folder)

    def create_project():
        try:
            project_folder = project_creator.create_project(
                game_name.get(),
                namespace.get(),
                destination.get(),
            )
        except project_creator.ProjectError as error:
            messagebox.showerror(error.title, error.message, parent=window)
            return

        messagebox.showinfo(
            "Project created",
            f"Copied your template to:\n{project_folder}",
            parent=window,
        )

    ttk.Label(panel, text="Game name").grid(
        row=0, column=0, sticky="w"
    )

    ttk.Entry(panel, textvariable=game_name).grid(
        row=1, column=0, columnspan=2,
        sticky="ew", pady=(4, 12),
    )

    ttk.Label(panel, text="C++ namespace").grid(
        row=2, column=0, sticky="w"
    )

    ttk.Entry(panel, textvariable=namespace).grid(
        row=3, column=0, columnspan=2,
        sticky="ew", pady=(4, 12),
    )

    ttk.Label(panel, text="Destination folder").grid(
        row=4, column=0, sticky="w"
    )

    ttk.Entry(panel, textvariable=destination).grid(
        row=5, column=0, sticky="ew", pady=(4, 16),
    )

    ttk.Button(
        panel, text="Browse…", command=browse_folder
    ).grid(
        row=5, column=1, padx=(8, 0), pady=(4, 16),
    )

    ttk.Button(
        panel, text="Create Project", command=create_project
    ).grid(
        row=6, column=0, columnspan=2, sticky="e",
    )

    panel.columnconfigure(0, weight=1)

    return window
