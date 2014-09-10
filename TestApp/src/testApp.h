#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxGui.h"
#include "ofxPostGlitch.h"

#include "BaseContentsInterface.h"


#define MAIN_LEVEL  scaledVol
#define MAIN_WAVE   left


class testApp : public ofBaseApp
{
	
public:
	
	void setup();
	void update();
	void draw();
    void exit();
	void keyPressed(int key);
    void audioIn(float * input, int bufferSize, int nChannels);
    
    
private:
    
    void drawInputSoundStates();
    void changeContents(const int num);
    
    // syphone
	ofxSyphonServer mServer;
	ofxSyphonClient mClient;
    ofFbo mMainFbo;
    
    // contents
    vector<shared_ptr<BaseContentsInterface> > mContents;
    typedef vector<shared_ptr<BaseContentsInterface> >::iterator content_it;
    int mCurrentContents;
    
    // sount input
    ofSoundStream soundStream;
    WAVE_TYPE left;
    WAVE_TYPE right;
    vector <float> volHistory;
    int 	bufferCounter;
    int 	drawCounter;
    float smoothedVol;
    float scaledVol;
    
    // gui
    ofxPanel mGuiPanel;
    ofParameterGroup mParamGroup;
    ofParameter<float> mLevel;
    ofParameter<float> mGain;
    ofParameter<float> mSmoothLevel;
    ofParameter<bool> bClipping;
    ofParameter<bool> bDrawInputSoundStates;
    
    // post glitch
    ofxPostGlitch * mPostGlitch;
};

#endif

