#ifndef CALLBACK_H
#define CALLBACK_H

//---------------------------
//Keys and cursors
//---------------------------

/*types*/
extern bool cursorInsideWindow;
extern bool rightMouseButtonPressed;
extern double zZoom;
extern double xRot, yRot; 

/*functions*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_callback(GLFWwindow* window, double xpos, double ypos); //Gets position
void cursorEnter_callback(GLFWwindow* window, int entered); //Check if mouse is inside window
void mouseButton_callback(GLFWwindow* window, int button, int action, int mods); //Get mouse button callback
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);


//---------------------------
//Errors
//---------------------------

//functions
void error_callback(int error, const char* description); //glfw errors
void GLAPIENTRY MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam ); //OpenGL


#endif