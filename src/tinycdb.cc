#include <fcntl.h>
#include <unistd.h>

#include "tinycdb.h"

void TinyCDB::StartMaking() {
  if (making) {
    return;
  }
  auto fd = open(location.c_str(), O_RDWR|O_CREAT, 0600);

  cdb_make_start(&cdbm, fd);
  making = true;
}

void TinyCDB::StartReading() {
  if (reading) {
    return;
  }

  if (making) {
    // finish writing
    cdb_make_finish(&cdbm);
    close(cdbm.cdb_fd);

    making = false;
  }

  // start reading
  int fd = open(location.c_str(), O_RDONLY);
  cdb_init(&cdb, fd);
  reading = true;
}

void TinyCDB::Put(refref_t const &item) {
  if (!making) {
    StartMaking();
  }

  auto [ key, klen, val, vlen ] = item;

  cdb_make_add(&cdbm, key, klen, val, vlen);
}

ref_t TinyCDB::Get(ref_t const &item) {
  if (!reading) {
    StartReading();
  }

  auto [ key, klen ] = item;

  if (cdb_find(&cdb, key, klen) <= 0) {
    return { reinterpret_cast<char*>(0), 0 };
  }

  auto vpos = cdb_datapos(&cdb);
  auto vlen = cdb_datalen(&cdb);

  auto val = malloc(vlen);
  cdb_read(&cdb, val, vlen, vpos);

  return { reinterpret_cast<char*>(val), vlen };
}
