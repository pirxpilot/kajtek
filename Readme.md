[![NPM version][npm-image]][npm-url]
[![Build Status][travis-image]][travis-url]
[![Dependency Status][deps-image]][deps-url]
[![Dev Dependency Status][deps-dev-image]][deps-dev-url]

# kajtek

Basic node bindings for [TinyCDB].
 - It does not support any encoding (works with buffers).
 - It allows for fast insertion and lookup through `putMany` and `getMany`

## Install

```sh
$ npm install --save kajtek
```

## Usage

```js
var Kajtek = require('kajtek');

let db = new Kajtek('/path/to/my/db.dat');

// no encoding/decoding - kajtek works with Buffers
let keys = [2, 3, 4].map(x => Buffer.from([x]));

// keys and values have to be Arrays of Buffer's
db.putMany(keys, values, function(err) {
  // for each key in an keys set corresponding value from values
});


// keys and values have to be Arrays of Buffer's
db.getMany(keys, function(err, values, missing) {
  if (err) {
    // something went wrong
  }
  if (missing) {
    // some of the keys were not in DB
    // missing has indexes in the keys array, so to print the keys
    console.log('Missing keys:', missing.map(i => keys[i]));
  }
  // empty Buffer is returned for each missing key
});

```

## License

MIT © [Damian Krzeminski](https://pirxpilot.me)

[npm-image]: https://img.shields.io/npm/v/kajtek.svg
[npm-url]: https://npmjs.org/package/kajtek

[travis-url]: https://travis-ci.org/pirxpilot/kajtek
[travis-image]: https://img.shields.io/travis/pirxpilot/kajtek.svg

[deps-image]: https://img.shields.io/david/pirxpilot/kajtek.svg
[deps-url]: https://david-dm.org/pirxpilot/kajtek

[deps-dev-image]: https://img.shields.io/david/dev/pirxpilot/kajtek.svg
[deps-dev-url]: https://david-dm.org/pirxpilot/kajtek?type=dev

[TinyCDB]: http://www.corpit.ru/mjt/tinycdb.html
