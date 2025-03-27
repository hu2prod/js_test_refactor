class Report;

void _Report_get_population_impl(
  std::string*& err,
  Report*      _this,
  u64          idx,
  i64&         ret
);
napi_value Report_get_population_sync(napi_env env, napi_callback_info info);
napi_value Report_get_population(napi_env env, napi_callback_info info);
