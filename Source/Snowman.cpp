#include "../Include/Snowman.h"

Snowman::Snowman(GLuint worldMatrixColorLoc,
	GLuint worldMatrixTextureLoc,
	GLuint colorLoc,
	int colshader,
	int texshader,
	int theSphereVertices,
	GLuint theCubeVAO,
	GLuint theSphereVAO,
	int theCarrotTexture,
	int theMetalTexture) {

	this->origin = glm::vec3(0.0f, 0.0f, 0.0f);
	this->scaleFactor = 1.0f;
	this->rotation = 0.0f;
	this->worldMatrixLocationColor = worldMatrixColorLoc;
	this->worldMatrixLocationTexture = worldMatrixTextureLoc;
	this->colorLocation = colorLoc;
	this->colorShaderProgram = colshader;
	this->textureShaderProgram = texshader;
	this->sphereVertices = theSphereVertices;
	this->cubeVAO = theCubeVAO;
	this->sphereVAO = theSphereVAO;

	this->carrotTexture = theCarrotTexture;
	this->metalTexture = theMetalTexture;

	this->animate = 0.0f;
	this->animateHat = 0.0f;

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

void Snowman::draw(bool texturing) {

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

	if (texturing) {
		drawTexture(hatBrim, metalTexture);
		drawTexture(hatBody, metalTexture);

		drawTexture(carrot, carrotTexture);
	}
	else {
		drawHelper(hatBrim, hatColor);
		drawHelper(hatBody, hatColor);

		drawHelper(carrot, carrotColor);
	}

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

void Snowman::drawTexture(glm::mat4 part, int texture) {
	glUseProgram(textureShaderProgram);

	glActiveTexture(GL_TEXTURE0);
	GLuint textureLocation = glGetUniformLocation(textureShaderProgram, "textureSampler");
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(textureLocation, 0);

	glBindVertexArray(0);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVAO);

	glUniformMatrix4fv(worldMatrixLocationTexture, 1, GL_FALSE, &part[0][0]);
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