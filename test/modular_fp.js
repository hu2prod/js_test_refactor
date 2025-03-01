var assert = require('assert');
var modular = require('../src/modular_fp');

// Прим. При переделке в modular был убран if (data)
//   который убивает вообще всё поведение в случае пустого файла

describe('modular fp', function() {
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
    var real = modular.augmentWithDensityPercent(CSVListOfCells);
    var expd = CSVListOfCellsAugmented;
    assert.deepEqual(expd, real);
  })
  it('sortByDensityPercentDescAugmented', function() {
    var real = modular.sortByDensityPercentDescAugmented(CSVListOfCellsAugmented);
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
