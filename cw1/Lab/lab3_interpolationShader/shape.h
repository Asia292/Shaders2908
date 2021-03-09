#ifndef shape_H
#define shape_H

#include "scene.h"

#include "gl_core_4_3.hpp"

class shape : public Scene
{
private:
	int width, height;
	GLuint vboHandles[3];

	GLuint vaoHandle;
	GLuint programHandle;

	void linkMe(GLint vertShader, GLint fragShader);
	GLuint indexSize;
	GLuint numberOfIndices;
	GLuint indices;

public:
	shape();

	void initScene();
	void update(float t);
	void render();
	void resize(int, int);

};
#endif  //shape_H