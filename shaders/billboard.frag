#version 450

layout(location = 0) in vec2 fragOffset;
layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 0) uniform sampler2D texSampler;

void main(){
	//float distance = sqrt(dot(fragOffset, fragOffset));
	
	outColor = texture(texSampler, fragOffset);
}