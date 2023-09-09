//
// License: MIT
//

#ifndef GLPP_INCLUDE_GLPP_SHADER_PROGRAM_H
#define GLPP_INCLUDE_GLPP_SHADER_PROGRAM_H

#include <string>
#include <vector>

#include <glpp/common.h>
#include <glpp/ui/ui_window.h>

namespace glpp {

enum class ShaderType {
  VertexShader,
  FragmentShader,
  GeometryShader,
  TessControlShader,
  TessEvaluationShader,
  ComputeShader
};
/**
 * RAII Wrapper of OpenGL shader
 */
class Shader : public UnCopyable {
public:
  ~Shader() { glDeleteShader(handle_); }

  bool Compile(const std::vector<std::string> &source_codes);

  GLuint GetHandle() { return handle_; }

private:
  friend class ShaderProgram;
  Shader(ShaderType shader_type);
  GLuint handle_;
};

class ShaderProgram : public UnCopyable {
public:
  ShaderProgram() { handle_ = glCreateProgram(); }

  ~ShaderProgram() { glDeleteProgram(handle_); }

  void Active() const { glUseProgram(handle_); }

  void Compile(const std::vector<std::string> &all_vtx_code,
               const std::vector<std::string> &all_frag_code);

  void CompileFromFile(const std::vector<std::string> &all_vtx_filepath,
                       const std::vector<std::string> &all_frag_filepath);

  template <class T> void SetUniform(const std::string &var_name_in_shader, const T &host_data);

  GLuint GetHandle() { return handle_; }

private:
  GLuint handle_;
};
} // namespace glpp
#endif // GLPP_INCLUDE_GLPP_SHADER_PROGRAM_H
