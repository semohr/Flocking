#ifndef BOID_H
#define BOID_H
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

extern glm::vec3 std_vector_boid = glm::vec3(1.0f,0.0f,0.0f);

class Boid{
	public:
		Boid(glm::vec3 position_in, glm::vec3 velocity_in); //constructor
		glm::mat4 getModelMatrix();
		void setModelMatrix(glm::mat4 model_in);
		int timestep(float rate);
	private:
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 acceleration = glm::vec3(0.0f);
		glm::mat4 modelMatrix;
};


Boid::Boid(glm::vec3 position_in,glm::vec3 velocity_in){
	position = position_in;
	velocity = velocity_in;
}

void Boid::setModelMatrix(glm::mat4 model_in){
	modelMatrix = model_in;
	//std::cout << glm::to_string(modelMatrix);
}

glm::mat4 Boid::getModelMatrix(){
	return modelMatrix;
}

int Boid::timestep(float rate){
	//Update to new values
	this->position += this->velocity;
	this->velocity += this->acceleration;
	
	//Generate model matrix
	//Inverse because lookat gives a inverse matrix...
	this->modelMatrix = glm::inverse(glm::lookAt(this->position, this->position + this->velocity, glm::vec3(0.0f,1.0f,0.0f)));
	//Rotate by 90deg because model is looking at 1,0,0
	this->modelMatrix = glm::rotate(this->modelMatrix, (float)90, glm::vec3(0.0f,1.0f,0.0f));
	//std::cout << glm::to_string(this->modelMatrix) << std::endl;
	return 0;
}




#endif