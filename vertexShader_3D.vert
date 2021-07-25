#version 330

in vec3 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUv;
in vec3 vertexNormal;

out vec3 fragmentPosition;
out vec4 fragmentColor;
out vec2 fragmentUv;
out vec3 fragmentNormal;

uniform mat4 P;

void main(){
 gl_Position = (P * vec4(vertexPosition, 1.0));
 
 fragmentPosition = vertexPosition;
 fragmentColor = vertexColor;
 fragmentUv = vec2(vertexUv.x, 1 - vertexUv.y);
 fragmentNormal = vertexNormal;
}
