#version 430

in vec3 vertPos;
in vec3 N;
in vec3 lightPos;
/*TODO:: Complete your shader code for a full Phong shading*/ 


struct ambiantVars
{
	 vec3 Ka;            // Amb reflectivity
	 vec3 La;            // amb light intensity
};

struct diffuseVars
{
	 vec3 Kd;            // Diffuse reflectivity
	 vec3 Ld;            // Diffuse light intensity
};

struct specularVars
{
	 vec3 Ks;            // Spec reflectivity
	 vec3 Ls;            // Spec light intensity
};

struct spotLight
{
	vec3 pos;			// spotlight position
	vec3 dir;			// spotlight direction
	float cutOff;		// cut off angle
	bool SpotLight;		// if spotlight is on

};

uniform ambiantVars aVars;
uniform diffuseVars dVars;
uniform specularVars sVars;
uniform spotLight spotlight;


uniform vec3 view;

layout( location = 0 ) out vec4 FragColour;

vec4 diffuse(vec3 light)	//calculate diffuse lighting
{
	vec4 Id = vec4(dVars.Kd,1.0)*vec4(dVars.Ld,1.0) * max(dot(N,light), 0.0);
	Id = clamp(Id, 0.0, 1.0);

	return Id;
}

vec4 ambiant()				// calculate ambiant lighting
{
	vec4 Ia = vec4(aVars.La, 1.0) * vec4(aVars.Ka, 1.0);
	Ia = clamp(Ia, 0.0, 1.0);

	return Ia;
}

vec4 specular(vec3 light)	//calculate specular lighting
{
	vec3 sth = normalize(view - vertPos);
	vec3 R = (reflect(-light, N));
	float cosP = dot(sth, R);

	vec4 Is = (vec4(sVars.Ls, 1.0) * vec4(sVars.Ks, 1.0)) * pow(cosP, 100);
	Is = clamp(Is, 0.0, 1.0);

	return Is;
}

float attenuation(float constant, float lin, float quad)	//calculate attenutation
{
	float att = 1 / (constant + (lin * (distance(lightPos , vertPos)) + (quad * pow((distance(lightPos, vertPos)), 2))));

	return att;
}

void main() 
{

   //Calculate the light vector
   vec3 L = normalize(lightPos - vertPos); 

	/*if (spotlight.SpotLight)	//calculates L for spotlight - normalised distance between spotlight and vertex
		vec3 L = normalize(spotlight.pos - vertPos);
	else
		vec3 L = normalize(lightPos - vertPos);	*/	//calculates L for normal lighting - normalised distance between light and vertex
  
	//assigns specular, ambiant, diffuse and attenuatuion variables
	vec4 spec = specular(L);	
	vec4 amb = ambiant();
	vec4 dif = diffuse(L);

	float atten = attenuation(1.0f, 0.001f, 0.001f);


   //Multiply the Reflectivity by the Diffuse intensity
   //FragColour =  amb + (atten*spec) + (atten*dif);

	/*if (spotlight.SpotLight)
	{
	   //spotlight
	   float angle = dot(normalize(spotlight.pos - vertPos), normalize(spotlight.dir));		// calculates the angle of the spotlight

	   // compares the calculated angle with that of the cut off
	   if (angle < spotlight.cutOff)
			FragColour =  amb + (atten*spec) + (atten*dif);	// if within spotlight apply full lighting
	   else
			FragColour =  amb;		// outside spotlight only apply ambiant
	}*/
	//else
		FragColour =  amb + (atten*spec) + (atten*dif);	// if spotlight off apply full lighting
}
