#version 330

in vec3 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUv;
in vec3 fragmentNormal;
out vec4 color;

uniform float time;
uniform sampler2D mySampler;
uniform vec3 lightPos;

void main() {
    vec3 ambient = vec3(0.2, 0.2, 0.2);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 lightDir = normalize(lightPos - fragmentPosition);
    vec4 textureColor = texture(mySampler, fragmentUv);
    float diff = max(dot(fragmentNormal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    color = vec4((ambient + diffuse) * textureColor.xyz, textureColor.w);
}
