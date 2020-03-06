#version 330
//#extension GL_ARB_separate_shader_objects : enable

layout(location=0) in vec3 Position;
layout(location=1) in vec3 Color;

//uniform float gScale;  //若没使用  会被优化掉 导致glGetUniformLocation(program, "gScale"); 释放返回-1(0xffffffff)
uniform mat4 gWorld;

//varying
//out vec3 color;
out vec4 OutColor;

void main()
{
	//gl_Position = vec4(Position.x*gScale, Position.y*gScale, Position.z*gScale, 1.0);
	gl_Position = gWorld * vec4(Position, 1.0);
	//color = vec3(Position.x * gScale, Position.y * gScale , Position.z * gScale);
	OutColor = vec4(Color, 1.0);
}
