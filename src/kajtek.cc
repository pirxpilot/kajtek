#include "workers.h"

#include "kajtek.h"

Nan::Persistent<v8::Function> Kajtek::constructor;

void Kajtek::NewInstance(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (!info.IsConstructCall()) {
    Nan::ThrowTypeError("Use `new` to call the constructor");
    return;
  }

  const auto argc = info.Length();

  if (argc < 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  auto str = new Nan::Utf8String(info[0]);

  Kajtek* obj = new Kajtek(**str);
  obj->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}

void Kajtek::GetMany(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  auto keys = info[0].As<v8::Array>();
  auto fn = new Nan::Callback(info[1].As<v8::Function>());

  Kajtek* kajtek = ObjectWrap::Unwrap<Kajtek>(info.Holder());
  auto worker = new GetWorker(fn, &kajtek->tcdb, keys);
  worker->SaveToPersistent("keys", keys);
  Nan::AsyncQueueWorker(worker);
}

void Kajtek::PutMany(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  auto fn = new Nan::Callback(info[2].As<v8::Function>());

  Kajtek* kajtek = ObjectWrap::Unwrap<Kajtek>(info.Holder());
  auto worker = new PutWorker(fn, &kajtek->tcdb, info[0], info[1]);
  Nan::AsyncQueueWorker(worker);
}

void Kajtek::Init(v8::Local<v8::Object> exports) {
  // Prepare constructor template
  auto tpl = Nan::New<v8::FunctionTemplate>(NewInstance);
  tpl->SetClassName(Nan::New("Kajtek").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetPrototypeMethod(tpl, "getMany", GetMany),
  Nan::SetPrototypeMethod(tpl, "putMany", PutMany),

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(
    exports,
    Nan::New("Kajtek").ToLocalChecked(),
    Nan::GetFunction(tpl).ToLocalChecked()
  );
}

void InitAll(v8::Local<v8::Object> exports) {
  Kajtek::Init(exports);
}

NODE_MODULE(addon, InitAll)
