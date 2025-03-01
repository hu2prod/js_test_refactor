var module = this
var fs = require('node:fs')
// NOTE. we replicate built-in bug which will kill last line
// Прим. Эта функция ничего не знает про структуру CSV
// BUG. Эта функция упадёт если файл будет >2GB (слишком длинная строка)
this.readCSVLines = function (path) {
  var cont = fs.readFileSync(path, 'utf-8')
  // NOTE cont.trim() тоже вариант. Новую строку пересоздать не должно, но зачем рисковать?
  var lines =  cont.split('\n')
  lines.shift() // kill header
  lines.pop() // BUG
  // this should be in real code
  // if (!lines[lines.length-1]) {
  //   lines.pop()
  // }
  // // OR
  // while (!lines[lines.length-1] && lines.length) {
  //   lines.pop()
  // }
  return lines
}
this.transformData = function(CSVLines) {
  var table = []
  for (const line of CSVLines) {
    const [city,population,area,density,country] = line.split(',')
    table.push({
      city,
      population: +population,
      area      : +area,
      density   : +density,
      country
    })
  }
  return table
}
this.maxD = function(table) {
  var max = 0
  for (const cells of table) {
    const d = cells.density
    if (d > max) max = d
  }
  return max
}
this.augmentWithDensityPercent = function(table) {
  var maxD = module.maxD(table)
  for (const row of table) {
    row.densityPercent = Math.round((row.density * 100) / maxD)
  }
}
this.sortByDensityPercentDescAugmented = function(table) {
  // Прим. всегда в такой сортировке порядок должен быть a, b
  //   В случае desc вчитываться очень сложно в имена переменных
  table.sort((r1, r2) => -(r1.densityPercent - r2.densityPercent))
}
this.prettyPrint = function(table) {
  for (const row of table) {
    let s = ''
    s += row.city                     .padEnd(18)
    s += row.population.toString()    .padStart(10)
    s += row.area.toString()          .padStart(8)
    s += row.density.toString()       .padStart(8)
    s += row.country                  .padStart(18)
    s += row.densityPercent.toString().padStart(6)
    console.log(s)
  }
}
