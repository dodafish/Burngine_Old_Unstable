/*
 * Sampler.h
 *
 *  Created on: 26.10.2013
 *      Author: Dominik
 */

#ifndef SAMPLER_H_
#define SAMPLER_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/OpenGL.h>

namespace burn {

class BURNGINE_API Sampler {
public:
	static void bind(const Sampler& sampler, const unsigned int& unit = 0);

	//////////////////////////////////////////////////

	Sampler();
	Sampler(const Sampler& other);
	Sampler& operator=(const Sampler& other);
	~Sampler();

	bool create();
	void destroy();

	bool isCreated() const;

private:
	GLuint _id;
	unsigned int* _referenceCounter;
};

} /* namespace burn */
#endif /* SAMPLER_H_ */
