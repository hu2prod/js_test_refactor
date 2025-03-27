class Report;

void _Report_csv_read_impl(
  std::string*& err,
  Report*      _this,
  char*        path,
  size_t       path_len
);
napi_value Report_csv_read_sync(napi_env env, napi_callback_info info);
napi_value Report_csv_read(napi_env env, napi_callback_info info);
