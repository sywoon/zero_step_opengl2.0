#version 330

out vec4 FragColor;
//in vec3 color;
in vec4 OutColor;

//uniform float gScale;


void main()
{
	//FragColor = vec4(color.x, color.y, color.z, 1.0);
	FragColor = OutColor;
}