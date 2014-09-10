#include "testApp.h"
#include "PictureSlideShow.hpp"
#include "GeometWave.hpp"


void testApp::setup(){
    ofSetWindowShape(common::width, common::height);
	ofSetCircleResolution(60);
    
    //===============================================
    // setup syphon
    //===============================================
	mServer.setName("Screen Output");
	mClient.setup();
    
    //using Syphon app Simple Server, found at http://syphon.v002.info/
    mClient.set("","Simple Server");
	   
	ofSetFrameRate(60); // if vertical sync is off, we can go a bit fast... this caps the framerate at 60fps.
    
    mMainFbo.allocate(common::width, common::height);
    
    //===============================================
    // setup ofxPostGlitch
    //===============================================
    mPostGlitch = new ofxPostGlitch();
    mPostGlitch->setup(&mMainFbo);
    
    //===============================================
    // create contents list
    //===============================================
    mContents.push_back(shared_ptr<BaseContentsInterface>(new PictureSlideShow("")));
    mContents.push_back(shared_ptr<BaseContentsInterface>(new GeometWave()));
    
    mCurrentContents = 0;
    mContents[mCurrentContents]->setup();
    
    //===============================================
    // sound input
    //===============================================
    ofSetVerticalSync(true);
	ofSetCircleResolution(80);
	ofBackground(54, 54, 54);
	
	// 0 output channels,
	// 2 input channels
	// 44100 samples per second
	// 256 samples per buffer
	// 4 num buffers (latency)
	soundStream.listDevices();
	
	//if you want to set a different device id
	soundStream.setDeviceID(common::soundDeviceID); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
    
	int bufferSize = 256;
	
	
	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
	volHistory.assign(400, 0.0);
	
	bufferCounter	= 0;
	drawCounter		= 0;
	smoothedVol     = 0.0;
	scaledVol		= 0.0;
    
	soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    
    //===============================================
    // init values
    //===============================================
    mGuiPanel.setName("GUI");
    mParamGroup.setName("PARAMETERS");
    mParamGroup.add(mLevel.set("level", 0.0, 0.0, 1.0));
    mParamGroup.add(mGain.set("gain", 1.0, 0.0, 30.0));
    mParamGroup.add(mSmoothLevel.set("smooth_level", 0.8, 0.0, 1.0));
    mParamGroup.add(bClipping.set("clipping", false));
    mParamGroup.add(bDrawInputSoundStates.set("show_input_status", false));
    mGuiPanel.setup(mParamGroup);
    mGuiPanel.loadFromFile("settings.xml");
    
}

void testApp::update()
{
    //===============================================
    // sound input update
    //===============================================
    //lets scale the vol up to a 0-1 range
	scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    
    // add gain
    MAIN_LEVEL *= mGain;
    MAIN_LEVEL = bClipping ? ofClamp(MAIN_LEVEL, 0.0, 1.0) : MAIN_LEVEL;
    
	//lets record the volume into an array
	volHistory.push_back( scaledVol );
	
	//if we are bigger the the size we want to record - lets drop the oldest value
	if( volHistory.size() >= 400 ){
		volHistory.erase(volHistory.begin(), volHistory.begin()+1);
	}

    
    //===============================================
    // contents update
    //===============================================
    mContents[mCurrentContents]->updateSoundStatus(&MAIN_WAVE, &MAIN_LEVEL);
    mContents[mCurrentContents]->update();
    mLevel = MAIN_LEVEL;
}

void testApp::draw()
{
    //===============================================
    // content draw
    //===============================================
    mMainFbo.begin();
    ofBackground(0);
    mContents[mCurrentContents]->draw();
    mMainFbo.end();
    
    mPostGlitch->generateFx();
    mMainFbo.draw(0, 0);
    
    //===============================================
    // sound input draw
    //===============================================
    ofSetColor(255, 255, 255);
    if (bDrawInputSoundStates) drawInputSoundStates();
    
    //===============================================
    // gui draw
    //===============================================
    mGuiPanel.draw();
    
    stringstream s;
    s << "fps: " << ofGetFrameRate() << endl;
    s << "current content: " << mCurrentContents << endl;
    ofDrawBitmapString(s.str(), mGuiPanel.getPosition().x, mGuiPanel.getPosition().y + mGuiPanel.getHeight() + 20);
    
    // publish syphone server
    mServer.publishTexture(&mMainFbo.getTextureReference());
}

