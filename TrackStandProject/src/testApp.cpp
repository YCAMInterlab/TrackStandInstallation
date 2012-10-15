#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0);
	ofEnableSmoothing();
	
	ofSetFrameRate(60);
	ofToggleFullscreen();
	
	trackController.setup(4);
	trackController.setWidth(ofGetScreenWidth()/2);
	ofxTimeline::removeCocoaMenusFromGlut("TrackStand");
}

//--------------------------------------------------------------
void testApp::update(){

	vector<ofVec2f> positions;
	positions.push_back( ofVec2f(mouseX, mouseY) );
	trackController.setPositions(positions);
}

//--------------------------------------------------------------
void testApp::draw(){
	trackController.draw();
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	if(key == 'b'){
		trackController.editMode = !trackController.editMode;
	}
	if(key == ' '){
		trackController.togglePlayForTrackAtPoint(ofVec2f(mouseX,mouseY));
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}