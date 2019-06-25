from conans import ConanFile, CMake

class Chess(ConanFile):
    requires = ("glm/0.9.9.1@g-truc/stable",
                "glew/2.1.0@bincrafters/stable",
                "glfw/3.2.1@bincrafters/stable",
                "libpng/1.6.36@bincrafters/stable",
                )
    generators = "cmake"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
