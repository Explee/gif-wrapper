
//
// init.cc
//
// Copyright (c) 2013 Explee <matthieu@explee.com>
//

#include <stdio.h>
#include "../headers/gif-wrapper.h"

extern "C" void
init (Handle<Object> target) {
  HandleScope scope;
  GifWrapper::Initialize(target);
}

NODE_MODULE(gif,init);
