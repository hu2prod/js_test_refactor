class Report;

void _Report_set_city_impl(
  std::string*& err,
  Report*      _this,
  u64          idx,
  char*        val,
  size_t       val_len
);
napi_value Report_set_city_sync(napi_env env, napi_callback_info info);
napi_value Report_set_city(napi_env env, napi_callback_info info);
