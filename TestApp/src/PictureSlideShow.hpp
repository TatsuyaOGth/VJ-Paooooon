#pragma once

#include "BaseContentsInterface.h"

class PictureSlideShow : public BaseContentsInterface
{
    string mDirPath;
    ofDirectory mDir;
    
public:
    PictureSlideShow(const string& dirPath):
    mDirPath(dirPath)
    {
    }
    
    void setup()
    {
        if (!mDirPath.empty()) {
            if (mDir.listDir(mDirPath) == 0) {
                ofLogError() << "not picture" ;
            }
        }
        
    }
    
    void update()
    {
        
    }
    
    void draw()
    {
        
    }
    
};
