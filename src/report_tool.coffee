fs = require "fs"
path = __dirname+"/report_tool.node"
module.exports = if fs.existsSync path
  require path
else
  require "../src_c_napi/report_tool"

global.__HACK_NAPI_report_tool_Report = module.exports.Report;