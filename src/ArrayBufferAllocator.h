//
//  ArrayBufferAllocator.h
//  cinderjs
//
//  Created by Sebastian Herrlinger on 26/08/15.
//
//

#ifndef cinderjs_ArrayBufferAllocator_h
#define cinderjs_ArrayBufferAllocator_h

namespace cjs {

class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
 public:
  // Impose an upper limit to avoid out of memory errors that bring down
  // the process.
  static const size_t kMaxLength = 0x3fffffff;
  static ArrayBufferAllocator the_singleton;
  virtual ~ArrayBufferAllocator() {}
  virtual void* Allocate(size_t length);
  virtual void* AllocateUninitialized(size_t length);
  virtual void Free(void* data, size_t length);
 private:
  ArrayBufferAllocator() {}
  ArrayBufferAllocator(const ArrayBufferAllocator&);
  void operator=(const ArrayBufferAllocator&);
};

ArrayBufferAllocator ArrayBufferAllocator::the_singleton;

void* ArrayBufferAllocator::Allocate(size_t length) {
  if (length > kMaxLength)
    return NULL;
  char* data = new char[length];
  memset(data, 0, length);
  return data;
}

void* ArrayBufferAllocator::AllocateUninitialized(size_t length) {
  if (length > kMaxLength)
    return NULL;
  return new char[length];
}

void ArrayBufferAllocator::Free(void* data, size_t length) {
  delete[] static_cast<char*>(data);
}

} // end namespace cjs

#endif
