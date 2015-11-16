#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    int bufferSize = 256;
    soundStream.setup(this, 0, 1, 44100, bufferSize, 4);
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    left.resize(bufferSize, 0.0);
    
    for(int i = 0; i < objectNum; i++){
        Primitive drawObject;
        drawObject.drawMode = 1;
        
        drawObject.rotAxisMode = 0;
        prims.push_back(drawObject);
    }
    
    ofSetSphereResolution(2);
    
    bang = false;
    bHide = false;
    
    panel.setup("panel");
//    panel.add(mLevel.set("level", 0.0, 0.0, 1.0));
    panel.add(pos.set("pos",ofVec3f(0,0,0),ofVec3f(0,0,0),ofVec3f(1200,1200,1200)));
    panel.add(size.set("scale",ofVec3f(50,60,80),ofVec3f(0,0,0),ofVec3f(1000,1000,1000)));
    panel.add(rotate.set("rotate",ofVec3f(0,0,100),ofVec3f(0,0,0),ofVec3f(360.0,360.0,360.0)));
    panel.add(bGlobalRotate.set("GlobalRotate",false));
    panel.add(thresh.set("thresh", 0.25, 0.0, 1.0));
    panel.add(color.set("color",ofColor(255,255,255,255),ofColor(0,0,0,0),ofColor(255,255,255,255)));
    panel.add(bFillTog.set("fIll", false));
    panel.add(bReverseTog.set("reverseTog",false));
    panel.add(mGain.set("gain", 1, 0, 10));
    panel.add(isRand.set("randam", false));
//    panel.add(mSelDrawMode.set("drawmode", 0, 0, 5));
    
    panel.loadFromFile("settings.xml");
    isRandDrawMode = false;
    
    mFbo.allocate(WIDTH, HEIGHT, GL_RGBA);
    mGlitch.setup(&mFbo);
    bGlitchTog = false;
    
    mOscReceiver.setPort(9000);
    ofAddListener(mOscReceiver.events, this, &testApp::gotBang);
}

//--------------------------------------------------------------
void testApp::update(){
    
    mLevel = mOscReceiver.getValue("/level", 0);
    
    if(bang == false && LEVEL > thresh){
        bang = true; //音量がthreshold超えたらthreshold以下になるまでは処理しないよ！
        randomiseAll();
    }
    
//    if(bFill.x && ofGetFrameNum() % 5 == 0){
//        bFill.y = !bFill.y;
//    }
    
    if(bGlobalRotate == false){
        globalRotate.set(0,0,0);
    }
    
    rotate.set(ofVec3f(rotate->x,rotate->y,rotate->z));
    globalRotate.update();
    
    if(LEVEL < thresh){
        bang = false;
    }
    
    //ここからはランダマイズされた値をインスタンスに突っ込んだりするよ！
    
    float aVal = 255;
    if(bFill)aVal = 60; //ofFillかましてると明る過ぎるので適当にアルファ下げるよ
    
    float tLevel = ofMap(LEVEL, 0, 1, 20, aVal);
//    color.set(ofColor(color->r,color->g,color->b,(int)(LEVEL * aVal)));
    color.set(ofColor(color->r,color->g,color->b, tLevel));
//    color.set(ofColor(mOscReceiver.getValue("/rgb", 0), mOscReceiver.getValue("/rgb", 1), mOscReceiver.getValue("/rgb", 2), tLevel));

    
    ofColor currentColor = color;
//    if(bReverse)currentColor.set(0,0,0,(int)(LEVEL * aVal));
    if(bReverse)currentColor.set(0,0,0,255);

    
    float cVol = LEVEL * 0.06;
    
    for(int i = 0; i <prims.size(); i++){
        prims[i].audioVal = LEVEL;
        
        prims[i].pos.x = (i - prims.size() * 0.5) * cVol * pos->x + WIDTH * 0.5;
        prims[i].pos.y = (i - prims.size() * 0.5) * cVol * pos->y + HEIGHT * 0.5;
        prims[i].pos.z = (i - prims.size() * 0.5) * cVol * pos->z;
        
        
        prims[i].rotate.x = i * cVol * rotate->x;
        prims[i].rotate.y = i * cVol * rotate->y;
        prims[i].rotate.z = i * cVol * rotate->z;
        
        prims[i].size.x = i * cVol * size->x + 10;
        prims[i].size.y = i * cVol * size->y + 10;
        prims[i].size.z = i * cVol * size->z + 10;
        
        prims[i].color = currentColor;
        
        prims[i].bFill = bFill;
        prims[i].rotAxisMode = rotAxisMode;
        prims[i].update();
    }
    
//    mLevel = ofClamp(LEVEL, 0, 1);
}

