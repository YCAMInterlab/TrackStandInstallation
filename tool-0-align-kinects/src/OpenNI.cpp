//
//  OpenNI.cpp
//
#include "OpenNI.h"

//---------
OpenNI::OpenNI(int index) {
	this->index = index;
	this->kinect.setupFromXML("openni/config/ofxopenni_config.xml",false);
	this->loadTransform();
	
	this->kinect.enableCalibratedRGBDepth();
	this->kinect.setGeneratePCColors(true);
	this->mesh.getVertices().resize(kinect.getWidth()*kinect.getHeight());
	this->mesh.getTexCoords().resize(kinect.getWidth()*kinect.getHeight());
	this->projective.resize(kinect.getWidth()*kinect.getHeight());
	
	for (int j=0; j<kinect.getWidth()*kinect.getHeight(); j++)
		for (int i=0; i<kinect.getWidth(); i++) {
			this->mesh.getTexCoords()[i + j * kinect.getHeight()] = ofVec2f(i, j);
			this->projective[i + j * kinect.getHeight()] = ofVec3f(i, j, 0.0f);
		}
}

//---------
void OpenNI::update() {
	this->kinect.update();
}

//---------
void OpenNI::draw() {
#ifdef USE_GRABSCENE
	this->getNode().transformGL();
#else
	this->transformGL();
#endif
	
	this->drawPointCloud();
	
	ofPushMatrix();
	ofScale(0.01, 0.01,0.01);
	origin.draw();
	ofPopMatrix();
	
	ofPushMatrix();
	ofScale(0.7f, 0.7f, 0.7f);
	ofTranslate(-1.0f, 1.0f);
	ofScale(2.0f/(float)kinect.getWidth(), -2.0f/(float)kinect.getHeight());
	ofTranslate(0.0f, 0.0f, -2.0f);
	this->getRgbTexture().draw(0.0f, 0.0f);
	ofPopMatrix();
	
#ifdef USE_GRABSCENE
	this->getNode().restoreTransformGL();
#else
	this->restoreTransformGL();
#endif
}

//---------
ofTexture & OpenNI::getDepthTexture() {
	return this->kinect.getDepthTextureReference();
}

//---------
ofTexture & OpenNI::getRgbTexture() {
	return this->kinect.getRGBTextureReference();
}

//---------
void OpenNI::drawPointCloud() {
	ofVec3f * world = this->mesh.getVerticesPointer();
	ofVec3f * projective = & this->projective[0];
	unsigned short * depth = this->kinect.getDepthRawPixels().getPixels();
	
	for (int j=0; j<kinect.getHeight(); j++)
		for (int i=0; i<kinect.getWidth(); i++) {
			(*projective++).z = *depth++;
		}

	this->kinect.getDepthGenerator().ConvertProjectiveToRealWorld(kinect.getWidth()*kinect.getHeight(), (XnVector3D*) & this->projective[0], (XnVector3D*) this->mesh.getVerticesPointer());

	ofPushMatrix();
	ofScale(0.001, 0.001, 0.001);
	kinect.getRGBTextureReference().bind();
	mesh.drawVertices();
	kinect.getRGBTextureReference().unbind();
	ofPopMatrix();
}