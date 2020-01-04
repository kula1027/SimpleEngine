#version 420 core
//! #include "../utils.glsl"
//! #include "../UniformBlock/ub_camera.glsl"
//! #include "../UniformBlock/ub_light.glsl"

out vec4 FragColor;
  
in vec2 texCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D shadowMap;


float calcShadowMap(vec3 gFragPos_) {
	vec4 projectedEyeDir = directionalLight[0].lightVP * vec4(gFragPos_,1);
    projectedEyeDir = projectedEyeDir/projectedEyeDir.w;
    vec2 textureCoordinates = projectedEyeDir.xy * vec2(0.5) + vec2(0.5);

    const float bias = 0.005;
    float depthValue = texture( shadowMap, textureCoordinates ).r + bias;  
	return min(projectedEyeDir.z * 0.5 + 0.5, 1) <= depthValue ? 1 : 0;
}

void main() {
    vec3 gFragPos = texture(gPosition, texCoords).rgb;
    vec3 gNormal = texture(gNormal, texCoords).rgb;
    vec3 gAlbedo = texture(gAlbedoSpec, texCoords).rgb;
    float gSpecular = texture(gAlbedoSpec, texCoords).a;        
	
	float shadow = calcShadowMap(gFragPos);

    vec3 lighting = texture(gAlbedoSpec, texCoords).rgb * ambient.xyz;
	vec3 viewDir = normalize(cameraPosition.xyz - gFragPos);
	for(int loop = 0; loop < lightCountDirectional; loop++){
		//diffuse
		vec3 lightDir = -directionalLight[loop].direction.xyz;
		vec3 diffuse =  max(dot(gNormal, lightDir), 0.0) * gAlbedo * directionalLight[loop].color.xyz;		   

		// specular		
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(gNormal, halfwayDir), 0.0), 8.0);
		vec3 specular = directionalLight[loop].color.xyz * spec;
		
		lighting += (diffuse + specular) * shadow;
	}

	

	FragColor = vec4(lighting, 1.0);		
	
	//FragColor = testcolor();// vec4(vec3(depthValue), 1.0);
}  