#include "ofxParticle.h"
#include "ofMain.h"

//------------------------------------------------------------
ofxParticle::ofxParticle(int width_,int height_, int lifeTime_, int radius_, float fadeOut_){
	setInitialCondition(0,0,0,0);
    
    width = width_;
    height = height_;
    isAlive = true;
    lifeTime = lifeTime_;
    radius = radius_;
    fadeOut = fadeOut_;
    creationTime = ofGetElapsedTimeMillis();
    
	damping	= 0.03f;
	
    seperation.distance		= ofRandom(5,50);
	//seperation.distance		= 35;
	alignment.distance		= 80;
	cohesion.distance		= 90;
	
	seperation.strength		= .03;
    alignment.strength		= .015;
	cohesion.strength		= .015;
	
}

//------------------------------------------------------------
void ofxParticle::resetForce(){
    // we reset the forces every frame
    frc.set(0,0);
	
	// reset the flock info: 
	cohesion.count 	    = 0;
	seperation.count	= 0;
	alignment.count		= 0;
	
	cohesion.sum.set(0,0); 	
	seperation.sum.set(0,0); 	
	alignment.sum.set(0,0); 	

}

void ofxParticle::addForce(ofVec2f f)
{
    addForce(f.x,f.y);
}

//------------------------------------------------------------
void ofxParticle::addForce(float x, float y){
    // add in a force in X and Y for this frame.
    frc.x = frc.x + x;
    frc.y = frc.y + y;
}

//------------------------------------------------------------
void ofxParticle::addRepulsionForce(float x, float y, float radius, float scale){
    
	// ----------- (1) make a vector of where this position is: 
	
	ofVec2f posOfForce;
	posOfForce.set(x,y);
	
	// ----------- (2) calculate the difference & length 
	
	ofVec2f diff	= pos - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
        diff.normalize();
		frc.x = frc.x + diff.x * scale * pct;
        frc.y = frc.y + diff.y * scale * pct;
    }
}

//------------------------------------------------------------
void ofxParticle::addAttractionForce(float x, float y, float radius, float scale){
    
	// ----------- (1) make a vector of where this position is: 
	
	ofVec2f posOfForce;
	posOfForce.set(x,y);
	
	// ----------- (2) calculate the difference & length 
	
	ofVec2f diff	= pos - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		frc.x = frc.x - diff.x * scale * pct;
        frc.y = frc.y - diff.y * scale * pct;
    }
}

//------------------------------------------------------------
void ofxParticle::addRepulsionForce(ofxParticle &p, float radius, float scale){
	
	// ----------- (1) make a vector of where this particle p is: 
	ofVec2f posOfForce;
	posOfForce.set(p.pos.x,p.pos.y);
	
	// ----------- (2) calculate the difference & length 
	
	ofVec2f diff	= pos - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		frc.x = frc.x + diff.x * scale * pct;
        frc.y = frc.y + diff.y * scale * pct;
		p.frc.x = p.frc.x - diff.x * scale * pct;
        p.frc.y = p.frc.y - diff.y * scale * pct;
    }
}

//------------------------------------------------------------
void ofxParticle::addAttractionForce(ofxParticle & p, float radius, float scale){
	
	// ----------- (1) make a vector of where this particle p is: 
	ofVec2f posOfForce;
	posOfForce.set(p.pos.x,p.pos.y);
	
	// ----------- (2) calculate the difference & length 
	
	ofVec2f diff	= pos - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		frc.x = frc.x - diff.x * scale * pct;
        frc.y = frc.y - diff.y * scale * pct;
		p.frc.x = p.frc.x + diff.x * scale * pct;
        p.frc.y = p.frc.y + diff.y * scale * pct;
    }
	
}

//------------------------------------------------------------
void ofxParticle::addClockwiseForce(ofxParticle &p, float radius, float scale){
	
	// ----------- (1) make a vector of where this particle p is: 
	ofVec2f posOfForce;
	posOfForce.set(p.pos.x,p.pos.y);
	
	// ----------- (2) calculate the difference & length 
	
	ofVec2f diff	= pos - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		frc.x = frc.x - diff.y * scale * pct;
        frc.y = frc.y + diff.x * scale * pct;
		p.frc.x = p.frc.x + diff.y * scale * pct;
        p.frc.y = p.frc.y - diff.x * scale * pct;
    }
}

