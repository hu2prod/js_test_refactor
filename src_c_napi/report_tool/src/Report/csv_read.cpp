#include "../common.hpp"
class Report;
#include "class.hpp"

void _Report_csv_read_impl(
  std::string*& err,
  Report*      _this,
  char*        path,
  size_t       path_len
) {
  // Convert path (which may not be null-terminated) into a std::string.
  std::string filepath(path, path_len);
  std::ifstream infile(filepath);
  if (!infile.is_open()) {
    err = new std::string("Failed to open file: " + filepath);
    return;
  }
  
  // Read all lines into a vector.
  std::vector<std::string> line_list;
  std::string line;
  while (std::getline(infile, line)) {
    line_list.push_back(line);
  }
  infile.close();
  
  if (line_list.size() < 1) {
    err = new std::string("CSV file does not have enough lines");
    return;
  }
  
  // Remove the header (first line)
  line_list.erase(line_list.begin());
  
  // BUG: Always remove the last line (even if it contains valid data)
  if (!line_list.empty()) {
    line_list.pop_back();
  }
  
  // Parse each remaining CSV line.
  for (const auto &l : line_list) {
    std::istringstream ss(l);
    Report_line rline;
    std::string token;
    
    if (!std::getline(ss, token, ',')) {
      err = new std::string("unexpected EOL at line=");
      *err += std::to_string(_this->line_list.size()+2);
      return;
    }
    rline.city = token;
    if (!std::getline(ss, token, ',')) {
      err = new std::string("unexpected EOL at line=");
      *err += std::to_string(_this->line_list.size()+2);
      return;
    }
    {
      i64 value;
      auto result = std::from_chars(token.data(), token.data() + token.size(), value);
      if (result.ec != std::errc{}) {
        err = new std::string("bad population at line=");
        *err += std::to_string(_this->line_list.size()+2);
        return;
      }
      rline.population = value;
    }
    if (!std::getline(ss, token, ',')) {
      err = new std::string("unexpected EOL at line=");
      *err += std::to_string(_this->line_list.size()+2);
      return;
    }
    {
      i64 value;
      auto result = std::from_chars(token.data(), token.data() + token.size(), value);
      if (result.ec != std::errc{}) {
        err = new std::string("bad area at line=");
        *err += std::to_string(_this->line_list.size()+2);
        return;
      }
      rline.area = value;
    }
    if (!std::getline(ss, token, ',')) {
      err = new std::string("unexpected EOL at line=");
      *err += std::to_string(_this->line_list.size()+2);
      return;
    }
    {
      i64 value;
      auto result = std::from_chars(token.data(), token.data() + token.size(), value);
      if (result.ec != std::errc{}) {
        err = new std::string("bad density at line=");
        *err += std::to_string(_this->line_list.size()+2);
        return;
      }
      rline.density = value;
    }
    if (!std::getline(ss, token, ',')) {
      err = new std::string("unexpected EOL at line=");
      *err += std::to_string(_this->line_list.size()+2);
      return;
    }
    rline.country = token;
    
    // Append the parsed line to the report.
    _this->line_list.push_back(rline);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//   sync
////////////////////////////////////////////////////////////////////////////////////////////////////
napi_value Report_csv_read_sync(napi_env env, napi_callback_info info) {
  FN_ARG_HEAD(1)
  FN_ARG_THIS(Report)
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  FN_ARG_STR(path)
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  std::string *err = nullptr;
  _Report_csv_read_impl(err, _this, path, path_len);
  if (err) {
    napi_throw_error(env, nullptr, err->c_str());
    delete err;
    return ret_dummy;
  }
  
  free(path);
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  return ret_dummy;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//   async
////////////////////////////////////////////////////////////////////////////////////////////////////
struct Worker_ctx_Report_csv_read {
  Report* _this;
  char* path;
  size_t path_len;
  
  std::string* err;
  napi_ref callback_reference;
  napi_async_work work;
};

void _worker_ctx_Report_csv_read_clear(napi_env env, struct Worker_ctx_Report_csv_read* worker_ctx) {
  if (worker_ctx->err) {
    delete worker_ctx->err;
    worker_ctx->err = nullptr;
  }
  napi_status status;
  free(worker_ctx->path);
  
  status = napi_delete_async_work(env, worker_ctx->work);
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_delete_async_work fail");
  }
  delete worker_ctx;
}

void _execute_Report_csv_read(napi_env env, void* _data) {
  struct Worker_ctx_Report_csv_read* worker_ctx = (struct Worker_ctx_Report_csv_read*)_data;
  _Report_csv_read_impl(worker_ctx->err, worker_ctx->_this, worker_ctx->path, worker_ctx->path_len);
}

void _complete_Report_csv_read(napi_env env, napi_status execute_status, void* _data) {
  napi_status status;
  struct Worker_ctx_Report_csv_read* worker_ctx = (struct Worker_ctx_Report_csv_read*)_data;
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  //    prepare for callback (common parts)
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  napi_value callback;
  status = napi_get_reference_value(env, worker_ctx->callback_reference, &callback);
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "Unable to get referenced callback (napi_get_reference_value)");
    _worker_ctx_Report_csv_read_clear(env, worker_ctx);
    return;
  }
  status = napi_delete_reference(env, worker_ctx->callback_reference);
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "Unable to delete reference callback_reference");
    _worker_ctx_Report_csv_read_clear(env, worker_ctx);
    return;
  }
  
  napi_value global;
  status = napi_get_global(env, &global);
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "Unable to create return value global (napi_get_global)");
    _worker_ctx_Report_csv_read_clear(env, worker_ctx);
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
    _worker_ctx_Report_csv_read_clear(env, worker_ctx);
    return;
  }
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  //    callback OK
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  napi_value result;
  napi_value call_argv[2];
  
  status = napi_call_function(env, global, callback, 0, call_argv, &result);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_call_function FAIL");
    _worker_ctx_Report_csv_read_clear(env, worker_ctx);
    return;
  }
  _worker_ctx_Report_csv_read_clear(env, worker_ctx);
}

napi_value Report_csv_read(napi_env env, napi_callback_info info) {
  FN_ARG_HEAD(2)
  FN_ARG_THIS(Report)
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  FN_ARG_STR(path)
  napi_value callback = argv[arg_idx];
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  Worker_ctx_Report_csv_read* worker_ctx = new Worker_ctx_Report_csv_read;
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
  worker_ctx->path = path;
  worker_ctx->path_len = path_len;
  
  status = napi_create_async_work(
    env,
    nullptr,
    async_resource_name,
    _execute_Report_csv_read,
    _complete_Report_csv_read,
    (void*)worker_ctx,
    &worker_ctx->work
  );
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_create_async_work fail");
    _worker_ctx_Report_csv_read_clear(env, worker_ctx);
    return ret_dummy;
  }
  
  status = napi_queue_async_work(env, worker_ctx->work);
  if (status != napi_ok) {
    napi_throw_error(env, nullptr, "napi_queue_async_work fail");
    _worker_ctx_Report_csv_read_clear(env, worker_ctx);
    return ret_dummy;
  }
  
  
  return ret_dummy;
}

