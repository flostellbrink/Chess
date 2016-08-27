/*
 * Copyright (C) 2013, 2014
 * Computer Graphics Group, University of Siegen
 * Written by Martin Lambers <martin.lambers@uni-siegen.de>
 * All rights reserved.
 */

#include <cstring>
#include <cstdio>

#include <GL/glew.h>

#include "gltool.hpp"

namespace CG {
  std::string replace(const std::string &str, const std::string &s, const std::string &r)
  {
      std::string ts(str);
      size_t s_len = s.length();
      size_t r_len = r.length();
      size_t p = 0;

      while ((p = ts.find(s, p)) != std::string::npos) {
	  ts.replace(p, s_len, r);
	  p += r_len;
      }
      return ts;
  }

  static void kill_crlf(char *str)
  {
      size_t l = std::strlen(str);
      if (l > 0 && str[l - 1] == '\n')
	  str[--l] = '\0';
      if (l > 0 && str[l - 1] == '\r')
	  str[l - 1] = '\0';
  }

  GLuint createCompileShader(GLenum type, const std::string& src, const std::string& name)
  {
      GLuint shader = glCreateShader(type);
      const GLchar *glsrc = src.c_str();
      glShaderSource(shader, 1, &glsrc, NULL);
      glCompileShader(shader);

      std::string log;
      GLint e, l;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &e);
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &l);
      if (l > 0) {
	  char *tmplog = new char[l];
	  glGetShaderInfoLog(shader, l, NULL, tmplog);
	  kill_crlf(tmplog);
	  log = std::string(tmplog);
	  delete[] tmplog;
      } else {
	  log = std::string("");
      }

      if (e && log.length() > 0) {
	  fprintf(stderr, "OpenGL %s '%s': compiler warning:\n",
		  type == GL_VERTEX_SHADER ? "vertex shader"
		  : type == GL_GEOMETRY_SHADER ? "geometry shader"
		  : "fragment shader",
		  name.c_str());
	  fprintf(stderr, "%s\n", log.c_str());
      } else if (e != GL_TRUE) {
	  fprintf(stderr, "OpenGL %s '%s': compiler error:\n",
		  type == GL_VERTEX_SHADER ? "vertex shader"
		  : type == GL_GEOMETRY_SHADER ? "geometry shader"
		  : "fragment shader",
		  name.c_str());
	  fprintf(stderr, "%s\n", log.c_str());
	  shader = 0;
      }
      return shader;
  }

  GLuint linkProgram(GLuint prg, const std::string& name)
  {
      glLinkProgram(prg);

      std::string log;
      GLint e, l;
      glGetProgramiv(prg, GL_LINK_STATUS, &e);
      glGetProgramiv(prg, GL_INFO_LOG_LENGTH, &l);
      if (l > 0) {
	  char *tmplog = new char[l];
	  glGetProgramInfoLog(prg, l, NULL, tmplog);
	  kill_crlf(tmplog);
	  log = std::string(tmplog);
	  delete[] tmplog;
      } else {
	  log = std::string("");
      }

      if (e && log.length() > 0) {
	  fprintf(stderr, "OpenGL program '%s': linker warning:\n", name.c_str());
	  fprintf(stderr, "%s\n", log.c_str());
      } else if (e != GL_TRUE) {
	  fprintf(stderr, "OpenGL program '%s': linker error:\n", name.c_str());
	  fprintf(stderr, "%s\n", log.c_str());
	  prg = 0;
      }
      return prg;
  }

  bool checkFbo()
  {
      GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      if (status != GL_FRAMEBUFFER_COMPLETE) {
	  std::fprintf(stderr, "OpenGL FBO error 0x%04X\n", status);
	  return false;
      }
      return true;
  }

  bool checkError()
  {
      GLenum e = glGetError();
      if (e != GL_NO_ERROR) {
	  std::fprintf(stderr, "OpenGL error 0x%04X\n", e);
	  return false;
      }
      return true;
  }
}
