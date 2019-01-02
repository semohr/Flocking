#include <glad/glad.h>
#include <GLFW/glfw3.h>

//STD Libs
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cmath>		//For sin, cos...
#include <cstdlib>  //For random
#include <time.h>
#include <random>
#include <thread>
#include <mutex>

// a global instance of std::mutex to protect global variable
std::mutex myMutex;

  std::mt19937 eng;  // a core engine class 
  std::uniform_real_distribution<float> pi(0,M_PI);   
	std::uniform_real_distribution<float> dis(-1,1);
#define GLM_ENABLE_EXPERIMENTAL

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp> //to string
//ourShader
#include "Shader.h"

//Key, cursor and error callback handling
#include "./src/callback.h"
#include "./src/Boid.h"

using namespace std;


const int WIDTH = 1280, HEIGHT = 720;

vector<Boid> BOIDS; //vector with boid objects
bool RUNNING = true;


glm::mat4 *setupBoids(int amount , float radius_max){

	/*Generates an amount of Boids inside a Sphere with a radius*/
	float x, y, z, rotAngle;
	glm::vec3 position;
	glm::vec3 rotVec;

	glm::mat4* modelMatrices = new glm::mat4[amount];

	for (int i = 0; i < amount; ++i){
		//Generating random points inside a sphere with "kugelkoordinaten"
		glm::mat4 model(1.0f);

		do{
			x = dis(eng) * radius_max;
			y = dis(eng) * radius_max;
			z = dis(eng) * radius_max;
		}while(x*x + y*y + z*z > radius_max);
		position = glm::vec3(x,y,z);

		cout << x << " : "<< y<<" : "<<z <<endl;
		//Adding points to model matrix
		model = glm::translate(model, position);

		//Generate random angle and rotate by random vec3
		rotAngle = dis(eng) * 360;
		rotVec = glm::normalize(glm::vec3(dis(eng),dis(eng),dis(eng)));

		//Push to model matirx
		model = glm::rotate(model, rotAngle, rotVec);
		//Velocity (also random) normalized to length 1
		


		//Create a object for each boid (WIP)
		float rotation[9] = {0.0};
		const float *pSource = (const float*)glm::value_ptr(model);
		for (int i = 0; i < 9; ++i)
		{
			rotation[i] = pSource[i];
		}
		glm::mat3 rotationmat3 = glm::make_mat3(rotation); 
		
		Boid my_boid( position, rotationmat3*std_vector_boid);
		BOIDS.push_back(my_boid); //pos,vel
		BOIDS[i].setModelMatrix(model);

	}
	return modelMatrices;
} 



void timestep(double function_call_delay){
	double startTime = clock()/(CLOCKS_PER_SEC/1000); //Start time in ms
	while(RUNNING){
		while( (clock()/(CLOCKS_PER_SEC/1000)) - startTime > function_call_delay){
			/*Apply Force*/
			//WIP

			/*updates models*/
			std::lock_guard<std::mutex> guard(myMutex); // the access to this function is mutually exclusive
			for (int i = 0; i < BOIDS.size(); ++i)
			{
				BOIDS[i].timestep(1.0f);
			}
			startTime = clock()/(CLOCKS_PER_SEC/1000); //Reset time
		}
	}
}





