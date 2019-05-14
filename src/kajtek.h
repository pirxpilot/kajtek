#pragma once

#include <memory>
#include <string>

#include <nan.h>
#include "tinycdb.h"

class Kajtek : public Nan::ObjectWrap
{
public:
  static void Init(v8::Local<v8::Object>);

  friend class OpenWorker;
  friend class CloseWorker;

private:
  Kajtek(std::string location):
    tcdb(location)
    {};

  static void NewInstance(const Nan::FunctionCallbackInfo<v8::Value>&);
  static void GetMany(const Nan::FunctionCallbackInfo<v8::Value>&);
  static void PutMany(const Nan::FunctionCallbackInfo<v8::Value>&);

  static Nan::Persistent<v8::Function> constructor;

  TinyCDB tcdb;
};
