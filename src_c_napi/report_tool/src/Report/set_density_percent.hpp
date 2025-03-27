class Report;

void _Report_set_density_percent_impl(
  std::string*& err,
  Report*      _this,
  u64          idx,
  i64          val
);
napi_value Report_set_density_percent_sync(napi_env env, napi_callback_info info);
napi_value Report_set_density_percent(napi_env env, napi_callback_info info);
