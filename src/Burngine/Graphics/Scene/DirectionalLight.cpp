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

#include <Burngine/Graphics/Scene/DirectionalLight.h>

namespace burn {

Vector4f DirectionalLight::getDirection() const {
	Matrix4f rotMat = glm::rotate(Matrix4f(1.f), _rotation.x, Vector3f(1.f, 0.f, 0.f));
	rotMat = glm::rotate(rotMat, _rotation.y, Vector3f(0.f, 1.f, 0.f));
	rotMat = glm::rotate(rotMat, _rotation.z, Vector3f(0.f, 0.f, 1.f));
	return (rotMat * Vector4f(1.f, 0.f, 0.f, 1.0f));
}

} /* namespace burn */
