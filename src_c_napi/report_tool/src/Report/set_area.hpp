class Report;

void _Report_set_area_impl(
  std::string*& err,
  Report*      _this,
  u64          idx,
  i64          val
);
napi_value Report_set_area_sync(napi_env env, napi_callback_info info);
napi_value Report_set_area(napi_env env, napi_callback_info info);
