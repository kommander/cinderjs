#ifndef js_natives_h
#define js_natives_h
namespace cjs {

  const char cinder_native[] = { 47, 47, 32, 77, 97, 105, 110, 32, 74, 83, 32, 97, 112, 112, 108, 105, 99, 97, 116, 105, 111, 110, 32, 101, 110, 116, 114, 121, 32, 112, 111, 105, 110, 116, 10, 116, 111, 103, 103, 108, 101, 65, 112, 112, 67, 111, 110, 115, 111, 108, 101, 40, 41, 59, 10, 108, 111, 103, 40, 39, 72, 101, 108, 108, 111, 32, 67, 105, 110, 100, 101, 114, 39, 41, 59, 0 };

struct _native {
  const char* name;
  const char* source;
  size_t source_len;
};

static const struct _native natives[] = {

  { "cinder", cinder_native, sizeof(cinder_native)-1 },

  { NULL, NULL, 0 } /* sentinel */

};

}
#endif
