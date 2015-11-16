#pragma once

#include "common.h"

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


class Primitive{
public:
    
    ofPoint pos;
    slide size;
    slide rotate;
    ofColor color;
    int drawMode;
    bool bFill;
    float audioVal;
    int rotAxisMode;
    
    Primitive():bFill(false),rotAxisMode(1){
        color.set(255,255,255,255);
        size.set(10,10);
        drawMode = 0;
        pos.set(WIDTH * 0.5 ,HEIGHT * 0.5);
    }
    void update(){
        //size.set(audioVal*200.0,audioVal*200.0);
        float max = WIDTH * 0.2;
        size.set(audioVal * max, audioVal * max);
    }
        
    void draw(){
        ofPushMatrix();
        ofPushStyle();
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofSetSphereResolution(2);
        ofSetConeResolution(2, 4);
        ofSetPlaneResolution(5, 5);
        ofSetCylinderResolution(2, 2);
        if(bFill)ofFill();
        else ofNoFill();
        
        ofSetColor(color.r,color.g,color.b,color.a);
        
        if(rotAxisMode == 1)ofTranslate(pos.x,pos.y,pos.z);
        else if(rotAxisMode == 0)ofTranslate(WIDTH*0.5, HEIGHT*0.5);
        
        ofRotateX(rotate.x);
        ofRotateY(rotate.y);
        ofRotateZ(rotate.z);
        
        if(rotAxisMode == 0){
            ofTranslate(-WIDTH*0.5, -HEIGHT*0.5);
            ofTranslate(pos.x,pos.y,pos.z);
        }
        switch (drawMode) {
            case 0: ofDrawBox(0,0,0,size.x,size.y,size.z);break;
            case 1: ofDrawSphere(0,0,0,size.x * 0.5); break;
            case 2: ofDrawCone(0, 0, 0, size.x, size.y); break;
            case 3: ofDrawCylinder(0, 0, size.x, size.y); break;
            case 4: ofDrawPlane(0, 0, 0, size.x, size.y); break;
        }
        ofPopStyle();
        ofPopMatrix();
        
    }
};
