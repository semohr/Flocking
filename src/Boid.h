#ifndef BOID_H
#define BOID_H
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

extern glm::vec3 std_vector_boid = glm::vec3(1.0f,0.0f,0.0f);

class Boid{
	public:
		Boid(glm::vec3 position_in, glm::vec3 velocity_in); //constructor

		void 			setModelMatrix(glm::mat4 model_in);		
		glm::mat4 getModelMatrix();
		void 			applyforce(glm::vec3 force);
		void 			resetforce();
		void 			timestep(float rate);
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




void Boid::force_cohesion(){
	/*Do that all threaded in main*/ 

}




#endif