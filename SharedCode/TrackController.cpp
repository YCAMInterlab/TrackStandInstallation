
#include "TrackController.h"
TrackController::TrackController(){
	editMode = false;
}

TrackController::~TrackController(){
	
}

void TrackController::setup(int numTracks){
	timelineWidth = ofGetWidth();
	for(int i = 0; i < numTracks; i++){
		ofxTimeline* timeline = new ofxTimeline();
		
		timeline->setup();
		timeline->setLoopType(OF_LOOP_NORMAL);
		timeline->setMinimalHeaders(true);
		timeline->setFootersHidden(true);
		timeline->setShowTimeControls(false);
		
		timeline->addColors("primary color");
		timeline->addColors("secondary color");
		timeline->addCurves("birth rate", ofRange(0, 1.0));
		timeline->addCurves("life span", ofRange(0, 100));
		timeline->addCurves("perlin amp", ofRange(0, .2));
		timeline->addCurves("perlin density", ofRange(0, .2));
		timeline->addCurves("gravity amp", ofRange(0, .2));
		timeline->addCurves("spin force", ofRange(0, .2));
		
		timelines.push_back(timeline);
	}
	
}

void TrackController::toggleFooters(){
	for(int i = 0; i < timelines.size(); i++){
		timelines[i]->setShowZoomer(timelines[i]->toggleShowFooters());
	}
}

void TrackController::setWidth(float width){
	if(width == timelineWidth){
		return;
	}
	timelineWidth = width;
	for(int i = 0; i < timelines.size(); i++){
		timelines[i]->setWidth(timelineWidth);
	}
}

void TrackController::draw(){
	
	for(int i = 0; i < timelines.size(); i++){
		if(i != 0){
			timelines[i]->setOffset(timelines[i-1]->getBottomLeft() + ofVec2f(0, 10));
		}
		if(!editMode){
			timelines[i]->getZoomer()->setViewRange(ofRange(timelines[i]->getPercentComplete()-.1,
															timelines[i]->getPercentComplete()+.1));
		}
		timelines[i]->draw();
	}
}

void TrackController::update(){
	
	for(int i  = 0; i < timelines.size(); i++){
		if(timelines[i]->getIsPlaying()){

			particles->perlinForce->amplitude = timelines[i]->getValue("perlin amp");
			particles->perlinForce->density = timelines[i]->getValue("perlin amp");
			particles->gravityForce->gravity = timelines[i]->getValue("gravity amp");
			particles->spinForce->power = timelines[i]->getValue("spin force");
			particles->primaryColor   = timelines[i]->getColor("primary color");
			particles->secondaryColor = timelines[i]->getColor("secondary color");
			particles->birthRate = timelines[i]->getValue("birth rate");
			particles->lifeSpan = timelines[i]->getValue("life span");
			
		}
	}
	
	particles->update();
	
}

void TrackController::setPositions(vector<ofVec2f> positions){
	drawRect = ofRectangle(0,0,0,0);
	for(int i = 0; i < timelines.size(); i++){
		drawRect = drawRect.getUnion(timelines[i]->getDrawRect());
	}
	
	for(int t = 0; t < timelines.size(); t++){
		bool occupied = false;
		for(int i = 0; i < positions.size(); i++){
//			ofVec2f screenPosition = positions[i] * ofVec2f(totalDrawRect.width,totalDrawRect.height);
			ofVec2f screenPosition = positions[i];
			if(timelines[t]->getDrawRect().inside(screenPosition)){
				occupied = true;
				break;
			}
		}
		
		if(!editMode){
			if(occupied && !timelines[t]->getIsPlaying()){
//				cout << t << " is occupied and now playing" << endl;
				timelines[t]->play();
			}
			else if(!occupied && timelines[t]->getIsPlaying()){
//				cout << t << " is no longer occupied" << endl;
				timelines[t]->stop();
			}
		}
	}
}

void TrackController::togglePlayForTrackAtPoint(ofVec2f point){
	for(int t = 0; t < timelines.size(); t++){
		if(timelines[t]->getDrawRect().inside(point)){
			timelines[t]->togglePlay();
		}
	}
}

