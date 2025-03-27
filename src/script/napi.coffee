#!/usr/bin/env iced
require "fy"
mod = require("../report_tool")

report = new mod.Report
report.csv_read_sync "asset/data.csv"
max_d = report.max_d_sync "asset/data.csv"
report.augment_with_density_percent_sync(max_d)
report.sort_by_density_percent_desc_sync()
report.pretty_print_sync()

###
cb = (err)->
  if err
    throw err
  puts "done"
  process.exit()

report = new mod.Report
await report.csv_read "asset/data.csv", defer(err);           return cb err if err
await report.max_d defer(err, max_d);                         return cb err if err
await report.augment_with_density_percent max_d, defer(err);  return cb err if err
await report.sort_by_density_percent_desc defer(err);         return cb err if err
# А вот тут не надо сражаться за консоль
report.pretty_print_sync()

cb()
###
