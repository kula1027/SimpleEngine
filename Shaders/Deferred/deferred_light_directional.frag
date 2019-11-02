#version 420 core

out vec4 FragColor;

struct DirectionalLight{
	vec4 direction;					// 0, 16
	vec4 color;						// 16, 32
	mat4 lightVP;					// 32, 96
};

struct PointLight{
	float intensity;			// 0, 4
	vec4 position;				// 16, 32
	vec4 color;					// 32, 48
	vec4 attenK_c_l_q_range;			// 48, 64		//attenuation func Kc, Kl, Kq, Range
};

layout (std140) uniform LightData{
	int lightCountDirectional;				// 0, 4
	int lightCountPoint;					// 4, 8
	vec4 ambient;							// 16, 32
	DirectionalLight directionalLight[16];	// 32, 1568		// 16 * 96 = 1536
	PointLight pointLight[512];				// 1568, 34336	// 512 * 64 = 32768
};

layout (std140) uniform CameraData{	
	mat4 V;					// 0, 64
	mat4 P;					// 64, 128
	mat4 VP;				// 128, 192
	vec4 cameraPosition;	// 192, 208
	vec4 cameraDirection;	// 208, 224
	vec4 screenWidthHeight; // 224, 240
};
  
in vec2 texCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D shadowMap;


float readShadowMap(vec3 viewDir_) {
    mat4 cameraViewToWorldMatrix = inverse(VP);
    mat4 cameraViewToProjectedLightSpace = directionalLight[0].lightVP * cameraViewToWorldMatrix;
	vec4 projectedEyeDir =  directionalLight[0].lightVP * inverse(VP) * vec4(-viewDir_,1);//cameraViewToProjectedLightSpace * vec4(-viewDir_,1);
    projectedEyeDir = projectedEyeDir/projectedEyeDir.w;

    vec2 textureCoordinates = projectedEyeDir.xy * vec2(0.5) + vec2(0.5);

    const float bias = 0.0001;
    float depthValue = texture( shadowMap, textureCoordinates ).r - bias;

    return projectedEyeDir.z * 0.5 + 0.5 < depthValue ? 1 : 0;
}

void main() {
    vec3 gFragPos = texture(gPosition, texCoords).rgb;
    vec3 gNormal = texture(gNormal, texCoords).rgb;
    vec3 gAlbedo = texture(gAlbedoSpec, texCoords).rgb;
    float gSpecular = texture(gAlbedoSpec, texCoords).a;        
	
    vec3 lighting = texture(gAlbedoSpec, texCoords).rgb * ambient.xyz;
	vec3 viewDir = normalize(cameraPosition.xyz - gFragPos);
	for(int loop = 0; loop < lightCountDirectional; loop++){
		//diffuse
		vec3 lightDir = -directionalLight[loop].direction.xyz;
		vec3 diffuse =  max(dot(gNormal, lightDir), 0.0) * gAlbedo * directionalLight[loop].color.xyz;		   

		// specular		
		vec3 halfwayDir = normalize(lightDir + viewDir);          		
		float spec = pow(max(dot(gNormal, halfwayDir), 0.0), 32.0);
		vec3 specular = directionalLight[loop].color.xyz * spec;//* gSpecular

		lighting += (diffuse + specular) * readShadowMap(viewDir);
	}
	

	FragColor = vec4(lighting, 1.0);	
	//FragColor = vec4(normal, 1);
}  