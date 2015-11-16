#pragma once

#include "ofMain.h"
#include "common.h"
#include "Primitive.h"
#include "ofxGui.h"
#include "ofxPostGlitch.h"
#include "OscReceiverController.hpp"

#define LEVEL mLevel

class testApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void audioIn(float * input, int bufferSize, int nChannels);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void randomiseAll();
    void changeDrawMode(int mode);
    void generateNoise();
    void gotBang(OscBang & e);
    
    ofSoundStream soundStream;
    vector <float> left;
    
    int 	bufferCounter;
    
    float smoothedVol;
    
    vector<Primitive>prims;
    
    static const int objectNum = 100; //何個描画するか。
    
    bool bang;
    bool bHide;
    ofxPanel panel;
//    ofParameter<float> mLevel;
    ofParameter<ofVec3f> pos;
    ofParameter<ofVec3f> size;
    ofParameter<ofVec3f> rotate;
    ofParameter<float> thresh;
    ofParameter<int> repeat;
    ofParameter<ofColor> color;
    ofParameter<bool> bGlobalRotate;
    ofParameter<bool> bFillTog;
    ofParameter<bool> isRand;
//    ofParameter<int> mSelDrawMode;
//    ofxToggle isRandDrawMode;
    bool bGlitchTog;
    bool isRandDrawMode;
    
    slide globalRotate;
    
    bool bFill;
    bool bReverse;
    ofParameter<bool> bReverseTog;
    int rotAxisMode;
    
    ofParameter<float> mGain;
    
    ofxPostGlitch mGlitch;
    ofFbo mFbo;
    
    // osc
    OscReceiverController mOscReceiver;
    float mLevel;
};
