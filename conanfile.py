from conan import ConanFile
import os.path

class mkshot(ConanFile):
    name = "mkshot-z"
    license = "GPL-3.0-or-later"
    url = "https://github.com/reverium/mkshot-z"
    description = "Experimental OneShot (2016) engine reimplementation for modders"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    exports_sources = "*"
    requires = (
        "vorbis/1.3.7",
        "physfs/3.2.0",
        "pixman/0.46.2",
        "sdl/3.4.0",
        "sdl_image/3.4.0",
        "sdl_ttf/3.2.2",
        "openal-soft/1.23.1",
        "cpp-httplib/0.30.1",
        "crc_cpp/1.2.0",
        "sigslot/1.2.3"
    )
    options = {}
    default_options = ()

    def requirements(self):
        if self.os_info.is_windows:
            self.requires("libiconv/1.18")

    def configure(self):
        if tools.os_info.is_windows:
            self.options["openal"].shared = True
            self.options["sdl"].shared = True

    def build_configure(self):
        cmake = CMake(self, msbuild_verbosity='minimal')
        cmake.configure()
        cmake.build()

    def build(self):
        self.build_configure()

    def package(self):
        self.copy("*", dst="bin", src="bin")

    def imports(self):
        self.do_copy_deps(self.copy)

    def deploy(self):
        self.copy("*")
        self.do_copy_deps(self.copy_deps)

    def do_copy_deps(self, copy):
        deps = set(self.deps_cpp_info.deps) - set(
            ("cygwin_installer", "msys2_installer", "ruby_installer"))
        for dep in deps:
            copy("*.dll",
                 dst="bin",
                 src="bin",
                 root_package=dep,
                 keep_path=False)
            copy("*.so*",
                 dst="lib",
                 src="lib",
                 root_package=dep,
                 keep_path=True)
            if self.settings.build_type == "Debug":
                copy("*.pdb", dst="bin", root_package=dep, keep_path=False)
