#!/usr/bin/env node
var modular = require('../modular')
var lines = modular.readCSVLines('asset/data.csv')
lines = modular.transformData(lines)
modular.augmentWithDensityPercent(lines)
modular.sortByDensityPercentDescAugmented(lines)
modular.prettyPrint(lines)
