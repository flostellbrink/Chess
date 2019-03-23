from conans import ConanFile, CMake

class Chess(ConanFile):
    requires = ("glm/0.9.9.1@g-truc/stable",

                # We are linking emscriptens version.
                # This one uses gl methods that do not exist.
                #"glew/2.1.0@bincrafters/stable",
                
                # We are linking emscriptens version.
                # This one does not compile.
                # "glfw/3.2.1@bincrafters/stable"

                # We are linking emscriptens version.
                # This one does not compile.
                # "libpng/1.6.36@bincrafters/stable",
                )
    generators = "cmake"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
