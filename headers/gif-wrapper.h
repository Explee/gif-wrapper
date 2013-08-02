// Plugin Node Template
//
// Copyright (c) 2013 Matt <matthieu@gonnet.tv>
//


#ifndef __NODE_GIF_H__
#define __NODE_GIF_H__

#include <v8.h>
#include <node.h>
#include <node_object_wrap.h>
#include <node_version.h>

#include <string>


#include "opencv2/opencv.hpp"
#include "gif_lib.h"
#include "gif-colors.h"
using namespace v8;
using namespace node;

#define PROGRAM_NAME  "gifEncoder"
#define COLOR_RESOLUTION 8


class GifWrapper : public node::ObjectWrap {
private:
	GifFileType *GifFile;
	int paletteSize;
	int *error;
	ColorMapObject* outputPalette;
	GifColorType *c;
	int width;
	int height;
	int nbFrames;
	int delay;
	std::vector<uchar *> *frames;
	std::vector<uchar *>* framesToEncode;

	GifWrapper(int width, int height);
    ~GifWrapper();

    inline void	setDelay(int d){this->delay = d;};
    uchar  *gifGetColor(GifFileType &GifFile, const uchar *data);
    void gifDecoder(const char *filename);
	// gif functions
	static v8::Handle<v8::Value> Info(const Arguments& args);
	static v8::Handle<Value> Encode(const Arguments& args);
	static v8::Handle<Value> Decode(const Arguments& args);
	static v8::Handle<Value> SetDelay(const Arguments& args);
	static v8::Handle<Value> Push(const Arguments& args);


	static v8::Handle<Value> GetDelay(Local<v8::String> prop, const AccessorInfo &info);

public:
    static Persistent<FunctionTemplate> constructor;
    static void Initialize(v8::Handle<v8::Object> target);
    static v8::Handle<Value> New(const Arguments &args);
	static v8::Handle<Value> GetToto(Local<v8::String> prop, const AccessorInfo &info);


};
#endif