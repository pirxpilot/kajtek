const test = require('tape');
const { dirSync } = require('tmp');

const Kajtek = require('../');

test('put and get', function (t) {
  const { name } = dirSync({ prefix: 'kajtek-' });

  let keys =  from([ 1, 13, 5, 88 ]);
  let values =  from([ 100, 101, 102, 103 ]);

  let db = new Kajtek(name + '/cdb.dat');
  db.putMany(keys, values, get);

  function get() {
    db.getMany(keys, check);
  }

  function check(err, result, missing) {
    t.error(err);
    t.notOk(missing, 'there should be no missing keys');
    t.same(result, values);
    t.end();
  }
});

test('put, remove, and get', function (t) {
  const { name } = dirSync({ prefix: 'kajtek-' });

  let keys =  from([ 1, 13, 5, 88 ]);
  let values =  from([ 100, 101, 102, 103 ]);
  let empty = Buffer.alloc(0);

  let db = new Kajtek(name  + '/cdb.dat');
  db.putMany(keys, values, get);

  function get() {
    db.getMany(from([ 1, 122, 5 ]), check);
  }

  function check(err, result, missing) {
    t.error(err);
    t.same(missing, [ 1 ]);
    t.equal(result.length, 3);
    t.same(result[0], values[0]);
    t.same(result[1], empty, 'empty buffer for missing values');
    t.same(result[2], values[2]);
    t.end();
  }
});

function from(array) {
  return array.map(x => Buffer.from([x]));
}
