// NodePlugin Template

/*
**
**  Started on 22/07/2013 by Matt
**  <matthieu.gonnet@gmail.com>
**
*/

#include <iostream>
#include <sstream>
//#include "Debug.hpp"

// Includes Systems specific on Unix
#include <dlfcn.h>
#include <pwd.h>
#include <unistd.h>
#include <string>
#include <time.h>

#include "../headers/node-gif.h"

using namespace v8;
using namespace std;
using namespace cv;

Persistent<FunctionTemplate> NodeGif::constructor;


Handle<Value> NodeGif::GetDelay(Local<v8::String> prop, const AccessorInfo &info) {
    HandleScope scope;
    NodeGif* gif = ObjectWrap::Unwrap<NodeGif>(info.This());
    return scope.Close(v8::Integer::New(gif->delay));
}

Handle<Value> NodeGif::SetDelay(const Arguments &args) {
    HandleScope scope;
    NodeGif* gif = ObjectWrap::Unwrap<NodeGif>(args.This());
    int d = args[0]->ToInteger()->Value();
    gif->setDelay(d);
    return scope.Close(Undefined());
}

Handle<Value> NodeGif::Encode(const Arguments& args){
    HandleScope scope;
    return scope.Close(Undefined());
}

uchar *NodeGif::gifGetColor(GifFileType &GifFile, const cv::Mat &test){
    cv::Mat ungif = Mat(test.size(), CV_8UC3, Scalar(0,0,0));

    unsigned char index = 0;
    if (GifFile.SColorMap)
    {
        for (unsigned int y=0; y < ungif.rows; y++){
          for (unsigned int x=0; x < ungif.cols; x++){
            index = test.at<uchar>(y,x);
            ungif.at<Vec3b>(y,x)[2] = GifFile.SColorMap->Colors[index].Red;
            ungif.at<Vec3b>(y,x)[1] = GifFile.SColorMap->Colors[index].Green;
            ungif.at<Vec3b>(y,x)[0] = GifFile.SColorMap->Colors[index].Blue;

        }
    }
}
return ungif.data;
}


void    NodeGif::gifDecoder(const char *filename) {
  int *error;
  GifFileType *GifFile = DGifOpenFileName(filename, error);

  int ret = DGifSlurp(GifFile);

  this->height = (int)GifFile->SHeight;
  this->width = GifFile->SWidth;
  this->nbFrames = GifFile->ImageCount;
  int colorSize = GifFile->SColorResolution;


  for (unsigned int i = 0; i < this->nbFrames; i++){
      GifImageDesc Image = GifFile->Image;
      SavedImage *img = &GifFile->SavedImages[i];
      uchar *data = (uchar *)malloc(this->width * this->height * sizeof(uchar));
      memcpy(data, img->RasterBits, this->width * this->height * sizeof(uchar));
      cv::Mat test = cv::Mat(cv::Size(this->width, this->height), CV_8UC1, data);
      this->frames->push_back(gifGetColor(*GifFile, test));
  }

}

Handle<Value> NodeGif::Decode(const Arguments& args){
    HandleScope scope;
    NodeGif* gif = ObjectWrap::Unwrap<NodeGif>(args.This());
    v8::String::Utf8Value v8string(args[0]->ToString());
    std::string filename = std::string(*v8string);
    gif->gifDecoder(filename.c_str());
    return scope.Close(Undefined());
}

Handle<Value> NodeGif::Push(const Arguments& args){
    HandleScope scope;
    NodeGif* gif = ObjectWrap::Unwrap<NodeGif>(args.This());

    return scope.Close(Undefined());
}

void NodeGif::Initialize(Handle<Object> target) {
    HandleScope scope;
    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(NodeGif::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(v8::String::NewSymbol("NodeGif"));

    Local<ObjectTemplate> proto = constructor->PrototypeTemplate();
    proto->Set(
        v8::String::NewSymbol("info"),
        FunctionTemplate::New(Info)->GetFunction()
        );
    proto->Set(
        v8::String::NewSymbol("encode"),
        FunctionTemplate::New(Encode)->GetFunction()
        );
    proto->Set(
        v8::String::NewSymbol("decode"),
        FunctionTemplate::New(Decode)->GetFunction()
        );
    proto->Set(
        v8::String::NewSymbol("push"),
        FunctionTemplate::New(Push)->GetFunction()
        );
    proto->Set(
        v8::String::NewSymbol("SetDelay"),
        FunctionTemplate::New(SetDelay)->GetFunction()
        );
    proto->SetAccessor(v8::String::NewSymbol("delay"), GetDelay);
    target->Set(v8::String::NewSymbol("NodeGif"), constructor->GetFunction());
}

Handle<Value> NodeGif::New(const Arguments& args) {
    HandleScope scope;
/*    if (args.Length() < 1){
        cout << "ERROR: Please specify input" << endl;
        return ThrowException(v8::Exception::TypeError(v8::String::New("Need Image path in first Argument")));
    }
*/
    if(!args[0]->IsNumber()){
        return ThrowException(v8::Exception::TypeError(v8::String::New("First argument must be a number")));
    }
    if(!args[1]->IsNumber()){
        return ThrowException(v8::Exception::TypeError(v8::String::New("First argument must be a number")));
    }
    int width = args[0]->ToInteger()->Value();
    int height = args[1]->ToInteger()->Value();
    NodeGif* gif = new NodeGif(width, height);
    cout << GIFLIB_MAJOR ; cout << "." <<  GIFLIB_MINOR;
    cout << "." << GIFLIB_RELEASE << endl;
    gif->Wrap(args.This());
    return (args.This());
}

NodeGif::NodeGif(int width, int height) {
    this->width = width;
    this->height = height;
    this->nbFrames = 0;
    this->delay = 100;
    this->paletteSize = 256;
    this->frames = new std::vector<uchar *>();
    /* if ((GifFileOut = EGifOpenFileHandle(fileno(pFile), &ErrorCode)) == NULL) {
        cout << "Cannot open file for encoding gif" << endl;
        exit(EXIT_FAILURE);
    }*/

    }

    NodeGif::~NodeGif() {
        for (int i=this->frames->size() - 1; i >= 0; i--){
            delete this->frames->at(i);
            this->frames->erase(this->frames->begin()+ i);
        }
    }

    v8::Handle<v8::Value>    NodeGif::Info(const Arguments& args){
        HandleScope scope;
        NodeGif* gif = ObjectWrap::Unwrap<NodeGif>(args.This());
        cout << "GifInfo:\twidth: " << gif->width << endl;
        cout << "         \theight: "<< gif->height << endl;
        cout << "         \tnbFrames: "<< gif->nbFrames << endl;
        cout << "         \tdelay: "<< gif->delay << endl;

        return scope.Close(Undefined());
    }
