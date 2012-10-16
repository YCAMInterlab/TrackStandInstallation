//
//  Device.h
//
#pragma once

//#define USE_GRABSCENE

#ifdef USE_GRABSCENE
#include "ofxGrabScene.h"
class Device : public GrabScene::Node {
#else
#include "ofNode.h"
class Device : public ofNode {
#endif
public:
	Device();
	string getFilename() const;
	void saveTransform() const;
	void loadTransform();
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void drawWhitePoints() = 0;
	virtual void close() = 0;
	
	virtual ofTexture & getDepthTexture() = 0;
	virtual ofTexture & getRgbTexture() = 0;
protected:
	int index;
#ifdef USE_GRABSCENE
	ofParameter<float> pointSize;
#else
	float pointSize;
#endif
};