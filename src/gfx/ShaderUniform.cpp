#include "../base/Logger.h"
#include "../base/Gu.h"
#include "../base/GLContext.h"
#include "../base/FileSystem.h"
#include "../base/Allocator.h"
#include "../base/Hash.h"
#include "../gfx/ShaderUniform.h"
#include "../gfx/ShaderMaker.h"
#include "../gfx/RenderUtils.h"
#include "../gfx/OpenGLUtils.h"

namespace BR2 {
ShaderUniform::ShaderUniform(std::shared_ptr<GLContext> ct, GLenum eType, GLint location, string_t name, size_t iArraySize) : _pContext(ct) {
  _glType = eType;
  _glLocation = location;
  _strName = name;  //this gets parsed and changed
  _iArraySize = iArraySize;
  _iNameHashed = STRHASH(name);

  _systemType = openglTypeToSystemType(_glType);
  _iComponentSize = getByteSizeForType(_systemType);
  _iComponentCount = getCountForType(_systemType);

  //Storage
  //_pStorage = new Allocator<char>(_iComponentSize * _iArraySize);
  _pStorage = std::vector<char>(_iComponentSize * _iArraySize);

  validate();
}
ShaderUniform::~ShaderUniform() {
  _pStorage.clear();
  //DEL_MEM(_pStorage);
}
void ShaderUniform::copyUniformData(void* pData, GLint count) {
  //*Who care.
  //#ifdef _DEBUG
  //    if(_bHasBeenSet == true) {
  //        BroLogWarn(getName(), "  Performance: Uniform was already set this frame.  Check Stack Info for previous set locations.");
  //    }
  //    _dbgSetStackInfos.push_back(StackInfo::getStackInfo2());
  //#endif
  if (count == -1) {
    static bool bAlreadyLogged = false;
    if (_iArraySize > 1 && bAlreadyLogged == false) {
      BRLogWarn(getName() +
                " NOTE: There are " + _iArraySize + " elements in the uniform array '" + getName() +
                "', and you did not explicitly set the count. Setting count to 1");
      bAlreadyLogged = true;
      Gu::debugBreak();
    }

    count = 1;  //prevent buffer overruns.
  }
  _iLastCount = count;

  //AssertOrThrow2(_pStorage != nullptr);
  size_t iCopySize = _iComponentSize * count;
  AssertOrThrow2(_pStorage.size() >= iCopySize);
  std::memcpy(_pStorage.data(), (const char*)pData, iCopySize);
  //_pStorage->copyFrom((const char*)pData, iCopySize, 0, 0);

  _bHasBeenSet = true;
}
OpenGLShaderVarType::e ShaderUniform::getSystemType() {
  return _systemType;
}
void ShaderUniform::validate() {
  validateSystemTypeEqualsOpenGlType(_systemType, _glType);
}

string_t ShaderUniform::debugGetUniformValueAsString(bool raw) {
  #define DDUMP(typ) ((std::string((const char*)((typ*)_pStorage.data()), _pStorage.size())) )

  switch (_systemType) {
  case OpenGLShaderVarType::e::GpuInt1:   return (raw) ? (DDUMP(int32_t*))  : (OpenGLUtils::stringifyUniform< int32_t>(_pStorage,     1, -1))   ; break;
  case OpenGLShaderVarType::e::GpuInt2:   return (raw) ? (DDUMP(ivec2*))    : (OpenGLUtils::stringifyUniform< int32_t>(_pStorage,     2, -1))   ; break;
  case OpenGLShaderVarType::e::GpuInt3:   return (raw) ? (DDUMP(ivec3*))    : (OpenGLUtils::stringifyUniform< int32_t>(_pStorage,     3, -1))   ; break;
  case OpenGLShaderVarType::e::GpuInt4:   return (raw) ? (DDUMP(ivec4*))    : (OpenGLUtils::stringifyUniform<uint32_t>(_pStorage,     4, -1))   ; break;
  case OpenGLShaderVarType::e::GpuUint1:  return (raw) ? (DDUMP(uint32_t*)) : (OpenGLUtils::stringifyUniform<uint32_t>(_pStorage,     1, -1))   ; break;
  case OpenGLShaderVarType::e::GpuUint2:  return (raw) ? (DDUMP(uvec2*))    : (OpenGLUtils::stringifyUniform<uint32_t>(_pStorage,     2, -1))   ; break;
  case OpenGLShaderVarType::e::GpuUint3:  return (raw) ? (DDUMP(uvec3*))    : (OpenGLUtils::stringifyUniform<uint32_t>(_pStorage,     3, -1))   ; break;
  case OpenGLShaderVarType::e::GpuUint4:  return (raw) ? (DDUMP(uvec4*))    : (OpenGLUtils::stringifyUniform<uint32_t>(_pStorage,     4, -1))   ; break;
  case OpenGLShaderVarType::e::GpuFloat1: return (raw) ? (DDUMP(float*))    : (OpenGLUtils::stringifyUniform<   float>(_pStorage,     1, -1))   ; break;
  case OpenGLShaderVarType::e::GpuFloat2: return (raw) ? (DDUMP(vec2*))     : (OpenGLUtils::stringifyUniform<   float>(_pStorage,     2, -1))   ; break;
  case OpenGLShaderVarType::e::GpuFloat3: return (raw) ? (DDUMP(vec3*))     : (OpenGLUtils::stringifyUniform<   float>(_pStorage,     3, -1))   ; break;
  case OpenGLShaderVarType::e::GpuFloat4: return (raw) ? (DDUMP(vec4*))     : (OpenGLUtils::stringifyUniform<   float>(_pStorage,     4, -1))   ; break;
  case OpenGLShaderVarType::e::GpuMat3:   return (raw) ? (DDUMP(mat3*))     : (OpenGLUtils::stringifyUniform<   float>(_pStorage, 9 * 4,  3))   ; break;
  case OpenGLShaderVarType::e::GpuMat4:   return (raw) ? (DDUMP(mat4*))     : (OpenGLUtils::stringifyUniform<   float>(_pStorage, 6 * 4,  4))   ; break;
  default:
    return Stz "Type string conversion " + (int)_systemType + " not supported.";
  }
}
/*

(std::string((const char*)((int32_t*)_pStorage.data()), _pStorage.size())) 
(std::string((const char*)((ivec2*)_pStorage.data()), _pStorage.size())    
(std::string((const char*)((ivec3*)_pStorage.data()), _pStorage.size())    
(std::string((const char*)((ivec4*)_pStorage.data()), _pStorage.size())    
(std::string((const char*)((uint32_t*)_pStorage.data()), _pStorage.size()) 
(std::string((const char*)((uvec2*)_pStorage.data()), _pStorage.size())    
(std::string((const char*)((uvec3*)_pStorage.data()), _pStorage.size())    
(std::string((const char*)((uvec4*)_pStorage.data()), _pStorage.size())    
(std::string((const char*)((float*)_pStorage.data()), _pStorage.size())    
(std::string((const char*)((vec2*)_pStorage.data()), _pStorage.size())     
(std::string((const char*)((vec3*)_pStorage.data()), _pStorage.size())     
(std::string((const char*)((vec4*)_pStorage.data()), _pStorage.size())     
(std::string((const char*)((mat3*)_pStorage.data()), _pStorage.size())     
(std::string((const char*)((mat4*)_pStorage.data()), _pStorage.size())     

*/
size_t ShaderUniform::getByteSizeForType(OpenGLShaderVarType::e etype) {
  switch (etype) {
    case OpenGLShaderVarType::e::GpuInt1:
      return sizeof(int32_t);
      break;
    case OpenGLShaderVarType::e::GpuInt2:
      return sizeof(ivec2);
      break;
    case OpenGLShaderVarType::e::GpuInt3:
      return sizeof(ivec3);
      break;
    case OpenGLShaderVarType::e::GpuInt4:
      return sizeof(ivec4);
      break;
    case OpenGLShaderVarType::e::GpuUint1:
      return sizeof(uint32_t);
      break;
    case OpenGLShaderVarType::e::GpuUint2:
      return sizeof(uvec2);
      break;
    case OpenGLShaderVarType::e::GpuUint3:
      return sizeof(uvec3);
      break;
    case OpenGLShaderVarType::e::GpuUint4:
      return sizeof(uvec4);
      break;
    case OpenGLShaderVarType::e::GpuFloat1:
      return sizeof(float);
      break;
    case OpenGLShaderVarType::e::GpuFloat2:
      return sizeof(vec2);
      break;
    case OpenGLShaderVarType::e::GpuFloat3:
      return sizeof(vec3);
      break;
    case OpenGLShaderVarType::e::GpuFloat4:
      return sizeof(vec4);
      break;
      // case OpenGLShaderVarType::e::GpuMat2:   return sizeof(mat2) * 1; break;
    case OpenGLShaderVarType::e::GpuMat3:
      return sizeof(mat3);
      break;
    case OpenGLShaderVarType::e::GpuMat4:
      return sizeof(mat4);
      break;
    default:
      BRThrowNotImplementedException();
  }
}
size_t ShaderUniform::getCountForType(OpenGLShaderVarType::e etype) {
  switch (etype) {
    case OpenGLShaderVarType::e::GpuInt1:
      return 1;
      break;
    case OpenGLShaderVarType::e::GpuInt2:
      return 2;
      break;
    case OpenGLShaderVarType::e::GpuInt3:
      return 3;
      break;
    case OpenGLShaderVarType::e::GpuInt4:
      return 4;
      break;
    case OpenGLShaderVarType::e::GpuUint1:
      return 1;
      break;
    case OpenGLShaderVarType::e::GpuUint2:
      return 2;
      break;
    case OpenGLShaderVarType::e::GpuUint3:
      return 3;
      break;
    case OpenGLShaderVarType::e::GpuUint4:
      return 4;
      break;
    case OpenGLShaderVarType::e::GpuFloat1:
      return 1;
      break;
    case OpenGLShaderVarType::e::GpuFloat2:
      return 2;
      break;
    case OpenGLShaderVarType::e::GpuFloat3:
      return 3;
      break;
    case OpenGLShaderVarType::e::GpuFloat4:
      return 4;
      break;
      // case OpenGLShaderVarType::e::GpuMat2:   return sizeof(mat2) * 1; break;
    case OpenGLShaderVarType::e::GpuMat3:
      return 1;
      break;
    case OpenGLShaderVarType::e::GpuMat4:
      return 1;
      break;
    default:
      BRThrowNotImplementedException();
  }
}
OpenGLShaderVarType::e ShaderUniform::openglTypeToSystemType(GLenum eType) {
  switch (eType) {
    case GL_UNSIGNED_INT:
      return OpenGLShaderVarType::GpuUint1;
      break;
    case GL_UNSIGNED_INT_VEC2:
      return OpenGLShaderVarType::GpuUint2;
      break;
    case GL_UNSIGNED_INT_VEC3:
      return OpenGLShaderVarType::GpuUint3;
      break;
    case GL_UNSIGNED_INT_VEC4:
      return OpenGLShaderVarType::GpuUint4;
      break;

    case GL_FLOAT:
      return OpenGLShaderVarType::GpuFloat1;
      break;
    case GL_FLOAT_VEC2:
      return OpenGLShaderVarType::GpuFloat2;
      break;
    case GL_FLOAT_VEC3:
      return OpenGLShaderVarType::GpuFloat3;
      break;
    case GL_FLOAT_VEC4:
      return OpenGLShaderVarType::GpuFloat4;
      break;

    case GL_INT:
      return OpenGLShaderVarType::GpuInt1;
      break;
    case GL_INT_VEC2:
      return OpenGLShaderVarType::GpuInt2;
      break;
    case GL_INT_VEC3:
      return OpenGLShaderVarType::GpuInt3;
      break;
    case GL_INT_VEC4:
      return OpenGLShaderVarType::GpuInt4;
      break;

    case GL_BOOL:
      return OpenGLShaderVarType::GpuBool1;
      break;
    case GL_BOOL_VEC2:
      return OpenGLShaderVarType::GpuBool2;
      break;
    case GL_BOOL_VEC3:
      return OpenGLShaderVarType::GpuBool3;
      break;
    case GL_BOOL_VEC4:
      return OpenGLShaderVarType::GpuBool4;
      break;

    case GL_FLOAT_MAT2:
      return OpenGLShaderVarType::GpuMat2;
      break;
    case GL_FLOAT_MAT3:
      return OpenGLShaderVarType::GpuMat3;
      break;
    case GL_FLOAT_MAT4:
      return OpenGLShaderVarType::GpuMat4;
      break;

      //Per the spec:
      //glUniform1i and glUniform1iv are the only two functions that may be used to load uniform variables
      //defined as sampler types. Loading samplers with any other function will result in a GL_INVALID_OPERATION error.
    case GL_SAMPLER_1D:
      return OpenGLShaderVarType::GpuInt1;
      break;
    case GL_SAMPLER_2D:
      return OpenGLShaderVarType::GpuInt1;
      break;
    case GL_SAMPLER_3D:
      return OpenGLShaderVarType::GpuInt1;
      break;
    case GL_SAMPLER_CUBE:
      return OpenGLShaderVarType::GpuInt1;
      break;
    case GL_SAMPLER_1D_SHADOW:
      return OpenGLShaderVarType::GpuInt1;
      break;
    case GL_SAMPLER_2D_SHADOW:
      return OpenGLShaderVarType::GpuInt1;
      break;
    case GL_SAMPLER_1D_ARRAY:
      return OpenGLShaderVarType::GpuInt1;
      break;
    case GL_SAMPLER_2D_ARRAY:
      return OpenGLShaderVarType::GpuInt1;
      break;
    case GL_SAMPLER_1D_ARRAY_SHADOW:
      return OpenGLShaderVarType::GpuInt1;
      break;
    case GL_SAMPLER_2D_ARRAY_SHADOW:
      return OpenGLShaderVarType::GpuInt1;
      break;
    case GL_SAMPLER_CUBE_SHADOW:
      return OpenGLShaderVarType::GpuInt1;
      break;
  }
  BRThrowException("OpenGL shader variable type not yet supported.");
}
void ShaderUniform::validateSystemTypeEqualsOpenGlType(OpenGLShaderVarType::e systemNameType, GLenum shaderDefinedUniformType) {
  //Samplers must be INT as per the GL spec.
  if (systemNameType == OpenGLShaderVarType::e::GpuInt1)
    AssertOrThrow2(
        (shaderDefinedUniformType == GL_INT) ||
        (shaderDefinedUniformType == GL_SAMPLER_2D) ||
        (shaderDefinedUniformType == GL_SAMPLER_3D) ||
        (shaderDefinedUniformType == GL_SAMPLER_CUBE) ||
        (shaderDefinedUniformType == GL_SAMPLER_2D_SHADOW) ||
        (shaderDefinedUniformType == GL_SAMPLER_CUBE_SHADOW));
  else if (systemNameType == OpenGLShaderVarType::e::GpuInt2)
    AssertOrThrow2(shaderDefinedUniformType == GL_INT_VEC2);
  else if (systemNameType == OpenGLShaderVarType::e::GpuInt3)
    AssertOrThrow2(shaderDefinedUniformType == GL_INT_VEC3);
  else if (systemNameType == OpenGLShaderVarType::e::GpuInt4)
    AssertOrThrow2(shaderDefinedUniformType == GL_INT_VEC4);
  else if (systemNameType == OpenGLShaderVarType::e::GpuUint1)
    AssertOrThrow2(shaderDefinedUniformType == GL_UNSIGNED_INT);
  else if (systemNameType == OpenGLShaderVarType::e::GpuUint2)
    AssertOrThrow2(shaderDefinedUniformType == GL_UNSIGNED_INT_VEC2);
  else if (systemNameType == OpenGLShaderVarType::e::GpuUint3)
    AssertOrThrow2(shaderDefinedUniformType == GL_UNSIGNED_INT_VEC3);
  else if (systemNameType == OpenGLShaderVarType::e::GpuUint4)
    AssertOrThrow2(shaderDefinedUniformType == GL_UNSIGNED_INT_VEC4);
  else if (systemNameType == OpenGLShaderVarType::e::GpuFloat1)
    AssertOrThrow2(shaderDefinedUniformType == GL_FLOAT);
  else if (systemNameType == OpenGLShaderVarType::e::GpuFloat2)
    AssertOrThrow2(shaderDefinedUniformType == GL_FLOAT_VEC2);
  else if (systemNameType == OpenGLShaderVarType::e::GpuFloat3)
    AssertOrThrow2(shaderDefinedUniformType == GL_FLOAT_VEC3);
  else if (systemNameType == OpenGLShaderVarType::e::GpuFloat4)
    AssertOrThrow2(shaderDefinedUniformType == GL_FLOAT_VEC4);
  else if (systemNameType == OpenGLShaderVarType::e::GpuMat3)
    AssertOrThrow2(shaderDefinedUniformType == GL_FLOAT_MAT3);
  else if (systemNameType == OpenGLShaderVarType::e::GpuMat4)
    AssertOrThrow2(shaderDefinedUniformType == GL_FLOAT_MAT4);
  else if (systemNameType == OpenGLShaderVarType::e::GpuDouble1)
    AssertOrThrow2(shaderDefinedUniformType == GL_DOUBLE);
  else if (systemNameType == OpenGLShaderVarType::e::GpuDouble2)
    AssertOrThrow2(shaderDefinedUniformType == GL_DOUBLE_VEC2);
  else if (systemNameType == OpenGLShaderVarType::e::GpuDouble3)
    AssertOrThrow2(shaderDefinedUniformType == GL_DOUBLE_VEC3);
  else if (systemNameType == OpenGLShaderVarType::e::GpuDouble4)
    AssertOrThrow2(shaderDefinedUniformType == GL_DOUBLE_VEC4);
  else
    BRThrowNotImplementedException();
}

void ShaderUniform::bindUniformFast() {
  //**Assume this is only called once per shader invocation.

  void* value = _pStorage.data();  //->ptr();
  GLint count = _iLastCount;

  //data must have value.
  //Not ven un ivorm arrays can be null..
  if (value == nullptr) {
    if (getIgnore() == false) {
      BRLogWarn("Shader Uniform variable " + getName() + " value was not set");
    }
    else {
      return;
    }
  }
  _pContext->chkErrDbg();

  OpenGLUtils::debugGetRenderState();

  switch (_systemType) {
    case OpenGLShaderVarType::e::GpuInt1:
      _pContext->glUniform1iv(_glLocation, count, (GLint*)value);
      break;
    case OpenGLShaderVarType::e::GpuInt2:
      _pContext->glUniform2iv(_glLocation, count, (GLint*)value);
      break;
    case OpenGLShaderVarType::e::GpuInt3:
      _pContext->glUniform3iv(_glLocation, count, (GLint*)value);
      break;
    case OpenGLShaderVarType::e::GpuInt4:
      _pContext->glUniform4iv(_glLocation, count, (GLint*)value);
      break;
    case OpenGLShaderVarType::e::GpuUint1:
      _pContext->glUniform1uiv(_glLocation, count, (GLuint*)value);
      break;
    case OpenGLShaderVarType::e::GpuUint2:
      _pContext->glUniform2uiv(_glLocation, count, (GLuint*)value);
      break;
    case OpenGLShaderVarType::e::GpuUint3:
      _pContext->glUniform3uiv(_glLocation, count, (GLuint*)value);
      break;
    case OpenGLShaderVarType::e::GpuUint4:
      _pContext->glUniform4uiv(_glLocation, count, (GLuint*)value);
      break;
    case OpenGLShaderVarType::e::GpuFloat1:
      _pContext->glUniform1fv(_glLocation, count, (GLfloat*)value);
      break;
    case OpenGLShaderVarType::e::GpuFloat2:
      _pContext->glUniform2fv(_glLocation, count, (GLfloat*)value);
      break;
    case OpenGLShaderVarType::e::GpuFloat3:
      _pContext->glUniform3fv(_glLocation, count, (GLfloat*)value);
      break;
    case OpenGLShaderVarType::e::GpuFloat4:
      _pContext->glUniform4fv(_glLocation, count, (GLfloat*)value);
      break;
    case OpenGLShaderVarType::e::GpuMat2:
      _pContext->glUniformMatrix2fv(_glLocation, count, GL_FALSE, (GLfloat*)value);
      break;
    case OpenGLShaderVarType::e::GpuMat3:
      _pContext->glUniformMatrix3fv(_glLocation, count, GL_FALSE, (GLfloat*)value);
      break;
    case OpenGLShaderVarType::e::GpuMat4:
      _pContext->glUniformMatrix4fv(_glLocation, count, GL_FALSE, (GLfloat*)value);
      break;

    default:
      BRLogError("Uniform type binding method has not been implemented for uniform " + getName() +
                 " of type " + ShaderMaker::systemTypeToSTring(_systemType) + " (" + _systemType + ") ");
      Gu::debugBreak();
      break;
  }
  _pContext->chkErrDbg();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

ShaderUniformBlock::ShaderUniformBlock(std::shared_ptr<GLContext> ct, string_t name, GLint iBlockIndex, GLint iBindingIndex, size_t iBufferByteSize) : _pContext(ct),
                                                                                                                                                       _iBlockIndex(iBlockIndex),
                                                                                                                                                       _iBindingIndex(iBindingIndex) {
  _strName = name;
  _iBufferSizeBytes = iBufferByteSize;

  AssertOrThrow2(_iBufferSizeBytes > 0);

  _pContext->glGenBuffers(1, (GLuint*)&_iUboId);
  _pContext->glBindBuffer(GL_UNIFORM_BUFFER, _iUboId);
  _pContext->glBufferData(GL_UNIFORM_BUFFER, _iBufferSizeBytes, 0, GL_DYNAMIC_DRAW);
  _pContext->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
ShaderUniformBlock::~ShaderUniformBlock() {
}
void ShaderUniformBlock::copyUniformData(void* pData, size_t copySizeBytes) {
  //Copy to the shader buffer
  AssertOrThrow2(copySizeBytes <= _iBufferSizeBytes);

  _pValue = pData;

  _pContext->glBindBuffer(GL_UNIFORM_BUFFER, _iUboId);
  //    void* pBuf =_pContext->glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
  //if(pBuf != nullptr) {
  //    memcpy(pBuf, pData, copySizeBytes);
  //    _pContext->glMapBuffer(GL_UNIFORM_BUFFER, 0);
  //}
  //else {
  //    BroLogError("Uniform buffer could not be mapped.");
  //}
  //_pContext->glBufferData(GL_UNIFORM_BUFFER, copySizeBytes, (void*)_pValue, GL_DYNAMIC_DRAW);
  _pContext->glBufferSubData(GL_UNIFORM_BUFFER, 0, copySizeBytes, pData);
  _pContext->glBindBuffer(GL_UNIFORM_BUFFER, 0);

  _pContext->chkErrDbg();
  _bHasBeenSet = true;
}
void ShaderUniformBlock::bindUniformFast() {
  if (_pValue == nullptr) {
    BRLogWarn("Shader Uniform Block '" + getName() + "' value was not set ");
  }
  _pContext->glBindBufferBase(GL_UNIFORM_BUFFER, _iBindingIndex, _iUboId);
  if(_iUboId==0){
    int x=0;
    x++;
  }

  _pContext->glBindBuffer(GL_UNIFORM_BUFFER, _iUboId);
}

}  // namespace BR2
