/*
*
* COMP 371 Project 
*
* Low poly procedural world
*
* TEAM
* Claudia Lapalme    - 40058454
* George  Mavroeidis - 40065356
* Armando Russo      - 40076164
* Daniel  Wiktorczyk - 40060894
*
*
* RESOURCES
* - Lab tutorials & learnopengl.com
* - Simple grass:        https://www.blendswap.com/blend/24539
* - Low Poly trees pack: https://www.blendswap.com/blend/23720
*
*/

#include "../Include/Main.h"

int main(int argc, char* argv[]) {

	///////////////////////////////////////////////////////////////////
	///////////////////////// Intialization ///////////////////////////
	///////////////////////////////////////////////////////////////////

	srand(time(NULL));
	glewExperimental = GL_TRUE;
	glfwInit();

#if defined(PLATFORM_OSX)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif
	GLFWwindow* window;
	float windowWidth;
	float windowHeight;
	if (HD) {
		windowWidth = 1920.0f;
		windowHeight = 1080.0f;
	}
	else {
		windowWidth = 1024;
		windowHeight = 768.0f;
	}

	if (StartInFullScreen) {
		window = glfwCreateWindow(windowWidth, windowHeight, "Comp371 - Final Project - Woodland Wanderer", glfwGetPrimaryMonitor(), NULL);
	}
	else {
		window = glfwCreateWindow(windowWidth, windowHeight, "Comp371 - Final Project - Woodland Wanderer", NULL, NULL);
	}

	if (window == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glViewport(0, 0, windowWidth, windowHeight);

	// Initialize GLEW
	glewExperimental = GL_TRUE; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to create GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	///////////////////////////////////////////////////////////////////
	////////////////////////// .obj set up ////////////////////////////
	///////////////////////////////////////////////////////////////////

	int cubeVertices;
	GLuint cubeVAO = setupModelVBO_OLD(cubeAsset, cubeVertices);
	int sphereVertices;
	GLuint sphereVAO = setupModelVBO_OLD(sphereAsset, sphereVertices);

	///////////////////////////////////////////////////////////////////
	/////////////////////////// Shaders ///////////////////////////////
	///////////////////////////////////////////////////////////////////

	// Compile and link shaders here ...
	int shaderProgram = compileAndLinkShaders("../References/Shaders/3.3.shader.vs", "../References/Shaders/3.3.shader.fs");

	// Set View and Projection matrices on both shaders
	setViewMatrix(shaderProgram);
	setProjectionMatrix(shaderProgram);

	///////////////////////////////////////////////////////////////////
	/////////////////////////// Uniforms //////////////////////////////
	///////////////////////////////////////////////////////////////////

	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLuint colorLocation       = glGetUniformLocation(shaderProgram, "objectColor");
	GLuint viewLocation        = glGetUniformLocation(shaderProgram, "viewPosition");
	GLuint lightLocation       = glGetUniformLocation(shaderProgram, "lightPosition");
	GLuint projLocation        = glGetUniformLocation(shaderProgram, "projectionMatrix");

	///////////////////////////////////////////////////////////////////
	/////////////////////////// Objects ///////////////////////////////
	///////////////////////////////////////////////////////////////////

	AssetsService::initInstance();

	// Initialize the Snowman
	Snowman snowman = Snowman(worldMatrixLocation,
							  colorLocation,
							  shaderProgram,
							  sphereVertices,
							  cubeVAO,
							  sphereVAO);

	Model light     = Model(AssetsService::getInstance()->getCube().getVAO(), AssetsService::getInstance()->getCube().getVectorSize(), white);
	
	Acre acre       = Acre(glm::vec3(0.0f, 0.0f, 0.0f));
	Forest forest   = Forest(snowman);

	// Sky
	glClearColor(0.2f, 0.0f, 0.2f, 1.0f);

	///////////////////////////////////////////////////////////////////
	//////////////////////////// Light ////////////////////////////////
	///////////////////////////////////////////////////////////////////

	glm::vec3 lightPosition = glm::vec3(15.0f, 1000.0f, 5.0f);

	///////////////////////////////////////////////////////////////////
	///////////////////////// Render Loop /////////////////////////////
	///////////////////////////////////////////////////////////////////

	while (!glfwWindowShouldClose(window)) {
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &glm::mat4(1.0f)[0][0]);

		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// For movement, if shift is held, it means a small movement for rotation, scaling, and transposing
		bool shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Commands::closeWindow(window);
		Commands::setRenderingMode(window);
		Commands::processCameraDirection(window, cameraPosition, cameraLookAt, cameraUp, deltaTime);
		
		glUseProgram(shaderProgram);

		if (DebugMode && DrawGridLines) {
			glm::mat4 gridLineMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.10f, 0.00f)) * glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 0.02f, 0.05f));
			glm::mat4 currentGridLineMatrix;
			for (int i = -ForestWidth * AcreWidth; i < ForestWidth * AcreWidth; i++) {
				currentGridLineMatrix = translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.01f, 5.0f + i * TileWidth)) * gridLineMatrix;
				glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &currentGridLineMatrix[0][0]);
				glUniform3fv(colorLocation, 1, value_ptr(gridColor));
				glDrawArrays(GL_TRIANGLES, 12, 18);
				glDrawArrays(GL_TRIANGLES, 30, 36);
			}
			gridLineMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.10f, 0.00f)) * scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.02f, 100.0f));
			for (int i = -ForestWidth * AcreWidth; i < ForestWidth * AcreWidth; i++) {
				currentGridLineMatrix = translate(glm::mat4(1.0f), glm::vec3(5.0f + i * TileWidth, -0.01f, 0.0f)) * gridLineMatrix;
				glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &currentGridLineMatrix[0][0]);
				glUniform3fv(colorLocation, 1, value_ptr(gridColor));
				glDrawArrays(GL_TRIANGLES, 12, 18);
				glDrawArrays(GL_TRIANGLES, 30, 36);
			}
		}
		
		// Abstractions
		forest.draw(worldMatrixLocation, colorLocation);

		// Light
		glm::mat4 lightBulbMatrix = translate(glm::mat4(1.0f), lightPosition) * scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
		light.setModelTransformMatrix(lightBulbMatrix);
		light.draw(worldMatrixLocation, colorLocation);

		//Snowman
		std::vector<CollidableModel> collidables = forest.getNearbyCollidables();
		bool checker = snowman.CheckCollision(collidables);
		snowman.CheckCollisionX(collidables, checker);
		snowman.CheckCollisionZ(collidables, checker);
		snowman.draw();
		snowman.scaleSnowman(window, shift, canScaleIncrement);
		snowman.rotateSnowman(window, shift, canRotateIncrement);
		snowman.translateSnowman(window, shift, canMoveIncrement);
		snowman.randomTranslationSnowman(window, shift, canRandomPlacement);
		snowman.update();

		setProjectionMatrix(shaderProgram);
		setViewMatrix(shaderProgram);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

/**
* Compile and link shader program
* @return shader program ID
*/
int compileAndLinkShaders(const std::string vertexPath, const std::string fragmentPath) {
	//retrieve the vertex / fragment source code from filepath

	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	//ensure ifstream can throw exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		//close file handlers
		vShaderFile.close();
		fShaderFile.close();

		//convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure ex) {
		std::cout << "ERROR: SHADER --> FILE NOT SUCCESSFULLY READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

// TODO refactor into Model class. Main should create Models with just path and color! 
GLuint setupModelVBO_OLD(std::string path, int& vertexCount) {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;

	//read the vertex data from the model's OBJ file
	loadOBJ(path.c_str(), vertices, normals, UVs);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

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

	glBindVertexArray(0);
	vertexCount = vertices.size();
	return VAO;
}

void setProjectionMatrix(const int& shaderProgram) {
	projectionMatrix = glm::perspective(fov, (HD ? WindowWidthHD : WindowWidth) / (HD ? WindowHeightHD : WindowHeight), 0.01f, 100.0f);
	glUseProgram(shaderProgram);
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void setViewMatrix(const int& shaderProgram) {
	viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
	glUseProgram(shaderProgram);
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
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

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}

	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraLookAt = glm::normalize(direction);
}