var assert = require('assert');
var modular = require('../src/mix');

// Прим. При переделке в modular был убран if (data)
//   который убивает вообще всё поведение в случае пустого файла

// лень переписывать каждый тест ручками
var transform = (lines)=>{
  var ret = [];
  for(const line of lines) {
    const [city,population,area,density,country] = line;
    ret.push({
      city,
      population: +population,
      area      : +area,
      density   : +density,
      country
    });
  }
  return ret;
}
var transformWithDensityPercent = (lines)=>{
  var ret = [];
  for(const line of lines) {
    const [city,population,area,density,country,densityPercent] = line;
    ret.push({
      city,
      population: +population,
      area      : +area,
      density   : +density,
      country,
      densityPercent : +densityPercent
    });
  }
  return ret;
}

describe('mix', function() {
  var CSVLines = [
    '  Shanghai,24256800,6340,3826,China',
    '  Delhi,16787941,1484,11313,India',
    '  Lagos,16060303,1171,13712,Nigeria',
    '  Istanbul,14160467,5461,2593,Turkey',
    '  Tokyo,13513734,2191,6168,Japan',
    '  Sao Paulo,12038175,1521,7914,Brazil',
    '  Mexico City,8874724,1486,5974,Mexico',
    '  London,8673713,1572,5431,United Kingdom',
    '  New York City,8537673,784,10892,United States'
  ];
  var CSVListOfCells = [
    ['  Shanghai','24256800','6340','3826','China'],
    ['  Delhi','16787941','1484','11313','India'],
    ['  Lagos','16060303','1171','13712','Nigeria'],
    ['  Istanbul','14160467','5461','2593','Turkey'],
    ['  Tokyo','13513734','2191','6168','Japan'],
    ['  Sao Paulo','12038175','1521','7914','Brazil'],
    ['  Mexico City','8874724','1486','5974','Mexico'],
    ['  London','8673713','1572','5431','United Kingdom'],
    ['  New York City','8537673','784','10892','United States']
  ];
  CSVListOfCells = transform(CSVListOfCells);
  // Прим. ctrl-c ctrl-V быстрее чем искать и вставлять deep clone
  var CSVListOfCellsMut = [
    ['  Shanghai','24256800','6340','3826','China'],
    ['  Delhi','16787941','1484','11313','India'],
    ['  Lagos','16060303','1171','13712','Nigeria'],
    ['  Istanbul','14160467','5461','2593','Turkey'],
    ['  Tokyo','13513734','2191','6168','Japan'],
    ['  Sao Paulo','12038175','1521','7914','Brazil'],
    ['  Mexico City','8874724','1486','5974','Mexico'],
    ['  London','8673713','1572','5431','United Kingdom'],
    ['  New York City','8537673','784','10892','United States']
  ];
  CSVListOfCellsMut = transform(CSVListOfCellsMut);
  var CSVListOfCellsAugmented = [
    ['  Shanghai','24256800','6340','3826','China','28'],
    ['  Delhi','16787941','1484','11313','India','83'],
    ['  Lagos','16060303','1171','13712','Nigeria','100'],
    ['  Istanbul','14160467','5461','2593','Turkey','19'],
    ['  Tokyo','13513734','2191','6168','Japan','45'],
    ['  Sao Paulo','12038175','1521','7914','Brazil','58'],
    ['  Mexico City','8874724','1486','5974','Mexico','44'],
    ['  London','8673713','1572','5431','United Kingdom','40'],
    ['  New York City','8537673','784','10892','United States','79']
  ];
  CSVListOfCellsAugmented = transformWithDensityPercent(CSVListOfCellsAugmented);
  // Прим. ctrl-c ctrl-V быстрее чем искать и вставлять deep clone
  var CSVListOfCellsAugmentedMut = [
    ['  Shanghai','24256800','6340','3826','China','28'],
    ['  Delhi','16787941','1484','11313','India','83'],
    ['  Lagos','16060303','1171','13712','Nigeria','100'],
    ['  Istanbul','14160467','5461','2593','Turkey','19'],
    ['  Tokyo','13513734','2191','6168','Japan','45'],
    ['  Sao Paulo','12038175','1521','7914','Brazil','58'],
    ['  Mexico City','8874724','1486','5974','Mexico','44'],
    ['  London','8673713','1572','5431','United Kingdom','40'],
    ['  New York City','8537673','784','10892','United States','79']
  ];
  CSVListOfCellsAugmentedMut = transformWithDensityPercent(CSVListOfCellsAugmentedMut);
  var CSVListOfCellsAugmentedSorted = [
    ['  Lagos','16060303','1171','13712','Nigeria','100'],
    ['  Delhi','16787941','1484','11313','India','83'],
    ['  New York City','8537673','784','10892','United States','79'],
    ['  Sao Paulo','12038175','1521','7914','Brazil','58'],
    ['  Tokyo','13513734','2191','6168','Japan','45'],
    ['  Mexico City','8874724','1486','5974','Mexico','44'],
    ['  London','8673713','1572','5431','United Kingdom','40'],
    ['  Shanghai','24256800','6340','3826','China','28'],
    ['  Istanbul','14160467','5461','2593','Turkey','19']
  ];
  CSVListOfCellsAugmentedSorted = transformWithDensityPercent(CSVListOfCellsAugmentedSorted);
  it('readCSVData', async function() {
    var real = await modular.readCSVLines('asset/data.csv');
    var expd = CSVLines;
    assert.deepEqual(expd, real);
  })
  it('transformData', function() {
    var real = modular.transformData(CSVLines);
    var expd = CSVListOfCells;
    assert.deepEqual(expd, real);
  })
  it('maxD', function() {
    var real = modular.maxD(CSVListOfCells);
    var expd = 13712;
    assert.strictEqual(expd, real);
  })
  it('augmentWithDensityPercent', function() {
    modular.augmentWithDensityPercent(CSVListOfCellsMut);
    var real = CSVListOfCellsMut;
    var expd = CSVListOfCellsAugmented;
    assert.deepEqual(expd, real);
  })
  it('sortByDensityPercentDescAugmented', function() {
    modular.sortByDensityPercentDescAugmented(CSVListOfCellsAugmentedMut);
    var real = CSVListOfCellsAugmentedMut;
    var expd = CSVListOfCellsAugmentedSorted;
    assert.deepEqual(expd, real);
  })
  it('prettyPrint', function() {
    var err;
    var ret = [];
    var console_log = console.log;
    console.log = function(t) {
      ret.push(t);
    }
    try {
      modular.prettyPrint(CSVListOfCellsAugmentedSorted);
    } catch (_err) {
      err = _err;
    }
    console.log = console_log;
    if (err) throw err;
    
    var real = ret;
    var expd = [
      '  Lagos             16060303    1171   13712           Nigeria   100',
      '  Delhi             16787941    1484   11313             India    83',
      '  New York City      8537673     784   10892     United States    79',
      '  Sao Paulo         12038175    1521    7914            Brazil    58',
      '  Tokyo             13513734    2191    6168             Japan    45',
      '  Mexico City        8874724    1486    5974            Mexico    44',
      '  London             8673713    1572    5431    United Kingdom    40',
      '  Shanghai          24256800    6340    3826             China    28',
      '  Istanbul          14160467    5461    2593            Turkey    19'
    ]
    assert.deepEqual(expd, real);
  })
})
