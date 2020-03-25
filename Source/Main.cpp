#include "../Include/Main.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 1600 / 2.0;
float lastY = 1200 / 2.0;
float fov = 45.0f;

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.5f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

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
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    //capture initial mouse position
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
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
        setTime();

		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glm::mat4 projection = glm::perspective(glm::radians(fov), 1600.0f / 1200.0f, 0.1f, 100.0f);
		glm::mat4 model = glm::mat4(1.0f);

		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setMat4("model", model);

		Commands::processCloseWindow(window);
		Commands::processCameraMovement(window, cameraPos, cameraFront, cameraUp, deltaTime);

        glClear(GL_COLOR_BUFFER_BIT);
     		
        env.draw(shader);

        glfwSwapBuffers(window); 
        glfwPollEvents();
    }
    
    glfwTerminate();
    
	return 0;
}

/*
* pan and tilt camera
*/
void mouseCallback(GLFWwindow* window, double xPos, double yPos) {

	//to avoid big jump from detecting the mouse where it actually is
	if (firstMouse) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xOffset = xPos - lastX;
	float YOffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	const float sensitivity = 0.05f;
	xOffset *= sensitivity;
	YOffset *= sensitivity;

	yaw += xOffset;
	pitch += YOffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

/*
* zoom in and zoom out
*/
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	if (fov >= 1.0f && fov <= 45.0f) {
		fov -= yOffset;
	}
	else if (fov <= 1.0f) {
		fov = 1.0f;
	}
	else if (fov >= 45.0f) {
		fov = 45.0f;
	}
}

/*
* per-frame time logic
*/
void setTime() {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar* message, const void* userParam) {
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
};
