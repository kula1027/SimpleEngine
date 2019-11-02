#pragma once


#define SizeStructDirectionalLight	96
#define SizeStructPointLight		64

#define MaxCountDirectionalLight 16
#define MaxCountPointLight 512

#define StartAddrDirectional 32
#define StartAddrPoint (StartAddrDirectional + SizeStructDirectionalLight * MaxCountDirectionalLight)

#define SizeStructCameraData 240

#define BindingPointCameraData 0
#define BindingPointLightData 1