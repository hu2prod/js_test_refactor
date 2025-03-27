#include "common.hpp"
#include "macro.cpp"





class Report;

#include "Report/class.hpp"

#include "Report/size_get.hpp"
#include "Report/size_set.hpp"
#include "Report/get_city.hpp"
#include "Report/set_city.hpp"
#include "Report/get_population.hpp"
#include "Report/set_population.hpp"
#include "Report/get_area.hpp"
#include "Report/set_area.hpp"
#include "Report/get_density.hpp"
#include "Report/set_density.hpp"
#include "Report/get_country.hpp"
#include "Report/set_country.hpp"
#include "Report/get_density_percent.hpp"
#include "Report/set_density_percent.hpp"
#include "Report/csv_read.hpp"
#include "Report/pretty_print.hpp"
#include "Report/max_d.hpp"
#include "Report/augment_with_density_percent.hpp"
#include "Report/sort_by_density_percent_desc.hpp"





////////////////////////////////////////////////////////////////////////////////////////////////////
napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  CLASS_DEF(Report)
  CLASS_METHOD(Report, size_get_sync)
  CLASS_METHOD(Report, size_get)
  CLASS_METHOD(Report, size_set_sync)
  CLASS_METHOD(Report, size_set)
  CLASS_METHOD(Report, get_city_sync)
  CLASS_METHOD(Report, get_city)
  CLASS_METHOD(Report, set_city_sync)
  CLASS_METHOD(Report, set_city)
  CLASS_METHOD(Report, get_population_sync)
  CLASS_METHOD(Report, get_population)
  CLASS_METHOD(Report, set_population_sync)
  CLASS_METHOD(Report, set_population)
  CLASS_METHOD(Report, get_area_sync)
  CLASS_METHOD(Report, get_area)
  CLASS_METHOD(Report, set_area_sync)
  CLASS_METHOD(Report, set_area)
  CLASS_METHOD(Report, get_density_sync)
  CLASS_METHOD(Report, get_density)
  CLASS_METHOD(Report, set_density_sync)
  CLASS_METHOD(Report, set_density)
  CLASS_METHOD(Report, get_country_sync)
  CLASS_METHOD(Report, get_country)
  CLASS_METHOD(Report, set_country_sync)
  CLASS_METHOD(Report, set_country)
  CLASS_METHOD(Report, get_density_percent_sync)
  CLASS_METHOD(Report, get_density_percent)
  CLASS_METHOD(Report, set_density_percent_sync)
  CLASS_METHOD(Report, set_density_percent)
  CLASS_METHOD(Report, csv_read_sync)
  CLASS_METHOD(Report, csv_read)
  CLASS_METHOD(Report, pretty_print_sync)
  CLASS_METHOD(Report, pretty_print)
  CLASS_METHOD(Report, max_d_sync)
  CLASS_METHOD(Report, max_d)
  CLASS_METHOD(Report, augment_with_density_percent_sync)
  CLASS_METHOD(Report, augment_with_density_percent)
  CLASS_METHOD(Report, sort_by_density_percent_desc_sync)
  CLASS_METHOD(Report, sort_by_density_percent_desc)
  CLASS_EXPORT(Report)
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
