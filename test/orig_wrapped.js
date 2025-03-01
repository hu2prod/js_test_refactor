var assert = require('assert');
var orig_wrapped = require('../src/orig_wrapped');

describe('orig_wrapped', function() {
  it('baseline', function() {
    // assert
    var real = orig_wrapped();
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
