/*
*
* COMP 371 Project
*
* TEAM
* Claudia Lapalme    - 40058454
* George  Mavroeidis - 40065356
* Armando Russo      - 40076164
* Daniel  Wiktorczyk - 40060894
* 
*
* SOURCES
* - Lab tutorials & learnopengl.com
* - Simple grass:        https://www.blendswap.com/blend/24539
* - Low Poly trees pack: https://www.blendswap.com/blend/23720
* 
*/

#include "../Include/Main.h"

int main(int argc, char*argv[]) {

	///////////////////////////////////////////////////////////////////
	///////////////////////// Intialization ///////////////////////////
	///////////////////////////////////////////////////////////////////

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

	GLFWwindow* window = glfwCreateWindow(1024, 768, "Comp371 - Final Project", NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Disable mouse cursor when over the window!
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

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

#if defined(PLATFORM_OSX)
	std::string cubePath   = "Models/cube.obj";
	std::string spherePath = "Models/sphere.obj";
	std::string treePath   = "Models/low-poly-trees-pack.obj";
#else
	std::string cubePath   = "../Assets/Models/cube.obj";
	std::string spherePath = "../Assets/Models/sphere.obj";
	std::string treePath   = "../Assets/Models/low-poly-trees-pack-tree.obj";
#endif
	int cubeVertices;
	GLuint cubeVAO = setupModelVBO(cubePath, cubeVertices);
	int sphereVertices;
	GLuint sphereVAO = setupModelVBO(spherePath, sphereVertices);
	int treeVertices;
	GLuint treeVAO = setupModelVBO(treePath, treeVertices);


	///////////////////////////////////////////////////////////////////
	/////////////////////////// Textures //////////////////////////////
	///////////////////////////////////////////////////////////////////

#if defined(PLATFORM_OSX)
	GLuint carrotTextureID = loadTexture("Textures/carrot.jpg");
	GLuint metalTextureID  = loadTexture("Textures/metal.jpg");
	GLuint snowTextureID   = loadTexture("Textures/snow.jpg");
#else
	GLuint carrotTextureID = loadTexture("../Assets/Textures/carrot.jpg");
	GLuint metalTextureID  = loadTexture("../Assets/Textures/metal.jpg");
	GLuint snowTextureID   = loadTexture("../Assets/Textures/snow.jpg");
#endif

	///////////////////////////////////////////////////////////////////
	/////////////////////////// Shaders ///////////////////////////////
	///////////////////////////////////////////////////////////////////

	// Compile and link shaders here ...
	int colorShaderProgram = compileAndLinkShaders("../References/Shaders/3.3.shader.vs", "../References/Shaders/3.3.shader.fs");
	int texturedShaderProgram = compileAndLinkShaders("../References/Shaders/3.3.texShader.vs", "../References/Shaders/3.3.texShader.fs");
	
	// Set View and Projection matrices on both shaders
	setViewMatrix(colorShaderProgram, viewMatrix);
	setViewMatrix(texturedShaderProgram, viewMatrix);

	setProjectionMatrix(colorShaderProgram, projectionMatrix);
	setProjectionMatrix(texturedShaderProgram, projectionMatrix);

	///////////////////////////////////////////////////////////////////
	/////////////////////////// Uniforms //////////////////////////////
	///////////////////////////////////////////////////////////////////

	GLuint worldMatrixLocationColor = glGetUniformLocation(colorShaderProgram, "worldMatrix");
	GLuint worldMatrixLocationTexture = glGetUniformLocation(texturedShaderProgram, "worldMatrix");
	GLuint colorLocation = glGetUniformLocation(colorShaderProgram, "objectColor");
	GLuint viewLocationColor = glGetUniformLocation(colorShaderProgram, "viewPosition");
	GLuint viewLocationTexture = glGetUniformLocation(texturedShaderProgram, "viewPosition");
	GLuint lightLocationColor = glGetUniformLocation(colorShaderProgram, "lightPosition");
	GLuint lightLocationTexture = glGetUniformLocation(texturedShaderProgram, "lightPosition");

	///////////////////////////////////////////////////////////////////
	/////////////////////////// Objects ///////////////////////////////
	///////////////////////////////////////////////////////////////////

	// Initialize the Snowman
	Snowman snowman = Snowman(worldMatrixLocationColor, 
							  worldMatrixLocationTexture, 
							  colorLocation, 
							  colorShaderProgram, 
							  texturedShaderProgram, 
							  sphereVertices, 
							  cubeVAO, 
							  sphereVAO,
							  carrotTextureID, 
							  metalTextureID);

	 LoadedObject light       = LoadedObject(cubeVAO);
	 NonCollidableObject tree = NonCollidableObject(treeVAO);
	 Forest forest            = Forest();
	 Acre testAcre            = Acre(cubeVAO);

	 // Baby Blue Background
	 glClearColor(0.53f, 0.81f, 0.94f, 1.0f);

	 ///////////////////////////////////////////////////////////////////
	 //////////////////////////// Light ////////////////////////////////
	 ///////////////////////////////////////////////////////////////////

	glm::vec3 lightPosition = glm::vec3(15.0f, 20.0f, 5.0f);
	glUniform3fv(lightLocationColor, 1, value_ptr(lightPosition));
	glUniform3fv(lightLocationTexture, 1, value_ptr(lightPosition));

	///////////////////////////////////////////////////////////////////
	///////////////////////// Render Loop /////////////////////////////
	///////////////////////////////////////////////////////////////////

	while (!glfwWindowShouldClose(window)) {

		// For movement, if shift is held, it means a small movement for rotation, scaling, and transposing
		bool shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Commands::areTexturesToggled(window, canToggleTexturing, texturing);
		Commands::closeWindow(window);
		Commands::setRenderingMode(window);

		// Draw Grid
		if (texturing) {
			glUseProgram(texturedShaderProgram);

			glActiveTexture(GL_TEXTURE0);
			GLuint textureLocation = glGetUniformLocation(texturedShaderProgram, "textureSampler");
			glBindTexture(GL_TEXTURE_2D, snowTextureID);
			glUniform1i(textureLocation, 0);

			glm::mat4 groundPatch = scale(glm::mat4(1.0f), glm::vec3(10.0f, 0.02f, 10.0f));
			glm::mat4 currentGroundPatch;
			for (int i = -5; i < 5; i++) {
				for (int j = -5; j < 5; j++) {
					currentGroundPatch = translate(glm::mat4(1.0f), glm::vec3(i * 10.0f, -0.01f, j * 10.0f)) * groundPatch;
					glUniformMatrix4fv(worldMatrixLocationTexture, 1, GL_FALSE, &currentGroundPatch[0][0]);
					glDrawArrays(GL_TRIANGLES, 12, 18);
				}
			}
		}
		else {
			glUseProgram(colorShaderProgram);

			glm::mat4 gridLineMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 0.02f, 0.05f));
			glm::mat4 currentGridLineMatrix;
			for (int i = -50; i < 50; i++) {
				currentGridLineMatrix = translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.01f, i * 1.0f)) * gridLineMatrix;
				glUniformMatrix4fv(worldMatrixLocationColor, 1, GL_FALSE, &currentGridLineMatrix[0][0]);
				glUniform3fv(colorLocation, 1, value_ptr(gridColor));
				glDrawArrays(GL_TRIANGLES, 12, 18);
				glDrawArrays(GL_TRIANGLES, 30, 36);
			}
			gridLineMatrix = scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.02f, 100.0f));
			for (int i = -50; i < 50; i++) {
				currentGridLineMatrix = translate(glm::mat4(1.0f), glm::vec3(i * 1.0f, -0.01f, 0.0f)) * gridLineMatrix;
				glUniformMatrix4fv(worldMatrixLocationColor, 1, GL_FALSE, &currentGridLineMatrix[0][0]);
				glUniform3fv(colorLocation, 1, value_ptr(gridColor));
				glDrawArrays(GL_TRIANGLES, 12, 18);
				glDrawArrays(GL_TRIANGLES, 30, 36);
			}
		}

		// Forest
		bind(colorShaderProgram, forest.getVAO());
		glm::mat4 forestLocation(1.0f);
		forest.draw(&forestLocation[0][0], value_ptr(green), worldMatrixLocationColor, colorLocation);

		// Acre
		bind(colorShaderProgram, cubeVAO);
		glm::mat4 acreModel(1.0f);
		acreModel = scale(glm::mat4(1.0f), glm::vec3(3.0f,   0.5f, 3.0f));
		acreModel = translate(acreModel, glm::vec3(0.0f, -0.5f, 0.0f ));
		testAcre.draw(&acreModel[0][0], value_ptr(orange), worldMatrixLocationColor, colorLocation, cubeVertices);

		// Light
		bind(colorShaderProgram, sphereVAO);
		glm::mat4 lightBulbMatrix = translate(glm::mat4(1.0f), lightPosition) * scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
		glUniform3fv(lightLocationColor, 1, value_ptr(lightPosition));
		light.draw(&lightBulbMatrix[0][0], value_ptr(white), worldMatrixLocationColor, colorLocation, sphereVertices);

		// Tree!
		bind(colorShaderProgram, treeVAO);
		glm::mat4 base = scale(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 5.0f));
		tree.draw(&base[0][0], value_ptr(turquoise), worldMatrixLocationColor, colorLocation, treeVertices);
		
		//Snowman
		snowman.draw(texturing);
		snowman.scaleSnowman(window, shift, canScaleIncrement);
		snowman.rotateSnowman(window, shift, canRandomPlacement);
		snowman.translateSnowman(window, shift, canMoveIncrement);
		snowman.randomTranslationSnowman(window, shift, canRandomPlacement);
		snowman.update();

		setCameraVariables(window, mousePosX, mousePosY, lastMousePosX, lastMousePosY, cameraHorizontalAngle, cameraVerticalAngle, cameraLookAt, cameraSideVector);
		Commands::panCamera(window, cameraHorizontalAngle, dx, cameraAngularSpeed);
		Commands::tiltCamera(window, cameraVerticalAngle, dy, cameraAngularSpeed);
		Commands::zoomCamera(window, currentFOV, dy, projectionMatrix, colorShaderProgram, texturedShaderProgram, setProjectionMatrix);

		Commands::setWorldRotation(window, worldRotationAboutYAxis, worldRotationAboutXAxis);
		sendViewMatrixToShader(cameraPosition, cameraLookAt, cameraUp, colorShaderProgram, texturedShaderProgram, viewLocationColor, viewLocationTexture);
		sendWorldRotationMatrixToShader(worldRotationMatrix, worldRotationAboutYAxis, worldRotationAboutXAxis, colorShaderProgram, texturedShaderProgram);
	
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

