#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in ivec2 uv; //tex coord?

//square
//0, 1 ,2 ,3
//0,1,2, 0,1,3

//layout(location = 2) in mat4 instanceTransform;
//layout(location = 6) in vec2 tileUVOffset;

layout(location = 0) out ivec2 tileUV;

struct PointLight{
	vec4 position; //ignore w
	vec4 color; //w is intensity
};

layout(set = 0, binding = 0) uniform GlobalUbo{
	mat4 projection;
	mat4 view;
	vec4 cameraPos;
} ubo;

layout(push_constant) uniform Push{
	mat4 transform;
	ivec2 mapDims;
} push;



void main(){
	vec4 positionWorld = push.transform * vec4(position, 1.0);
	gl_Position = ubo.projection * ubo.view * positionWorld;
	
	tileUV = uv;
	
//	tileUV = uv + tileUVOffset;
}