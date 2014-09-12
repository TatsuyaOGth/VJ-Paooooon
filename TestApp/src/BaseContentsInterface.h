#pragma once

#include "ofMain.h"
#include "AnimationUnits.hpp"
#include "../../common.h"

#define WIDTH   common::width
#define HEIGHT  common::height


class BaseContentsInterface
{
public:
    BaseContentsInterface():mLevel(0),mWave(0){}
    virtual ~BaseContentsInterface(){}
    
    virtual void setup(){}
    virtual void update(){}
    virtual void draw(){}
    virtual void exit(){}
    
    virtual void windowResized(int w, int h){}
    
    virtual void keyPressed( int key ){}
    virtual void keyReleased( int key ){}
    
    virtual void mouseMoved( int x, int y ){}
    virtual void mouseDragged( int x, int y, int button ){}
    virtual void mousePressed( int x, int y, int button ){}
    virtual void mouseReleased(int x, int y, int button ){}
    
    virtual void dragEvent(ofDragInfo dragInfo) { }
    virtual void gotMessage(ofMessage msg){ }
    
    // update sound status
    void updateSoundStatus(WAVE_TYPE * wave, LEVEL_TYPE level)
    {
        mWave = wave;
        mLevel = level;
    }
    virtual void getBang(){}
    
protected:
    WAVE_TYPE *     getWavePtr() const { return  mWave; }
    WAVE_TYPE       getWave() const { return  *mWave; }
    LEVEL_TYPE      getLevel() const { return mLevel; }
    
    typedef BaseContentsInterface base;
    typedef list<shared_ptr<BaseAnimationUnit> >::iterator anm_it;
    
private:
    LEVEL_TYPE mLevel;
    WAVE_TYPE  * mWave;
    
};
