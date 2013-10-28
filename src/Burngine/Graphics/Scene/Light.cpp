/*
 * Light.cpp
 *
 *  Created on: 26.10.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Scene/Light.h>

#include <Burngine/Graphics/General/Shader.h>
#include <Burngine/Graphics/General/OpenGlControl.h>
#include <Burngine/Graphics/Scene/StaticMeshNode.h>
#include <Burngine/Graphics/Scene/Mesh.h>
#include <Burngine/Graphics/Window/Window.h>

namespace burn {

void Light::removeAllParents() {
	if(_parents.size() == 0)
		return;

	Scene* parents[_parents.size()];
	for(size_t i = 0; i != _parents.size(); ++i)
		parents[i] = _parents[i];
	size_t size = _parents.size();
	for(size_t i = 0; i != size; ++i)
		parents[i]->detachLight(*this);

	_parents.clear();
}

Light::Light() :
_color(Vector3f(1.f)),
_intensity(10.f) {

	if(!_shadowCubeMap.create())
		exit(20);

}

Light::Light(const Light& other) :
_color(other._color),
_intensity(other._intensity) {
	_parents = other._parents;
	for(size_t i = 0; i < _parents.size(); ++i){
		_parents[i]->attachLight(*this);
	}
}

Light& Light::operator=(const Light& other) {

	_color = other._color;
	_intensity = other._intensity;

	removeAllParents();

	_parents = other._parents;
	for(size_t i = 0; i < _parents.size(); ++i){
		_parents[i]->attachLight(*this);
	}

	return *this;

}

Light::~Light() {
	removeAllParents();
}

void Light::addParentScene(Scene* scene) {
	for(size_t i = 0; i < _parents.size(); ++i){
		if(_parents[i] == scene)
			return; //Already added as parent
	}
	_parents.push_back(scene); //Add to parents
}

void Light::removeParentScene(Scene* scene) {
	for(size_t i = 0; i < _parents.size(); ++i){
		if(_parents[i] == scene){
			_parents.erase(_parents.begin() + i);
			return; //addParentScene() ensures, that a scene is added only once. So return
		}
	}
}

void Light::setColor(const Vector3f& color) {
	_color = color;
}

const Vector3f& Light::getColor() const {
	return _color;
}

void Light::setIntensity(const float& intensity) {
	_intensity = (intensity > 0) ? intensity : 0;
}

const float& Light::getIntensity() const {
	return _intensity;
}

const Matrix4f& Light::getBiasViewMatrix() const {
	return _biasViewMatrix;
}

const Matrix4f& Light::getBiasProjectionMatrix() const {
	return _biasProjectionMatrix;
}

void Light::bindShadowCubeMap() const {
	_shadowCubeMap.bind();
}

void Light::updateShadowMap(const std::vector<SceneNode*>& nodes) {

	if(!Window::isContextCreated() || !_shadowCubeMap.isCreated())
		return;

	//Clear old shadowMap and bind it for rendering
	_shadowCubeMap.clear();

	//Select shader
	const Shader& shader = BurngineShaders::getShader(BurngineShaders::DEPTH);

	//Calculate matrix
	Matrix4f projectionMatrix = glm::perspective<float>(90.f, 1.f, 0.1f, 100.f);

	//Set uniforms
	shader.setUniform("projectionMatrix", projectionMatrix);
	_biasProjectionMatrix = projectionMatrix;
	_biasViewMatrix = findViewMatrix(5);

	//Scan through all nodes
	for(size_t i = 0; i < nodes.size(); ++i){

		if(!nodes[i]->isCastingShadows())
			continue;

		if(typeid(*(nodes[i])) == typeid(StaticMeshNode)){
			StaticMeshNode* node = static_cast<StaticMeshNode*>(nodes[i]);

			node->update();

			//Skip if updating failed or is incomplete
			if(!node->getModel().isUpdated())
				continue;

			Matrix4f modelMatrix = node->getModelMatrix();
			shader.setUniform("modelMatrix", modelMatrix);

			for(int face = 0; face != 6; ++face){

				_shadowCubeMap.bindAsRendertarget(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face);
				shader.setUniform("viewMatrix", findViewMatrix(face));

				for(size_t j = 0; j < node->getModel().getMeshCount(); ++j){

					//0 = Positions
					glEnableVertexAttribArray(0);
					node->getModel().getMesh(j).getPositionVbo().bind();
					glVertexAttribPointer(0, // attribute 0
					3,                  // size
					GL_FLOAT,           // type
					GL_FALSE,           // normalized?
					0,                  // stride
					(void*)0            // array buffer offset
					);

					//Draw
					OpenGlControl::draw(OpenGlControl::TRIANGLES, 0, node->getModel().getMesh(j).getVertexCount(),
										shader);

					glDisableVertexAttribArray(0);

				}

			}

		}

	}

}

Matrix4f Light::findViewMatrix(const int& face) const {

	Vector3f direction(1.f, 0.f, 0.f);
	Vector3f headUp(0.f, -1.f, 0.f);

	if(face == 0){
		direction = Vector3f(1.f, 0.f, 0.f);
	}else if(face == 1){
		direction = Vector3f(-1.f, 0.f, 0.f);
	}else if(face == 2){
		direction = Vector3f(0.f, 1.f, 0.f);
		headUp = Vector3f(0.f, 0.f, 1.f);
	}else if(face == 3){
		direction = Vector3f(0.f, -1.f, 0.f);
		headUp = Vector3f(0.f, 0.f, -1.f);
	}else if(face == 4){
		direction = Vector3f(0.f, 0.f, 1.f);
	}else{
		direction = Vector3f(0.f, 0.f, -1.f);
	}

	return glm::lookAt(_position, _position + direction, headUp);
}

} /* namespace burn */
