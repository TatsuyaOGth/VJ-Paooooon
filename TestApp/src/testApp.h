#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "ofxPostGlitch.h"

#include "BaseContentsInterface.h"

// input mode
//#define USE_OF_AUDIO_IN
#define USE_OSC_RECEIVER

#define MAIN_LEVEL  mLevel
#define MAIN_WAVE   left

#define CONTENT_LIST_TYPE vector
#define VIDEO_TYPE ofVideoPlayer

static const int RECEIVE_PORT = 9000;


class Media
{
    float duration;
    float counter;
    bool bDidStartChange;
    bool bHorizon;
    int current;
    float diff;
    bool additional;
    float diff2;
    
    typedef struct
    {
        int current;
        float counter;
        float duration;
        bool bHorizon;
        bool additional;
        float imgW;
        float imgH;
        float diffImgW;
        float diffImgH;
        float diffImgW2;
        float diffImgH2;
        float x, y, w, h;
        
        void update() {
            float a;
            if (counter > duration * 0.7) {
                a = ((duration - counter) / (duration * 0.3)) * 255;
            } else if (counter < duration * 0.3) {
                a = ofMap(counter, 0, duration * 0.3, 0, 255, true);
            } else {
                a = 255;
            }
            
            if (bHorizon) {
                ofSetColor(255, 255, 255, a);
                x = ofMap(counter, 0, duration, -diffImgW, 0);
                y = 0;
                w = imgW + (additional ? diffImgW2 : diffImgW);
                h = HEIGHT;
            } else {
                ofSetColor(255, 255, 255, a);
                x = 0;
                y = ofMap(counter, 0, duration, -diffImgH, 0);
                w = WIDTH;
                h = imgH + (additional ? diffImgH2 : diffImgH);
            }
            counter -= ofGetLastFrameTime();
        }
    } picture;
    deque<picture> mPictures;
    
public:
    vector<ofImage> mImages;
    vector<VIDEO_TYPE> mVideos;
    bool bVideo;
    int mSelVideo;
    ofFbo mFbo;
    
    Media()
    {
        mFbo.allocate(WIDTH, HEIGHT, GL_RGBA);
        duration = 5;
        counter = 3;
        bDidStartChange = false;
        bHorizon = false;
        additional = false;
        diff2 = 0;
    }
    
    inline bool loadImages(const string & path)
    {
        ofDirectory dir;
        if (dir.listDir(path) > 0) {
            for (int i = 0; i < dir.size(); i++) {
                mImages.push_back(ofImage());
                if (mImages.back().loadImage(dir.getPath(i))) {
                    ofLogNotice() << "load image: " << dir.getPath(i);
                } else {
                    ofLogError() << "faild load picture: " << dir.getPath(i);
                    return false;
                }
            }
        } else {
            ofLogError() << "faild load pictures dir";
            return false;
        }
        return true;
    }
    
    inline bool loadVideos(const string & path)
    {
        ofDirectory dir;
        if (dir.listDir(path) > 0) {
            for (int i = 0; i < dir.size(); i++) {
                mVideos.push_back(VIDEO_TYPE());
                if (mVideos.back().loadMovie(dir.getPath(i))) {
                    mVideos.back().setVolume(0);
                    ofLogNotice() << "load video: " << dir.getPath(i);
                } else {
                    ofLogError() << "faild load video: " << dir.getPath(i);
                    return false;
                }
            }
        } else {
            ofLogError() << "faild load videos dir";
            return false;
        }
        return true;
    }
    
