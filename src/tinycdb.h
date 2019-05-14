#pragma once

#include <string>
#include <tuple>

#include <tinycdb/cdb.h>

class TinyCDB {
public:

  TinyCDB(std::string location):
    location(location)
    {};

  void Put(std::tuple<char*, unsigned, char*, unsigned> item);
  std::tuple<char*, unsigned> Get(std::tuple<char*, unsigned> key);

private:
  void StartReading();
  void StartMaking();

  bool making = false;
  bool reading = false;

  std::string location;
  struct cdb_make cdbm;
  struct cdb cdb;
};
