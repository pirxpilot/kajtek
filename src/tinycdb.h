#pragma once

#include <string>
#include <tuple>

#include <tinycdb/cdb.h>

#include "ref.h"

class TinyCDB {
public:

  TinyCDB(std::string location):
    location(location)
    {};

  void Put(refref_t const & item);
  ref_t Get(ref_t const & key);

private:
  void StartReading();
  void StartMaking();

  bool making = false;
  bool reading = false;

  std::string location;
  struct cdb_make cdbm;
  struct cdb cdb;
};
