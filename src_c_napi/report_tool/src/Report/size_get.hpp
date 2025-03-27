class Report;

void _Report_size_get_impl(
  std::string*& err,
  Report*      _this,
  u64&         ret_size
);
napi_value Report_size_get_sync(napi_env env, napi_callback_info info);
napi_value Report_size_get(napi_env env, napi_callback_info info);
