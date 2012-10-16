//
//  OpenNI.h
//
#pragma once

#include "Device.h"
#include "ofxOpenNI.h"

class OpenNI : public Device {
public:
	OpenNI(int index);
	void update();
	void draw();

	ofTexture & getDepthTexture();
	ofTexture & getRgbTexture();
	
protected:
	void drawPointCloud();
	void updateAngle(float &);
	ofxOpenNI kinect;
	ofNode origin;
	ofMesh mesh;
	vector<ofVec3f> projective;
};