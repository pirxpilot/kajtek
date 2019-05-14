#include "workers.h"

#include "kajtek.h"
#include "tinycdb.h"


PutWorker::PutWorker(Nan::Callback* fn, TinyCDB* tcdb, v8::Local<v8::Value> ks, v8::Local<v8::Value> vs):
  Nan::AsyncWorker(fn, "put"),
  tcdb(tcdb) {

  SaveToPersistent("keys", ks);
  SaveToPersistent("values", vs);

  auto kbuffers = ks.As<v8::Array>();
  auto vbuffers = vs.As<v8::Array>();

  const auto len = kbuffers->Length();

  for(auto i = 0; i < len; i++) {
    auto k = kbuffers->Get(i);
    auto v = vbuffers->Get(i);

    auto key = node::Buffer::Data(k);
    auto klen = node::Buffer::Length(k);

    auto val = node::Buffer::Data(v);
    auto vlen = node::Buffer::Length(v);

    data.emplace_back(key, klen, val, vlen);
  }
}

void PutWorker::Execute() {
  for(auto const& item: data) {
    tcdb->Put(item);
  }
}

GetWorker::GetWorker(Nan::Callback* fn, TinyCDB* tcdb, v8::Local<v8::Value> ks):
    Nan::AsyncWorker(fn, "get"),
    tcdb(tcdb) {

  SaveToPersistent("keys", ks);

  auto kbuffers = ks.As<v8::Array>();

  const auto len = kbuffers->Length();
  keys.reserve(len);

  for(auto i = 0; i < len; i++) {
    auto k = kbuffers->Get(i);

    auto key = node::Buffer::Data(k);
    auto klen = node::Buffer::Length(k);

    keys.emplace_back(key, klen);
  }
}

void GetWorker::Execute() {
  values.reserve(keys.size());

  for(auto const& item: keys) {
    values.push_back(tcdb->Get(item));
  }
}

void GetWorker::HandleOKCallback() {
  auto len = values.size();
  auto results = Nan::New<v8::Array>(len);
  std::vector<unsigned int> missing;

  for(auto i = 0; i < len; i++) {
    auto [ val, vlen ] = values[i];

    if (!val) {
      missing.push_back(i);
    }

    auto buffer = Nan::NewBuffer(reinterpret_cast <char *>(val), vlen).ToLocalChecked();
    Nan::Set(results, i, buffer);
  }

  if (missing.size() == 0) {
    v8::Local<v8::Value> argv[] = { Nan::Null(), results };
    callback->Call(2, argv, async_resource);
  } else {

    auto orphans = Nan::New<v8::Array>(missing.size());
    for (int i = 0; i < missing.size(); i++) {
      Nan::Set(orphans, i, Nan::New(missing[i]));
    }

    v8::Local<v8::Value> argv[] = { Nan::Null(), results, orphans };
    callback->Call(3, argv, async_resource);
  }
}
