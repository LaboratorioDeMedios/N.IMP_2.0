#ifndef PARTICLE_H
#define PARTICLE_H

#include "ofMain.h"



typedef struct{
	int			count;
	ofVec2f	sum;
	float		distance;
	float		strength;
} flockingForce;



class ofxParticle{

	public:
        ofVec2f pos;
        ofVec2f vel;
        ofVec2f frc;   // frc is also know as acceleration (newton says "f=ma")
			
        ofxParticle(int width_ = 1024,int height = 768, int lifeTime_ = 20000, int radius_ = 10,float fadeOut_= 0);
		virtual ~ofxParticle(){};

        void resetForce();
        void addForce(ofVec2f f);
		void addForce(float x, float y);
		void addRepulsionForce(float x, float y, float radius, float scale);
		void addAttractionForce(float x, float y, float radius, float scale);
		
		void addRepulsionForce(ofxParticle &p, float radius, float scale);
		void addAttractionForce(ofxParticle &p, float radius, float scale);
		void addClockwiseForce(ofxParticle &p, float radius, float scale);
		void addCounterClockwiseForce(ofxParticle &p, float radius, float scale);
	
		void addDampingForce();
 
		void addForFlocking(ofxParticle &p);
		void addFlockingForce();
	
		flockingForce cohesion;
		flockingForce seperation;
		flockingForce alignment;
	
	
		void setInitialCondition(float px, float py, float vx, float vy);
        void update();
        void draw();
	
		void bounceOffWalls();
	
    bool getAlive();
		float damping;

    protected:
    private:
    bool isAlive;
    int width,height;
    int lifeTime,radius;
    long int creationTime;
    float fadeOut;
};

#endif // PARTICLE_H
