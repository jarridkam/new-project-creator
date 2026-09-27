from pathlib import Path
import re
import shutil


ROOT = Path(__file__).resolve().parent

TEMPLATE_PATH = ROOT / "project-template"

DEFAULT_DESTINATION = ROOT.parent

IGNORED_FILES = shutil.ignore_patterns(
    ".git",
    ".venv",
    ".venv-*",
    ".idea",
    ".DS_Store",
    "__pycache__",
    "build",
    "cmake-build-*",
)

TEMPLATE_NAMESPACE = "template_game"

NAMESPACE_FILES = (
    Path("src") / "main.cpp",
    Path("src") / "game.h",
    Path("src") / "game.cpp",
)

TEMPLATE_TITLE = "TEMPLATE GAME"

TITLE_FILE = Path("src") / "game.cpp"

CMAKE_FILE = Path("CMakeLists.txt")

CMAKE_NAME_INVALID = re.compile(r"[^A-Za-z0-9_.+-]+")

# Allows nested namespaces such as "studio::my_game".
NAMESPACE_PATTERN = re.compile(
    r"[A-Za-z_][A-Za-z0-9_]*(::[A-Za-z_][A-Za-z0-9_]*)*"
)


class ProjectError(Exception):
    def __init__(self, title, message):
        super().__init__(message)
        self.title = title
        self.message = message


def create_project(name, cpp_namespace, folder):
    name = name.strip()
    cpp_namespace = cpp_namespace.strip()
    folder = folder.strip()

    if not name or not cpp_namespace or not folder:
        raise ProjectError(
            "Missing information",
            "Enter a game name, namespace, and destination.",
        )

    if name in (".", "..") or any(c in name for c in '/\\:"\0'):
        raise ProjectError(
            "Invalid game name",
            "Choose a folder name without slashes, colons, or quotes.",
        )

    if not NAMESPACE_PATTERN.fullmatch(cpp_namespace):
        raise ProjectError(
            "Invalid namespace",
            "Use letters, digits, and underscores, not starting with a digit.",
        )

    if not TEMPLATE_PATH.is_dir():
        raise ProjectError(
            "Template not found",
            f"Couldn't find the template at:\n{TEMPLATE_PATH}",
        )

    parent_folder = Path(folder).expanduser().resolve()
    project_folder = parent_folder / name

    if not parent_folder.is_dir():
        raise ProjectError(
            "Invalid destination",
            "Choose an existing destination folder.",
        )

    if project_folder.exists():
        raise ProjectError(
            "Project already exists",
            f"This location already exists:\n{project_folder}",
        )

    template = TEMPLATE_PATH.resolve()

    if project_folder == template or template in project_folder.parents:
        raise ProjectError(
            "Invalid destination",
            "Choose a location outside the template folder.",
        )

    try:
        shutil.copytree(template, project_folder, ignore=IGNORED_FILES)
        replace_namespace(project_folder, cpp_namespace)
        replace_title(project_folder, name)
        replace_cmake_name(project_folder, cmake_name(name, cpp_namespace))
    except (OSError, shutil.Error) as error:
        raise ProjectError(
            "Couldn't create project",
            f"{error}\n\nAn incomplete project folder may remain.",
        ) from error

    return project_folder


def replace_namespace(project_folder, cpp_namespace):
    token = re.compile(rf"\b{TEMPLATE_NAMESPACE}\b")

    for relative_path in NAMESPACE_FILES:
        path = project_folder / relative_path
        text = path.read_text(encoding="utf-8")
        path.write_text(token.sub(cpp_namespace, text), encoding="utf-8")


def replace_title(project_folder, name):
    path = project_folder / TITLE_FILE
    text = path.read_text(encoding="utf-8")
    path.write_text(text.replace(TEMPLATE_TITLE, name), encoding="utf-8")


def cmake_name(name, cpp_namespace):
    slug = CMAKE_NAME_INVALID.sub("_", name).strip("_").lower()
    return slug or cpp_namespace.split("::")[-1]


def replace_cmake_name(project_folder, name):
    token = re.compile(rf"\b{TEMPLATE_NAMESPACE}\b")
    path = project_folder / CMAKE_FILE
    text = path.read_text(encoding="utf-8")
    path.write_text(token.sub(name, text), encoding="utf-8")
