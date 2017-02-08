#version 450
#extension GL_ARB_separate_shader_objects : enable

uniform Matrix
{
	mat4 MVP;
};

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 incolor;
layout(location = 0) out vec4 outcolor;

void main()
{
	outcolor = incolor;
	gl_Position = pos*MVP;
}