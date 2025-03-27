project "js_test_refactor", ()->
  policy_set "package_manager", "snpm"
  npm_i "fy"
  
  napi_package "report_tool", ()->
    type_to_cpp_type_hash = {
      "str" : "std::string"
    }
    field_gen = (name, type, pad_size, pad_dir="r", is_calc = false)->
      {
        name
        type
        cpp_type : type_to_cpp_type_hash[type] ? type
        pad_size
        pad_dir
        is_calc
      }
    
    field_list = [
      field_gen "city",       "str", 18, "l"
      field_gen "population", "i64", 10
      field_gen "area",       "i64", 8
      field_gen "density",    "i64", 8
      field_gen "country",    "str", 18
      # calculated
      field_gen "density_percent", "i64", 6, "r", true
    ]
    
    
    jl = []
    for field in field_list
      jl.push "#{field.cpp_type} #{field.name};"
    
    # TODO use includes only for particular functions
    napi_include """
      #include <fstream>
      #include <sstream>
      #include <charconv>
      #include <iostream>
      #include <iomanip>
      #include <cmath>
      #include <algorithm>
      """
    napi_file_header_raw_pre "report.hpp", """
      class Report_line {
        public:
        #{join_list jl, '  '}
      };
      """
    
    # NOTE no Report_line public class, so we can change impl later with SoA if we want
    napi_class "Report", ()->
      # napi_class_kt "a", "u32"
      napi_class_field_raw """
        std::vector<Report_line> line_list;
        """
      
      
      # ###################################################################################################
      #    size
      # ###################################################################################################
      napi_fn "size_get", ()->
        napi_fn_arg "ret_size", "u64"
        napi_fn_raw_fixed_code """
          ret_size = _this->line_list.size();
          """
      
      napi_fn "size_set", ()->
        napi_fn_arg "size", "u64"
        napi_fn_raw_fixed_code """
          _this->line_list.resize(size);
          """
      
      # ###################################################################################################
      #    getter/setter
      # ###################################################################################################
      for field in field_list
        napi_fn "get_#{field.name}", ()->
          napi_fn_arg "idx", "u64"
          napi_fn_arg "ret", field.type
          
          set_code = """
            ret = line->#{field.name};
            """
          if field.type == "str"
            set_code = """
              // yes, voilate, we will not mutate that
              ret     = (char*)(void*)line->#{field.name}.c_str();
              ret_len = line->#{field.name}.size();
              """
          
          napi_fn_raw_fixed_code """
            if (idx >= _this->line_list.size()) {
              err = new std::string("OOB get");
              return;
            }
            auto line = &(_this->line_list[idx]);
            #{set_code}
            """
        
        napi_fn "set_#{field.name}", ()->
          napi_fn_arg "idx", "u64"
          napi_fn_arg "val", field.type
          
          set_code = """
            line->#{field.name} = val;
            """
          if field.type == "str"
            set_code = """
              line->#{field.name}.assign(val, val_len);
              """
          
          napi_fn_raw_fixed_code """
            if (idx >= _this->line_list.size()) {
              err = new std::string("OOB set");
              return;
            }
            auto line = &(_this->line_list[idx]);
            #{set_code}
            """
      
      # ###################################################################################################
      #    I/O
      # ###################################################################################################
      napi_fn "csv_read", ()->
        napi_fn_arg "path", "str"
        
        jl = []
        for field in field_list
          continue if field.is_calc
          # NOTE +1 - header
          # NOTE +1 - line numbers start from 1
          token_read = """
            if (!std::getline(ss, token, ',')) {
              err = new std::string("unexpected EOL at line=");
              *err += std::to_string(_this->line_list.size()+2);
              return;
            }
            """
          switch field.type
            when "str"
              jl.push """
                #{token_read}
                rline.#{field.name} = token;
                """
            when "i64"
              # requires -fexceptions
              # jl.push """
              #   #{token_read}
              #   try {
              #     rline.population = std::stoll(token);
              #   } catch (const std::exception&) {
              #     err = new std::string("bad #{field.name} at line=");
              #     *err += std::to_string(_this->line_list.size()+2);
              #     return;
              #   }
              #   """
              jl.push """
                #{token_read}
                {
                  i64 value;
                  auto result = std::from_chars(token.data(), token.data() + token.size(), value);
                  if (result.ec != std::errc{}) {
                    err = new std::string("bad #{field.name} at line=");
                    *err += std::to_string(_this->line_list.size()+2);
                    return;
                  }
                  rline.#{field.name} = value;
                }
                """
            else
              throw new Error "can't generate code for type #{field.type}"
        
        napi_fn_raw_fixed_code """
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
            
            #{join_list jl, '  '}
            
            // Append the parsed line to the report.
            _this->line_list.push_back(rline);
          }
          """
      
      napi_fn "pretty_print", ()->
        jl = []
        for field in field_list
          align = if field.pad_dir == "l" then "std::left" else "std::right"
          jl.push """
            << #{align} << std::setw(#{field.pad_size}) << row.#{field.name}
            """
        napi_fn_raw_fixed_code """
          for (const auto& row : _this->line_list) {
            std::cout
              #{join_list jl, '    '}
              << std::endl;
          }
          """
      
      # ###################################################################################################
      #    logic
      # ###################################################################################################
      napi_fn "max_d", ()->
        napi_fn_arg "ret", "i64"
        napi_fn_raw_fixed_code """
          ret = 0;
          for (const auto& row : _this->line_list) {
            if (row.density > ret) {
              ret = row.density;
            }
          }
          """
      
      napi_fn "augment_with_density_percent", ()->
        napi_fn_arg "max_d", "i64"
        napi_fn_raw_fixed_code """
          // Avoid division by zero.
          if (max_d == 0) {
            err = new std::string("max_d is zero, cannot compute density percent");
            return;
          }
      
          // Compute and assign density percent for each report line.
          for (auto& row : _this->line_list) {
            double percentage = (row.density * 100.0) / max_d;
            row.density_percent = static_cast<i64>(std::round(percentage));
          }
          """
      
      napi_fn "sort_by_density_percent_desc", ()->
        napi_fn_raw_fixed_code """
          std::sort(_this->line_list.begin(), _this->line_list.end(), [](const Report_line& a, const Report_line& b) {
            return a.density_percent > b.density_percent;
          });
          """
        
      