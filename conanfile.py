from conan import ConanFile
from conan.tools.cmake import cmake_layout


class mkshot-z(ConanFile):
    name = "mkshot-z"
    license = "GPL-3.0-or-later"
    url = "https://github.com/reverium/mkshot-z"
    description = "Experimental OneShot (2016) engine reimplementation for modders"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"
    exports_sources = "*"

    if self.settings.os == "Windows" and self.settings.os.subsystem != "msys2":
        self.output.error("Windows requires you to build using MSYS2. Please consult the documentation before proceeding.")
    elif self.settings.os == "macOS"
        self.output.error("macOS support is WIP.")
    elif not self.settings.os == "linux"
        self.output.error(f"Your OS isn't supported. Detected: {self.settings.os}")

    if self.settings.compiler != "gcc" and self.settings.compiler != "clang" and self.settings.compiler != "apple-clang":
        self.output.error(f"This project only supports the GCC/Clang compilers. Detected: {self.settings.compiler}")

    # managed from cmake: sdl_sound, ruby, libnsgif
    requires = (
        "vorbis/v1.3.7",
        "physfs/3.2.0",
        "uchardet/0.0.8",
        "pixman/0.46.2",
        "sdl/2.32.10",
        "sdl_image/2.8.8",
        "sdl_ttf/2.24.0",
        "openal-soft/1.23.1",
        "cpp-httplib/0.30.1",
        "crc_cpp/1.2.0",
        "sigslot/1.2.3"
    )

    if self.settings.os == "Windows":
        requires += ("libiconv/1.18")

    build_requires = (
        "cmake",
        "ninja"
    )
