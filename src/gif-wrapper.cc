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

#include "../headers/gif-wrapper.h"
#include "../headers/gif-colors.h"

using namespace v8;
using namespace std;
using namespace cv;

Persistent<FunctionTemplate> GifWrapper::constructor;


int findNearestColor(int r, int g, int b, ColorMapObject *ColorMap){

    int idx = 0;
    int res = INT_MAX;
    int min = INT_MAX;
    for (unsigned int i = 0; i <= 255; i++){
        int rr = std::abs(r - ColorMap->Colors[i].Red);
        int gg = std::abs(r - ColorMap->Colors[i].Green);
        int bb = std::abs(r - ColorMap->Colors[i].Blue);
        res  = rr + gg + bb;
        if(res <= min){
            idx = i;
            min = res;
        }
    }

    return idx;
}


Handle<Value> GifWrapper::GetDelay(Local<v8::String> prop, const AccessorInfo &info) {
    HandleScope scope;
    GifWrapper* gif = ObjectWrap::Unwrap<GifWrapper>(info.This());
    return scope.Close(v8::Integer::New(gif->delay));
}

Handle<Value> GifWrapper::SetDelay(const Arguments &args) {
    HandleScope scope;
    GifWrapper* gif = ObjectWrap::Unwrap<GifWrapper>(args.This());
    int d = args[0]->ToInteger()->Value();
    gif->setDelay(d);
    return scope.Close(Undefined());
}


cv::Mat quantification(cv::Mat &img, ColorMapObject *ColorMap){
    cv::Mat q = Mat(img.size(), CV_8UC1, Scalar(0));
    if (ColorMap){
        for (unsigned int y=0; y < img.rows; y++){
            for (unsigned int x=0; x < img.cols; x++){
                q.at<uchar>(y,x) = (uchar)findNearestColor(img.at<Vec3b>(y,x)[0], img.at<Vec3b>(y,x)[1], img.at<Vec3b>(y,x)[2], ColorMap);
            }
        }
        imwrite("quantified.png", q);
    } else {
        cout << "Error in Quantization" << endl;
        exit(-1);
    }
    return q;
}

static bool AddLoop(GifFileType *gf){
    int  error;
    if (EGifPutExtensionLeader(gf, error) == GIF_ERROR) {
     return false;
 }
 if (EGifPutExtensionTrailer(gf) == GIF_ERROR) {
     return false;
 }
 return true;
}


Handle<Value> GifWrapper::Encode(const Arguments& args){
    HandleScope scope;
    GifWrapper* gif = ObjectWrap::Unwrap<GifWrapper>(args.This());
    
      int paletteSize=256;
    ColorMapObject* outputPalette;
    int *error;
    GifFileType *GifFile = EGifOpenFileName("test.gif", false, error);

   outputPalette = GifMakeMapObject(256, gifColors);//GifMakeMapObject(paletteSize, NULL);
   GifColorType *c = outputPalette->Colors;

   if (!outputPalette) exit(EXIT_FAILURE);

   if (EGifPutScreenDesc(
    GifFile,
    gif->width, gif->height, 8, 0,
    outputPalette
    ) == GIF_ERROR) exit(EXIT_FAILURE);

  /*char netscape_extension[] = "NETSCAPE2.0";
        EGifPutExtension(GifFile, APPLICATION_EXT_FUNC_CODE, 11, netscape_extension);
        char animation_extension[] = { 1, 1, 0 }; // repeat one time
        EGifPutExtension(GifFile, APPLICATION_EXT_FUNC_CODE, 3, animation_extension);
 */    AddLoop(GifFile);

        for (unsigned int p = 1; p < gif->framesToEncode->size(); p++){
            Mat src = Mat(Size(gif->width, gif->height), CV_8UC1, gif->framesToEncode->at(p));
            cvtColor(src.clone(), src, CV_GRAY2RGB);
            Mat q = quantification(src, outputPalette);
    //imwrite("test.png", src);
            int npix = q.size().width * q.size().height;
            int gifsx = q.size().width;
            int gifsy = q.size().height;
            uchar *output;
            output = (uchar *)malloc(npix * sizeof(uchar *));


            int delay = 1;

            for (int i = 0, j=0; i < npix; i++) {
                int minIndex = 0,
                minDist = 3 * 256 * 256;
            }

        static unsigned char
        ExtStr[4] = { 0x04, 0x00, 0x00, 0xff };


        ExtStr[0] = (false) ? 0x06 : 0x04;
        ExtStr[1] = delay % 256;
        ExtStr[2] = delay / 256;

    /* Dump graphics control block. */
        EGifPutExtension(GifFile, GRAPHICS_EXT_FUNC_CODE, 4, ExtStr);


        if (EGifPutImageDesc(
         GifFile,
         0, 0, gifsx, gifsy, false, NULL
         ) == GIF_ERROR)  exit(EXIT_FAILURE);

            uchar * data = (uchar *)malloc(gifsx * sizeof(uchar) + 1);

        int j = 0;
        while( j < gifsy){
            mempcpy(data, &q.data[(j * gifsx)], gifsx);

            if (EGifPutLine(GifFile, data, gifsx) == GIF_ERROR) {
                cout << "error in putline" << endl;
                exit(-1);
            }

            j++;
        }
    }

    if (EGifCloseFile(GifFile) == GIF_ERROR) exit(EXIT_FAILURE);
    return scope.Close(Undefined());
}

