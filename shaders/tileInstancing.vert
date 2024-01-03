#version 450

//model
layout(location = 0) in vec2 uv;

//instancing
layout(location = 1) in vec2 tileUVOffset;


layout(location = 0) out vec2 tileUV;
//layout(location = 1) out vec3 fragColor;

struct PointLight{
	vec4 position; //ignore w
	vec4 color; //w is intensity
};

layout(set = 0, binding = 0) uniform GlobalUbo{
	mat4 projection;
	mat4 view;
	vec4 cameraPos;
} ubo;


layout(set = 1, binding = 0) readonly buffer tileVertices {
	vec4 tileVertex[]; //its already in world space 
	//this defines a max grid of 127x127 tiles
};
layout(set = 1, binding = 1) readonly buffer tileIndices {
	int tileIndex[]; //128 x 128 x 6
	//
};
layout(push_constant) uniform Push{
	vec2 uvScaling;
} push;

//if this doesnt work, use a storage buffer for UVs, and the vertex indices to check out UV

void main(){
	gl_Position = ubo.projection * ubo.view * tileVertex[tileIndex[gl_InstanceIndex * 6 + gl_VertexIndex]];
	
	tileUV = uv + tileUVOffset;
	
	//fragColor = INDEX_COLORS[gl_VertexIndex];
}