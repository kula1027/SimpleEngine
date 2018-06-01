#version 420 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in Vertex_Out{
	vec2 uv;
	vec3 position_worldSpace;
	vec3 normal_cameraSpace;
	vec3 viewDirection_cameraSpace;
	vec3 lightDirection_cameraSpace;
	vec4 fragPos_lightSpace;
} gs_in[];

out Geometry_Out{
	vec2 uv;
	vec3 position_worldSpace;
	vec3 normal_cameraSpace;
	vec3 viewDirection_cameraSpace;
	vec3 lightDirection_cameraSpace;
	vec4 fragPos_lightSpace;
} gs_out;

void main() {    
	for(int loop = 0; loop < gl_in.length(); loop++){	
		gs_out.uv = gs_in[loop].uv;
		gs_out.position_worldSpace = gs_in[loop].position_worldSpace;
		gs_out.normal_cameraSpace = gs_in[loop].normal_cameraSpace;
		gs_out.viewDirection_cameraSpace = gs_in[loop].viewDirection_cameraSpace;
		gs_out.lightDirection_cameraSpace = gs_in[loop].lightDirection_cameraSpace;
		gs_out.fragPos_lightSpace = gs_in[loop].fragPos_lightSpace;
		gl_Position = gl_in[loop].gl_Position;
		
		EmitVertex();
	}
    
    EndPrimitive();
	
} 