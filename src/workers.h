#pragma once

#include <memory>
#include <tuple>

#include <nan.h>

#include "ref.h"

class TinyCDB;

class PutWorker : public Nan::AsyncWorker
{
public:
  PutWorker(Nan::Callback*, TinyCDB*, v8::Local<v8::Value>, v8::Local<v8::Value>);

  void Execute() override;

private:
  TinyCDB* tcdb;
  std::vector<refref_t> data;
};

class GetWorker : public Nan::AsyncWorker
{
public:
  GetWorker(Nan::Callback* fn, TinyCDB* tcdb, v8::Local<v8::Value>);

  void Execute() override;
  void HandleOKCallback() override;

private:
  TinyCDB* tcdb;
  std::vector<ref_t> keys;
  std::vector<ref_t> values;
};
