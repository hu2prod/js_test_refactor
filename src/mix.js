var module = this
var fs = require('node:fs')
var readline = require('node:readline')
// NOTE. we replicate built-in bug which will kill last line
// Прим. Эта функция ничего не знает про структуру CSV
this.readCSVLines = function (path) {
  return new Promise((resolve, reject)=>{
    var fileStream = fs.createReadStream(path)
    var rl = readline.createInterface({
      input: fileStream,
      crlfDelay: Infinity
    })
    lines = []
    rl.on('line', (line) => {
      lines.push(line)
    })
    rl.on('close', () => {
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
      resolve(lines)
    })
    fileStream.on('error', (err) => {
      reject(err)
    })
  })
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