void testApp::exit()
{
    mGuiPanel.saveToFile("settings.xml");
}

void testApp::drawInputSoundStates()
{
    ofSetColor(225);
	ofDrawBitmapString("AUDIO INPUT EXAMPLE", 32, 32);
	
	ofNoFill();
	
	// draw the left channel:
	ofPushStyle();
    ofPushMatrix();
    ofTranslate(32, 170, 0);
    
    ofSetColor(225);
    ofDrawBitmapString("Left Channel", 4, 18);
    
    ofSetLineWidth(1);
    ofRect(0, 0, 512, 200);
    
    ofSetColor(245, 58, 135);
    ofSetLineWidth(3);
    
    ofBeginShape();
    for (unsigned int i = 0; i < left.size(); i++){
        ofVertex(i*2, 100 -left[i]*180.0f);
    }
    ofEndShape(false);
    
    ofPopMatrix();
	ofPopStyle();
    
	// draw the right channel:
	ofPushStyle();
    ofPushMatrix();
    ofTranslate(32, 370, 0);
    
    ofSetColor(225);
    ofDrawBitmapString("Right Channel", 4, 18);
    
    ofSetLineWidth(1);
    ofRect(0, 0, 512, 200);
    
    ofSetColor(245, 58, 135);
    ofSetLineWidth(3);
    
    ofBeginShape();
    for (unsigned int i = 0; i < right.size(); i++){
        ofVertex(i*2, 100 -right[i]*180.0f);
    }
    ofEndShape(false);
    
    ofPopMatrix();
	ofPopStyle();
	
	// draw the average volume:
	ofPushStyle();
    ofPushMatrix();
    ofTranslate(565, 170, 0);
    
    ofSetColor(225);
    ofDrawBitmapString("Scaled average vol (0-100): " + ofToString(scaledVol * 100.0, 0), 4, 18);
    ofRect(0, 0, 400, 400);
    
    ofSetColor(245, 58, 135);
    ofFill();
    ofCircle(200, 200, scaledVol * 190.0f);
    
    //lets draw the volume history as a graph
    ofBeginShape();
    for (unsigned int i = 0; i < volHistory.size(); i++){
        if( i == 0 ) ofVertex(i, 400);
        
        ofVertex(i, 400 - volHistory[i] * 70);
        
        if( i == volHistory.size() -1 ) ofVertex(i, 400);
    }
    ofEndShape(false);
    
    ofPopMatrix();
	ofPopStyle();
	
	drawCounter++;
	
	ofSetColor(225);
	string reportString = "buffers received: "+ofToString(bufferCounter)+"\ndraw routines called: "+ofToString(drawCounter)+"\nticks: " + ofToString(soundStream.getTickCount());
	ofDrawBitmapString(reportString, 32, 589);
}

void testApp::changeContents(const int num)
{
    mCurrentContents = num;
    if (mCurrentContents < 0) mCurrentContents = mContents.size() - 1;
    if (mCurrentContents >= mContents.size()) mCurrentContents = 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  KEY PRESSED                                                     //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void testApp::keyPressed(int key)
{
    switch (key) {
        case 'f': ofToggleFullscreen(); break;
        case ' ': bDrawInputSoundStates = !bDrawInputSoundStates; break;
        case OF_KEY_LEFT: changeContents(--mCurrentContents); break;
        case OF_KEY_RIGHT: changeContents(++mCurrentContents); break;
        case '1': mPostGlitch->toggleFx(OFXPOSTGLITCH_CONVERGENCE); break;
        default: mContents[mCurrentContents]->keyPressed(key); break;
    }
    
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  AUDIO IN                                                        //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void testApp::audioIn(float * input, int bufferSize, int nChannels)
{
	
	float curVol = 0.0;
	
	// samples are "interleaved"
	int numCounted = 0;
    
	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume
	for (int i = 0; i < bufferSize; i++){
		left[i]		= input[i*2]*0.5;
		right[i]	= input[i*2+1]*0.5;
        left[i] *= mGain;
        right[i] *= mGain;
        
		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted+=2;
        
        
	}
	
	//this is how we get the mean of rms :)
	curVol /= (float)numCounted;
	
	// this is how we get the root of rms :)
	curVol = sqrt( curVol );
	
	smoothedVol *= mSmoothLevel;
	smoothedVol += (-mSmoothLevel + 1) * curVol;
	
	bufferCounter++;
	
}
