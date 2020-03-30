#include "../Include/Main.h"
#include "../VS2017/OBJloader.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 1600 / 2.0;
float lastY = 1200 / 2.0;
float fov = 45.0f;

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.1f, 1.5f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

GLuint setupModelVBO(std::string path, int& vertexCount) {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;

	//read the vertex data from the model's OBJ file
	loadOBJ(path.c_str(), vertices, normals, UVs);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); //Becomes active VAO
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).

	//Vertex VBO setup
	GLuint vertices_VBO;
	glGenBuffers(1, &vertices_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Normals VBO setup
	GLuint normals_VBO;
	glGenBuffers(1, &normals_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	//UVs VBO setup
	GLuint uvs_VBO;
	glGenBuffers(1, &uvs_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, uvs_VBO);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs, as we are using multiple VAOs)
	vertexCount = vertices.size();
	return VAO;
}

int main() {
	// Initialize GLFW and OpenGL version
	glfwInit();

#if defined(PLATFORM_OSX)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
	// On windows, we set OpenGL version to 2.1, to support more hardware
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

	// Create Window and rendering context using GLFW, resolution is 1024x768
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Comp371 - Project", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Disable mouse cursor when over the window!
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW
	glewExperimental = GL_TRUE; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to create GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}
    
	//----------------------------------------------------------------------
	//----------------------------- Shaders --------------------------------
	//----------------------------------------------------------------------

	Shader shader("../References/Shaders/3.3.shader.vs", "../References/Shaders/3.3.shader.fs");
    
	//----------------------------------------------------------------------
	//----------------------------- Objects --------------------------------
	//----------------------------------------------------------------------

    std::string treePath = "../Assets/low-poly-trees-pack.obj";
    int treeVertices;
    GLuint treeVAO = setupModelVBO(treePath, treeVertices);
    
	//----------------------------------------------------------------------
	//------------------------------ Render --------------------------------
	//----------------------------------------------------------------------
    while(!glfwWindowShouldClose(window)) {

        setTime();
		setWorld(shader);

		Commands::processCloseWindow(window);
		Commands::processCameraMovement(window, cameraPos, cameraFront, cameraUp, deltaTime);

        glClear(GL_COLOR_BUFFER_BIT);
     		
		// Tree!
		glBindVertexArray(0);
		glBindVertexArray(treeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, treeVAO);

		glDrawArrays(GL_TRIANGLES, 0, treeVertices);


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

void setWorld(Shader& shader) {
    shader.use();
    
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glm::mat4 projection = glm::perspective(glm::radians(fov), 1600.0f / 1200.0f, 0.1f, 100.0f);
	glm::mat4 model = glm::mat4(1.0f);

	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setMat4("model", model);
}

/*
* per-frame time logic
*/
void setTime() {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

/**
* Prints errors in the console
*/
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar* message, const void* userParam) {
    std::cout << "---------------------opengl-callback-start------------" << std::endl;
    std::cout << "message: " << message << std::endl;
    std::cout << "type: ";
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        std::cout << "ERROR";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        std::cout << "DEPRECATED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        std::cout << "UNDEFINED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        std::cout << "PORTABILITY";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        std::cout << "PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_OTHER:
        std::cout << "OTHER";
        break;
    }
    std::cout << std::endl;

    std::cout << "id: " << id << std::endl;
    std::cout << "severity: ";
    switch (severity) {
    case GL_DEBUG_SEVERITY_LOW:
        std::cout << "LOW";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        std::cout << "MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        std::cout << "HIGH";
        break;
    }
    std::cout << std::endl;
    std::cout << "---------------------opengl-callback-end--------------" << std::endl;
};


