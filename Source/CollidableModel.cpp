#include "..\Include\CollidableModel.h"

/**
* CollidableModel Constructor
*/
CollidableModel::CollidableModel(GLuint VAO,GLuint numVertices, glm::vec3 color) 
	: Model(VAO, numVertices, color), colliderTransformMatrix(colliderTransformMatrix) { }

/**
* CollidableModel destructor
*/
CollidableModel::~CollidableModel() {}

/**
* Overrides the Model's setter for the ModelTransformationMatrix to polymorphically set the collider's box.
* This is to ensure that the box's transformations match the model's transformation.
*/
void CollidableModel::setModelTransformMatrix(glm::mat4 modelTransformMatrix) { 
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(modelTransformMatrix, scale, rotation, translation, skew, perspective);

	glm::mat4 model(1.0f);
	model = glm::translate(model, translation);
	model = glm::scale(model, scale);

	Model::setModelTransformMatrix(modelTransformMatrix);
	this->colliderTransformMatrix = model; 
};

/**
* Get the collider's position from the transformation matrix
*/
glm::vec3 CollidableModel::getColliderPosition() {
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(this->colliderTransformMatrix, scale, rotation, translation, skew, perspective);

	return translation;
}

/**
* Get the collider's scale from the transformation matrix
*/
glm::vec3 CollidableModel::getColliderScale() {
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(this->colliderTransformMatrix, scale, rotation, translation, skew, perspective);

	return scale;
}

void CollidableModel::draw(const GLuint& worldMatrixLocation, const GLuint& colorLocation) {
	// This is for drawing collidable object
	Model::draw(worldMatrixLocation, colorLocation);

	// This is for drawing the collider box for Debugging
	if (DebugMode && DrawCollisionBoxes) {
		glBindVertexArray(this->getColliderVAO());
		glBindBuffer(GL_ARRAY_BUFFER, this->getColliderVAO());

		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &(this->getColliderTransformMatrix())[0][0]);
		glUniform3fv(colorLocation, 1, value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

		glDrawArrays(GL_TRIANGLES, 0, 36);
		glEnable(GL_CULL_FACE);
	}

}