#pragma once

#define ShadowMapID 8

#define UBO_SIZE_LIGHT_DIRECTIONAL	96
#define UBO_SIZE_LIGHT_POINT		64

#define MaxCountDirectionalLight 16
#define MaxCountPointLight 512

#define UBO_ADDR_DIRECTIONAL 32
#define UBO_ADDR_POINT (UBO_ADDR_DIRECTIONAL + UBO_SIZE_LIGHT_DIRECTIONAL * MaxCountDirectionalLight)

#define SizeStructCameraData 240

#define BindingPointCameraData 0
#define BindingPointLightData 1