class Report;

void _Report_max_d_impl(
  std::string*& err,
  Report*      _this,
  i64&         ret
);
napi_value Report_max_d_sync(napi_env env, napi_callback_info info);
napi_value Report_max_d(napi_env env, napi_callback_info info);
