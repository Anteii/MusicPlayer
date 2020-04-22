#version 330 core

layout(location = 0) in vec3 vertexPosition;
uniform vec3 transform;

void main() {
	gl_Position.x = (vertexPosition.x + transform.y)*1.8/transform.x - 0.9;
	gl_Position.y = vertexPosition.y * transform.z;
	gl_Position.z = vertexPosition.z;
	gl_Position.w = 1.0;
 }