#version 450

const vec2 OFFSETS[6] = vec2[](
	vec2(-1.0, -1.0),
	vec2(-1.0, 1.0),
	vec2(1.0, -1.0),
	vec2(1.0, -1.0),
	vec2(-1.0, 1.0),
	vec2(1.0, 1.0)
);
const vec2 GRID[6] = vec2[](
	vec2(-0.5, -0.5),
	vec2(-0.5, 0.5),
	vec2(0.5, -0.5),
	vec2(0.5, -0.5),
	vec2(-0.5, 0.5),
	vec2(0.5, 0.5)
);

layout(location = 0) out vec2 gridCoordinate;

layout(push_constant) uniform Push {
  vec4 scaleOffset;
  vec3 color;
} push;

void main() {
	gl_Position = vec4(OFFSETS[gl_VertexIndex], 0.0, 1.0);
	//gl_Position = vec4(push.scaleOffset.xy * position + push.scaleOffset.zw, 0.0, 1.0);
  
	gridCoordinate = (GRID[gl_VertexIndex] + push.scaleOffset.zw) * push.scaleOffset.xy;
}