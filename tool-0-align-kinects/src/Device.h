//
//  Device.h
//
#pragma once

#include "ofxGrabScene.h"

class Device : public GrabScene::Node {
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
	ofParameter<float> pointSize;
};