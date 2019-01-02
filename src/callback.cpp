#include <GLFW/glfw3.h>
#include <iostream>
#include "callback.h"
using namespace std;


float increment = 1;
//Key callback for now only escape ;)
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	    glfwSetWindowShouldClose(window, GL_TRUE);
	if ((key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) && action == GLFW_PRESS){
		increment = 5;
	}
	if ((key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) && action == GLFW_RELEASE){
		increment = 1;
	}	
}


//Cursor callbacks
double xRot = 0;
double yRot = 0; //extern in callback.h
double oldMouseX, oldMouseY;
void cursor_callback(GLFWwindow* window, double xpos, double ypos){
	if (rightMouseButtonPressed)
	{
		//X rotation
		if ((xpos - oldMouseX) > 0){
			xRot += 0.05f * increment;
		}
		else if ((xpos - oldMouseX) < 0){
			xRot -= 0.05f * increment;
		}

		//Y rotation
		if ((ypos - oldMouseY) > 0){
			yRot += 0.05f * increment;
		}
		else if ((ypos - oldMouseY) < 0){
			yRot -= 0.05f * increment;
		}
		oldMouseX = xpos;
		oldMouseY = ypos;
		//cout << xRot << " : " << yRot << endl;
	}

}

bool cursorInsideWindow = true;
void cursorEnter_callback(GLFWwindow* window, int entered){
	if (entered) cursorInsideWindow = true;
	else cursorInsideWindow = false;
}
bool rightMouseButtonPressed = false;
void mouseButton_callback(GLFWwindow* window, int button, int action, int mods){
	if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
		cout << "Right mouse button pressed" <<endl;
		rightMouseButtonPressed = true;
	}
	if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE){
		cout << "Right mouse button released" <<endl;
		rightMouseButtonPressed = false;
	}

}

double zZoom = -200; //init
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset){
	if(yOffset > 0){
		//Zoom in
		zZoom += 0.1 * increment; 
	}
	else if (yOffset < 0){
		//Zoom out
		zZoom -= 0.1 * increment;
	}
}


//Error OpenGL
void GLAPIENTRY MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}



//Error glfw
void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
