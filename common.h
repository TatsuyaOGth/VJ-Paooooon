#pragma once

//==============================================================
// common parameters
//==============================================================
namespace common
{
    // main window size
    static const float width = 1200;
    static const float height = 800;
    
    // sound devise
    static const int soundDeviceID = 3;
    static const int bufferSize = 256;
}

namespace share
{
    static float elapsedTime = 0;
}

#define WIDTH   common::width
#define HEIGHT  common::height


//==============================================================
// share types
//==============================================================
typedef vector<float>   WAVE_TYPE;
typedef float           LEVEL_TYPE;


//==============================================================
// share data
//==============================================================
typedef ofImage SHARED_IMAGE_TYPE;
typedef vector<SHARED_IMAGE_TYPE> SHARED_IMAGES_CONTAINER_TYPE;
typedef vector<ofVideoPlayer> SHARED_VIDEOS_CONTAINER_TYPE;


class SharedData
{
    SharedData(); // singleton
    
    static SHARED_IMAGES_CONTAINER_TYPE mImages;
    static SHARED_VIDEOS_CONTAINER_TYPE mVideos;
    
public:
    
    static SHARED_IMAGES_CONTAINER_TYPE * getImagesPtr() { return &mImages; }
    static SHARED_VIDEOS_CONTAINER_TYPE * getVideosPtr() { return &mVideos; }
    
    static SHARED_IMAGES_CONTAINER_TYPE & getImagesRef() { return mImages; }
    static SHARED_VIDEOS_CONTAINER_TYPE & getVideosRef() { return mVideos; }
    
    static SHARED_IMAGE_TYPE * getImagesAt(int at)
    {
        if (!mImages.empty()) {
            if (at >= 0 && at < mImages.size()) {
                return &mImages[at];
            }
        }
        return NULL;
    }
};

