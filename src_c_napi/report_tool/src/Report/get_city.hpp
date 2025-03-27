class Report;

void _Report_get_city_impl(
  std::string*& err,
  Report*      _this,
  u64          idx,
  char*&       ret,
  size_t&      ret_len
);
napi_value Report_get_city_sync(napi_env env, napi_callback_info info);
napi_value Report_get_city(napi_env env, napi_callback_info info);
