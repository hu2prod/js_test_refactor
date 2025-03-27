class Report;

void _Report_size_set_impl(
  std::string*& err,
  Report*      _this,
  u64          size
);
napi_value Report_size_set_sync(napi_env env, napi_callback_info info);
napi_value Report_size_set(napi_env env, napi_callback_info info);
