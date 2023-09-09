//
// License: MIT
//

#include "glpp/shader_program.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <fstream>

namespace glpp {
bool Shader::Compile(const std::vector<std::string> &source_codes) {
  std::vector<const GLchar *> codes_string(source_codes.size(), nullptr);
  std::vector<GLint> codes_length(source_codes.size(), 0);
  for (int i = 0; i < source_codes.size(); ++i) {
    codes_string[i] = source_codes[i].c_str();
    codes_length[i] = source_codes[i].size();
  }
  glShaderSource(handle_, source_codes.size(), &codes_string[0], &codes_length[0]);
  glCompileShader(handle_);

  int success;
  char infoLog[512];
  glGetShaderiv(handle_, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(handle_, 512, NULL, infoLog);
    printf("SHADER COMPILE FAILED\n");
  }
  return success;
}

static GLenum GlShaderType(const ShaderType &shader_type) {
  GLenum gl_shader_type;
  switch (shader_type) {
  case ShaderType::VertexShader:
    gl_shader_type = GL_VERTEX_SHADER;
    break;
  case ShaderType::FragmentShader:
    gl_shader_type = GL_FRAGMENT_SHADER;
    break;
  case ShaderType::GeometryShader:
    gl_shader_type = GL_GEOMETRY_SHADER;
    break;
  case ShaderType::TessControlShader:
    gl_shader_type = GL_TESS_CONTROL_SHADER;
    break;
  case ShaderType::TessEvaluationShader:
    gl_shader_type = GL_TESS_EVALUATION_SHADER;
    break;
  case ShaderType::ComputeShader:
    gl_shader_type = GL_COMPUTE_SHADER;
    break;
  default:
    throw std::runtime_error("Unknown Shader Type");
  }
  return gl_shader_type;
}

Shader::Shader(ShaderType shader_type) { handle_ = glCreateShader(GlShaderType(shader_type)); }

void ShaderProgram::Compile(const std::vector<std::string> &all_vtx_code,
                            const std::vector<std::string> &all_frag_code) {
  Active();
  Shader vtx(ShaderType::VertexShader);
  vtx.Compile(all_vtx_code);
  glAttachShader(handle_, vtx.GetHandle());
  Shader frag(ShaderType::FragmentShader);
  frag.Compile(all_frag_code);
  glAttachShader(handle_, frag.GetHandle());
  glLinkProgram(handle_);
  int success;
  char info_log[512];
  glGetProgramiv(handle_, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(handle_, 512, NULL, info_log);
    printf("Program Link Failed\n");
  }
}
void ShaderProgram::CompileFromFile(const std::vector<std::string> &all_vtx_filepath,
                                    const std::vector<std::string> &all_frag_filepath) {

  std::vector<std::string> vtx_strings;
  std::vector<std::string> frag_strings;
  for (auto &vtx_filepath : all_vtx_filepath) {
    std::ifstream vtx_file(vtx_filepath);
    std::string vtx_string((std::istreambuf_iterator<char>(vtx_file)),
                           (std::istreambuf_iterator<char>()));
    vtx_strings.push_back(std::move(vtx_string));
  }

  for (auto &frag_filepath : all_frag_filepath) {
    std::ifstream frag_file(frag_filepath);
    std::string frag_string((std::istreambuf_iterator<char>(frag_file)),
                            (std::istreambuf_iterator<char>()));
    frag_strings.push_back(std::move(frag_string));
  }

  Compile(vtx_strings, frag_strings);
}

template <>
void ShaderProgram::SetUniform<glm::mat4>(const std::string &var_name_in_shader,
                                          const glm::mat4 &host_data) {

  Active();
  glUniformMatrix4fv(glGetUniformLocation(handle_, var_name_in_shader.c_str()), 1, GL_FALSE,
                     glm::value_ptr(host_data));
}

} // namespace glpp