GLuint setupModelVBO(std::string path, int& vertexCount) {
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

GLuint loadTexture(const char* filename) {
	// Step1 Create and bind textures
	GLuint textureId = 0;
	glGenTextures(1, &textureId);
	assert(textureId != 0);

	glBindTexture(GL_TEXTURE_2D, textureId);

	// Step2 Set filter parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Step3 Load Textures with dimension data
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (!data) {
		std::cerr << "Error::Texture could not load texture file:" << filename << std::endl;
		return 0;
	}

	// Step4 Upload the texture to the PU
	GLenum format = 0;
	if (nrChannels == 1) {
		format = GL_RED;
	}
	else if (nrChannels == 3) {
		format = GL_RGB;
	}
	else if (nrChannels == 4) {
		format = GL_RGBA;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	// Step5 Free resources
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureId;
}

void setProjectionMatrix(int shaderProgram, glm::mat4 projectionMatrix) {
	glUseProgram(shaderProgram);
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void setViewMatrix(int shaderProgram, glm::mat4 viewMatrix) {
	glUseProgram(shaderProgram);
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}

void setWorldMatrix(int shaderProgram, glm::mat4 worldMatrix) {
	glUseProgram(shaderProgram);
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
}

void setWorldRotationMatrix(int shaderProgram, glm::mat4 worldRotationMatrix) {
	glUseProgram(shaderProgram);
	GLuint worldRotationMatrixLocation = glGetUniformLocation(shaderProgram, "worldRotationMatrix");
	glUniformMatrix4fv(worldRotationMatrixLocation, 1, GL_FALSE, &worldRotationMatrix[0][0]);
}

void sendViewMatrixToShader(const glm::vec3& camPos, 
							const glm::vec3& camLookAt, 
							const glm::vec3 camUp, 
							const int& colorShader, 
							const int& textShader, 
							const GLuint& viewLocationColor, 
							const GLuint& viewLocationTexture) {
	glm::mat4 viewMatrix(1.0f);
	viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
	setViewMatrix(colorShader, viewMatrix);
	setViewMatrix(textShader, viewMatrix);
	glUniform3fv(viewLocationColor, 1, value_ptr(cameraPosition));
	glUniform3fv(viewLocationTexture, 1, value_ptr(cameraPosition));
}

void sendWorldRotationMatrixToShader(glm::mat4& worldRotationMatrix, const float& rotYaxis, const float& rotXaxis, const int& colorShader, const int& textShader) {
	worldRotationMatrix = rotate(glm::mat4(1.0f), worldRotationAboutYAxis, glm::vec3(0.0f, 1.0f, 0.0f)) * rotate(glm::mat4(1.0f), worldRotationAboutXAxis, glm::vec3(1.0f, 0.0f, 0.0f));
	setWorldRotationMatrix(colorShader, worldRotationMatrix);
	setWorldRotationMatrix(textShader, worldRotationMatrix);
}

void setCameraVariables(GLFWwindow* window, 
						double& mousePosX, 
						double& mousePosY, 
						double& lastMousePosX, 
						double& lastMousePosY, 
						float& camHorAngle, 
						float& camVertAngle,
						glm::vec3& cameraLookAt,
						glm::vec3& cameraSideVector) {

	glfwGetCursorPos(window, &mousePosX, &mousePosY);

	dx = mousePosX - lastMousePosX;
	dy = mousePosY - lastMousePosY;

	lastMousePosX = mousePosX;
	lastMousePosY = mousePosY;

	theta = glm::radians(cameraHorizontalAngle);
	phi = glm::radians(cameraVerticalAngle);

	// Clamp vertical angle to [-85, 85] degrees
	camVertAngle = glm::max(-85.0f, glm::min(85.0f, camVertAngle));

	// Allow camera to rotate about horizontally
	if (camHorAngle > 360) {
		camHorAngle -= 360;
	}
	else if (camHorAngle < -360) {
		camHorAngle += 360;
	}

	cameraLookAt = glm::vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
	cameraSideVector = cross(cameraLookAt, glm::vec3(0.0f, 1.0f, 0.0f));
	normalize(cameraSideVector);
}

/**
* Bind the desired VAO to the buffer
*/
void bind(int& currentShader, GLuint const& VAO) {
	glUseProgram(currentShader);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VAO);
}
