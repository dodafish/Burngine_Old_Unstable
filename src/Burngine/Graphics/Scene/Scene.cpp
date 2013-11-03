/*
 * Scene.cpp
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Scene/Scene.h>

#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/Graphics/General/OpenGlControl.h>

#include <Burngine/Graphics/Scene/SceneNode.h>
#include <Burngine/Graphics/Scene/StaticMeshNode.h>
#include <Burngine/Graphics/Scene/Camera.h>
#include <Burngine/Graphics/Scene/Light.h>
#include <Burngine/Graphics/Scene/Mesh.h>

#include <Burngine/System/Reporter.h>

namespace burn {

Scene::Scene(const Window& parentWindow) :
_window(parentWindow) {

	/*if(!_diffuseLightTexture.create(Vector2ui(_window.getSettings().getWidth(), _window.getSettings().getHeight()))){
	 Reporter::report("Scene could not be created. Unable to create RenderTexture!", Reporter::ERROR);
	 exit(10);
	 }
	 if(!_specularLightTexture.create(Vector2ui(_window.getSettings().getWidth(), _window.getSettings().getHeight()))){
	 Reporter::report("Scene could not be created. Unable to create RenderTexture!", Reporter::ERROR);
	 exit(11);
	 }*/

	if(!_gBuffer.create(Vector2ui(_window.getSettings().getWidth(), _window.getSettings().getHeight()))){
		Reporter::report("Unable to create gBuffer!", Reporter::ERROR);
		exit(11);
	}

}

Scene::~Scene() {

	detachAll();

}

void Scene::drawGBuffers(const Camera& camera) {

	OpenGlControl::useSettings(OpenGlControl::Settings());

	_gBuffer.clear();
	_gBuffer.bindAsTarget();

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::G_BUFFER);

	//Calculate matrices
	glm::mat4 projectionMatrix = glm::perspective(camera.getFov(), camera.getAspectRatio(), 0.1f, 10000.0f);
	glm::mat4 viewMatrix = glm::lookAt(camera.getPosition(), camera.getLookAt(), glm::vec3(0, 1, 0));

	shader.setUniform("viewMatrix", viewMatrix);
	shader.setUniform("projectionMatrix", projectionMatrix);

	for(size_t i = 0; i < _nodes.size(); ++i){

		//_nodes[i] is a StaticMeshNode
		if(typeid(*(_nodes[i])) == typeid(StaticMeshNode)){

			StaticMeshNode* node = static_cast<StaticMeshNode*>(_nodes[i]);

			node->update();
			const Model& model = node->getModel();
			if(!model.isUpdated())
				continue; //Next node

			Matrix4f normalMatrix = glm::transpose(glm::inverse(viewMatrix * node->getModelMatrix()));
			shader.setUniform("modelMatrix", node->getModelMatrix());
			shader.setUniform("normalMatrix", normalMatrix);

			for(size_t j = 0; j < model.getMeshCount(); ++j){

				const Mesh& mesh = model.getMesh(j);

				if(mesh.getMaterial().getType() == Material::SOLID_COLOR){
					shader.setUniform("diffuseType", 1);
					shader.setUniform("meshColor", mesh.getMaterial().getDiffuseColor());
				}else{
					shader.setUniform("diffuseType", 0); //Type = TEXTURED
					mesh.getTexture().bind();
				}

				glEnableVertexAttribArray(0);
				glEnableVertexAttribArray(1);
				glEnableVertexAttribArray(2);

				mesh.getPositionVbo().bind();
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
				mesh.getNormalVbo().bind();
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
				mesh.getUvVbo().bind();
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

				OpenGlControl::draw(OpenGlControl::TRIANGLES, 0, mesh.getVertexCount(), shader);

				glDisableVertexAttribArray(0);
				glDisableVertexAttribArray(1);
				glDisableVertexAttribArray(2);

			}

		}

	}

}

