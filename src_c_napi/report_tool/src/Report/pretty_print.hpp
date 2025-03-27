class Report;

void _Report_pretty_print_impl(
  std::string*& err,
  Report*      _this
);
napi_value Report_pretty_print_sync(napi_env env, napi_callback_info info);
napi_value Report_pretty_print(napi_env env, napi_callback_info info);
