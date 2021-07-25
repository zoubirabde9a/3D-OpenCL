#version 330

in vec3 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUv;
out vec4 color;

uniform float time;
uniform sampler2D mySampler;


void main() {  
    vec2 lightPos = vec2(0, 0);
    float distance = length(vec2(fragmentPosition.x - lightPos.x, fragmentPosition.t - lightPos.y));
    vec4 textureColor = texture(mySampler, fragmentUv);
	vec4 chosenColor = vec4(1.0, 1.0, 1.0, 1.0);
	if (textureColor.a != 0){
	color = vec4(textureColor.rgb, textureColor.a * time);
	} else 
    color = vec4(textureColor.rgb, textureColor.a);
}