void Scene::draw(const Camera& camera, const RenderModus& modus) {

	if(!Window::isContextCreated())
		return;

	drawGBuffers(camera);

	_window.bind(); // As Target
	_gBuffer.bindAsSource();
	switch (modus) {
		case COMPOSITION:
			//COMING LATER. NO LIGHTPASS YET
			break;
		case DIFFUSE:
			_gBuffer.setSourceBuffer(GBuffer::DIFFUSE);
			glBlitFramebuffer(0, 0, _gBuffer.getDimensions().x, _gBuffer.getDimensions().y, 0, 0,
								_window.getSettings().getWidth(), _window.getSettings().getHeight(),
								GL_COLOR_BUFFER_BIT,
								GL_LINEAR);
			break;
		case NORMAL:
			_gBuffer.setSourceBuffer(GBuffer::NORMAL);
			glBlitFramebuffer(0, 0, _gBuffer.getDimensions().x, _gBuffer.getDimensions().y, 0, 0,
								_window.getSettings().getWidth(), _window.getSettings().getHeight(),
								GL_COLOR_BUFFER_BIT,
								GL_LINEAR);
			break;
		case DEPTH:
			dumpOutDepthGBuffer(); //Special, because no GL_COLOR_BUFFER
			break;
	}

//Update all lights' shadowMaps
//for(size_t i = 0; i < _lights.size(); ++i){
//	_lights[i]->updateShadowMap(_nodes);
//}

	/*switch (modus) {
	 case ALL:
	 drawNodes(camera);

	 if(drawDiffusepart(camera)){
	 //Add diffuse lighting to scene:
	 OpenGlControl::Settings oglSettings; //default
	 oglSettings.setBlendMode(OpenGlControl::MULTIPLY);
	 oglSettings.enableDepthtest(false);
	 oglSettings.enableDepthbufferWriting(false);
	 OpenGlControl::useSettings(oglSettings);
	 _window.bind();
	 _diffuseLightTexture.drawFullscreen(); //Diffuse lightings

	 if(drawSpecularpart(camera)){
	 //Add specular lighting to scene:
	 oglSettings = OpenGlControl::Settings(); //default
	 oglSettings.setBlendMode(OpenGlControl::ADD);
	 oglSettings.enableDepthtest(false);
	 oglSettings.enableDepthbufferWriting(false);
	 OpenGlControl::useSettings(oglSettings);
	 _window.bind();
	 _specularLightTexture.drawFullscreen(); //Specular lightings
	 }
	 }

	 break;
	 case COLOR:
	 drawNodes(camera);
	 break;
	 case DIFFUSE:
	 if(drawDiffusepart(camera)){
	 //Add diffuse lighting to scene:
	 OpenGlControl::Settings oglSettings; //default
	 OpenGlControl::useSettings(oglSettings);
	 _window.bind();
	 _diffuseLightTexture.drawFullscreen(); //Diffuse lightings
	 }
	 break;
	 case SPECULAR:
	 if(drawSpecularpart(camera)){
	 //Add diffuse lighting to scene:
	 OpenGlControl::Settings oglSettings; //default
	 OpenGlControl::useSettings(oglSettings);
	 _window.bind();
	 _specularLightTexture.drawFullscreen(); //Diffuse lightings
	 }
	 break;
	 case LIGHTING:
	 if(drawDiffusepart(camera)){
	 //Add diffuse lighting to scene:
	 OpenGlControl::Settings oglSettings; //default
	 oglSettings.setBlendMode(OpenGlControl::OVERWRITE);
	 oglSettings.enableDepthtest(false);
	 oglSettings.enableDepthbufferWriting(false);
	 OpenGlControl::useSettings(oglSettings);
	 _window.bind();
	 _diffuseLightTexture.drawFullscreen(); //Diffuse lightings

	 if(drawSpecularpart(camera)){
	 //Add specular lighting to scene:
	 oglSettings = OpenGlControl::Settings(); //default
	 oglSettings.setBlendMode(OpenGlControl::ADD);
	 oglSettings.enableDepthtest(false);
	 oglSettings.enableDepthbufferWriting(false);
	 OpenGlControl::useSettings(oglSettings);
	 _window.bind();
	 _specularLightTexture.drawFullscreen(); //Specular lightings
	 }
	 }
	 break;
	 }

	 //Restore default OpenGL settings
	 OpenGlControl::useSettings(OpenGlControl::Settings());

	 _skyBox.draw();*/

}

void Scene::ambientPass() {

}

void Scene::drawFullscreenQuad(const Shader& shader) const {

	Vector3f posData[] = {
	Vector3f(-1.f, -1.f, 0.f),
	Vector3f(1.f, -1.f, 0.f),
	Vector3f(-1.f, 1.f, 0.f),
	Vector3f(1.f, 1.f, 0.f) };

	Vector2f uvData[] = {
	Vector2f(0.f, 0.f),
	Vector2f(1.f, 0.f),
	Vector2f(0.f, 1.f),
	Vector2f(1.f, 1.f), };

	VertexBufferObject vbo;
	for(int i = 0; i != 4; ++i){
		vbo.addData(&posData[i], sizeof(Vector3f));
		vbo.addData(&uvData[i], sizeof(Vector2f));
	}
	vbo.uploadDataToGpu(GL_ARRAY_BUFFER, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	vbo.bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f) + sizeof(Vector2f), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector3f) + sizeof(Vector2f), (void*)sizeof(Vector3f));

	OpenGlControl::draw(OpenGlControl::TRIANGLE_STRIP, 0, 4, shader);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

}

