#!/usr/bin/env node
var modular = require('../modular_fp')
Array.prototype.apply = function (fn) {
  return fn(this)
}
;(async function() {
  var lines = await modular.readCSVLines('asset/data.csv')
  lines
    .apply(modular.transformData)
    .apply(modular.augmentWithDensityPercent)
    .apply(modular.sortByDensityPercentDescAugmented)
    .apply(modular.prettyPrint)
})()
