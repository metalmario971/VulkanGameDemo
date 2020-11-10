#include "../base/Logger.h"
#include "../model/IboData.h"

namespace BR2 {
IboData::IboData(const string_t& mesh_name, std::shared_ptr<GLContext> ctx, size_t iElementSize) :
  GpuBufferData(mesh_name + "-ibo", ctx, GL_ELEMENT_ARRAY_BUFFER, iElementSize) {
  if (iElementSize != 4 && iElementSize != 2) {
    BRLogWarn("Index buffer element size invalid:" + iElementSize);
    Gu::debugBreak();
  }
  //set GL index format.
}
IboData::~IboData() {
}


}//ns game