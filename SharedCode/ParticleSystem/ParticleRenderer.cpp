
#include "ParticleRenderer.h"
	
ParticleRenderer::ParticleRenderer(){
	birthRate = .2;
	lifeSpan = 100;
	lifeSpanVariance = 5;
	points = NULL;
	minX = 0;
	maxX = 100;
	points = NULL;
	
}

void ParticleRenderer::setup(int maxParticles){
	
//	meshBuilder.setDepthOnly();
//	meshBuilder.setSimplification(4);
//	meshBuilder.cacheValidVertices = true;
	
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
//	perlinForce->amplitude = .2;
	return;
	if(points == NULL){
		return;
	}
	
	perlinForce->update();
	totalParticles = 0;
	for(int i = 0; i < emitters.size(); i++){
		emitters[i].birthRate = 0;
		emitters[i].freeze = false;
		totalParticles += emitters[i].particles.size();
	}
	
	int particlesPerEmitter = 0;
	if(points->size() > 0 ){
//	if(meshBuilder.validVertIndices.size() > 0 ){
		particlesPerEmitter = (maxAllowedParticles - totalParticles) / points->size() - 1;
	}
	
	//cout << "particles per emitter " << particlesPerEmitter << " max allowd particles " << maxAllowedParticles << endl;
	//for(int i = 0; i < meshBuilder.getMesh().getVertices().size(); i++){
	for(int i = 0; i < points->size(); i++){
	//for(int i = 0; i < meshBuilder.validVertIndices.size(); i++){
		ofVec3f* pos = (*points)[i];
		if(pos->x > minX && pos->x < maxX){
		
			ParticleGenerator& g = emitters[i];
			g.birthRate = birthRate; //disable invisible verts
			g.lifespan  = lifeSpan;
			g.lifespanVariance = lifeSpanVariance;
	//		g.position =  meshBuilder.getMesh().getVertices()[meshBuilder.validVertIndices[i]];
			g.position = *pos;
			g.remainingParticles = particlesPerEmitter;
		}		
	}
	
	//    cout << " total particles " << totalParticles << endl;
	for(int i = 0; i < emitters.size(); i++){
		emitters[i].update();
	}
	
	//cout << "updated " << meshBuilder.validVertIndices.size() << endl;
	
	copyVertsToMesh();
	
}

void ParticleRenderer::draw(){
//	meshBuilder.draw();
	ofMesh m;
	for(int i = 0; i < points->size(); i++){
		
		//for(int i = 0; i < meshBuilder.validVertIndices.size(); i++){
		ofVec3f* pos = (*points)[i];
		m.addVertex(*pos);
	}
	m.drawVertices();
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
