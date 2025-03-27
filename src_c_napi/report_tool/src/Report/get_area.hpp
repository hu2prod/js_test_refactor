class Report;

void _Report_get_area_impl(
  std::string*& err,
  Report*      _this,
  u64          idx,
  i64&         ret
);
napi_value Report_get_area_sync(napi_env env, napi_callback_info info);
napi_value Report_get_area(napi_env env, napi_callback_info info);
