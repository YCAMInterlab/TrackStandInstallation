
#include "ofMain.h"
#include "ForceGravity.h"
#include "ForcePerlin.h"
#include "ForceSpin.h"
#include "ParticleGenerator.h"
#include "ofxRGBDMeshBuilder.h"

class ParticleRenderer {
  public:
	
	//update this ever frame;
	ofxRGBDMeshBuilder meshBuilder;
	
	ofVboMesh mesh;
	ofShader pointCloudDOF;
	int totalParticles;
	int maxAllowedParticles;
	
	vector<ParticleGenerator> emmiters;
    ForcePerlin* perlinForce;
    ForceGravity* gravityForce;
	ForceSpin* spinForce;

	//global emmitter properties
	float birthRate;
	float lifeSpan;
	float lifeSpanVariance;

	ParticleRenderer(){
		
	}
	
	void setup(int maxParticles){
		int absoluteMaxParticles = 30000;
		
		meshBuilder.setDepthOnly();
		
		perlinForce = new ForcePerlin();
		spinForce = new ForceSpin();
		gravityForce = new ForceGravity();
		
		for(int i = 0; i < 10000; i++){
			ParticleGenerator g;
			g.addForce(perlinForce);
			g.addForce(spinForce);
			g.addForce(gravityForce);
			emmiters.push_back(g);
		}
		
		for(int i = 0; i < absoluteMaxParticles; i++){
			mesh.addVertex(ofVec3f(0,0,0));
			mesh.addColor(ofFloatColor(1.0,1.0,1.0,1.0));
			mesh.addTexCoord(ofVec2f(0.0,0.0));
		}

	}
	
	void update(){
		/*
		perlinForce->amplitude = powf( masterTimeline.getValue("Perlin Amplitude"), 2.0);
		perlinForce->density = powf( masterTimeline.getValue("Perlin Density"), 2.0);
		perlinForce->speed = powf( masterTimeline.getValue("Perlin Speed"), 2.0);
		spinForce->power = powf( masterTimeline.getValue("Wind Speed"), 2.0);
		gravityForce->gravity = powf( masterTimeline.getValue("Gravity Force"), 2.0);
		gravityForce->maxDelay = masterTimeline.getValue("Gravity Delay");
		*/
		
		perlinForce->update();
		/*
		//GENERATOR
		float birthRate = masterTimeline.getValue("Birthrate");
		float lifeSpan  = masterTimeline.getValue("Lifespan");
		float lifeSpanVariance = masterTimeline.getValue("Lifespan Variance");
		int maxParticles = masterTimeline.getValue("Max Particles");
		*/
		totalParticles = 0;
		for(int i = 0; i < emmiters.size(); i++){
			emmiters[i].birthRate = 0;
			emmiters[i].freeze = false;
			totalParticles += emmiters[i].particles.size();
		}

		
		int particlesPerEmitter = 0;
		if(meshBuilder.validVertIndices.size() > 0 ){
			particlesPerEmitter = (maxAllowedParticles - totalParticles) / meshBuilder.validVertIndices.size() - 1;
		}
		
		//for(int i = 0; i < meshBuilder.getMesh().getVertices().size(); i++){
		for(int i = 0; i < meshBuilder.validVertIndices.size(); i++){
			
			ParticleGenerator& g = emmiters[i];
			g.birthRate = birthRate; //disable invisible verts
			g.lifespan  = lifeSpan;
			g.lifespanVariance = lifeSpanVariance;
			g.position =  meshBuilder.getMesh().getVertices()[meshBuilder.validVertIndices[i]];
			g.remainingParticles = particlesPerEmitter;
			//            if(useColors && colorPalette.isAllocated()){
			//                g.texcoord = renderer.getMesh().getTexCoord( renderer.vertexIndex(i) );
			//            }
		}
		
		//    cout << " total particles " << totalParticles << endl;
		for(int i = 0; i < emmiters.size(); i++){
			emmiters[i].update();
		}
		
		copyVertsToMesh();
		
	}
	
	void draw(){
		meshBuilder.draw();
		
		//render dots
		ofPushStyle();
		glPushMatrix();
		glPushAttrib(GL_ENABLE_BIT);
		ofScale(1,-1,1);
		ofEnableAlphaBlending();
		//		if(useShaderToggle){
		if(false){
			pointCloudDOF.begin();
			//			if(depthRangeMultiply) focalRange *= 10;
//			pointCloudDOF.setUniform1f("minSize", masterTimeline.getValue("Min Point Size"));
//			pointCloudDOF.setUniform1f("maxSize", masterTimeline.getValue("Max Point Size"));
//			pointCloudDOF.setUniform1f("focalRange", focalRange);
//			pointCloudDOF.setUniform1f("focalDistance", focalDistance);
			pointCloudDOF.setUniform1i("useTexture", 0);
			//			pointCloudDOF.setUniform1i("useTexture", useColors && colorPalette.isAllocated() ? 1 : 0);
		}
		else{
			//glPointSize(masterTimeline.getValue("Min Point Size"));
			glPointSize(1.0);
		}
		
		//		if(useColors && colorPalette.isAllocated()){
		//			colorPalette.bind();
		//		}
		//    if(drawPointcloud){
		//        ofPushMatrix();
		//        ofScale(1,-1,1);
		//        ofEnableAlphaBlending();
		//        renderer.drawPointCloud(false);
		//        ofPopMatrix();
		//	}
		
		//ofEnableBlendMode(OF_BLENDMODE_ADD);
		ofEnableBlendMode(OF_BLENDMODE_SCREEN);
		glEnable(GL_POINT_SMOOTH); // makes circular points
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);	// allows per-point size
		//		glPointSize(4);
		
		//		cout << "Drawing " << mesh.getNumVertices() << " Particles " << endl;
		mesh.drawVertices();
		
		//		if(useColors && colorPalette.isAllocated()){
		//			colorPalette.unbind();
		//		}
		//
		if(false){
			pointCloudDOF.end();
		}
		
		glPopAttrib();
		glPopMatrix();
		ofPopStyle();
		
	}
	
	void copyVertsToMesh(){
		int meshIndex = 0;
		vector<ofVec3f>& meshVertices = mesh.getVertices();
		vector<ofFloatColor>& meshColors = mesh.getColors();
		vector<ofVec2f>& meshTexCoords = mesh.getTexCoords();
		for(int i = 0; i < emmiters.size(); i++){
			for(int v = 0; v < emmiters[i].particles.size(); v++){
				meshVertices[meshIndex] = emmiters[i].particles[v].position;
				float color = emmiters[i].particles[v].energy / emmiters[i].particles[v].initialEnergy;
				meshColors[meshIndex] = ofFloatColor(color,color,color,color);
				//            if(useColors){
				//                meshTexCoords[meshIndex] = emmiters[i].particles[v].texcoord;
				//            }
				meshIndex++;
				if(meshIndex == meshVertices.size()){
					//                ofLogError("exceeded max particles");
					return;
				}
			}
		}
		//	cout << "mesh index reached " << meshIndex << endl;
		memset(&(meshColors[meshIndex].r), 0, sizeof(ofFloatColor)*(meshColors.size()-meshIndex));
	}
	
};