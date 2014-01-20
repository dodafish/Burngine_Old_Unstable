//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2014 Dominik David (frischer-hering@gmx.de)
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

#ifndef CUBEMAP_H_
#define CUBEMAP_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Texture/BaseTexture.h>

#include <Burngine/Graphics/General/OpenGL.h>

namespace burn {

/**
 * @brief Set of six textures in form of a cube
 */
class BURNGINE_API CubeMap : public BaseTexture {
public:
	/**
	 * @brief Default constructor initializing some values
	 */
	CubeMap();

	/**
	 * @brief Copyconstructor copies another CubeMap and holds
	 * its data seperatly
	 */
	CubeMap(const CubeMap& other);

	/**
	 * @brief Assignmentoperator copies another CubeMap and holds
	 * its data seperatly
	 */
	CubeMap& operator=(const CubeMap& other);

	/**
	 * @brief Default destructor cleaning up the CubeMap
	 */
	~CubeMap();

	/**
	 * @brief Loads a set of six textures from file and settings those to each side
	 * of the CubeMap
	 *
	 * @return Returns true on success. False otherwise
	 */
	bool loadFromFile(const std::string& filePositiveX, const std::string& fileNegativeX,
	const std::string& filePositiveY, const std::string& fileNegativeY, const std::string& filePositiveZ,
	const std::string& fileNegativeZ);

	/**
	 * @brief Returns true if the cubemap is created aka. loaded
	 *
	 * @return False if CubeMap is not loaded
	 *
	 * @see loadFromFile()
	 */
	bool isCreated() const;

private:
	virtual void onBind(const unsigned int& unit) const;

	void generate();
	void cleanup();

	GLuint _cubemap;
	unsigned int* _referenceCount;
};

} /* namespace burn */
#endif /* CUBEMAP_H_ */
