//
//  Kinect.h
//
#pragma once

#include "Device.h"
#include "ofxKinect.h"

#define STEP 2

class Kinect : public Device {
public:
	Kinect(int index, bool registration);
	void update();
	void draw();
	void drawWhitePoints();
	void close();
	int outputPointCount() const;
	
	ofTexture & getDepthTexture();
	ofTexture & getRgbTexture();
	
	vector<ofVec3f> & getObjectPoints();
	vector<ofVec3f> getWorldPoints();
protected:
	void updatePointCloud();
	void updateAngle(float &);
	ofxKinect kinect;
	ofNode origin;
#ifdef USE_GRABSCENE
	ofParameter<float> angle;
	ofParameter<bool> useRgb;
#else
	float angle;
#endif
	ofMesh coloredMesh;
	ofMesh whiteMesh;
	int _objectPointCount;
};