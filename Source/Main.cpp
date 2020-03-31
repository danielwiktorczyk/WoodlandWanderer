/*
*
* COMP 371 Project
*
*
* Adapted from template code:
*   COMP 371 Labs Framework
* 
* Used lab tutorials from class and learnopengl.com tutorials 
*/

#include "../Include/Main.h"

int main(int argc, char*argv[]) {
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
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Comp371 - Final Project", NULL, NULL);
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

	//Setup models
#if defined(PLATFORM_OSX)
	string cubePath = "Models/cube.obj";
	string heraclesPath = "Models/heracles.obj";
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

	// Camera parameters for view transform
	glm::vec3 cameraPosition(3.0f, 5.0f, 25.0f);
	//glm::vec3 cameraPosition(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraLookAt(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

	// Other camera parameters
	float cameraSpeed = 0.05f;
	float cameraHorizontalAngle = 90.0f;
	float cameraVerticalAngle = 0.0f;
	glm::vec3 cameraSideVector = glm::vec3(1.0f);

	// Set projection matrix for shader
	float currentFOV = 70.0f;
	glm::mat4 projectionMatrix = glm::perspective(currentFOV,            // field of view in degrees
		1024.0f / 768.0f,  // aspect ratio
		0.01f, 100.0f);   // near and far (near > 0)

	// Set initial view matrix
	glm::mat4 viewMatrix = lookAt(cameraPosition,  // eye
		cameraPosition + cameraLookAt,  // center
		cameraUp); // up
	
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

	// Initialize the Snowman
	Snowman snowman = Snowman(worldMatrixLocationColor, worldMatrixLocationTexture, colorLocation, colorShaderProgram, texturedShaderProgram, sphereVertices, cubeVAO, sphereVAO,
		carrotTextureID, metalTextureID);

	// World rotation
	glm::mat4 worldRotationMatrix = glm::mat4(1.0f);
	float worldRotationAboutXAxis = 0.0f;
	float worldRotationAboutYAxis = 0.0f;

	// Texturing toggle
	bool texturing = false;
	bool canToggleTexturing = true;

	// Controlling increment helper variables
	bool canScaleIncrement = true;
	bool canRotateIncrement = true;
	bool canMoveIncrement = true;
	bool canRandomPlacement = true;

	// Set lighting point source location
	glm::vec3 lightPosition = glm::vec3(0.0f, 30.0f, 0.0f);
	glUniform3fv(lightLocationColor, 1, value_ptr(lightPosition));
	glUniform3fv(lightLocationTexture, 1, value_ptr(lightPosition));

	// Entering Main Loop
	while (!glfwWindowShouldClose(window))
	{
		// Each frame, reset color of each pixel to glClearColor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Commands::areTexturesToggled(window, canToggleTexturing, texturing);

		// Draw geometry
		glBindVertexArray(cubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVAO);

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

			glm::mat4 gridLineMatrix = scale(glm::mat4(1.0f), glm::vec3(100.0f, 0.02f, 0.05f));
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

		// Draw x y and z Axis
		glUseProgram(colorShaderProgram);

		float const axisWidth = 0.1f;
		float const axisLength = 5.0f;

		glm::mat4 xAxisMatrix = translate(glm::mat4(1.0f), glm::vec3(axisLength / 2, 0.0f, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(axisLength, axisWidth, axisWidth));
		glUniformMatrix4fv(worldMatrixLocationColor, 1, GL_FALSE, &xAxisMatrix[0][0]);
		glUniform3fv(colorLocation, 1, value_ptr(red));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 yAxisMatrix = translate(glm::mat4(1.0f), glm::vec3(0.0f, axisLength / 2, 0.0f)) * scale(glm::mat4(1.0f), glm::vec3(axisWidth, axisLength, axisWidth));
		glUniformMatrix4fv(worldMatrixLocationColor, 1, GL_FALSE, &yAxisMatrix[0][0]);
		glUniform3fv(colorLocation, 1, value_ptr(blue));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 zAxisMatrix = translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, axisLength / 2)) * scale(glm::mat4(1.0f), glm::vec3(axisWidth, axisWidth, axisLength));
		glUniformMatrix4fv(worldMatrixLocationColor, 1, GL_FALSE, &zAxisMatrix[0][0]);
		glUniform3fv(colorLocation, 1, value_ptr(green));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Draw an object representing the light source
		glm::mat4 lightBulbMatrix = translate(glm::mat4(1.0f), lightPosition) * scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
		glm::vec3 lightBulbColor = glm::vec3(1.0f, 1.0f, 1.0f);

		glUseProgram(colorShaderProgram);

		glBindVertexArray(0);
		glBindVertexArray(sphereVAO);
		glBindBuffer(GL_ARRAY_BUFFER, sphereVAO);

		glUniformMatrix4fv(worldMatrixLocationColor, 1, GL_FALSE, &lightBulbMatrix[0][0]);
		glUniform3fv(colorLocation, 1, value_ptr(lightBulbColor));
		glDrawArrays(GL_TRIANGLES, 0, sphereVertices);

		// Tree!
		glUseProgram(colorShaderProgram);
		glBindVertexArray(0);
		glBindVertexArray(treeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, treeVAO);
		glm::mat4 base = scale(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(worldMatrixLocationColor, 1, GL_FALSE, &base[0][0]);
		glUniform3fv(colorLocation, 1, value_ptr(turquoise));
		glDrawArrays(GL_TRIANGLES, 0, treeVertices);

		// Draw Snowman. This object will handle drawing itself!
		snowman.draw(texturing);

		// Handle inputs
		// For movement, if shift is held, it means a small movement for rotation, scaling, and transposing
		bool shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;

		Commands::closeWindow(window);
		Commands::setRenderingMode(window);

		// Snowman Scaling
		float snowmanScalingSpeed = 0.005f;
		bool scaleUp = glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS;
		bool scaleDown = glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS;
		if (scaleUp && (!shift || canScaleIncrement)) // Scale snowman up
			snowman.scaleFactor += snowmanScalingSpeed;
		if (scaleDown && (!shift || canScaleIncrement)) // Scale snowman down
			snowman.scaleFactor -= snowmanScalingSpeed;
		if (snowman.scaleFactor < 0)
			snowman.scaleFactor = 0;
		canScaleIncrement = !(scaleUp || scaleDown);


		// Snowman Rotation by Q and E keys
		float snowmanRotationSpeed = 0.0064f; // degrees
		bool rotateLeft = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;
		bool rotateRight = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
		if (rotateLeft) // Rotate snowman left 5 degrees
		{
			if (!shift) {
				snowman.rotation += snowmanRotationSpeed;
				if (snowman.rotation >= 2 * M_PI)
					snowman.rotation -= 2 * M_PI;
				snowman.animateHat -= 0.01f;
			}
			else if (canRotateIncrement) {
				snowman.rotation += 10 * snowmanRotationSpeed;
				if (snowman.rotation >= 2 * M_PI)
					snowman.rotation -= 2 * M_PI;
				snowman.animateHat -= 0.01f;
			}
		}
		if (rotateRight && (!shift || canRotateIncrement)) // Rotate snowman right 5 degrees
		{
			if (!shift) {
				snowman.rotation -= snowmanRotationSpeed;
				if (snowman.rotation <= 2 * M_PI)
					snowman.rotation += 2 * M_PI;
				snowman.animateHat += 0.01f;
			}
			else if (canRotateIncrement) {
				snowman.rotation -= 10 * snowmanRotationSpeed;
				if (snowman.rotation <= 2 * M_PI)
					snowman.rotation += 2 * M_PI;
				snowman.animateHat += 0.01f;
			}
		}
		canRotateIncrement = !(rotateLeft || rotateRight);

		
		// Snowman Translation by WASD keys
		float snowmanTranslationSpeed = 0.03f;
		bool moveLeft = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
		bool moveRight = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
		bool moveUp = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
		bool moveDown = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
		if (moveLeft) // Move snowman left (+ x direction)
		{
			if (!shift) {
				snowman.origin.x += cos(snowman.rotation) * snowmanTranslationSpeed;
				snowman.origin.z -= sin(snowman.rotation) * snowmanTranslationSpeed;
				if (!moveUp && !moveDown)
					snowman.animate += 0.02f;
			}
			else if (canMoveIncrement) {
				snowman.origin.x += cos(snowman.rotation) * 10.0f * snowmanTranslationSpeed;
				snowman.origin.z -= sin(snowman.rotation) * 10.0f * snowmanTranslationSpeed;
				if (!moveUp && !moveDown)
					snowman.animate += 10.0f * 0.02f;
			}
		}
		if (moveRight) // Move snowman right (- x direction)
		{
			if (!shift) {
				snowman.origin.x -= cos(snowman.rotation) * snowmanTranslationSpeed;
				snowman.origin.z += sin(snowman.rotation) * snowmanTranslationSpeed;
				if (!moveUp && !moveDown)
					snowman.animate += 0.02f;
			}
			else if (canMoveIncrement) {
				snowman.origin.x -= cos(snowman.rotation) * 10.0f * snowmanTranslationSpeed;
				snowman.origin.z += sin(snowman.rotation) * 10.0f * snowmanTranslationSpeed;
				if (!moveUp && !moveDown)
					snowman.animate += 10.0f * 0.02f;
			}
		}
		if (moveUp) // Move snowman up (+ z direction)
		{
			if (!shift) {
				snowman.origin.x += sin(snowman.rotation) * snowmanTranslationSpeed;
				snowman.origin.z += cos(snowman.rotation) * snowmanTranslationSpeed;
				snowman.animate += 0.02f;
			}
			else if (canMoveIncrement) {
				snowman.origin.x += sin(snowman.rotation) * 10.0f * snowmanTranslationSpeed;
				snowman.origin.z += cos(snowman.rotation) * 10.0f * snowmanTranslationSpeed;
				snowman.animate += 10.0f * 0.02f;
			}
		}
		if (moveDown) // Move snowman down (- z direction)
		{
			if (!shift) {
				snowman.origin.x -= sin(snowman.rotation) * snowmanTranslationSpeed;
				snowman.origin.z -= cos(snowman.rotation) * snowmanTranslationSpeed;
				snowman.animate -= 0.02f;
			}
			else if (canMoveIncrement) {
				snowman.origin.x -= sin(snowman.rotation) * 10.0f * snowmanTranslationSpeed;
				snowman.origin.z -= cos(snowman.rotation) * 10.0f * snowmanTranslationSpeed;
				snowman.animate -= 10.0f * 0.02f;
			}
		}
		canMoveIncrement = !(moveLeft || moveRight || moveUp || moveDown);

		// Snowman Random location
		int randomBounds = 24; // Let's use 24 units for now, easier to spot the snowman
		bool randomPlacement = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
		if (randomPlacement) // Random position 
		{
			if (!shift || canRandomPlacement) {
				float randx = (rand() % randomBounds) - randomBounds / 2.0f;
				float randy = (rand() % randomBounds) - randomBounds / 2.0f;
				snowman.origin = glm::vec3(randx, 0.0f, randy);
			}
		}
		canRandomPlacement = !randomPlacement;

		// Snowman Update
		snowman.update();

		/*
			Camera controls
		*/
		double mousePosX, mousePosY;
		glfwGetCursorPos(window, &mousePosX, &mousePosY);

		double dx = mousePosX - lastMousePosX;
		double dy = mousePosY - lastMousePosY;

		lastMousePosX = mousePosX;
		lastMousePosY = mousePosY;

		constexpr float cameraAngularSpeed = glm::radians(5.0f);
		float theta = glm::radians(cameraHorizontalAngle);
		float phi = glm::radians(cameraVerticalAngle);

		// Clamp vertical angle to [-85, 85] degrees
		cameraVerticalAngle = glm::max(-85.0f, glm::min(85.0f, cameraVerticalAngle));

		// Allow camera to rotate about horizontally
		if (cameraHorizontalAngle > 360)
			cameraHorizontalAngle -= 360;
		else if (cameraHorizontalAngle < -360)
			cameraHorizontalAngle += 360;
		
		cameraLookAt = glm::vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));
		cameraSideVector = cross(cameraLookAt, glm::vec3(0.0f, 1.0f, 0.0f));
		normalize(cameraSideVector);

		Commands::panCamera(window, cameraHorizontalAngle, dx, cameraAngularSpeed);
		Commands::tiltCamera(window, cameraVerticalAngle, dy, cameraAngularSpeed);

		// Camera Zooming
		float cameraZoomSpeed = 0.001f;
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) // 
		{
			currentFOV += cameraZoomSpeed * dy;

			projectionMatrix = glm::perspective(currentFOV,            // field of view in degrees
				1024.0f / 768.0f,  // aspect ratio
				0.01f, 100.0f);   // near and far (near > 0)
			
			setProjectionMatrix(colorShaderProgram, projectionMatrix);
			setProjectionMatrix(texturedShaderProgram, projectionMatrix);
		}

		glm::mat4 viewMatrix(1.0f);		
		viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
		setViewMatrix(colorShaderProgram, viewMatrix);
		setViewMatrix(texturedShaderProgram, viewMatrix);
		glUniform3fv(viewLocationColor, 1, value_ptr(cameraPosition));
		glUniform3fv(viewLocationTexture, 1, value_ptr(cameraPosition));

		/*
		World Orientation
		*/
		float worldRotationSpeed = 0.005f;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			worldRotationAboutYAxis += worldRotationSpeed;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			worldRotationAboutYAxis -= worldRotationSpeed;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			worldRotationAboutXAxis += worldRotationSpeed;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			worldRotationAboutXAxis -= worldRotationSpeed;
		if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) {
			worldRotationAboutXAxis = 0.0f;
			worldRotationAboutYAxis = 0.0f;
		}
		worldRotationMatrix = rotate(glm::mat4(1.0f), worldRotationAboutYAxis, glm::vec3(0.0f, 1.0f, 0.0f)) * rotate(glm::mat4(1.0f), worldRotationAboutXAxis, glm::vec3(1.0f, 0.0f, 0.0f));
		setWorldRotationMatrix(colorShaderProgram, worldRotationMatrix);
		setWorldRotationMatrix(texturedShaderProgram, worldRotationMatrix);
	
		// End Frameset
		glfwSwapBuffers(window); // for double buffering
		glfwPollEvents();
	}

	// Finally, shut down
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
		"	vec3 result = (ambientLight + diffusedLight + specularLight) * glm::vec3(textureColor.r, textureColor.g, textureColor.b);"
		"   FragColor = vec4(result, textureColor.g);"
		"}";
}

/**
* Compile and link shader program
@return shader program ID
*/
int compileAndLinkShaders(const char* vertexShaderSource, const char* fragmentShaderSource) {
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
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
	if (!data)
	{
		std::cerr << "Error::Texture could not load texture file:" << filename << std::endl;
		return 0;
	}

	// Step4 Upload the texture to the PU
	GLenum format = 0;
	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
		0, format, GL_UNSIGNED_BYTE, data);

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
