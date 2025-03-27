class Report;

void _Report_augment_with_density_percent_impl(
  std::string*& err,
  Report*      _this,
  i64          max_d
);
napi_value Report_augment_with_density_percent_sync(napi_env env, napi_callback_info info);
napi_value Report_augment_with_density_percent(napi_env env, napi_callback_info info);