//--------------------------------------------------------------
void testApp::draw(){

    //ofBackgroundGradient(ofFloatColor(1,1,1,1), ofFloatColor(0,0,0,0));
    mFbo.begin();

    ofSetColor(255, 255, 255);
    
    if(bReverse == false){
        ofBackground(0,0,0,0);
        ofEnableBlendMode(OF_BLENDMODE_ADD);
    }
    else {
        ofBackground(190);
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    }
    
    ofPushMatrix();
    ofTranslate(WIDTH*0.5, HEIGHT*0.5);
    ofRotateX(globalRotate.x);
    ofRotateY(globalRotate.y);
    ofRotateZ(globalRotate.z);
    ofTranslate(-WIDTH*0.5, -HEIGHT*0.5);
    
    for(int i = 0; i <prims.size(); i++){
        prims[i].draw();
    }
    
    ofPopMatrix();
    
    mFbo.end();
    
    mGlitch.generateFx();
    
    ofSetColor(255, 255, 255);
    mFbo.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    if(bHide == false){
        panel.draw();
        stringstream s;
        s << "lebel: " << LEVEL << endl;
        s << "isRandDrawMode: " << isRandDrawMode << endl;
        s << "isGlitchRoggle: " << bGlitchTog << endl;
        ofDrawBitmapString(s.str(), panel.getShape().getMinX(), panel.getShape().getMaxY() + 20);
        ofRect(panel.getShape().getMinX(), panel.getShape().getMaxY() + 50, ofMap(LEVEL, 0, 1, 0, panel.getWidth(), true), 5);
    }
}

void testApp::exit() {
    panel.saveToFile("settings.xml");
}



//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if( key == 'a' ){
		isRand = !isRand;
	}
    
    if( key == 'h' ){
		bHide = !bHide;
	}
    
    if( key == 'z' ){
		bGlobalRotate = !bGlobalRotate;
	}
    
    if( key == 'x' ){
		bFillTog = !bFillTog;
	}
    
    if( key == 'c' ){
		bReverseTog = !bReverseTog;
	}
    
    if( key == 'f' ){
        ofToggleFullscreen();
	}
    
    if (key == 'v') isRandDrawMode ^= true;
    if (key == 'b') randomiseAll();
    if (key == 'g') bGlitchTog = !bGlitchTog;
    
    if (key == '1') mGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE	, true);
    if (key == '2') mGlitch.setFx(OFXPOSTGLITCH_GLOW			, true);
    if (key == '3') mGlitch.setFx(OFXPOSTGLITCH_SHAKER			, true);
    if (key == '4') mGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER		, true);
    if (key == '5') mGlitch.setFx(OFXPOSTGLITCH_TWIST			, true);
    if (key == '6') mGlitch.setFx(OFXPOSTGLITCH_OUTLINE		, true);
    if (key == '7') mGlitch.setFx(OFXPOSTGLITCH_NOISE			, true);
    if (key == '8') mGlitch.setFx(OFXPOSTGLITCH_SLITSCAN		, true);
    if (key == '9') mGlitch.setFx(OFXPOSTGLITCH_SWELL			, true);
    if (key == '0') mGlitch.setFx(OFXPOSTGLITCH_INVERT			, true);
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if (key == '1') mGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE	, false);
    if (key == '2') mGlitch.setFx(OFXPOSTGLITCH_GLOW			, false);
    if (key == '3') mGlitch.setFx(OFXPOSTGLITCH_SHAKER			, false);
    if (key == '4') mGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER		, false);
    if (key == '5') mGlitch.setFx(OFXPOSTGLITCH_TWIST			, false);
    if (key == '6') mGlitch.setFx(OFXPOSTGLITCH_OUTLINE		, false);
    if (key == '7') mGlitch.setFx(OFXPOSTGLITCH_NOISE			, false);
    if (key == '8') mGlitch.setFx(OFXPOSTGLITCH_SLITSCAN		, false);
    if (key == '9') mGlitch.setFx(OFXPOSTGLITCH_SWELL			, false);
    if (key == '0') mGlitch.setFx(OFXPOSTGLITCH_INVERT			, false);
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

