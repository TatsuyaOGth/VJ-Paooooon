#pragma once

#include "BaseContentsInterface.h"

class GeometWave : public BaseContentsInterface
{
    list<shared_ptr<BaseAnimationUnit> > mPerticle;
    
public:
    GeometWave()
    {
    }
    
    void setup()
    {
        
    }
    
    void update()
    {
        for (anm_it it = mPerticle.begin(); it != mPerticle.end(); it++) {
            (*it)->update();
            if ((*it)->isDied()) mPerticle.erase(it);
        }
    }
    
    void draw()
    {
        ofPushStyle();
        ofSetRectMode(OF_RECTMODE_CENTER);
        const int size = base::getWave().size() * 0.15;
        const float level = base::getLevel();
        ofSetLineWidth(1);
        ofSetColor(255, 255, 255, 255);
        ofNoFill();
        ofBeginShape();
        for (unsigned int i = 0; i < size; i++) {
            float x = ofMap(i, 0, size, 0, WIDTH);
            float y = (HEIGHT * 0.5) - base::getWave()[i] * (HEIGHT * 0.5);
            ofVertex(x, y);
            ofCircle(x, y, level * 10, level * 10);
        }
        
        ofEndShape(false);
        ofPopStyle();
        
        mPerticle.push_back(shared_ptr<BaseAnimationUnit>(new Perticle(400, 300, ofRandom(-60, 40), ofRandom(-10, 10), 1, 0.9, 2000)));
        
        for (anm_it it = mPerticle.begin(); it != mPerticle.end(); it++) {
            (*it)->draw();
        }
        
    }
    
};