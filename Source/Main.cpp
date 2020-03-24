#include "../Include/Main.h"

int main() {

	glfwInit();

#if defined(PLATFORM_OSX)	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    GLFWwindow* window = glfwCreateWindow(1600, 1200, "Comp371 - Project", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
    
	//----------------------------------------------------------------------
	//----------------------------- Shaders --------------------------------
	//----------------------------------------------------------------------

	Shader shader("../References/Shaders/3.3.shader.vs", "../References/Shaders/3.3.shader.fs");
    
	//----------------------------------------------------------------------
	//----------------------------- Objects --------------------------------
	//----------------------------------------------------------------------

    EnvironmentPlane env = EnvironmentPlane();
    
	//----------------------------------------------------------------------
	//------------------------------ Render --------------------------------
	//----------------------------------------------------------------------
    while(!glfwWindowShouldClose(window)) {

		Commands::processInput(window);
      		
        env.draw(shader);

        glfwSwapBuffers(window); 
        glfwPollEvents();
    }
    
    glfwTerminate();
    
	return 0;
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar* message, const void* userParam) {
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
};
