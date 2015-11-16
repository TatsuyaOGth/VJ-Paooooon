#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

class OscBang : ofEventArgs
{
public:
    ofxOscMessage * message;
};

class OscReceiverController
{
    ofxOscReceiver mReceiver;
    deque< shared_ptr<ofxOscMessage> > messages;
    typedef deque< shared_ptr<ofxOscMessage> >::iterator mit;
    
    void onUpdate(ofEventArgs &data)
    {
        if (mReceiver.hasWaitingMessages()) messages.clear();
        while (mReceiver.hasWaitingMessages())
        {
            messages.push_back(shared_ptr<ofxOscMessage>(new ofxOscMessage));
            ofxOscMessage * m = messages.back().get();
            mReceiver.getNextMessage(m);
            if (m->getAddress() == "/bang")
            {
                OscBang bang;
                if (m->getNumArgs() > 0) bang.message = m;
                ofNotifyEvent(events, bang);
            }
        }
    }
    
    float getValueAsFloat(ofxOscMessage * m, int index)
    {
        if (m->getArgType(index) == OFXOSC_TYPE_INT32) return static_cast<float>(m->getArgAsInt32(index));
        if (m->getArgType(index) == OFXOSC_TYPE_INT64) return static_cast<float>(m->getArgAsInt64(index));
        if (m->getArgType(index) == OFXOSC_TYPE_FLOAT) return m->getArgAsFloat(index);
        return -1;
    }
    
public:
    OscReceiverController()
    {
        ofAddListener(ofEvents().update, this, &OscReceiverController::onUpdate);
    }
    
    ~OscReceiverController()
    {
        ofRemoveListener(ofEvents().update, this, &OscReceiverController::onUpdate);
    }
    
    ofEvent<OscBang> events;
    
    void setPort(int port)
    {
        mReceiver.setup(port);
    }
    
    ofxOscMessage * getMessage(const string & addr, int index)
    {
        for (mit it = messages.begin(); it != messages.end(); it++)
        {
            if (addr == (*it)->getAddress() && (*it)->getNumArgs() > index) return it->get();
        }
        return NULL;
    }
    
    int32_t getValueInt(const string & addr, int index)
    {
        ofxOscMessage * m = getMessage(addr, index);
        if (m != NULL) {
            return m->getArgAsInt32(index);
        } else return -1;
    }
    
    float getValueFloat(const string & addr, int index)
    {
        ofxOscMessage * m = getMessage(addr, index);
        if (m != NULL) {
            return m->getArgAsFloat(index);
        } else return -1;
    }
    
    string getValueString(const string & addr, int index)
    {
        ofxOscMessage * m = getMessage(addr, index);
        if (m != NULL) {
            return m->getArgAsString(index);
        } else return "";
    }
    
    float getValue(const string & addr, int index)
    {
        ofxOscMessage * m = getMessage(addr, index);
        if (m != NULL) {
            return getValueAsFloat(m, index);
        } else return -1;
    }
    
};
