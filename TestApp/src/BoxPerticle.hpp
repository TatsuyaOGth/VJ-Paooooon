#pragma once

#include "BaseContentsInterface.h"
#include "ofxEasingCam.h"

class BoxPerticle : public BaseContentsInterface
{
    
//    ofImage mTexImg; // texture
    ofTexture * mTex;
    ofLight light;

    
    const int maxNumBox;
    vector<ofBoxPrimitive> mBox;
    typedef vector<ofBoxPrimitive>::iterator box_it;
    
    ofxEasingCam rollCam;
    
    
public:
    ofParameter<int> mBoxNum;
    ofParameter<float> mRotationSpeed;
    ofParameter<float> mBoxSize;
    ofParameter<float> mSpacing;
    ofParameter<float> mCloudSize;
    
    BoxPerticle(ofTexture * tex = NULL, int max_num_box = 400):mTex(tex), maxNumBox(max_num_box)
    {
        base::mParamGroup.add(mBoxNum.set("num_box", 10, 1, maxNumBox));
        base::mParamGroup.add(mRotationSpeed.set("rotation_speed", 0.1, 0, 0.4));
        base::mParamGroup.add(mBoxSize.set("box_size", 10, 0, 600));
        base::mParamGroup.add(mSpacing.set("spaceing", 10, 1, 100));
        base::mParamGroup.add(mCloudSize.set("cloud_size", 600, 100, 2000));
        
        for (int i = 0; i < maxNumBox; i++) {
            mBox.push_back(ofBoxPrimitive());
        }
    }
    
    void setup()
    {
        rollCam.setup();
        rollCam.setPos(0, 0, 800);
        rollCam.setCamSpeed(0.05);
        
//        light.setup();

        // setup boxs
        for (vector<ofBoxPrimitive>::iterator box_it = mBox.begin(); box_it != mBox.end(); box_it++) {
            box_it->setResolution(20);
            box_it->setMode( OF_PRIMITIVE_TRIANGLE_STRIP );
            vector<ofMeshFace> triangles = box_it->getMesh().getUniqueFaces();
            box_it->getMesh().setFromTriangles( triangles, true );
        }
    }
    
    void update()
    {
        float et = ofGetElapsedTimef();
        for (int i = 0; i < mBoxNum; i++) {
            float t = (et + i * mSpacing) * mRotationSpeed;
            ofVec3f pos(ofSignedNoise(t, 0, 0),
                        ofSignedNoise(0, t, 0),
                        ofSignedNoise(0, 0, t));
            float boxSize = mBoxSize * ofNoise(pos.x, pos.y, pos.z);
            
            pos *= mCloudSize;
            mBox[i].setPosition(pos);
            mBox[i].rotate(pos.x * 0.01, 1, 0, 0);
            mBox[i].rotate(pos.y * 0.01, 0, 1, 0);
            mBox[i].rotate(pos.z * 0.01, 0, 0, 1);
            mBox[i].set(boxSize);
        }
        
        rollCam.update();
    }
    
    void draw()
    {
        ofEnableLighting();
        light.setPosition(0, 0, 0);
        light.enable();
        ofEnableDepthTest();
        
        rollCam.begin(getWidth(), getHeight());
        
        ofFill();

        if (mTex != NULL && mTex->isAllocated()) {
            // TODO: i dont know why cant bind
            mTex->bind();
            for (int i = 0; i < mBoxNum; i++) {
                mBox[i].draw();
            }
            mTex->unbind();
        } else {
            for (int i = 0; i < mBoxNum; i++) {
                mBox[i].draw();
            }
        }

        
        rollCam.end();
        
        light.disable();
        ofDisableLighting();
        ofDisableDepthTest();
    }

    void getBang()
    {
        
    }
    

};

