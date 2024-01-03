#version 450

layout(location = 0) in vec2 tileUV;
//layout(location = 1) in vec3 fragColor;

layout (location = 0) out vec4 outColor;

struct PointLight{
	vec4 position; //ignore w
	vec4 color; //w is intensity
};

layout(set = 0, binding = 0) uniform GlobalUbo{
	mat4 projection;
	mat4 view;
	vec4 cameraPos;
	//vec4 ambientLightColor;
} ubo;

layout(set = 2, binding = 0) uniform sampler2D texSampler;

void main(){
	vec4 texColor = texture(texSampler, tileUV);

	outColor = vec4(texColor.rgb, 1.0);
	//outColor = vec4(fragColor, 1.0);
}