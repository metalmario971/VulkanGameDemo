/**
*  @file GpuBufferData.h
*  @date September 18, 2014
*  @author MetalMario971
*/
#pragma once
#pragma once
#ifndef __GPUBUFFERDATA_777819900224202740029606_H__
#define __GPUBUFFERDATA_777819900224202740029606_H__

#include "../model/ModelHeader.h"

namespace BR2 {
/**
*  @class GpuBufferData
*  @brief Generic data for the GPU buffer.
*/
class GpuBufferData : public VirtualMemory {
public:
  GpuBufferData(const string_t& mesh_name, std::shared_ptr<GLContext>, GLenum bufferType, size_t iElementSize);
  virtual ~GpuBufferData() override;

  void bindBuffer();
  void bindBuffer(GLenum e);

  bool isAllocated() { return _bIsAllocated; }
  uint32_t getGlId() { return _glId; }
  size_t getByteSize() { return _iElementSize * _iNumElements; } // Returns the size in bytes!.
  size_t getEleSize() { return _iElementSize; }    // Reutrns the number of fragments in this VBO.
  size_t getNumElements() { return _iNumElements; }

  void allocate(size_t iNumElements);

  void copyDataClientServer(std::shared_ptr<ByteBuffer> gb);    //buffer must be allocated first by calling alloc()  NOte: Returns NULL for SSBO buffers
  void copyDataServerClient(std::shared_ptr<ByteBuffer> gb);    // Same as calling readBytes(), UNLESS buffer is an SSBO
  void copyDataClientServer(size_t num_elements, const void* elements, int32_t elementSize = -1);    //buffer must be allocated first by calling alloc()o
  void copyDataServerClient(size_t num_elements, void* __out_ elements, int32_t elementSize = -1);    // Same as calling readBytes()

  void mapBuffer(GLenum e, void*& __out_ buf);
  void unmapBuffer();

  void unbindBuffer();
  void setGlBufferType(GLenum type) {
    _glBufferType = type;
  }
protected:
  uint32_t _glId = 0;
  GLenum _glBufferType;    // GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER

  size_t _iElementSize = 0; // The element size
  size_t _iNumElements = 0; // The number of elements we allocated

string_t _meshName = "";

  bool _isMapped = false;
  bool _isBound = false;
  bool _bIsAllocated = false;
  //GLRenderSystem* _pRenderSystem;

  //Mapping
  std::shared_ptr<GLContext> _pContext;

  void verifyValidBuffer();
  void readbytes(size_t num_elements, void* __out_ buf, int32_t elementSize = -1);
};



}//ns game



#endif
