#pragma once
#include "class.hpp"

u32 Report_tag = 1;
void Report::free() {
  if (this->_deleted) return;
  this->_deleted = true;
}

void Report_destructor(napi_env env, void* native_object, void* /*finalize_hint*/) {
  Report* _this = static_cast<Report*>(native_object);
  _this->free();
  delete _this;
}

napi_value Report_constructor(napi_env env, napi_callback_info info) {
  napi_status status;
  
  napi_value _js_this;
  status = napi_get_cb_info(env, info, nullptr, nullptr, &_js_this, nullptr);
  if (status != napi_ok) {
    napi_throw_error(env, nullptr, "Unable to create class Report");
    return nullptr;
  }
  
  Report* _this = new Report();
  
  
  status = napi_wrap(env, _js_this, _this, Report_destructor, nullptr /* finalize_hint */, &_this->_wrapper);
  if (status != napi_ok) {
    napi_throw_error(env, nullptr, "Unable to napi_wrap for class Report");
    return nullptr;
  }
  
  return _js_this;
}
