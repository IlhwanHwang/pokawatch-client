//
// CNH 2016.05.20
// sprite class
//

#pragma once

#include <GL/glew.h>
#include <vector>

class Sprite {
private:
	float ofx, ofy;
	float w, h;
	unsigned uw, uh;
	std::vector<GLuint> bufs;

public:
	Sprite& load(const char* fn);
	Sprite& setOffset(float x, float y);
	Sprite& dot();
	GLuint getBuf(int i) const { return bufs[i]; }
	float getW() const { return w; }
	float getH() const { return h; }
	float getOfx() const { return ofx; }
	float getOfy() const { return ofy; }
};