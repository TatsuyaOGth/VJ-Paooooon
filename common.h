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

