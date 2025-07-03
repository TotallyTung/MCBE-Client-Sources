
class mce::ShaderProgram : public mce::ShaderProgramNull, protected mce::ResourceBase<mce::ShaderProgramNull> { /* Size=0x68 */
  /* 0x0000: fields for mce::ShaderProgramNull */
  /* 0x0068: fields for mce::ResourceBase<mce::ShaderProgramNull> */
  
  ShaderProgram(const mce::ShaderProgram&);
  ShaderProgram(const mce::ShaderType, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const Core::Path&);
  ~ShaderProgram();
  bool isValid() const;
  void* __vecDelDtor(uint32_t);
};
