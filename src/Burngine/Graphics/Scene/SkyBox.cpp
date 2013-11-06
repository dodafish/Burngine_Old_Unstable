//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2013 Dominik David (frischer-hering@gmx.de)
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////

#include <Burngine/Graphics/Scene/SkyBox.h>

#include <Burngine/Graphics/General/OpenGlControl.h>
#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/Graphics/General/Shader.h>

namespace burn {

SkyBox::SkyBox() {
	//Generate the box

	Vector3f boxData[] = {
	Vector3f(-1.f, -1.f, -1.f), //back triangle1
	Vector3f(1.f, -1.f, -1.f),
	Vector3f(-1.f, 1.f, -1.f),
	Vector3f(1.f, -1.f, -1.f), //back triangle2
	Vector3f(-1.f, 1.f, -1.f),
	Vector3f(1.f, 1.f, -1.f),
	Vector3f(-1.f, -1.f, 1.f), //front triangle1
	Vector3f(1.f, -1.f, 1.f),
	Vector3f(-1.f, 1.f, 1.f),
	Vector3f(1.f, -1.f, 1.f), //front triangle2
	Vector3f(-1.f, 1.f, 1.f),
	Vector3f(1.f, 1.f, 1.f),
	Vector3f(-1.f, 1.f, 1.f), //left triangle1
	Vector3f(-1.f, -1.f, 1.f),
	Vector3f(-1.f, -1.f, -1.f),
	Vector3f(-1.f, 1.f, 1.f), //left triangle2
	Vector3f(-1.f, 1.f, -1.f),
	Vector3f(-1.f, -1.f, -1.f),
	Vector3f(1.f, 1.f, 1.f), //right triangle1
	Vector3f(1.f, -1.f, 1.f),
	Vector3f(1.f, -1.f, -1.f),
	Vector3f(1.f, 1.f, 1.f), //right triangle2
	Vector3f(1.f, 1.f, -1.f),
	Vector3f(1.f, -1.f, -1.f),
	Vector3f(-1.f, 1.f, 1.f), //top triangle1
	Vector3f(1.f, 1.f, 1.f),
	Vector3f(-1.f, 1.f, -1.f),
	Vector3f(-1.f, 1.f, -1.f), //top triangle2
	Vector3f(1.f, 1.f, 1.f),
	Vector3f(1.f, 1.f, -1.f),
	Vector3f(-1.f, -1.f, 1.f), //bottom triangle1
	Vector3f(1.f, -1.f, 1.f),
	Vector3f(-1.f, -1.f, -1.f),
	Vector3f(-1.f, -1.f, -1.f), //bottom triangle2
	Vector3f(1.f, -1.f, 1.f),
	Vector3f(1.f, -1.f, -1.f), };

	_boxVbo.create();
	_boxVbo.addData(&boxData, sizeof(Vector3f) * 6 * 2 * 3); //6 sides, 2 triangles each, 3 vertices each
	_boxVbo.uploadDataToGpu(GL_ARRAY_BUFFER, GL_STATIC_DRAW);

}

SkyBox::SkyBox(const SkyBox& other) :
_cubeMap(other._cubeMap),
_boxVbo(other._boxVbo) {
}

SkyBox& SkyBox::operator=(const SkyBox& other) {

	if(this == &other)
		return *this;

	_cubeMap = other._cubeMap;
	_boxVbo = other._boxVbo;

	return *this;
}

void SkyBox::setCubeMap(const CubeMap& cubeMap) {
	_cubeMap = cubeMap;
}

const CubeMap& SkyBox::getCubeMap() const {
	return _cubeMap;
}

void SkyBox::draw() {

	if(!Window::isContextCreated())
		return;

	if(!_cubeMap.isCreated())
		return;

	OpenGlControl::Settings ogl;
	ogl.enableCulling(false);
	ogl.enableDepthbufferWriting(false);
	ogl.setDepthtestTechnique(OpenGlControl::LEQUAL);
	OpenGlControl::useSettings(ogl);

	const Shader& shader = BurngineShaders::getShader(BurngineShaders::SKY_BOX);

	_cubeMap.bindAsSource();

	glEnableVertexAttribArray(0);
	_boxVbo.bind();
	glVertexAttribPointer(0, // attribute 0
	3,                  // size
	GL_FLOAT,           // type
	GL_FALSE,           // normalized?
	0,                  // stride
	(void*)0            // array buffer offset
	);
	OpenGlControl::draw(OpenGlControl::TRIANGLES, 0, 6 * 2 * 3, shader);
	glDisableVertexAttribArray(0);

	OpenGlControl::useSettings(OpenGlControl::Settings());
}

} /* namespace burn */
