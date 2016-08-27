/*
 * Copyright (C) 2013, 2014
 * Computer Graphics Group, University of Siegen
 * Written by Martin Lambers <martin.lambers@uni-siegen.de>
 * All rights reserved.
 */

#ifndef GLTOOL_H
#define GLTOOL_H

#include <string>
#include <cstdio>
#include <cstdlib>

namespace CG {
  // Replace all occurences of 'original' in 'str' with 'replacement'.
  // Useful to fill in compile-time constants into shader source code.
  std::string replace(const std::string &str,
	  const std::string &original, const std::string &replacement);

  // Compile a shader and print warnings/errors if necessary.
  // The shader source is 'src', and its 'type' is GL_VERTEX_SHADER,
  // GL_FRAGMENT_SHADER or similar.
  // The shader 'name' is optional and only used in warning or error messages.
  // Returns a shader handle on success, and 0 on failure.
  GLuint createCompileShader(GLenum type, const std::string& src,
	  const std::string& name = std::string());

  // Link the shader program 'prg' and print warnings/errors if necessary.
  // The program 'name' is optional and only used in warning or error messages.
  // Returns 'prg' on success, or 0 on error.
  GLuint linkProgram(GLuint prg, const std::string& name = std::string());

  // Functions that check for errors and FBO status problems.
  // They return true if everything is ok, or false if there is a problem.
  bool checkError();
  bool checkFbo();
}

// A macro VERIFY(condition) for quick-and-dirty error checking: if the
// condition is not met, the program aborts immediately with an error message.
#define VERIFY(condition)  \
    if (!(condition)) \
    { \
        fprintf(stderr, "%s:%d: Verification '%s' failed.\n", \
                __FILE__, __LINE__, #condition); \
        exit(-1); \
    }

#endif
