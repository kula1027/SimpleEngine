struct DirectionalLight {
	vec4 direction;					// 0, 16
	vec4 color;						// 16, 32
	mat4 lightVP;					// 32, 96
};

struct PointLight {
	float intensity;			// 0, 4
	vec4 position;				// 16, 32
	vec4 color;					// 32, 48
	vec4 attenK_c_l_q_range;			// 48, 64		//attenuation func Kc, Kl, Kq, Range
};

layout(std140) uniform LightData{
	int lightCountDirectional;				// 0, 4
	int lightCountPoint;					// 4, 8
	vec4 ambient;							// 16, 32
	DirectionalLight directionalLight[16];	// 32, 1568		// 16 * 96 = 1536
	PointLight pointLight[512];				// 1568, 34336	// 512 * 64 = 32768
};