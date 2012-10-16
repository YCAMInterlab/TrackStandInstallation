//
//  Kinect.h
//
#pragma once

#include "Device.h"
#include "ofxKinect.h"

#define STEP 1

class Kinect : public Device {
public:
	Kinect(int index, bool registration);
	void update();
	void draw();
	void drawWhitePoints();
	void close();
	
	ofTexture & getDepthTexture();
	ofTexture & getRgbTexture();
	
protected:
	void updatePointCloud();
	void updateAngle(float &);
	ofxKinect kinect;
	ofNode origin;
#ifdef USE_GRABSCENE
	ofParameter<float> angle;
#else
	float angle;
#endif
	ofMesh coloredMesh;
	ofMesh whiteMesh;
};