    inline void update()
    {
        mFbo.begin();
        ofClear(0, 0, 0);
        ofSetColor(255, 255, 255);
        
        if (bVideo) {
            // video
            if (mVideos[mSelVideo].isPaused()) mVideos[mSelVideo].play();
            mVideos[mSelVideo].update();
            mVideos[mSelVideo].draw(0, 0, WIDTH, HEIGHT);
            
        } else {
            if (mVideos[mSelVideo].isPlaying()) mVideos[mSelVideo].stop();
            
            
            // image
            if (counter < 0) {
                counter = duration;
                int tmp = current;
                additional = false;
                diff2 = 0;
                for (int i = 0; i < 10000; i++) {
                    current = ofRandom(0, mImages.size());
                    if (current != tmp) break;
                }

                float imgW = mImages[current].getWidth();
                float imgH = mImages[current].getHeight();
                
                if (imgW > imgH) {
                    bHorizon = true;
                } else {
                    bHorizon = false;
                }
                if (bHorizon) {
                    diff = (HEIGHT - imgH) / HEIGHT;
                    if (imgW + (imgW * diff) < WIDTH) {
                        bHorizon = false;
                        additional = true;
                        diff  = (WIDTH - imgW) / WIDTH;
                        diff2 = (WIDTH - (imgW + (imgW * diff))) / WIDTH;
                    }
                } else {
                    diff = (WIDTH - imgW) / WIDTH;
                    if (imgH + (imgH * diff) < HEIGHT) {
                        bHorizon = true;
                        additional = true;
                        diff = (HEIGHT - imgH) / HEIGHT;
                        diff2 = (HEIGHT - (imgH + (imgH * diff))) / HEIGHT;
                    }
                }
                
                float diffImgW = mImages[current].getWidth() * (additional ? diff2 : diff);
                float diffImgH = mImages[current].getHeight() * (additional ? diff2 : diff);
                float diffImgW2 = mImages[current].getWidth() * (diff + diff2);
                float diffImgH2 = mImages[current].getHeight() * (diff + diff2);
                
//                picture p = {&mImages[current], current, duration, duration, bHorizon, imgW, imgH, diffImgW, diffImgH, diffImgW2, diffImgH2};
                mPictures.push_back(picture());
                mPictures.back().current = current;
                mPictures.back().counter = duration;
                mPictures.back().duration = duration;
                mPictures.back().bHorizon = bHorizon;
                mPictures.back().additional = additional;
                mPictures.back().imgW = imgW;
                mPictures.back().imgH = imgH;
                mPictures.back().diffImgW = diffImgW;
                mPictures.back().diffImgH = diffImgH;
                mPictures.back().diffImgW2 = diffImgW2;
                mPictures.back().diffImgH2 = diffImgH2;
                
            } else {
                counter -= ofGetLastFrameTime();
            }
            
            ofEnableAlphaBlending();
            deque<picture>::iterator it = mPictures.begin();
            while (it != mPictures.end()) {
                it->update();
                mImages[it->current].draw(it->x, it->y, it->w, it->h);
                if (it->counter < 0) {
                    it = mPictures.erase(it);
                } else ++it;
            }
            ofDisableAlphaBlending();
            
        }
        mFbo.end();
    }
    
};


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
    void addContentsSwitch(const int add);
    void removeContentsSwitch(const int rm);
    void toggleContentsSwitch(const int toggle);
    bool checkUnique(const int num);
    void sendBang();
    void generateWave(WAVE_TYPE & wave);
    void generateNoise();
    void stopNoise();
    
    
    // syphone
	ofxSyphonServer mServer;
	ofxSyphonClient mClient;
    ofFbo mMainFbo;
    
    // contents
    CONTENT_LIST_TYPE<shared_ptr<BaseContentsInterface> > mContents;
    typedef CONTENT_LIST_TYPE<shared_ptr<BaseContentsInterface> >::iterator content_it;
    deque<int> mContentsSwitches;
    
    // sount input
    ofSoundStream soundStream;
    WAVE_TYPE left;
    WAVE_TYPE right;
    vector <float> volHistory;
    int 	bufferCounter;
    int 	drawCounter;
    float smoothedVol;
    float scaledVol;
    
    // osc
    ofxOscReceiver mReceiver;
    
    // gui
    ofxPanel mGuiPanel;
    ofParameterGroup mParamGroup;
    ofParameter<float> mLevel;
    ofParameter<float> mGain;
    ofParameter<float> mSmoothLevel;
    ofParameter<bool> bDrawInputSoundStates;
    ofParameter<bool> bVideo;
    ofParameter<int> mSelVideo;
    ofParameter<bool> bNoise;
    
    // post glitch
    ofxPostGlitch mPostGlitch;
    int mGlitchTime;
    int mGlitchCount;
    
    // textures
    Media media;
    
    ofShader shader;
//    vector<ofImage> mImages;
//    vector<VIDEO_TYPE> mVideos;
//    ofTexture mTex;

};

#endif

