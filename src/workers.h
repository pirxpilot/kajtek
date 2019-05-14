#pragma once

#include <memory>
#include <tuple>

#include <nan.h>

class TinyCDB;

class PutWorker : public Nan::AsyncWorker
{
public:
  PutWorker(Nan::Callback*, TinyCDB*, v8::Local<v8::Value>, v8::Local<v8::Value>);

  void Execute() override;

private:
  TinyCDB* tcdb;
  std::vector<std::tuple<char*, unsigned, char*, unsigned>> data;
};

class GetWorker : public Nan::AsyncWorker
{
public:
  GetWorker(Nan::Callback* fn, TinyCDB* tcdb, v8::Local<v8::Value>);

  void Execute() override;
  void HandleOKCallback() override;

private:
  TinyCDB* tcdb;
  std::vector<std::tuple<char*, unsigned>> keys;
  std::vector<std::tuple<char*, unsigned>> values;
};
