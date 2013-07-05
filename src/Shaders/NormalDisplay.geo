#version 330

precision highp float;

layout (triangles) in;
layout (line_strip) out;
layout (max_vertices = 8) out;

in Vertex
{
	vec3 
}