uchar *GifWrapper::gifGetColor(GifFileType &GifFile, const uchar *data){
    cv::Mat ungif = Mat(Size(this->width, this->height), CV_8UC3, Scalar(0,0,0));
   // uchar *ungif = (uchar *)malloc(this->width * this->height * 3 * sizeof(uchar));

    unsigned char index = 0;
    if (GifFile.SColorMap)
    {
        for (unsigned int y=0; y < this->height; y++){
          for (unsigned int x=0; x < this->width; x++){
            index = data[(this->height * y) + x ];
  /*          ungif[(this->height * y ) + x + 2] =  GifFile.SColorMap->Colors[index].Red;
            ungif[(this->height * y ) + x + 1] =  GifFile.SColorMap->Colors[index].Green;
            ungif[(this->height * y) + x] =  GifFile.SColorMap->Colors[index].Blue;

*/            ungif.at<Vec3b>(y,x)[2] = GifFile.SColorMap->Colors[index].Red;
            ungif.at<Vec3b>(y,x)[1] = GifFile.SColorMap->Colors[index].Green;
            ungif.at<Vec3b>(y,x)[0] = GifFile.SColorMap->Colors[index].Blue;

        }
    }
}
return ungif.data;
}


void    GifWrapper::gifDecoder(const char *filename) {
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
      //cv::Mat test = cv::Mat(cv::Size(this->width, this->height), CV_8UC1, data);
      this->frames->push_back(gifGetColor(*GifFile, data));
  }

}

Handle<Value> GifWrapper::Decode(const Arguments& args){
    HandleScope scope;
    GifWrapper* gif = ObjectWrap::Unwrap<GifWrapper>(args.This());
    v8::String::Utf8Value v8string(args[0]->ToString());
    std::string filename = std::string(*v8string);
    gif->gifDecoder(filename.c_str());
    Handle<v8::Array> arr = v8::Array::New(gif->frames->size());
    for (unsigned int j = 0; j < gif->frames->size(); j++){
        Handle<v8::Object> obj = v8::Object::New();
        obj->SetIndexedPropertiesToPixelData((uint8_t *)gif->frames->at(j), (gif->width * gif->height) );
        arr->Set(j, obj);
    }
    return scope.Close(arr);
}

Handle<Value> GifWrapper::Push(const Arguments& args){
    HandleScope scope;
    GifWrapper* gif = ObjectWrap::Unwrap<GifWrapper>(args.This());
    Local<v8::Object> obj = args[0]->ToObject();
    uchar *data = obj->GetIndexedPropertiesPixelData();
    gif->framesToEncode->push_back(data);
    return scope.Close(Undefined());
}

void GifWrapper::Initialize(Handle<Object> target) {
    HandleScope scope;
    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(GifWrapper::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(v8::String::NewSymbol("GifWrapper"));

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
    target->Set(v8::String::NewSymbol("GifWrapper"), constructor->GetFunction());
}

Handle<Value> GifWrapper::New(const Arguments& args) {
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
    GifWrapper* gif = new GifWrapper(width, height);
    cout << GIFLIB_MAJOR ; cout << "." <<  GIFLIB_MINOR;
    cout << "." << GIFLIB_RELEASE << endl;
    gif->Wrap(args.This());
    return (args.This());
}

GifWrapper::GifWrapper(int width, int height) {
    this->width = width;
    this->height = height;
    this->nbFrames = 0;
    this->delay = 100;
    this->paletteSize = 256;
    this->frames = new std::vector<uchar *>();
    this->framesToEncode = new std::vector<uchar *>();
    /* if ((GifFileOut = EGifOpenFileHandle(fileno(pFile), &ErrorCode)) == NULL) {
        cout << "Cannot open file for encoding gif" << endl;
        exit(EXIT_FAILURE);
    }*/

    }

    GifWrapper::~GifWrapper() {
        for (int i=this->frames->size() - 1; i >= 0; i--){
            delete this->frames->at(i);
            this->frames->erase(this->frames->begin()+ i);
        }
        for (int i=this->framesToEncode->size() - 1; i >= 0; i--){
            delete this->framesToEncode->at(i);
            this->framesToEncode->erase(this->framesToEncode->begin()+ i);
        }
    }

    v8::Handle<v8::Value>    GifWrapper::Info(const Arguments& args){
        HandleScope scope;
        GifWrapper* gif = ObjectWrap::Unwrap<GifWrapper>(args.This());
        cout << "GifInfo:\twidth: " << gif->width << endl;
        cout << "         \theight: "<< gif->height << endl;
        cout << "         \tnbFrames: "<< gif->nbFrames << endl;
        cout << "         \tdelay: "<< gif->delay << endl;

        return scope.Close(Undefined());
    }
