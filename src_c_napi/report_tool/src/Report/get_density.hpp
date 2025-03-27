class Report;

void _Report_get_density_impl(
  std::string*& err,
  Report*      _this,
  u64          idx,
  i64&         ret
);
napi_value Report_get_density_sync(napi_env env, napi_callback_info info);
napi_value Report_get_density(napi_env env, napi_callback_info info);
