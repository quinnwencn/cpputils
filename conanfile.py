from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout, CMakeToolchain


class CppUtil(ConanFile):
    name = "cpputils"
    version = "0.1.0"
    description = "cpputils is a cpp library that provides basic functions."
    settings = "os", "compiler", "build_type", "arch"
    generator = "CMakeDeps"
    exports_sources = "CMakeLists.txt", "include/*", "src/*"

    def build_requirements(self):
        self.build_requires("cmake/3.26.3")

    def layout(self):
        cmake_layout(self)

    def generate(self):
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