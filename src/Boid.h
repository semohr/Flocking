#ifndef BOID_H
#define BOID_H
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <vector>

extern glm::vec3 std_vector_boid = glm::vec3(1.0f,0.0f,0.0f);


class Boid{
	public:
		Boid(glm::vec3 position_in, glm::vec3 velocity_in); //constructor

		void 			setModelMatrix(glm::mat4 model_in);		
		glm::mat4 getModelMatrix();
		void 			applyforce(glm::vec3 force);
		void 			resetforce();
		void 			timestep(float rate);
		glm::vec3 force_cohesion();
		glm::vec3 force_alignment();
		glm::vec3 force_separation();
		glm::vec3 force_toCenter();
	private:
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 acceleration;
		glm::mat4 modelMatrix;
};


Boid::Boid(glm::vec3 position_in,glm::vec3 velocity_in){
	this->position = position_in;
	this->velocity = velocity_in;
	this->acceleration = glm::vec3(0.0f,0.0f,0.0f);
}

void Boid::setModelMatrix(glm::mat4 model_in){
	this->modelMatrix = model_in;
	//std::cout << glm::to_string(modelMatrix);
}

glm::mat4 Boid::getModelMatrix(){
	return this->modelMatrix;
}

void Boid::applyforce(glm::vec3 force){
	this->acceleration += force;
}

void Boid::resetforce(){
	this->acceleration = glm::vec3(0.0f,0.0f,0.0f); //resets to 0 force ;)
}

void Boid::timestep(float rate){
	//Update to new values
	this->position += this->velocity;
	this->velocity += this->acceleration;
	
	//Generate model matrix
	//Inverse because lookat gives a inverse matrix...
	this->modelMatrix = glm::inverse(glm::lookAt(this->position, this->position + this->velocity, glm::vec3(0.0f,1.0f,0.0f)));
	//Rotate by 90deg because model is looking at 1,0,0
	this->modelMatrix = glm::rotate(this->modelMatrix, (float)90, glm::vec3(0.0f,1.0f,0.0f));
	//std::cout << glm::to_string(this->modelMatrix) << std::endl;
}


extern const int PERCEPTION_RADIUS_COHESION;
extern const int PERCEPTION_RADIUS_ALIGNMENT;
extern const int PERCEPTION_RADIUS_SEPARATION;
extern const float MAX_SPEED;
extern const float MAX_FORCE_MAGNITUDE;
extern const float WEIGHT_FAKTOR_RADIUS_TOCENTER_FORCE;
extern std::vector<Boid> BOIDS;


glm::vec3 Boid::force_cohesion(){ //Bindung
	/* steer to move toward the average position of local flockmates */
	float distance;
	int count = 0;
	glm::vec3 sum = glm::vec3(0.0f,0.0f,0.0f);

	for (int i = 0; i < BOIDS.size(); i++)
	{
		distance = glm::distance(this->position, BOIDS[i].position);
		if((distance > 0) && (distance < PERCEPTION_RADIUS_COHESION)){
			sum += BOIDS[i].position;
			count++;
		}
	}
	glm::vec3 force = glm::vec3(0.0f,0.0f,0.0f);
	if (count>0){
		force = sum/(float)count;
	}

	/* Limit force*/
	if (glm::length(force) > MAX_FORCE_MAGNITUDE)
	{
		force = glm::normalize(force);
		force *= MAX_FORCE_MAGNITUDE;
	}

	return force;
}


glm::vec3 Boid::force_alignment(){ //Ausrichten
	/* steer towards the average heading of local flockmates */
	float distance;
	int count = 0;
	glm::vec3 sum = glm::vec3(0.0f,0.0f,0.0f);
	for (int i = 0; i < BOIDS.size(); i++)
	{
		distance = glm::distance(this->position, BOIDS[i].position);
		if((distance > 0) && (distance < PERCEPTION_RADIUS_ALIGNMENT)){
			sum += BOIDS[i].velocity;
			count++;
		}
	}
	glm::vec3 force = glm::vec3(0.0f,0.0f,0.0f);
	if (count>0){
		force = sum/(float)count;
	}
	/* Limit force*/
	if (glm::length(force) > MAX_FORCE_MAGNITUDE)
	{
		force = glm::normalize(force);
		force *= MAX_FORCE_MAGNITUDE;
	}

	return force;
}

glm::vec3 Boid::force_separation(){
	/* steer to avoid crowding local flockmates */
	glm::vec3 sum = glm::vec3(0.0f,0.0f,0.0f);
	glm::vec3 temp = glm::vec3(0.0f,0.0f,0.0f);
	float distance;
	int count = 0;
	for (int i = 0; i < BOIDS.size(); i++)
	{
		distance = glm::distance(this->position, BOIDS[i].position);
		if((distance > 0) && (distance < PERCEPTION_RADIUS_SEPARATION)){
			temp = this->position - BOIDS[i].position;
			temp = glm::normalize(temp)/distance; //Weight by distance
			sum += temp;
			count++;
		}
	}
	glm::vec3 force = glm::vec3(0.0f,0.0f,0.0f);
	if (count>0){
		force = sum/(float)count;
	}

	if (glm::length(force) > 0)
	{
		force = glm::normalize(force);
		force *= MAX_SPEED;

		force -= this->velocity; // Implement Reynolds: Steering = Desired - Velocity

		/* Limit force*/
		if (glm::length(force) > MAX_FORCE_MAGNITUDE)
		{
			force = glm::normalize(force);
			force *= MAX_FORCE_MAGNITUDE;
		}
	}

	return force;
}

glm::vec3 Boid::force_toCenter(){

	float distance = glm::distance(this->position,glm::vec3(0.0f,0.0f,0.0f));//Distance to middle (0,0,0)
	float weight_faktor = 1/M_PI * atan((distance - WEIGHT_FAKTOR_RADIUS_TOCENTER_FORCE) / 4.0f) + 0.5f;
	glm::vec3 force = -this->position;
	force = glm::normalize(force);
	if (glm::length(force) > MAX_FORCE_MAGNITUDE*8)
	{
		force = glm::normalize(force);
		force *= MAX_FORCE_MAGNITUDE*8;
		force *= weight_faktor;
	}
	else{
		force *= weight_faktor;
	}



	return force;
}




#endif