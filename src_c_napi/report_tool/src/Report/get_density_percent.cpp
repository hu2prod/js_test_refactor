#include "../common.hpp"
class Report;
#include "class.hpp"

void _Report_get_density_percent_impl(
  std::string*& err,
  Report*      _this,
  u64          idx,
  i64&         ret
) {
  if (idx >= _this->line_list.size()) {
    err = new std::string("OOB get");
    return;
  }
  auto line = &(_this->line_list[idx]);
  ret = line->density_percent;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//   sync
////////////////////////////////////////////////////////////////////////////////////////////////////
napi_value Report_get_density_percent_sync(napi_env env, napi_callback_info info) {
  FN_ARG_HEAD(1)
  FN_ARG_THIS(Report)
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  FN_ARG_U64(idx)
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  std::string *err = nullptr;
  i64 ret;
  _Report_get_density_percent_impl(err, _this, idx, ret);
  if (err) {
    napi_throw_error(env, nullptr, err->c_str());
    delete err;
    return ret_dummy;
  }
  
  
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  FN_RET_I64(ret)
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//   async
////////////////////////////////////////////////////////////////////////////////////////////////////
struct Worker_ctx_Report_get_density_percent {
  Report* _this;
  u64 idx;
  i64 ret;
  
  std::string* err;
  napi_ref callback_reference;
  napi_async_work work;
};

void _worker_ctx_Report_get_density_percent_clear(napi_env env, struct Worker_ctx_Report_get_density_percent* worker_ctx) {
  if (worker_ctx->err) {
    delete worker_ctx->err;
    worker_ctx->err = nullptr;
  }
  napi_status status;
  
  
  status = napi_delete_async_work(env, worker_ctx->work);
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_delete_async_work fail");
  }
  delete worker_ctx;
}

void _execute_Report_get_density_percent(napi_env env, void* _data) {
  struct Worker_ctx_Report_get_density_percent* worker_ctx = (struct Worker_ctx_Report_get_density_percent*)_data;
  _Report_get_density_percent_impl(worker_ctx->err, worker_ctx->_this, worker_ctx->idx, worker_ctx->ret);
}

void _complete_Report_get_density_percent(napi_env env, napi_status execute_status, void* _data) {
  napi_status status;
  struct Worker_ctx_Report_get_density_percent* worker_ctx = (struct Worker_ctx_Report_get_density_percent*)_data;
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  //    prepare for callback (common parts)
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  napi_value callback;
  status = napi_get_reference_value(env, worker_ctx->callback_reference, &callback);
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "Unable to get referenced callback (napi_get_reference_value)");
    _worker_ctx_Report_get_density_percent_clear(env, worker_ctx);
    return;
  }
  status = napi_delete_reference(env, worker_ctx->callback_reference);
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "Unable to delete reference callback_reference");
    _worker_ctx_Report_get_density_percent_clear(env, worker_ctx);
    return;
  }
  
  napi_value global;
  status = napi_get_global(env, &global);
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "Unable to create return value global (napi_get_global)");
    _worker_ctx_Report_get_density_percent_clear(env, worker_ctx);
    return;
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  if (execute_status != napi_ok) {
    // avoid code duplication
    if (!worker_ctx->err) {
      worker_ctx->err = new std::string("execute_status != napi_ok");
    }
  }
  
  if (worker_ctx->err) {
    napi_helper_error_cb(env, worker_ctx->err->c_str(), callback);
    _worker_ctx_Report_get_density_percent_clear(env, worker_ctx);
    return;
  }
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  //    callback OK
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  napi_value result;
  napi_value call_argv[2];
  
  status = napi_get_undefined(env, &call_argv[0]);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_get_undefined FAIL");
    _worker_ctx_Report_get_density_percent_clear(env, worker_ctx);
    return;
  }
  status = napi_create_bigint_int64(env, worker_ctx->ret, &call_argv[1]);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_create_bigint_int64 FAIL");
    _worker_ctx_Report_get_density_percent_clear(env, worker_ctx);
    return;
  }
  
  status = napi_call_function(env, global, callback, 2, call_argv, &result);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_call_function FAIL");
    _worker_ctx_Report_get_density_percent_clear(env, worker_ctx);
    return;
  }
  _worker_ctx_Report_get_density_percent_clear(env, worker_ctx);
}

napi_value Report_get_density_percent(napi_env env, napi_callback_info info) {
  FN_ARG_HEAD(2)
  FN_ARG_THIS(Report)
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  FN_ARG_U64(idx)
  napi_value callback = argv[arg_idx];
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  Worker_ctx_Report_get_density_percent* worker_ctx = new Worker_ctx_Report_get_density_percent;
  worker_ctx->err = nullptr;
  
  status = napi_create_reference(env, callback, 1, &worker_ctx->callback_reference);
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_helper_error_cb(env, "Unable to create reference for callback. napi_create_reference", callback);
    delete worker_ctx;
    return ret_dummy;
  }
  
  // NOTE no free utf8 string
  napi_value async_resource_name;
  status = napi_create_string_utf8(env, "dummy", 5, &async_resource_name);
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "Unable to create value async_resource_name set to 'dummy'");
    delete worker_ctx;
    return ret_dummy;
  }
  
  worker_ctx->_this = _this;
  worker_ctx->idx = idx;
  
  status = napi_create_async_work(
    env,
    nullptr,
    async_resource_name,
    _execute_Report_get_density_percent,
    _complete_Report_get_density_percent,
    (void*)worker_ctx,
    &worker_ctx->work
  );
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_create_async_work fail");
    _worker_ctx_Report_get_density_percent_clear(env, worker_ctx);
    return ret_dummy;
  }
  
  status = napi_queue_async_work(env, worker_ctx->work);
  if (status != napi_ok) {
    napi_throw_error(env, nullptr, "napi_queue_async_work fail");
    _worker_ctx_Report_get_density_percent_clear(env, worker_ctx);
    return ret_dummy;
  }
  
  
  return ret_dummy;
}

