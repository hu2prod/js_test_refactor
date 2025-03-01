#!/usr/bin/env node
var mod = require('../mix')

;(async function() {
  var lines = await mod.readCSVLines('asset/data.csv')
  lines = mod.transformData(lines)
  mod.augmentWithDensityPercent(lines)
  mod.sortByDensityPercentDescAugmented(lines)
  mod.prettyPrint(lines)
})()
