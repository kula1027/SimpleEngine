#version 420 core

//! #include "../UniformBlock/ub_camera.glsl"
//! #include "../UniformBlock/ub_light.glsl"

out vec4 out_color;

in V2F{
	vec2 uv;
	vec3 position_world;	
	vec3 normal_world;				
	vec4 position_light_directional[16];
}v2f;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform sampler2D texture_specular;

uniform sampler2D shadowMap;

float calcShadowMap(int idx) {		
    vec3 projectedEyeDir = v2f.position_light_directional[idx].xyz/v2f.position_light_directional[idx].w;

    vec2 textureCoordinates = projectedEyeDir.xy * 0.5f + 0.5f;

    const float bias = 0.005;
    float depthValue = texture( shadowMap, textureCoordinates ).r + bias;  
	return min(projectedEyeDir.z * 0.5 + 0.5, 1) <= depthValue ? 1 : 0;
}

void main(){				
	vec3 albedo = texture(texture_diffuse, v2f.uv).rgb;

	vec3 lighting = albedo * ambient.xyz;	
	vec3 viewDir = normalize(cameraPosition.xyz - v2f.position_world);

	//Directional Lights
	for(int loop = 0; loop < lightCountDirectional; loop++){
		//diffuse
		vec3 lightDir = -directionalLight[loop].direction.xyz;
		vec3 diffuse =  max(dot(v2f.normal_world, lightDir), 0.0) * albedo * directionalLight[loop].color.xyz;		   

		// specular		
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(v2f.normal_world, halfwayDir), 0.0), 8.0);
		vec3 specular = directionalLight[loop].color.xyz * spec;

		lighting += (diffuse + specular) * calcShadowMap(loop);
	}

	
	//Point Lights
	for(int loop = 0; loop < lightCountPoint; loop++){
		//diffuse
		vec3 lightDir = normalize(pointLight[loop].position.xyz - v2f.position_world);
		vec3 diffuse =  max(dot(v2f.normal_world, lightDir), 0.0) * albedo * pointLight[loop].color.xyz;		   

		// specular		
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(v2f.normal_world, halfwayDir), 0.0), 8.0);
		vec3 specular = pointLight[loop].color.xyz * spec;

		// attenuation	
		float dist = length(pointLight[loop].position.xyz -  v2f.position_world) * (100 / pointLight[loop].attenK_c_l_q_range.w);    
		float attenuation = 1.0 / 
			(pointLight[loop].attenK_c_l_q_range.x +
			pointLight[loop].attenK_c_l_q_range.y * dist + 
			pointLight[loop].attenK_c_l_q_range.z * dist * dist);

		lighting += (diffuse + specular) * attenuation;
	}
	        
	out_color = vec4(lighting, 1);	
}