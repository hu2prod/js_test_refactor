class Report;

void _Report_sort_by_density_percent_desc_impl(
  std::string*& err,
  Report*      _this
);
napi_value Report_sort_by_density_percent_desc_sync(napi_env env, napi_callback_info info);
napi_value Report_sort_by_density_percent_desc(napi_env env, napi_callback_info info);
