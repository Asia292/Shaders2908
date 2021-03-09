#ifndef cube3D_H
#define cube3D_H

#include "scene.h"

#include "gl_core_4_3.hpp"

class cube3D : public Scene
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
	cube3D();

	void initScene();
	void update(float t);
	void render();
	void resize(int, int);

};
#endif  //cube3D_H