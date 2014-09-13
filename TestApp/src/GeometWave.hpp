#pragma once

#include "BaseContentsInterface.h"

class GeometWave : public BaseContentsInterface
{
    list<shared_ptr<BaseAnimationUnit> > mPerticle;
    
    int mode;
    float mBitPer;
    
    ofParameter<ofColor> mColor;
    
    vector<RotationArc *> mArcs;
    
public:
    GeometWave()
    {
        base::mParamGroup.setName(base::getName());
        base::setParameter(mColor.set("color", ofColor(255,255,255,255),ofColor(0,0,0,0),ofColor(255,255,255,255)));
    }
    
    void setup()
    {
        mode = 0;
        mBitPer = 0.15;
        
        for (int i = 0; i < WAVE.size(); i++) {
            mArcs.push_back(new RotationArc(300, 600, ofRandom(30, 180), 0, ofRandom(5, 20)));
        }
    }
    
    void update()
    {
        for (anm_it it = mPerticle.begin(); it != mPerticle.end(); it++) {
            (*it)->update();
            if ((*it)->isDied()) mPerticle.erase(it);
        }
        
        for (vector<RotationArc *>::iterator it = mArcs.begin(); it != mArcs.end(); it++) {
            (*it)->update();
        }
    }
    
    void draw()
    {
        ofPushStyle();
        ofSetRectMode(OF_RECTMODE_CENTER);
//        const int size = WAVE.size() * mBitPer;
        const int size = WAVE.size() * (LEVEL * 0.1 + 0.05);
        const float level = LEVEL;
        ofSetLineWidth(1);
        ofNoFill();
        ofBeginShape();
        for (unsigned int i = 0; i < size; i++) {
            float x = ofMap(i, 0, size, 0, WIDTH);
            float y = (HEIGHT * 0.5) - WAVE[i] * (HEIGHT * 0.5);
            ofVertex(x, y);
        }
        ofEndShape(false);
        for (unsigned int i = 0; i < size; i++) {
            float x = ofMap(i, 0, size, 0, WIDTH);
            float y = (HEIGHT * 0.5) - WAVE[i] * (HEIGHT * 0.5);
            ofSetColor(mColor);
            drawPoint(x, y, LEVEL, i);
        }
        ofPopStyle();
        
//        mPerticle.push_back(shared_ptr<BaseAnimationUnit>(new Perticle(400, 300, ofRandom(-60, 40), ofRandom(-10, 10), 1, 0.9, 2000)));
//        for (anm_it it = mPerticle.begin(); it != mPerticle.end(); it++) {
//            (*it)->draw();
//        }
    }
    
    void getBang()
    {
        randamPattern();
    }
    
    void randamPattern()
    {
//        mBitPer = ofRandom(0.05, 0.3);
        mode = (int)ofRandom(5);
    }
    
    void drawPoint(int x, int y, float size, int i)
    {
        switch (mode) {
            default:
            case 0:
                ofFill();
                ofCircle(x, y, size * 10);
                ofNoFill();
                ofCircle(x, y, size * 20);
                ofCircle(x, y, size * 30);
                break;
                
            case 1:
                ofFill();
                ofRect(x, y, size * 7, size * 7);
                ofNoFill();
                ofRect(x, y, size * 30, size * 10);
                ofRect(x, y, size * 10, size * 30);
                break;
                
            case 2:
                ofFill();
                mArcs[i]->pos.set(x, y);
                mArcs[i]->size = size * 70;
                mArcs[i]->draw();
                break;
                
            case 3:
                ofNoFill();
                ofSetLineWidth(2);
                mArcs[i]->pos.set(x, y);
                mArcs[i]->size = size * 100;
                mArcs[i]->draw();
                break;
                
        }
    }
    
};