#version 420 core

//! #include "../UniformBlock/ub_camera.glsl"
//! #include "../UniformBlock/ub_light.glsl"

out vec4 FragColor;

uniform int lightIdx;
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;


void main() {         	
	vec2 texCoords = vec2(gl_FragCoord.x / screenWidthHeight.x, gl_FragCoord.y / screenWidthHeight.y);

    vec3 gFragPos = texture(gPosition, texCoords).rgb;
    vec3 gNormal = texture(gNormal, texCoords).rgb;
    vec3 gAlbedo = texture(gAlbedoSpec, texCoords).rgb;
    float gSpecular = texture(gAlbedoSpec, texCoords).a;
        
   // diffuse
    vec3 lightDir = normalize(pointLight[lightIdx].position.xyz - gFragPos );
    vec3 diffuse = max(dot(gNormal, lightDir), 0) * pointLight[lightIdx].color.xyz;

    // specular
	vec3 viewDir = normalize(cameraPosition.xyz - gFragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(gNormal, halfwayDir), 0.0),8.0);
    vec3 specular = pointLight[lightIdx].color.xyz * spec;//* gSpecular

	// attenuation	
	float dist = length(pointLight[lightIdx].position.xyz - gFragPos) * (100 / pointLight[lightIdx].attenK_c_l_q_range.w);    
    float attenuation = 1.0 / 
		(pointLight[lightIdx].attenK_c_l_q_range.x +
		pointLight[lightIdx].attenK_c_l_q_range.y * dist + 
		pointLight[lightIdx].attenK_c_l_q_range.z * dist * dist);

	FragColor = vec4( (diffuse + specular) * attenuation, 1.0);	
}  