void testApp::randomiseAll(){
    
    if(isRand) {
        int rand = (int)ofRandom(0,5);
        if      (rand == 0)pos.set(ofVec3f(1200,0,0));
        else if (rand == 1)pos.set(ofVec3f(0,1200,0));
        else if (rand == 2)pos.set(ofVec3f(0,0,1200));
        else if (rand == 3)pos.set(ofVec3f(ofRandom(50,200),ofRandom(50,200),ofRandom(50,200)));
        else if (rand == 4)pos.set(ofVec3f(0,0,0));
        
        rand = (int)ofRandom(0,5);
        if      (rand == 0)rotate.set(ofVec3f(100,0,0));
        else if (rand == 1)rotate.set(ofVec3f(0,100,0));
        else if (rand == 2)rotate.set(ofVec3f(0,0,100));
        else if (rand == 3)rotate.set(ofVec3f(ofRandom(0,100),ofRandom(0,100),ofRandom(0,100)));
        else if (rand == 4)rotate.set(ofVec3f(0,0,0));
        
        rand = (int)ofRandom(0,4);
        if      (rand == 0)size.set(ofVec3f(ofRandom(100,300),200,200));
        else if (rand == 1)size.set(ofVec3f(200,ofRandom(100,300),0));
        else if (rand == 2)size.set(ofVec3f(0,200,ofRandom(100,300)));
        else if (rand == 3)size.set(ofVec3f(ofRandom(0,300),ofRandom(0,300),ofRandom(0,300)));
    }
    
    if((int)ofRandom(0,5))rotAxisMode = !rotAxisMode;
    
    if(bFillTog){
        bFill = !bFill;
    }
    
    if(bReverseTog){
        bReverse = !bReverse;
    }
    
    if(bGlobalRotate){
        globalRotate.set(ofRandom(0,720),
                         ofRandom(0,720),
                         ofRandom(0,720));
        
        // rotate.set(ofVec3f(ofRandom(0,100),ofRandom(0,100),ofRandom(0,100)));
    }
    
    if (isRandDrawMode) {
        changeDrawMode((int)ofRandom(5));
    }
    
    if (bGlitchTog) generateNoise();
    
}

void testApp::changeDrawMode(int mode)
{
    for(int i = 0; i < objectNum; i++) prims[i].drawMode = mode;
//    switch (mode) {
//        case 0: for(int i = 0; i < objectNum; i++) prims[i].drawMode = mode; break;
//            
//        default:
//            break;
//    }
}

void testApp::generateNoise()
{
    
    int rnd = ofRandom(8);
    int t   = ofRandom(0.2, 0.5);
    switch (rnd) {
        case 0: mGlitch.setFxTo(OFXPOSTGLITCH_CONVERGENCE, t); break;
        case 1: mGlitch.setFxTo(OFXPOSTGLITCH_SHAKER, t); break;
        case 2: mGlitch.setFxTo(OFXPOSTGLITCH_CUTSLIDER, t); break;
        case 3: mGlitch.setFxTo(OFXPOSTGLITCH_TWIST, t); break;
        case 4: mGlitch.setFxTo(OFXPOSTGLITCH_OUTLINE, t); break;
        case 5: mGlitch.setFxTo(OFXPOSTGLITCH_NOISE, t); break;
        case 6: mGlitch.setFxTo(OFXPOSTGLITCH_SLITSCAN, t); break;
        case 7: mGlitch.setFxTo(OFXPOSTGLITCH_SWELL, t); break;
    }
}

void testApp::gotBang(OscBang &e)
{
    bang = true;
}

void testApp::audioIn(float * input, int bufferSize, int nChannels){
	if (left.empty()) return;
    
	float curVol = 0.0;
	
	// samples are "interleaved"
	int numCounted = 0;
    
	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume
    
	for (int i = 0; i < bufferSize; i++){
		left[i]	= input[i];
        left[i] *= mGain;
		curVol += left[i] * left[i];
		numCounted++;
	}
	
	//this is how we get the mean of rms :)
	curVol /= (float)numCounted;
	
	// this is how we get the root of rms :)
	curVol = sqrt( curVol );
	
	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;
	
	bufferCounter++;
	
}