void Scene::dumpOutDepthGBuffer() {

	OpenGlControl::Settings ogl;
	ogl.enableDepthtest(false);
	ogl.enableDepthbufferWriting(false);
	ogl.enableCulling(false);
	OpenGlControl::useSettings(ogl);

	_window.bind();
	_gBuffer.bindDepthBufferAsSourceTexture();

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::TEXTURE);
	shader.setUniform("modelMatrix", Matrix4f(1.f));
	shader.setUniform("viewMatrix", Matrix4f(1.f));
	shader.setUniform("projectionMatrix", Matrix4f(1.f));

	drawFullscreenQuad(shader);

	OpenGlControl::useSettings(OpenGlControl::Settings());

}

/*void Scene::drawNodes(const Camera& camera) {
 //Render objects without lighting:
 _window.bind();
 OpenGlControl::useSettings(OpenGlControl::Settings());
 for(size_t i = 0; i < _nodes.size(); ++i){
 _nodes[i]->draw(camera);
 }
 }

 bool Scene::drawDiffusepart(const Camera& camera) {

 if(!_diffuseLightTexture.isCreated())
 return false;

 //Render objects' lightings:

 OpenGlControl::Settings oglSettings;
 oglSettings.setClearColor(Vector4f(1.f));
 OpenGlControl::useSettings(oglSettings);

 _diffuseLightTexture.clear();
 _diffuseLightTexture.bindAsRendertarget();

 for(size_t i = 0; i < _nodes.size(); ++i){
 _nodes[i]->drawSingleColor(camera, Vector4f(0.f, 0.f, 0.f, 0.f));
 }

 oglSettings.setDepthtestTechnique(OpenGlControl::EQUAL);
 oglSettings.setBlendMode(OpenGlControl::ADD);
 OpenGlControl::useSettings(oglSettings);

 for(size_t i = 0; i < _nodes.size(); ++i){
 _nodes[i]->drawLighting(SceneNode::DIFFUSE, camera, _lights, _ambientColor);
 }

 return true;
 }

 bool Scene::drawSpecularpart(const Camera& camera) {
 if(!_specularLightTexture.isCreated())
 return false;

 OpenGlControl::Settings oglSettings;
 oglSettings.setClearColor(Vector4f(0.f, 0.f, 0.f, 1.f));
 OpenGlControl::useSettings(oglSettings);

 _specularLightTexture.clear();
 _specularLightTexture.bindAsRendertarget();

 for(size_t i = 0; i < _nodes.size(); ++i){
 _nodes[i]->drawSingleColor(camera, Vector4f(0.f, 0.f, 0.f, 0.f));
 }

 oglSettings.setDepthtestTechnique(OpenGlControl::EQUAL);
 oglSettings.setBlendMode(OpenGlControl::ADD);
 OpenGlControl::useSettings(oglSettings);
 for(size_t i = 0; i < _nodes.size(); ++i){
 _nodes[i]->drawLighting(SceneNode::SPECULAR, camera, _lights, _ambientColor);
 }

 return true;
 }*/

void Scene::detachAll() {
//All SceneNodes:
	for(size_t i = 0; i < _nodes.size(); ++i){
		_nodes[i]->removeParentScene(this);
	}
	_nodes.clear();

//All Lights:
	for(size_t i = 0; i < _lights.size(); ++i){
		_lights[i]->removeParentScene(this);
	}
	_lights.clear();

}

void Scene::attachSceneNode(SceneNode& node) {
	for(size_t i = 0; i < _nodes.size(); ++i){
		if(_nodes[i] == &node)
			return; //Already attached
	}
	_nodes.push_back(&node);
	node.addParentScene(this);
}

void Scene::detachSceneNode(SceneNode& node) {

	node.removeParentScene(this);

//Remove from attachement-list
	for(size_t i = 0; i < _nodes.size(); ++i){
		if(_nodes[i] == &node){
			_nodes.erase(_nodes.begin() + i);
			return;
		}
	}

}

void Scene::attachLight(Light& light) {
	for(size_t i = 0; i < _lights.size(); ++i){
		if(_lights[i] == &light)
			return; //Already attached
	}
	_lights.push_back(&light);
	light.addParentScene(this);
}

void Scene::detachLight(Light& light) {

	light.removeParentScene(this);

//Remove from attachement-list
	for(size_t i = 0; i < _lights.size(); ++i){
		if(_lights[i] == &light){
			_lights.erase(_lights.begin() + i);
			return;
		}
	}

}

void Scene::setAmbientColor(const Vector3f& color) {
	_ambientColor = color;
}

const Vector3f& Scene::getAmbientColor() const {
	return _ambientColor;
}

void Scene::setSkyBox(const SkyBox& skyBox) {
	_skyBox = skyBox;
}

} /* namespace burn */
