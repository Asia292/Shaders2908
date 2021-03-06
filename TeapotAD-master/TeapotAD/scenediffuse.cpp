
#include "scenediffuse.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
using std::cerr;
using std::endl;

#include "defines.h"

using glm::vec3;


#include <gtc/matrix_transform.hpp>
#include <gtx/transform2.hpp>


namespace imat2908
{
/////////////////////////////////////////////////////////////////////////////////////////////
// Default constructor
/////////////////////////////////////////////////////////////////////////////////////////////
SceneDiffuse::SceneDiffuse()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////
//Initialise the scene
/////////////////////////////////////////////////////////////////////////////////////////////
void SceneDiffuse::initScene(QuatCamera camera)
{
    //|Compile and link the shader  
	compileAndLinkShader();

    gl::Enable(gl::DEPTH_TEST);

 	//Set up the lighting
	setLightParams(camera);


	//Create the plane to represent the ground
	plane = new VBOPlane(100.0,100.0,100,100);



	//A matrix to move the teapot lid upwards
	glm::mat4 lid = glm::mat4(1.0);
	//lid *= glm::translate(vec3(0.0,1.0,0.0));

	//Create the teapot with translated lid
	teapot = new VBOTeapot(16,lid);


}
//// ALLOWS SWITCHING ON/OFF OF SPOTLIGHT ////
void SceneDiffuse::sptlight()
{
	spotLight = !spotLight;		// sets spotlight bool to its opposite
	prog.setUniform("spotlight.SpotLight", spotLight);		// assigns variable of spotlight bool to the one that controls if statements

	std::cout << spotLight << std::endl;		// debugging - does bool change
}

/////////////////////////////////////////////////////////////////////////////////////////////
//Update not used at present
/////////////////////////////////////////////////////////////////////////////////////////////
void SceneDiffuse::update( float t )
{

}

/////////////////////////////////////////////////////////////////////////////////////////////
// Set up the lighting variables in the shader
/////////////////////////////////////////////////////////////////////////////////////////////
void SceneDiffuse::setLightParams(QuatCamera camera)
{

	vec3 worldLight = vec3(10.0f,10.0f,10.0f);
   
	//diffuse
	prog.setUniform("dVars.Ld", 0.9f, 0.9f, 0.9f);		// diffuse light intensity
	//ambiant
	prog.setUniform("aVars.La", 0.3f, 0.3f, 0.3f);		// ambiant light intensity
	//specular
	prog.setUniform("view", camera.view());
	prog.setUniform("sVars.Ls", 0.3f, 0.3f, 0.3f);		// specular light intensity

	//spotlight
	prog.setUniform("spotlight.pos", 0.0f, 5.0f, -20.0f);	// position of spotlight
	prog.setUniform("spotlight.dir", 0.0f, -1.0f, 0.0f);	// direction of spotlight
	prog.setUniform("spotlight.cutOff", glm::cos(glm::radians(15.f)));		// cut off angle for spotlight converted to radians
	prog.setUniform("spotlight.SpotLight", false);		// sets spotlight default to off

	prog.setUniform("LightPosition", worldLight );		// position of standard light
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Render the scene
/////////////////////////////////////////////////////////////////////////////////////////////
void SceneDiffuse::render(QuatCamera camera)
{
    gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	//First deal with the plane to represent the ground

	//Initialise the model matrix for the plane
	model = mat4(1.0f);
	//Set the matrices for the plane although it is only the model matrix that changes so could be made more efficient
    setMatrices(camera);
	//Set the plane's material properties in the shader and render
	//diffuse
	prog.setUniform("dVars.Kd", 0.51f, 1.0f, 0.49f);		// diffuse plane reflection coefficent
	//ambniabnt
	prog.setUniform("aVars.Ka", 0.51f, 1.0f, 0.49f);		// ambiant plane reflection coefficent
	//speculat
	prog.setUniform("sVars.Ks", 0.1f, 0.1f, 0.1f);			// specular plane reflection coefficent
	
	plane->render();// renders the plane with defined variables



	//Now set up the teapot 
	 model = mat4(1.0f);
	 setMatrices(camera);
	 //Set the Teapot material properties in the shader and render
	 //diffuse
	 prog.setUniform("dVars.Kd", 0.46f, 0.29f, 0.0f);		// diffuse teapot reflection coefficent
	 //ambiant
	 prog.setUniform("aVars.Ka", 0.46f, 0.29f, 0.0f);		// ambiant teapot reflection coefficent
	 //specular
	 prog.setUniform("sVars.Ks", 0.29f, 0.29f, 0.29f);		// specular teapot reflection coefficent
	 
	 teapot->render(); // renders the teapot with defined variables
	
}


/////////////////////////////////////////////////////////////////////////////////////////////
//Send the MVP matrices to the GPU
/////////////////////////////////////////////////////////////////////////////////////////////
void SceneDiffuse::setMatrices(QuatCamera camera)
{

    mat4 mv = camera.view() * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", camera.projection() * mv);
	mat3 normMat = glm::transpose(glm::inverse(mat3(model)));// What does this line do?
	prog.setUniform("M", model);
	prog.setUniform("V", camera.view() );
	prog.setUniform("P", camera.projection() );

	
}

/////////////////////////////////////////////////////////////////////////////////////////////
// resize the viewport
/////////////////////////////////////////////////////////////////////////////////////////////
void SceneDiffuse::resize(QuatCamera camera,int w, int h)
{
    gl::Viewport(0,0,w,h);
    width = w;
    height = h;
	camera.setAspectRatio((float)w/h);

}

/////////////////////////////////////////////////////////////////////////////////////////////
// Compile and link the shader
/////////////////////////////////////////////////////////////////////////////////////////////
void SceneDiffuse::compileAndLinkShader()
{
   
	try {
    	prog.compileShader("Shaders/phong.vert");
    	prog.compileShader("Shaders/phong.frag");
    	prog.link();
    	prog.validate();
    	prog.use();
    } catch(GLSLProgramException & e) {
 		cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}

}
