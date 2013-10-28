/*
 * CubeMap.h
 *
 *  Created on: 28.10.2013
 *      Author: Dominik
 */

#ifndef CUBEMAP_H_
#define CUBEMAP_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Texture/BaseTexture.h>

#include <Burngine/Graphics/General/OpenGL.h>

namespace burn {

class BURNGINE_API CubeMap : public BaseTexture {
public:
	CubeMap();
	CubeMap(const CubeMap& other);
	CubeMap& operator=(const CubeMap& other);

	~CubeMap();

	bool loadFromFile(const std::string& filePositiveX, const std::string& fileNegativeX,
	const std::string& filePositiveY, const std::string& fileNegativeY, const std::string& filePositiveZ,
	const std::string& fileNegativeZ);

	bool isCreated() const;

private:
	virtual void onBind(const unsigned int& unit) const;
	virtual void onUnbind(const unsigned int& unit) const;

	void generate();
	void cleanup();

	GLuint _cubemap;
	unsigned int* _referenceCount;
};

} /* namespace burn */
#endif /* CUBEMAP_H_ */
