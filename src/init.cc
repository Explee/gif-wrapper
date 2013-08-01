
//
// init.cc
//
// Copyright (c) 2013 Explee <matthieu@explee.com>
//

#include <stdio.h>
#include "../headers/node-gif.h"

extern "C" void
init (Handle<Object> target) {
  HandleScope scope;
  NodeGif::Initialize(target);
}

NODE_MODULE(gif,init);
