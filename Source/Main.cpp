/*
*
* COMP 371 Project
*
*
* Adapted from template code:
*   COMP 371 Labs Framework
* 
* Used lab tutorials from class and learnopengl.com tutorials 



WE NEED TO SOURCE THE OBJECT FILES !!!



*/

#include "../Include/Main.h"

int main(int argc, char*argv[]) {
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

	// Initialize GLEW
	glewExperimental = GL_TRUE; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to create GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}

#if defined(PLATFORM_OSX)
	std::string cubePath = "Models/cube.obj";
	std::string heraclesPath = "Models/heracles.obj";
#else
	std::string cubePath = "../Assets/Models/cube.obj";
	std::string spherePath = "../Assets/Models/sphere.obj";
	std::string treePath = "../Assets/Models/low-poly-trees-pack.obj";
#endif
	int cubeVertices;
	GLuint cubeVAO = setupModelVBO(cubePath, cubeVertices);
	int sphereVertices;
	GLuint sphereVAO = setupModelVBO(spherePath, sphereVertices);
	int treeVertices;
	GLuint treeVAO = setupModelVBO(treePath, treeVertices);

	// Load Textures
#if defined(PLATFORM_OSX)
	GLuint carrotTextureID = loadTexture("Textures/carrot.jpg");
	GLuint metalTextureID = loadTexture("Textures/metal.jpg");
	GLuint snowTextureID = loadTexture("Textures/snow.jpg");
#else
	GLuint carrotTextureID = loadTexture("../Assets/Textures/carrot.jpg");
	GLuint metalTextureID = loadTexture("../Assets/Textures/metal.jpg");
	GLuint snowTextureID = loadTexture("../Assets/Textures/snow.jpg");
#endif

	// Baby Blue Background
	glClearColor(0.53f, 0.81f, 0.94f, 1.0f);

	// Compile and link shaders here ...
	int colorShaderProgram = compileAndLinkShaders(getVertexShaderSource(), getFragmentShaderSource());
	int texturedShaderProgram = compileAndLinkShaders(getTexturedVertexShaderSource(), getTexturedFragmentShaderSource());
	
	// Set View and Projection matrices on both shaders
	setViewMatrix(colorShaderProgram, viewMatrix);
	setViewMatrix(texturedShaderProgram, viewMatrix);

	setProjectionMatrix(colorShaderProgram, projectionMatrix);
	setProjectionMatrix(texturedShaderProgram, projectionMatrix);

	double lastMousePosX, lastMousePosY;
	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

	// Enable Backface culling and Depth Test
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

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

	 LoadedObject light = LoadedObject(cubeVAO);
	 LoadedObject tree = LoadedObject(treeVAO);

	 ///////////////////////////////////////////////////////////////////
	 //////////////////////////// Light ////////////////////////////////
	 ///////////////////////////////////////////////////////////////////

	glm::vec3 lightPosition = glm::vec3(0.0f, 30.0f, 0.0f);
	glUniform3fv(lightLocationColor, 1, value_ptr(lightPosition));
	glUniform3fv(lightLocationTexture, 1, value_ptr(lightPosition));

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Commands::areTexturesToggled(window, canToggleTexturing, texturing);

		// Light
		bind(colorShaderProgram, sphereVAO);
		glm::mat4 lightBulbMatrix = translate(glm::mat4(1.0f), lightPosition) * scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
		light.draw(&lightBulbMatrix[0][0], value_ptr(white), worldMatrixLocationColor, colorLocation, sphereVertices);

		// Tree!
		bind(colorShaderProgram, treeVAO);
		glm::mat4 base = scale(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 5.0f));
		tree.draw(&base[0][0], value_ptr(turquoise), worldMatrixLocationColor, colorLocation, treeVertices);
		
		//Snowman
		snowman.draw(texturing);

		// Handle inputs
		// For movement, if shift is held, it means a small movement for rotation, scaling, and transposing
		bool shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;

		Commands::closeWindow(window);
		Commands::setRenderingMode(window);

		snowman.scaleSnowman(window, shift, canScaleIncrement);
		snowman.rotateSnowman(window, shift, canRandomPlacement);
		snowman.translateSnowman(window, shift, canMoveIncrement);
		snowman.randomTranslationSnowman(window, shift, canRandomPlacement);
		
		// Snowman Update
		snowman.update();

		setCameraVariables(window, mousePosX, mousePosY, lastMousePosX, lastMousePosY, cameraHorizontalAngle, cameraVerticalAngle, cameraLookAt, cameraSideVector);

		Commands::panCamera(window, cameraHorizontalAngle, dx, cameraAngularSpeed);
		Commands::tiltCamera(window, cameraVerticalAngle, dy, cameraAngularSpeed);
		Commands::zoomCamera(window, currentFOV, dy, projectionMatrix, colorShaderProgram, texturedShaderProgram, setProjectionMatrix);

		sendViewMatrixToShader(cameraPosition, cameraLookAt, cameraUp, colorShaderProgram, texturedShaderProgram, viewLocationColor, viewLocationTexture);

		Commands::setWorldRotation(window, worldRotationAboutYAxis, worldRotationAboutXAxis);
		sendWorldRotationMatrixToShader(worldRotationMatrix, worldRotationAboutYAxis, worldRotationAboutXAxis, colorShaderProgram, texturedShaderProgram);
	
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}


