//
//  SendPoints.h
//
#pragma once

#include "ofxNetwork.h"

class SendPoints {
public:
	void setup();
	void send(const vector<ofVec3f> &);
	void send(const vector<ofVec3f*> &);
protected:
	ofxUDPManager tx;
	char buffer[640*480*2+4];
};