#version 450

layout (location = 0) in vec2 gridCoord;

layout (location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform sampler2D texSampler;

layout(push_constant) uniform Push {
  vec4 scaleOffset;
  vec3 color;
} push;

void main() {
	outColor = vec4(push.color, 1.0);
	bool gridXNeg = gridCoord.x < 0.0;
	bool gridYNeg = gridCoord.y < 0.0;
	vec2 tiled = gridCoord;
	tiled.x = tiled.x * (1.0 - (2.0 * float(gridXNeg)));
	tiled.y = tiled.y * (1.0 - (2.0 * float(gridYNeg)));
	
	vec2 fractaled = fract(tiled * 10.0);
	fractaled.x -= 0.5;
	fractaled.y -= 0.5;
	
	//outColor = vec4(push.color * texture(texSampler, fragTexCoord).rgb, texture(texSampler, fragTexCoord).a);
	//float curve = 0.00000000000001 / (fractaled.y * fractaled.y * fractaled.y * fractaled.y * fractaled.x * fractaled.x * fractaled.x * fractaled.x);
	float curve =  1 / (100000000 * fractaled.y * fractaled.y * fractaled.x * fractaled.x);
	fractaled = fractaled / 2;
	
	float fractaledLength = sqrt(fractaled.x * fractaled.x + fractaled.y * fractaled.y);
	outColor *= float(fractaledLength < curve) * float(fractaledLength < 0.0625);
}