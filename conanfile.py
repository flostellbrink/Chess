from conans import ConanFile, CMake

class Chess(ConanFile):
    requires = ("glm/0.9.9.7@_/_",
                "glew/2.1.0@bincrafters/stable",
                "glfw/3.3.2@bincrafters/stable",
                # Defining zlib explicitly resolves dependency conflict
                "zlib/1.2.11@_/_",
                )
    generators = "cmake"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
