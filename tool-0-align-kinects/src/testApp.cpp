#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofEnableSmoothing();
	
	kinect[0] = new DeviceType(0, REGISTRATION);
	kinect[1] = new DeviceType(1, REGISTRATION);

	set.devices.push_back(kinect[0]);
	set.devices.push_back(kinect[1]);

	scene.init(camera);
#ifdef USE_GRABSCENE
	scene.add(kinect[0]);
	scene.add(kinect[1]);
	scene.add(&set);
#endif
	scene.add(new GrabScene::Nodes::Grid());

	
	camera.setPosition(10.0f, 3.0f, 5.0f);
	camera.lookAt(ofVec3f(2.0f, 1.0f, -3.0f));
	camera.setFov(30);
}

//--------------------------------------------------------------
void testApp::update(){
	set.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackgroundGradient(ofColor(50), ofColor(0));
	scene.draw();
	ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 20);
}

//--------------------------------------------------------------
void testApp::exit() {
	//kinect[0]->close();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == 'c') {
		camera.toggleCursorDraw();
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	if (key == 's') {
		kinect[0]->saveTransform();
		kinect[1]->saveTransform();
		set.save();
	}
	if (key == 'f') {
		ofToggleFullscreen();
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

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