/**
 *
 * TrackStand interactive installation
 * researching creative uses of ofxTimeline in installation context
 */
 
#pragma once

#include "ofMain.h"
#include "ofxTimeline.h"
#include "ParticleRenderer.h"

class TrackController {
  public:
	TrackController();
	~TrackController();
	
	void setup(int numTracks);
	void update();
	void draw();
	
	void setPositions(vector<ofVec2f> positions);
	vector<ofxTimeline*> timelines;
	void setWidth(float width);
	bool editMode;
	
	ofRectangle drawRect;
	ParticleRenderer* particles;
	
	//where to draw to the floor
//	ofRectangle floorRectangle;
	//where to draw into the screen for editing
//	ofRectangle editingRectangle;
//	void setToFloor(bool onFloor);
	
	void toggleFooters();
	void togglePlayForTrackAtPoint(ofVec2f point);
	
  protected:
	bool isOnFloor;
	float timelineWidth;
};

