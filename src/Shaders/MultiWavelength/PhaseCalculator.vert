#version 330    // This is needed for the round operator

// uniform mat4 projectionMatrix;
// uniform mat4 modelViewMatrix;

in vec3 vertex;
in vec3 color;
in vec2 textureCoordinate;

out vec2 fragmentTextureCoordinate;

void main() 
{
	fragmentTextureCoordinate = textureCoordinate;

	gl_Position = vec4(vertex, 1.0);
}

