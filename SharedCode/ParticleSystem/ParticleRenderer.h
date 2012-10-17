
#pragma once

#include "ofMain.h"
#include "ForceGravity.h"
#include "ForcePerlin.h"
#include "ForceSpin.h"
#include "ParticleGenerator.h"
//#include "ofxRGBDMeshBuilder.h"
#include "Kinect.h"

class ParticleRenderer {
  public:
	
	//update this ever frame;
//	ofxRGBDMeshBuilder meshBuilder;
	
	Kinect* kinect;
	vector<ofVec3f>* points;
	
	float minX;
	float maxX;
	
	ofVboMesh mesh;
	ofShader pointCloudDOF;
	int totalParticles;
	int maxAllowedParticles;
	
	vector<ParticleGenerator> emitters;
    ForcePerlin* perlinForce;
    ForceGravity* gravityForce;
	ForceSpin* spinForce;

	ofFloatColor primaryColor;
	ofFloatColor secondaryColor;
	
	//global emmitter properties
	float birthRate;
	float lifeSpan;
	float lifeSpanVariance;

	float bottomClip;
	float maxFlicker; //betweeen 0 and one then based on particle
	
	ParticleRenderer();
	
	void setup(int maxParticles);
	
	void update();
	void draw();
	void setAudioData(vector<float>& fft, int minBin, int maxBin);
	
	void copyVertsToMesh();
	
};