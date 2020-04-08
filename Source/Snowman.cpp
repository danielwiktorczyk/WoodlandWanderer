#include "../Include/Snowman.h"

Snowman::Snowman(GLuint worldMatrixColorLoc,
	GLuint colorLoc,
	int colshader,
	int theSphereVertices,
	GLuint theCubeVAO,
	GLuint theSphereVAO) {

	this->origin = glm::vec3(0.0f, 0.0f, 0.0f);
	this->scaleFactor = 0.85f;
	this->rotation = 0.0f;
	this->worldMatrixLocationColor = worldMatrixColorLoc;
	this->colorLocation = colorLoc;
	this->colorShaderProgram = colshader;
	this->sphereVertices = theSphereVertices;
	this->cubeVAO = theCubeVAO;
	this->sphereVAO = theSphereVAO;
	this->globalSpeed = 5.0f;

	this->animate = 0.0f;
	this->animateHat = 0.0f;

	this->keyA = true;
	this->keyD = true;
	this->keyW = true;
	this->keyS = true;

	update();
}

void Snowman::update() {

	glm::mat4 partRotation = rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 groupTranslation = translate(glm::mat4(1.0f), origin);
	glm::mat4 groupScaling = scale(glm::mat4(1.0f), glm::vec3(scaleFactor));

	float chubbyFactor = 1.3f;
	leftFoot = groupTranslation * groupScaling * partRotation * translate(glm::mat4(1.0f), scaleFactor * (glm::vec3(-1.0f, 0.25f, 0.5f * sin(animate)))) * rotate(glm::mat4(1.0f), 0.25f * sin(animate), glm::vec3(1.0f, 125.0f, 0.0f)) * scale(glm::mat4(1.0f), chubbyFactor * scaleFactor * glm::vec3(1.0, 0.5, 1.0));
	rightFoot = groupTranslation * groupScaling * partRotation * translate(glm::mat4(1.0f), scaleFactor * (glm::vec3(+1.0f, 0.25f, 0.5f * -sin(animate)))) * rotate(glm::mat4(1.0f), 0.25f * -sin(animate), glm::vec3(1.0f, 125.0f, 0.0f)) * scale(glm::mat4(1.0f), chubbyFactor * scaleFactor * glm::vec3(1.0, 0.5, 1.0));

	base = groupTranslation * groupScaling * partRotation * translate(glm::mat4(1.0f), scaleFactor * (glm::vec3(0.0f, 3.0f, 0.0f))) * scale(glm::mat4(1.0f), chubbyFactor * scaleFactor * glm::vec3(4.0, 4.0, 4.0));
	middle = groupTranslation * groupScaling * partRotation * translate(glm::mat4(1.0f), scaleFactor * (glm::vec3(0.0f, 5.5f, 0.0f))) * scale(glm::mat4(1.0f), chubbyFactor * scaleFactor * glm::vec3(3.0, 3.0, 3.0));
	head = groupTranslation * groupScaling * partRotation * translate(glm::mat4(1.0f), scaleFactor * (glm::vec3(0.0f, 8.0f, 0.0f))) * scale(glm::mat4(1.0f), chubbyFactor * scaleFactor * glm::vec3(2.0, 2.0, 2.0));

	button1 = groupTranslation * groupScaling * partRotation * translate(glm::mat4(1.0f), scaleFactor * (glm::vec3(0.0f, 6.0f, 0.92f * chubbyFactor * 1.55f))) * scale(glm::mat4(1.0f), scaleFactor * glm::vec3(0.3, 0.3, 0.1));
	button2 = groupTranslation * groupScaling * partRotation * translate(glm::mat4(1.0f), scaleFactor * (glm::vec3(0.0f, 3.0f, chubbyFactor * 2.05f))) * scale(glm::mat4(1.0f), scaleFactor * glm::vec3(0.3, 0.3, 0.1));
	button3 = groupTranslation * groupScaling * partRotation * translate(glm::mat4(1.0f), scaleFactor * (glm::vec3(0.0f, 4.5f, 0.82f * chubbyFactor * 2.05f))) * scale(glm::mat4(1.0f), scaleFactor * glm::vec3(0.3, 0.3, 0.1));

	glm::mat4 leftArmPivot = translate(glm::mat4(1.0f), scaleFactor * (glm::vec3(-1.5f, 5.5f, 0.0f))) * rotate(glm::mat4(1.0f), 0.75f * sin(animate), glm::vec3(0.0f, 0.0f, 1.0f)) * translate(glm::mat4(1.0f), scaleFactor * (glm::vec3(1.5f, -5.5f, 0.0f)));
	leftArm = groupTranslation * groupScaling * partRotation * leftArmPivot * translate(glm::mat4(1.0f), scaleFactor * (glm::vec3(-2.5f, 5.5f, 0.0f))) * scale(glm::mat4(1.0f), scaleFactor * glm::vec3(2.5, 0.3, 0.3));
	leftArmBranch1 = groupTranslation * groupScaling * partRotation * leftArmPivot * translate(glm::mat4(1.0f), scaleFactor * (glm::vec3(-3.25f, 5.20f, 0.0f))) * rotate(glm::mat4(1.0f), glm::radians(-130.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * scale(glm::mat4(1.0f), scaleFactor * glm::vec3(1.0, 0.25, 0.25));
	leftArmBranch2 = groupTranslation * groupScaling * partRotation * leftArmPivot * translate(glm::mat4(1.0f), scaleFactor * (glm::vec3(-2.0f, 5.65f, 0.0f))) * rotate(glm::mat4(1.0f), glm::radians(150.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * scale(glm::mat4(1.0f), scaleFactor * glm::vec3(0.5, 0.25, 0.25));

	glm::mat4 rightArmPivot = translate(glm::mat4(1.0f), scaleFactor * (glm::vec3(+1.5f, +4.5f, 0.0f))) * rotate(glm::mat4(1.0f), 0.75f * sin(animate), glm::vec3(0.0f, 0.0f, 1.0f)) * translate(glm::mat4(1.0f), scaleFactor * (glm::vec3(-1.5f, -4.5f, 0.0f)));
	rightArm = groupTranslation * groupScaling * partRotation * rightArmPivot * translate(glm::mat4(1.0f), scaleFactor * (glm::vec3(+2.5f, 5.5f, 0.0f))) * scale(glm::mat4(1.0f), scaleFactor * glm::vec3(2.5, 0.3, 0.5));
	rightArmBranch1 = groupTranslation * groupScaling * partRotation * rightArmPivot * translate(glm::mat4(1.0f), scaleFactor * (glm::vec3(+2.0f, 5.65f, 0.0f))) * rotate(glm::mat4(1.0f), glm::radians(-50.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * scale(glm::mat4(1.0f), scaleFactor * glm::vec3(0.5, 0.25, 0.25));
	rightArmBranch2 = groupTranslation * groupScaling * partRotation * rightArmPivot * translate(glm::mat4(1.0f), scaleFactor * (glm::vec3(+3.25f, 5.20f, 0.0f))) * rotate(glm::mat4(1.0f), glm::radians(140.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * scale(glm::mat4(1.0f), scaleFactor * glm::vec3(1.0, 0.25, 0.25));

	hatBrim = groupTranslation * groupScaling * partRotation * translate(glm::mat4(1.0f), scaleFactor * (glm::vec3(0.0f, 9.5f - 0.5f * cos(2.0f * animate), 0.0f))) * rotate(glm::mat4(1.0f), sin(animateHat), glm::vec3(0.0f, 1.0f, 0.0f)) * scale(glm::mat4(1.0f), scaleFactor * glm::vec3(4.0, 1.0, 4.0));
	hatBody = groupTranslation * groupScaling * partRotation * translate(glm::mat4(1.0f), scaleFactor * (glm::vec3(0.0f, 12.0f - 0.5f * cos(2.0f * animate), 0.0f))) * rotate(glm::mat4(1.0f), sin(animateHat), glm::vec3(0.0f, 1.0f, 0.0f)) * scale(glm::mat4(1.0f), scaleFactor * glm::vec3(2.0, 4.0, 2.0));

	leftEye = groupTranslation * groupScaling * partRotation * translate(glm::mat4(1.0f), scaleFactor * (glm::vec3(-0.5f, 8.0f, chubbyFactor * 0.95f))) * scale(glm::mat4(1.0f), scaleFactor * glm::vec3(0.25, 0.25, 0.1));
	rightEye = groupTranslation * groupScaling * partRotation * translate(glm::mat4(1.0f), scaleFactor * (glm::vec3(+0.5f, 8.0f, chubbyFactor * 0.95f))) * scale(glm::mat4(1.0f), scaleFactor * glm::vec3(0.25, 0.25, 0.1));
	carrot = groupTranslation * groupScaling * partRotation * translate(glm::mat4(1.0f), scaleFactor * (glm::vec3(0.0f, 7.75f, chubbyFactor * 1.05f))) * scale(glm::mat4(1.0f), scaleFactor * glm::vec3(0.25, 0.25, 1.64));
	mouth = groupTranslation * groupScaling * partRotation * translate(glm::mat4(1.0f), scaleFactor * (glm::vec3(0.0f, 7.25f, chubbyFactor * 0.84f))) * scale(glm::mat4(1.0f), scaleFactor * glm::vec3(0.25, 0.125, 0.1));
}

void Snowman::draw() {

	drawHelper(leftArm, snowmanBranchColor);
	drawHelper(leftArmBranch1, snowmanBranchColor);
	drawHelper(leftArmBranch2, snowmanBranchColor);
	drawHelper(rightArm, snowmanBranchColor);
	drawHelper(rightArmBranch1, snowmanBranchColor);
	drawHelper(rightArmBranch2, snowmanBranchColor);

	drawHelper(button1, buttonColor);
	drawHelper(button2, buttonColor);
	drawHelper(button3, buttonColor);

	drawHelper(leftEye, hatColor);
	drawHelper(rightEye, hatColor);
	drawHelper(mouth, hatColor);

	drawBody(leftFoot, snowmanColor);
	drawBody(rightFoot, snowmanColor);
	drawBody(base, snowmanColor);
	drawBody(middle, snowmanColor);
	drawBody(head, snowmanColor);

	drawHelper(hatBrim, hatColor);
	drawHelper(hatBody, hatColor);

	drawHelper(carrot, carrotColor);

}

void Snowman::drawHelper(glm::mat4 part, glm::vec3 color) {
	glUseProgram(colorShaderProgram);

	glBindVertexArray(0);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVAO);

	glUniformMatrix4fv(worldMatrixLocationColor, 1, GL_FALSE, &part[0][0]);
	glUniform3fv(colorLocation, 1, value_ptr(color));
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Snowman::drawBody(glm::mat4 part, glm::vec3 color) {
	glUseProgram(colorShaderProgram);

	glBindVertexArray(0);
	glBindVertexArray(sphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, sphereVAO);

	glUniformMatrix4fv(worldMatrixLocationColor, 1, GL_FALSE, &part[0][0]);
	glUniform3fv(colorLocation, 1, value_ptr(color));
	glDrawArrays(GL_TRIANGLES, 0, sphereVertices);
}

/**
* Change the snowman's size using the U and J keys
*/
void Snowman::scaleSnowman(GLFWwindow* window, const bool& shift, bool& canScaleIncrement) {
	float snowmanScalingSpeed = globalSpeed * 0.005f;
	bool scaleUp = glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS;
	bool scaleDown = glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS;

	if (scaleUp && (!shift || canScaleIncrement)) { // Scale snowman up
		this->scaleFactor += snowmanScalingSpeed;
	}
	if (scaleDown && (!shift || canScaleIncrement)) { // Scale snowman down
		this->scaleFactor -= snowmanScalingSpeed;
	}
	if (this->scaleFactor < 0) {
		this->scaleFactor = 0;
	}
	canScaleIncrement = !(scaleUp || scaleDown);
}

/**
* Rotate the snowman using the Q and E keys
*/
void Snowman::rotateSnowman(GLFWwindow* window, const bool& shift, bool& canRotateIncrement) {
	float snowmanRotationSpeed = globalSpeed * 0.0064f; // degrees
	bool rotateLeft = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;
	bool rotateRight = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;

	// Rotate snowman left 5 degrees
	if (rotateLeft) {
		if (!shift) {
			this->rotation += snowmanRotationSpeed;
			if (this->rotation >= 2 * M_PI) {
				this->rotation -= 2 * M_PI;
			}
			this->animateHat -= globalSpeed * 0.01f;
		}
		else if (canRotateIncrement) {
			this->rotation += 10 * snowmanRotationSpeed;
			if (this->rotation >= 2 * M_PI) {
				this->rotation -= 2 * M_PI;
			}
			this->animateHat -= globalSpeed * 0.01f;
		}
	}

	// Rotate snowman right 5 degrees
	if (rotateRight) {
		if (!shift) {
			this->rotation -= snowmanRotationSpeed;
			if (this->rotation <= 2 * M_PI) {
				this->rotation += 2 * M_PI;
			}
			this->animateHat += globalSpeed * 0.01f;
		}
		else if (canRotateIncrement) {
			this->rotation -= 10 * snowmanRotationSpeed;
			if (this->rotation <= 2 * M_PI) {
				this->rotation += 2 * M_PI;
			}
			this->animateHat += globalSpeed * 0.01f;
		}
	}
	canRotateIncrement = !(rotateLeft || rotateRight);
}

/**
* Translate the snowman using the WASD keys
*/
void Snowman::translateSnowman(GLFWwindow* window, const bool& shift, bool& canMoveIncrement) {

	// Snowman Translation by WASD keys
	float snowmanTranslationSpeed = globalSpeed * 0.03f;
	bool moveLeft = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
	bool moveRight = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
	bool moveUp = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
	bool moveDown = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;

	// Move snowman left (+ x direction)
	if (moveLeft && keyA) {
		if (!shift) {
			this->origin.x += cos(this->rotation) * snowmanTranslationSpeed;
			this->origin.z -= sin(this->rotation) * snowmanTranslationSpeed;
			if (!moveUp && !moveDown) {
				this->animate += globalSpeed * 0.02f;
			}
		}
		else if (canMoveIncrement) {
			this->origin.x += cos(this->rotation) * 10.0f * snowmanTranslationSpeed;
			this->origin.z -= sin(this->rotation) * 10.0f * snowmanTranslationSpeed;
			if (!moveUp && !moveDown) {
				this->animate += globalSpeed * 10.0f * 0.02f;
			}
		}
	}

	// Move snowman right (- x direction)
	if (moveRight && keyD) {
		if (!shift) {
			this->origin.x -= cos(this->rotation) * snowmanTranslationSpeed;
			this->origin.z += sin(this->rotation) * snowmanTranslationSpeed;
			if (!moveUp && !moveDown)
				this->animate += globalSpeed * 0.02f;
		}
		else if (canMoveIncrement) {
			this->origin.x -= cos(this->rotation) * 10.0f * snowmanTranslationSpeed;
			this->origin.z += sin(this->rotation) * 10.0f * snowmanTranslationSpeed;
			if (!moveUp && !moveDown)
				this->animate += globalSpeed * 10.0f * 0.02f;
		}
	}

	// Move snowman up (+ z direction)
	if (moveUp && keyW) {
		if (!shift) {
			this->origin.x += sin(this->rotation) * snowmanTranslationSpeed;
			this->origin.z += cos(this->rotation) * snowmanTranslationSpeed;
			this->animate += globalSpeed * 0.02f;
		}
		else if (canMoveIncrement) {
			this->origin.x += sin(this->rotation) * 10.0f * snowmanTranslationSpeed;
			this->origin.z += cos(this->rotation) * 10.0f * snowmanTranslationSpeed;
			this->animate += globalSpeed * 10.0f * 0.02f;
		}
	}

	// Move snowman down (- z direction)
	if (moveDown && keyS){
		if (!shift) {
			this->origin.x -= sin(this->rotation) * snowmanTranslationSpeed;
			this->origin.z -= cos(this->rotation) * snowmanTranslationSpeed;
			this->animate -= globalSpeed * 0.02f;
		}
		else if (canMoveIncrement) {
			this->origin.x -= sin(this->rotation) * 10.0f * snowmanTranslationSpeed;
			this->origin.z -= cos(this->rotation) * 10.0f * snowmanTranslationSpeed;
			this->animate -= globalSpeed * 10.0f * 0.02f;
		}
	}
	canMoveIncrement = !(moveLeft || moveRight || moveUp || moveDown);
}

/**
* Move the snowman to a random location using the space bar
*/
void Snowman::randomTranslationSnowman(GLFWwindow* window, const bool& shift, bool& canRandomPlacement) {
	int randomBounds = 24; // Let's use 24 units for now, easier to spot the snowman
	bool randomPlacement = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
	if (randomPlacement) // Random position 
	{
		if (!shift || canRandomPlacement) {
			float randx = (rand() % randomBounds) - randomBounds / 2.0f;
			float randy = (rand() % randomBounds) - randomBounds / 2.0f;
			this->origin = glm::vec3(randx, 0.0f, randy);
		}
	}
	canRandomPlacement = !randomPlacement;
}

/**
* Check collision of the snowman to another collider object using its vec3
*/
bool Snowman::CheckCollision(std::vector<CollidableModel> colliders) {
 	bool collisionX = false;
	bool collisionZ = false;

	for (auto& collider : colliders) {
		glm::vec3 positionCollider = collider.getColliderPosition();
		glm::vec3 scaleCollider = collider.getColliderScale();

		collisionX = origin.x + (positionCollider.x * scaleCollider.x) >= positionCollider.x &&
			positionCollider.x + (positionCollider.x * scaleCollider.x) >= origin.x;

		collisionZ = origin.z + (positionCollider.z * scaleCollider.z) >= positionCollider.z &&
			positionCollider.z + (positionCollider.z * scaleCollider.z) >= origin.z;

		if (collisionX && collisionZ) {
			break;
		}
	}

	return collisionX && collisionZ;
}

void Snowman::CheckCollisionX(std::vector<CollidableModel> colliders, bool isColliding) {

	for (auto& collider : colliders) {
		glm::vec3 positionCollider = collider.getColliderPosition();
		glm::vec3 scaleCollider = collider.getColliderScale();
		// keyA
		if ((origin.x + (positionCollider.x * scaleCollider.x) - positionCollider.x) > 0 &&
			(static_cast<double>(origin.x) + static_cast<double>(positionCollider.x) * scaleCollider.x - positionCollider.x) < 0.1 && isColliding) {
			keyA = false;
			keyD = true;
		}
		// keyD
		if ((positionCollider.x + (positionCollider.x * scaleCollider.x) - origin.x) > 0 &&
			(static_cast<double>(positionCollider.x) + static_cast<double>(positionCollider.x) * scaleCollider.x - origin.x) < 0.1 && isColliding) {
			keyA = false;
			keyD = true;
		}
		else {
			keyA = true;
			keyD = true;
		}
	}
}

void Snowman::CheckCollisionZ(std::vector<CollidableModel> colliders, bool isColliding) {

	for (auto& collider : colliders) {
		glm::vec3 positionCollider = collider.getColliderPosition();
		glm::vec3 scaleCollider = collider.getColliderScale();
		// keyA
		if ((origin.z + (positionCollider.z * scaleCollider.z) - positionCollider.x) > 0 &&
			(static_cast<double>(origin.z) + static_cast<double>(positionCollider.z) * scaleCollider.z - positionCollider.z) < 0.1 && isColliding) {
			keyW = false;
			keyS = true;
		}
		// keyD
		if ((positionCollider.z + (positionCollider.z * scaleCollider.z) - origin.z) > 0 &&
			(static_cast<double>(positionCollider.z) + static_cast<double>(positionCollider.z) * scaleCollider.z - origin.z) < 0.1 && isColliding) {
			keyW = true;
			keyS = false;
		}
		else {
			keyW = true;
			keyS = true;
		}
	}
}

