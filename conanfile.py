from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout, CMakeToolchain, CMakeDeps


class CppUtilRecipe(ConanFile):
    name = "cpputils"
    version = "0.5.0"
    package_type = "library"
    license = "MIT"
    url = "https://github.com/quinnwencn/cpputils"
    author = "Quinn"
    description = "cpputils is a cpp library that provides basic functions."
    
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": True, "fPIC": True}
    generator = "CMakeDeps", "CMakeToolchain"

    exports_sources = "CMakeLists.txt", "include/*", "src/*"

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def requirements(self):
        self.requires("gtest/1.13.0")
        self.requires("fmt/10.1.0")

    def build_requirements(self):
        self.build_requires("cmake/3.26.3")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()
    
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["cpputils"]
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.set_property("cmake_target_name", "cpputils::cpputils")
