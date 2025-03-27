class Report;

void _Report_set_country_impl(
  std::string*& err,
  Report*      _this,
  u64          idx,
  char*        val,
  size_t       val_len
);
napi_value Report_set_country_sync(napi_env env, napi_callback_info info);
napi_value Report_set_country(napi_env env, napi_callback_info info);
