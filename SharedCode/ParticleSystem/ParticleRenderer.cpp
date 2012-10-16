
#include "ParticleRenderer.h"
	
ParticleRenderer::ParticleRenderer(){
	birthRate = .2;
	lifeSpan = 100;
	lifeSpanVariance = 5;
	
}

void ParticleRenderer::setup(int maxParticles){
	//		int absoluteMaxParticles = 30000;
	
	meshBuilder.setDepthOnly();
	meshBuilder.setSimplification(4);
	meshBuilder.cacheValidVertices = true;
	
	perlinForce = new ForcePerlin();
	spinForce = new ForceSpin();
	gravityForce = new ForceGravity();
	
	for(int i = 0; i < 10000; i++){
		ParticleGenerator g;
		g.addForce(perlinForce);
		g.addForce(spinForce);
		g.addForce(gravityForce);
		g.primaryColor = &primaryColor;
		g.secondaryColor = &secondaryColor;
		emitters.push_back(g);
	}
	
	for(int i = 0; i < maxParticles; i++){
		mesh.addVertex(ofVec3f(0,0,0));
		mesh.addColor(ofFloatColor(1.0,1.0,1.0,1.0));
		mesh.addTexCoord(ofVec2f(0.0,0.0));
	}
	
	maxAllowedParticles = maxParticles;
}

void ParticleRenderer::update(){
	perlinForce->amplitude = .2;
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
	for(int i = 0; i < emitters.size(); i++){
		emitters[i].birthRate = 0;
		emitters[i].freeze = false;
		totalParticles += emitters[i].particles.size();
	}
	
	
	int particlesPerEmitter = 0;
	if(meshBuilder.validVertIndices.size() > 0 ){
		particlesPerEmitter = (maxAllowedParticles - totalParticles) / meshBuilder.validVertIndices.size() - 1;
	}
	//cout << "particles per emitter " << particlesPerEmitter << " max allowd particles " << maxAllowedParticles << endl;
	//for(int i = 0; i < meshBuilder.getMesh().getVertices().size(); i++){
	for(int i = 0; i < meshBuilder.validVertIndices.size(); i++){
		
		ParticleGenerator& g = emitters[i];
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
	for(int i = 0; i < emitters.size(); i++){
		emitters[i].update();
	}
	
	//cout << "updated " << meshBuilder.validVertIndices.size() << endl;
	
	copyVertsToMesh();
	
}

void ParticleRenderer::draw(){
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
		glPointSize(1.5);
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

void ParticleRenderer::copyVertsToMesh(){
	int meshIndex = 0;
	vector<ofVec3f>& meshVertices = mesh.getVertices();
	vector<ofFloatColor>& meshColors = mesh.getColors();
	vector<ofVec2f>& meshTexCoords = mesh.getTexCoords();
	for(int i = 0; i < emitters.size(); i++){
		for(int v = 0; v < emitters[i].particles.size(); v++){
			meshVertices[meshIndex] = emitters[i].particles[v].position;
			float color = emitters[i].particles[v].energy / emitters[i].particles[v].initialEnergy;
			meshColors[meshIndex] = ofFloatColor(emitters[i].particles[v].color,color);
			//            if(useColors){
			//                meshTexCoords[meshIndex] = emitters[i].particles[v].texcoord;
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
