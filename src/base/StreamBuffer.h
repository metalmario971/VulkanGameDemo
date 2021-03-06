/**
*  @file GrowBufferBase.h
*  @date July 9, 2016
*  @author MetalMario971
*/
#pragma once
#ifndef __GROWBUFFERBASE_14681206432405686591_H__
#define __GROWBUFFERBASE_14681206432405686591_H__

#include "../base/BaseHeader.h"
#include "../base/IOBase.h"

namespace BR2 {
/**
*  @class StreamBuffer
*  @brief A buffer that streams data in and out.  Grows/shrinks.  ADDS - To the end.  REMOVES - From the beginning.
*   This is to be used for networking.
*/
class StreamBuffer : public IOBase<char> {
public:
  StreamBuffer(int32_t chunkSize = 512);
  virtual ~StreamBuffer() override;

  bool getIsEmpty() { return _iAddCountBytes == 0; }
  void copyFrom(StreamBuffer* rhs);
  virtual RetCode write(const char* bytes, size_t len, int32_t offset = -1);
  virtual RetCode read(char* buf, size_t len, int64_t buflen = -1, int32_t offset = -1);
  virtual void clear();
  Allocator<char>* getData();
  void shiftOutFirstByte();
  void next(size_t allocCount);
  string_t toString();

protected:
  size_t getAddedByteCount() { return _iAddCountBytes; }

private:
  size_t _iAddCountBytes; // bytes in the buffer
  size_t _iChunkSizeBytes; // Amount to allocate when running out of room
  Allocator<char> _data;

  void checkToGrow();
  void checkToShrink();
  //void checkToGrowOrShrink();
};

}//ns Game



#endif
