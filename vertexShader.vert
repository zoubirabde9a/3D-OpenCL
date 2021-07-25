#version 330

in vec3 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUv;


out vec3 fragmentPosition;
out vec4 fragmentColor;
out vec2 fragmentUv;

uniform mat4 P;

void main(){
 gl_Position.xy = (P * vec4(vertexPosition, 1.0)).xy;
 gl_Position.z = 0.0;
 gl_Position.w = 1.0;
 fragmentPosition = vertexPosition;
 fragmentColor = vertexColor;
 fragmentUv = vec2(vertexUv.x, 1 - vertexUv.y);
}
