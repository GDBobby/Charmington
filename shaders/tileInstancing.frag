#version 450

layout(location = 0) in ivec2 tileUV;

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

layout(set = 1, binding = 0) uniform sampler2D texSampler;

layout(set = 2, binding = 0) uniform mapTiles {
	ivec2 tileIDs[1024];
};

layout(push_constant) uniform Push{
	mat4 transform;
	ivec2 mapDims;
} push;


void main(){
	

	vec4 texColor = texture(texSampler, tileUV);

	outColor = vec4(texColor.rgb, 1.0);
}