#version 420 core

out vec4 FragColor;
  
struct DirectionalLight{
	vec4 direction;					// 0, 16
	vec4 color;						// 16, 32
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
	DirectionalLight directionalLight[16];	// 32, 544		// 16 * 32 = 512
	PointLight pointLight[512];				// 544,	33312	// 512 * 64 = 32768
};


layout (std140) uniform CameraData{	
	mat4 V;					// 0, 64
	mat4 P;					// 64, 128
	mat4 VP;				// 128, 192
	vec4 cameraPosition;	// 192, 208
	vec4 cameraDirection;	// 208, 224
};



uniform int lightIdx;
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;


void main() {                 
	vec2 texCoords = vec2(gl_FragCoord.x / 1536, gl_FragCoord.y/ 864);

    vec3 gFragPos = texture(gPosition, texCoords).rgb;
    vec3 gNormal = texture(gNormal, texCoords).rgb;
    vec3 gAlbedo = texture(gAlbedoSpec, texCoords).rgb;
    float gSpecular = texture(gAlbedoSpec, texCoords).a;
        
   // diffuse
    vec3 lightDir = normalize(pointLight[lightIdx].position.xyz - gFragPos );
    vec3 diffuse = max(dot(gNormal, lightDir), 0) * pointLight[lightIdx].color.xyz;

    // specular
    vec3 halfwayDir = normalize(lightDir + cameraDirection.xyz);  
    float spec = pow(max(dot(gNormal, halfwayDir), 0.0),32.0);
    vec3 specular = pointLight[lightIdx].color.xyz * spec;//* gSpecular

	float dist = length(pointLight[lightIdx].position.xyz - gFragPos) * (100 / pointLight[lightIdx].attenK_c_l_q_range.w);
    // attenuation	
    float attenuation = 1.0 / 
		(pointLight[lightIdx].attenK_c_l_q_range.x +
		pointLight[lightIdx].attenK_c_l_q_range.y * dist * + 
		pointLight[lightIdx].attenK_c_l_q_range.z * dist * dist);

	FragColor = vec4( (diffuse + specular) * attenuation, 1.0);
	//FragColor = vec4(pointLight[lightIdx].color.xyz, 1);
}  
