#include "..\Include\Firefly.h"

Firefly::Firefly(GLuint VAO, GLuint numVertices, glm::vec3 color)
	: NonCollidableModel(VAO, numVertices, color) { 


	glm::mat4 transformMatrix = glm::mat4(1.0f);
	float fireflyOffset = 6.0f - 5 * sin((rand() % 314) / 3.14f);
	transformMatrix = glm::translate(transformMatrix, glm::vec3(fireflyOffset, fireflyOffset, fireflyOffset));
	transformMatrix = glm::scale(transformMatrix, glm::vec3(0.14f, 0.14f, 0.14f));

	this->setModelTransformMatrix(transformMatrix);
	this->animation = (rand() % 6) * 1.0;

}

void Firefly::draw(const GLuint& worldMatrixLocation, const GLuint& colorLocation) {
	
	this->animation += 0.005;
	if (this->animation >= 6.3)
		this->animation = 0.0;

	glm::mat4 savedLocation = this->getModelTransformMatrix();

	glm::mat4 flightPath = glm::translate(savedLocation, glm::vec3(5 * cos(2 * this->animation), 3 * sin(this->animation), 4 * cos(3 * this->animation)));
	float pulsation = 1.0f - 0.5 * sin(5.0 * animation);
	flightPath = glm::scale(flightPath, glm::vec3(pulsation, pulsation, pulsation));
	this->setModelTransformMatrix(flightPath);

	Model::draw(worldMatrixLocation, colorLocation);

	this->setModelTransformMatrix(savedLocation);
}
