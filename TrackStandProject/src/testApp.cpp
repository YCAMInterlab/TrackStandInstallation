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

	particleRenderer.setup(30000);
	useTestRecording = true;
	//set up test
	if(useTestRecording){
		recordingTest.setup();
		string testSequencePath = "/Users/focus/Desktop/__RGBD_Bins/YCAM_SPIN/YCAM_Y_CAM1/TAKE_09_21_16_01_16/depth";
		depthSequence.loadSequence(testSequencePath);
		recordingTest.addTrack("depth sequence",&depthSequence);

		particleRenderer.meshBuilder.setDepthPixels(depthSequence.getDepthImageSequence()->getPixels());
		recordingTest.setDurationInMillis(depthSequence.getDepthImageSequence()->getDurationInMillis());
		
	}
	
	cam.setup();
	cam.autosavePosition = true;
	cam.loadCameraPosition();

}

//--------------------------------------------------------------
void testApp::update(){

	vector<ofVec2f> positions;
	positions.push_back( ofVec2f(mouseX, mouseY) );
	trackController.setPositions(positions);

	if(useTestRecording){
		if(depthSequence.isFrameNew()){
			particleRenderer.meshBuilder.update();
		}
	}
	particleRenderer.update();
	
}

//--------------------------------------------------------------
void testApp::draw(){
	trackController.draw();
	if(useTestRecording){
		recordingTest.setOffset(trackController.drawRect.getBottomLeft());
		recordingTest.draw();
	}
	
	previewRect = ofRectangle(trackController.drawRect.getTopRight(),
										  trackController.drawRect.width,
										  trackController.drawRect.height);
	cam.begin(previewRect);
	particleRenderer.draw();
	cam.end();
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
	if(key == 'p' && useTestRecording){
		recordingTest.togglePlay();
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
	cam.applyRotation = cam.applyTranslation = previewRect.inside(mouseX, mouseY);
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

//--------------------------------------------------------------
void testApp::exit(){
	depthSequence.disable();
}