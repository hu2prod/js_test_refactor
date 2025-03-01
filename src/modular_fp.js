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
  return CSVLines.map((line)=>line.split(','))
}
this.maxD = function(table) {
  return table.reduce((max, cells) => {
    const d = parseInt(cells[3])
    return d > max ? d : max
  }, 0)
}
this.augmentWithDensityPercent = function(table) {
  var maxD = module.maxD(table)
  return table.map((row)=>{
    row = row.slice()
    const a = Math.round((row[3] * 100) / maxD)
    row.push(a.toString()) // PERF. Мы в sort потом будем делать минус между строками (ненужный str2float)
    return row
  })
}
this.sortByDensityPercentDescAugmented = function(table) {
  // toSorted слишком свежий
  table = table.slice()
  table.sort((r1, r2) => r2[5] - r1[5])
  return table
}
this.prettyPrint = function(table) {
  table.forEach((row)=>{
    let s = row[0].padEnd(18)
    s += row[1].padStart(10)
    s += row[2].padStart(8)
    s += row[3].padStart(8)
    s += row[4].padStart(18)
    s += row[5].padStart(6)
    console.log(s)
  })
}
