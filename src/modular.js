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
    const cells = line.split(',')
    table.push([cells[0], cells[1], cells[2], cells[3], cells[4]])
  }
  return table
}
this.maxD = function(table) {
  var max = 0
  for (const cells of table) {
    const d = parseInt(cells[3])
    if (d > max) max = d
  }
  return max
}
this.augmentWithDensityPercent = function(table) {
  var maxD = module.maxD(table)
  for (const row of table) {
    const a = Math.round((row[3] * 100) / maxD)
    row.push(a.toString()) // PERF. Мы в sort потом будем делать минус между строками (ненужный str2float)
  }
}
this.sortByDensityPercentDescAugmented = function(table) {
  table.sort((r1, r2) => r2[5] - r1[5])
}
this.prettyPrint = function(table) {
  for (const row of table) {
    let s = row[0].padEnd(18)
    s += row[1].padStart(10)
    s += row[2].padStart(8)
    s += row[3].padStart(8)
    s += row[4].padStart(18)
    s += row[5].padStart(6)
    console.log(s)
  }
}