//------------------------------------------------------------
void ofxParticle::addCounterClockwiseForce(ofxParticle &p, float radius, float scale){
	
	// ----------- (1) make a vector of where this particle p is: 
	ofVec2f posOfForce;
	posOfForce.set(p.pos.x,p.pos.y);
	
	// ----------- (2) calculate the difference & length 
	
	ofVec2f diff	= pos - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		frc.x = frc.x + diff.y * scale * pct;
        frc.y = frc.y - diff.x * scale * pct;
		p.frc.x = p.frc.x - diff.y * scale * pct;
        p.frc.y = p.frc.y + diff.x * scale * pct;
		
    }
}


//------------------------------------------------------------
void ofxParticle::addForFlocking(ofxParticle &p){
	
	ofVec3f diff, diffNormalized;
	float distance;
	
	diff			= p.pos - pos;
	distance		= diff.length();
	diffNormalized	= diff;
	diffNormalized.normalize();

	if( distance > 0 && distance < seperation.distance ){
		seperation.sum += diffNormalized;
		seperation.count++;
	}
	
	if( distance > 0 && distance < alignment.distance ){
		alignment.sum += p.vel.normalized();
		alignment.count++;
	}
	
	if( distance > 0 && distance < cohesion.distance ){
		cohesion.sum += p.pos;
		cohesion.count++;
	}
}

//------------------------------------------------------------
void ofxParticle::addFlockingForce(){
	
	
	// seperation
	if(seperation.count > 0){
		seperation.sum /= (float)seperation.count;
	}
	
	// alignment
	if(alignment.count > 0){
		alignment.sum /= (float)alignment.count;
	}
	
	// cohesion
	if(cohesion.count > 0){
		cohesion.sum /= (float)cohesion.count;
		cohesion.sum -= pos;
	}
	
	float sepFrc 	= seperation.strength;
	float cohFrc 	= cohesion.strength;
	float alignFrc 	= alignment.strength;
	
	frc -= (seperation.sum.normalized()		    * sepFrc);
	frc += (alignment.sum.normalized()			* alignFrc);
	frc += (cohesion.sum.normalized()			* cohFrc);
}


//------------------------------------------------------------
void ofxParticle::addDampingForce(){
	
	// the usual way to write this is  vel *= 0.99
	// basically, subtract some part of the velocity 
	// damping is a force operating in the oposite direction of the 
	// velocity vector
	
    frc.x = frc.x - vel.x * damping;
    frc.y = frc.y - vel.y * damping;
}

//------------------------------------------------------------
void ofxParticle::setInitialCondition(float px, float py, float vx, float vy){
    pos.set(px,py);
	vel.set(vx,vy);
	
	//trail.clear();
}

//------------------------------------------------------------
void ofxParticle::update(){	
	vel = vel + frc;
	pos = pos + vel;
    if (lifeTime !=0 && lifeTime < ofGetElapsedTimeMillis()-creationTime) {
        int t = ofGetElapsedTimeMillis()-lifeTime;
        isAlive = false;
    }
    
}

//------------------------------------------------------------
void ofxParticle::draw(){
    
	ofFill();
    
    if (fadeOut>0) {
        ofSetColor(255, ofMap(ofGetElapsedTimeMillis(), creationTime + (lifeTime*(1.0-fadeOut)), creationTime+lifeTime, 255, 0));
    }else{
        ofSetColor(255);
    }
    
	ofCircle(pos.x, pos.y, radius);
	
    /*
	ofVec2f velNormal = vel;
	velNormal.normalize();
	
	ofVec2f velPerp;
	velPerp.x = -velNormal.y;
	velPerp.y = velNormal.x;*/
	  
}


//------------------------------------------------------------
void ofxParticle::bounceOffWalls(){
	
	// sometimes it makes sense to damped, when we hit
	bool bDampedOnCollision = true;
	bool bDidICollide = false;
	
	// what are the walls
	float minx = 0;
	float miny = 0;
	float maxx = width;
	float maxy = height;
	
	if (pos.x > maxx){
		pos.x = maxx; // move to the edge, (important!)
		vel.x *= -1;
		bDidICollide = true;
	} else if (pos.x < minx){
		pos.x = minx; // move to the edge, (important!)
		vel.x *= -1;
		bDidICollide = true;
	}
	
	if (pos.y > maxy){
		pos.y = maxy; // move to the edge, (important!)
		vel.y *= -1;
		bDidICollide = true;
	} else if (pos.y < miny){
		pos.y = miny; // move to the edge, (important!)
		vel.y *= -1;
		bDidICollide = true;
	}
	
	if (bDidICollide == true && bDampedOnCollision == true){
		vel *= 0.3;
	}
	
}

bool ofxParticle::getAlive(){
    return isAlive;
}
