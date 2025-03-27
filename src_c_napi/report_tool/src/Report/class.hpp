#pragma once
#include "../common.hpp"



extern u32 Report_tag;
class Report {
  public:
  u32   _class_tag = Report_tag;
  bool  _deleted = false;
  std::vector<Report_line> line_list;
  
  napi_ref _wrapper;
  void free();
};
void Report_destructor(napi_env env, void* native_object, void* /*finalize_hint*/);

napi_value Report_constructor(napi_env env, napi_callback_info info);
