#pragma once

#include "../../common.h"

class slide : public ofPoint{
private:
    
    ofPoint p;
    
    float speed;
    
public:
    
    slide():speed(0.1){
    }
    
    void set(float _px,float _py,float _pz){
        p.x = _px;
        p.y = _py;
        p.z = _pz;
    }
    
    void set(float _px,float _py){
        p.x = _px;
        p.y = _py;
    }
    
    void imSet(float _px,float _py,float _pz){
        p.x = _px;
        p.y = _py;
        p.z = _pz;
        
        x = _px;
        y = _py;
        z = _pz;
    }
    
    void imSet(float _px,float _py){
        p.x = _px;
        p.y = _py;
        
        x = _px;
        y = _py;
        
    }
    
    void update(){
        x += (p.x - x) * speed;
        y += (p.y - y) * speed;
        z += (p.z - z) * speed;
    }
};

class PrimitiveObject
{
public:
    
    ofPoint pos;
    ofVec3f size;
    ofVec3f rotate;
    ofColor color;
    int drawMode;
    bool bFill;
    float audioVal;
    int rotAxisMode;
    
    PrimitiveObject():bFill(false),rotAxisMode(1){
        color.set(255,255,255,255);
        size.set(10,10);
        drawMode = 0;
        pos.set(WIDTH * 0.5 ,HEIGHT * 0.5);
    }
    void update(){
        size.set(audioVal*200.0,audioVal*200.0);
    }
    
    void draw(){
        ofPushMatrix();
        ofPushStyle();
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofSetSphereResolution(2);
        if(bFill)ofFill();
        else ofNoFill();
        
        ofSetColor(color.r,color.g,color.b,color.a);
        
        if(rotAxisMode == 1)ofTranslate(pos.x,pos.y,pos.z);
        else if(rotAxisMode == 0)ofTranslate(WIDTH * 0.5, HEIGHT * 0.5);
        
        ofRotateX(rotate.x);
        ofRotateY(rotate.y);
        ofRotateZ(rotate.z);
        
        if(rotAxisMode == 0){
            ofTranslate(-WIDTH * 0.5, -HEIGHT * 0.5);
            ofTranslate(pos.x,pos.y,pos.z);
        }
        if(drawMode == 0) ofDrawBox(0,0,0,size.x,size.y,size.z);
        else ofDrawSphere(0,0,0,size.x * 0.5);
//        ofDrawSphere(0,0,0,size.x * 0.5);
        
        ofPopStyle();
        ofPopMatrix();
        
    }
};