const char* getVertexShaderSource() {
	return
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;"
		"layout (location = 1) in vec3 aNormal;"
		"layout (location = 2) in vec3 aColor;"
		""
		"uniform mat4 worldMatrix = mat4(1.0f);"
		"uniform mat4 worldRotationMatrix = mat4(1.0f);"
		"uniform mat4 viewMatrix = mat4(1.0);"
		"uniform mat4 projectionMatrix = mat4(1.0);"
		""
		"out vec3 vertexColor;"
		"out vec3 Normal;"
		"out vec3 FragPos;"
		""
		"void main()"
		"{"
		"   vertexColor = aColor;"
		"   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldRotationMatrix * worldMatrix;"
		"   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
		"	Normal = mat3(transpose(inverse(worldMatrix))) * aNormal;" // inverse and transpose since scaling distorts the normal! 
		"	FragPos = vec3(worldMatrix * vec4(aPos, 1.0f));"
		"}";
}

const char* getFragmentShaderSource() {
	return
		"#version 330 core\n"
		"uniform vec3 objectColor;"
		"uniform vec3 lightPosition;"
		"uniform vec3 viewPosition;"
		"in vec3 vertexColor;"
		"in vec3 Normal;"
		"in vec3 FragPos;"
		"out vec4 FragColor;"
		""
		"void main()"
		"{"
		// Add Ambient light, part 1/3 for Phong Point Lighting
		"	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);"
		"	float ambientFactor = 0.24;"
		"	vec3 ambientLight = ambientFactor * lightColor;"
		""
		// Add Diffuse lighting, part 2/3 for Phong Point Lighting
		"	vec3 norm = normalize(Normal);" // We want direction only so normalize
		"	vec3 lightDirection = normalize(lightPosition - FragPos); "
		"	float diff = max(dot(norm, lightDirection), 0.0);" // Non negative, so max
		"	vec3 diffusedLight = diff * lightColor;" // Diffuse impact
		""
		// Add Specular lighting, part 3/3 for Phong Point Lighting
		"	float specularStrength = 0.5;"
		"	vec3 viewDirection = normalize(viewPosition - FragPos);"
		"	vec3 reflectDirection = reflect(-lightDirection, norm);"
		"	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 24);"
		"	vec3 specularLight = specularStrength * spec * lightColor;"
		""
		// Result of Phong Point Lighting: 
		"	vec3 result = (ambientLight + diffusedLight + specularLight) * objectColor;"
		"   FragColor = vec4(result, 1.0f);"
		"}";
}

const char* getTexturedVertexShaderSource() {
	return

		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;"
		"layout (location = 1) in vec3 aNormal;"
		"layout (location = 2) in vec2 aUV;"
		""
		"uniform mat4 worldMatrix = mat4(1.0f);"
		"uniform mat4 worldRotationMatrix = mat4(1.0f);"
		"uniform mat4 viewMatrix = mat4(1.0);"
		"uniform mat4 projectionMatrix = mat4(1.0);"
		""
		"out vec2 vertexUV;"
		"out vec3 Normal;"
		"out vec3 FragPos;"
		""
		"void main()"
		"{"
		"   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldRotationMatrix * worldMatrix;"
		"   gl_Position = modelViewProjection * vec4(aPos, 1.0);"
		"	Normal = mat3(transpose(inverse(worldMatrix))) * aNormal;" // inverse and transpose since scaling distorts the normal! 
		"	FragPos = vec3(worldMatrix * vec4(aPos, 1.0f));"
		"   vertexUV = aUV;"
		"}";
}

const char* getTexturedFragmentShaderSource() {
	return
		"#version 330 core\n"
		"uniform vec3 lightPosition;"
		"uniform vec3 viewPosition;"
		"uniform sampler2D textureSampler;"
		"in vec3 vertexColor;"
		"in vec2 vertexUV;"
		"in vec3 Normal;"
		"in vec3 FragPos;"
		"out vec4 FragColor;"
		""
		"void main()"
		"{"
		"	vec4 textureColor = texture(textureSampler, vertexUV);"
		""
		// Add Ambient light, part 1/3 for Phong Point Lighting
		"	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);"
		"	float ambientFactor = 0.24;"
		"	vec3 ambientLight = ambientFactor * lightColor;"
		""
		// Add Diffuse lighting, part 2/3 for Phong Point Lighting
		"	vec3 norm = normalize(Normal);" // We want direction only so normalize
		"	vec3 lightDirection = normalize(lightPosition - FragPos); "
		"	float diff = max(dot(norm, lightDirection), 0.0);" // Non negative, so max
		"	vec3 diffusedLight = diff * lightColor;" // Diffuse impact
		""
		// Add Specular lighting, part 3/3 for Phong Point Lighting
		"	float specularStrength = 0.5;"
		"	vec3 viewDirection = normalize(viewPosition - FragPos);"
		"	vec3 reflectDirection = reflect(-lightDirection, norm);"
		"	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 24);"
		"	vec3 specularLight = specularStrength * spec * lightColor;"
		""
		// Result of Phong Point Lighting: 
		"	vec3 result = (ambientLight + diffusedLight + specularLight) * vec3(textureColor.r, textureColor.g, textureColor.b);"
		"   FragColor = vec4(result, textureColor.g);"
		"}";
}

/**
* Compile and link shader program
* @return shader program ID
*/
int compileAndLinkShaders(const char* vertexShaderSource, const char* fragmentShaderSource) {

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
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
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
void bind(int& currentShader, GLuint& VAO) {
	glUseProgram(currentShader);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VAO);
}
