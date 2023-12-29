#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv; //tex coord?

layout(location = 2) in mat4 instanceTransform;
layout(location = 6) in vec2 tileUVOffset;

layout(location = 0) out vec2 tileUV;

struct PointLight{
	vec4 position; //ignore w
	vec4 color; //w is intensity
};

layout(set = 0, binding = 0) uniform GlobalUbo{
	mat4 projection;
	mat4 view;
	vec4 cameraPos;
} ubo;

void main(){
	vec4 positionWorld = instanceTransform * vec4(position, 1.0);
	gl_Position = ubo.projection * ubo.view * positionWorld;
	
	tileUV = uv + tileUVOffset;
}