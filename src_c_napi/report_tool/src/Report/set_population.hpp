class Report;

void _Report_set_population_impl(
  std::string*& err,
  Report*      _this,
  u64          idx,
  i64          val
);
napi_value Report_set_population_sync(napi_env env, napi_callback_info info);
napi_value Report_set_population(napi_env env, napi_callback_info info);