int main(void){
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	/* Initialize the library */
	if (!glfwInit()){
		printf("Glfw could not be initialized!\n");
		exit(EXIT_FAILURE);
	}

	//Set OpenGl to use version 4.4
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WIDTH, HEIGHT, "Birds", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Add GLFW Callbacks */
	glfwSetKeyCallback(					window, key_callback); //Add (Escape) key handler
	glfwSetCursorPosCallback(		window, cursor_callback); //Add cursor callback
	glfwSetCursorEnterCallback(	window, cursorEnter_callback);
	glfwSetMouseButtonCallback(	window, mouseButton_callback);
	glfwSetScrollCallback(			window, scroll_callback);

	glfwSetInputMode(						window, GLFW_STICKY_MOUSE_BUTTONS, 1);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
	    std::cout << "Failed to initialize OpenGL context" << std::endl;
	    return -1;
	}
	gladLoadGL();

	cout << "OpenGL shader language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	

	printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback( MessageCallback, 0 );

	///////////////////////////////////////////////////////////
	//Setup GL Vertecies
	//////////////////////////////////////////////////////////
	//enable alpha support
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	//Build and compile our shader program
	Shader ourShader( "/media/sebastian/Daten/Programming/Flocking/c++ (Ubuntu)/res/shaders/core.vs", "/media/sebastian/Daten/Programming/Flocking/c++ (Ubuntu)/res/shaders/core.frag" );
	
	GLfloat vertices2[] =
	{
	// Positions         // Colors					
	 0.0f,  0.5f,  0.0f,   1.0f, 0.0f, 0.0f,   //Bottom Layer
	 1.0f,  0.0f,  0.0f,   1.0f, 0.0f, 0.0f,   //Bottom Layer
	 0.0f, -0.5f,  0.0f,   1.0f, 0.0f, 0.0f,   //Bottom Layer

	 0.0f,  0.0f,  0.5f,	 0.0f, 1.0f, 0.0f,   //Side 1
	 0.0f,  0.5f,  0.0f, 	 0.0f, 1.0f, 0.0f,
	 1.0f,  0.0f,  0.0f,   0.0f, 1.0f, 0.0f,

	 0.0f,  0.0f,  0.5f,	 0.0f, 0.0f, 1.0f,   //Side 2
	 0.0f, -0.5f,  0.0f,   0.0f, 0.0f, 1.0f,
	 1.0f,  0.0f,  0.0f,   0.0f, 0.0f, 1.0f,

	 0.0f,  0.5f,  0.0f,	 1.0f, 1.0f, 0.0f,   //Side 3
	 0.0f, -0.5f,  0.0f,   1.0f, 1.0f, 0.0f,
	 0.0f,  0.0f,  0.5f,   1.0f, 1.0f, 0.0f,

	};

	GLuint VBO, VAO;
	glGenVertexArrays( 1, &VAO);
	glGenBuffers( 1, &VBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointe
	glBindVertexArray( VAO );

	//Bind VBO
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertices2 ), vertices2, GL_STATIC_DRAW );

	//Position attribute
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), ( GLvoid * ) 0 );
	glEnableVertexAttribArray( 0 );
	//Color attribute
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), ( GLvoid * ) ( 3 * sizeof( GLfloat ) ) );
	glEnableVertexAttribArray( 1 );	

	glBindVertexArray( 0 ); // Unbind VAO (it's always a good thing to unbind any buffer/array //to prevent strange bugs)
  glEnable(GL_DEPTH_TEST);  
  
  //Init rng
  srand (time(NULL));
  int amount = 1500;



  //-----------------------------------------------------------------------------
  // Setup Boids
  //-----------------------------------------------------------------------------
  glm::mat4* modelMatrices;
  modelMatrices = setupBoids(amount, 200);


  //--------------------------------------
  // Timestep threaded (async & timed)
	//--------------------------------------
  std::thread t1(timestep, 40);




  /*FPS counter*/
  double lastTime = glfwGetTime();
  int nbFrames = 0;
	/* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
  	//------------------------------------------
  	//*fps*
  	//------------------------------------------
		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
			// printf and reset timer
			cout << 1000.0/double(nbFrames) <<"ms/frame (" << nbFrames << " fps)" << endl;
			nbFrames = 0;
			lastTime += 1.0;


		}
		//

		/* Poll for and process events */
		
  	float ratio;

		/*Set Viewport to window size*/
  	int width, height;
  	glfwGetFramebufferSize(window, &width, &height);
  	ratio = width / (float) height;
  	glViewport(0, 0, width, height);


		/* Render here */

		//clear color and depth buffer 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//Use our Shader
		ourShader.Use();

		// Set projections matrix
		glm::mat4 projection = glm::frustum(-1.0, 1.0, -1.0, 1.0, 1.1, 2000.0); //set identity
		ourShader.setMat4( "projection", projection); //Push into gpu

		//------------------------------------------
		// Handle Camera callback (src/callback.cpp)
		//------------------------------------------
		glm::mat4 camera(1.0f); //Set uniform

		camera = glm::translate( camera, glm::vec3(0.0f, 0.0f, zZoom));	//from callback
		camera = glm::rotate( camera, (float)yRot, glm::vec3( 0.05f, 0.0f, 0.0f ) ); //from callback
		camera = glm::rotate( camera, (float)xRot, glm::vec3( 0.0f, 0.05f, 0.0f ) ); //from callback

		ourShader.setMat4( "camera", camera); //Push into gpu

		//-------------------------------------
		// Draw containers (Boids)
		//-------------------------------------

		for (int i = 0; i < amount; ++i)
		{
			//cout << glm::to_string(modelMatrices[i]);
			std::lock_guard<std::mutex> guard(myMutex);
			ourShader.setMat4("model",BOIDS[i].getModelMatrix());
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 12);
			glBindVertexArray(0);
		}


		//------------------------------------
		//* Update all Boids* (timestep)
		//------------------------------------

		/*
		for (int i = 0; i < amount; ++i)
		{
			BOIDS[i].timestep(0.002f);
		}
		*/

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		glfwPollEvents();

  }
  RUNNING = false;
  glfwTerminate();
  return 0;


}