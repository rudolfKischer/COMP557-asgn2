//Rudolf C. Kischer
//260956107
#include "Camera.h"

using namespace std;
#include <iostream>


Camera::Camera(float aspect):lightColor(glm::vec3(1,1,1))
{
	this->setPerspective((float)(45.0 * M_PI / 180.0), aspect, 0.01f, 100.0f);
	this->updateView();
}

Camera::~Camera()
{
}

void Camera::setPerspective(float fov, float aspect, float near, float far) {
	// TODO: Replace the default projection matrix with one constructed from the parameters available in this class!
//	P = glm::mat4(1.0);
    P = glm::perspective(
             fov, // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
             aspect,       // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
             near,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
             far             // Far clipping plane. Keep as little as possible.
         );
}

void Camera::updateView() {
	// TODO: Replace the default viewing matrix with one constructed from the parameters available in this class!
    V = glm::lookAt(
                      this->position,
                      this->lookAt,
                      this->up
                      );
}


void Camera::draw(const shared_ptr<Program> program, glm::mat4 P, glm::mat4 V, shared_ptr <MatrixStack> M, glm::mat4 LightPV, Axis &axis) {
	program->bind();
	M->pushMatrix();
	
	//set uniform variables
	glUniformMatrix4fv(program->getUniform("P"), 1, GL_FALSE, &P[0][0]);
	glUniformMatrix4fv(program->getUniform("V"), 1, GL_FALSE, &V[0][0]);
	glUniformMatrix4fv(program->getUniform("M"), 1, GL_FALSE, &M->topMatrix()[0][0]);
	glUniformMatrix4fv(program->getUniform("MinvT"), 1, GL_FALSE, &M->topInvMatrix()[0][0]);
	glUniformMatrix4fv(program->getUniform("lightPV"), 1, GL_FALSE, &LightPV[0][0]);
	
	// TODO: draw the light frame using a fancy axis... You must set up the right transformation!
//    glm::vec3 point = normalize(this->lookAt - this->position);
    
    M->loadIdentity();
    M->multMatrix(glm::inverse(this->V));
//    M->translate(this->position);
    
	axis.draw(program, M);
//    M->popMatrix();

	//draw the camera's wirecube
	M->pushMatrix();

	// TODO: draw the light camera frustum using the inverse projection with a wire cube. You must set up the right transformation!
    M->loadIdentity();
    M->multMatrix(glm::inverse(this->V));
    M->multMatrix(glm::inverse(this->P));
	debugWireCube->draw(program, P, V, M, LightPV);

	program->unbind();

	M->pushMatrix();	
	// TODO: draw the light view on the near plane of the frustum. You must set up the right transformation! 
	// That is, translate and scale the x and y directions of the -1 to 1 quad so that the quad fits exactly the l r t b portion of the near plane
    M->translate(0.0f,0.0f,-1.0f);
	debugDepthMapQuad->draw(quadShader, P, V, M, LightPV);

	M->popMatrix();
	M->popMatrix();
	M->popMatrix();
	
	GLSL::checkError(GET_FILE_LINE);
}
