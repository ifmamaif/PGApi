#include "pch.h"
#include "Utils.h"

bool OutSidei(int x1, int y1, int x2, int y2)
{
    return x1 >= x2
        || x1 < -x2
        || y1 >= y2
        || y1 < -y2;
}

bool OutSidef(float x1, float y1, float x2, float y2)
{
    return x1 >= x2
        || x1 < -x2
        || y1 >= y2
        || y1 < -y2;
}

//#define ONE 0.00390625
//#define ONEHALF 0.001953125
//
//float noise(float3 P)
//{
//	float3 Pi = ONE * floor(P) + ONEHALF;
//
//	float3 Pf = P - floor(P);
//
//	// Noise contributions from (x=0, y=0), z=0 and z=1
//	float perm00 = tex2D(permTexture, Pi.xy).a;
//	float3  grad000 = tex2D(permTexture, float2(perm00, Pi.z)).rgb * 4.0 - 1.0;
//	float n000 = dot(grad000, Pf);
//	float3  grad001 = tex2D(permTexture, float2(perm00, Pi.z + ONE)).rgb * 4.0 - 1.0;
//	float n001 = dot(grad001, Pf - float3(0.0, 0.0, 1.0));
//
//	// Noise contributions from (x=0, y=1), z=0 and z=1
//	float perm01 = tex2D(permTexture, Pi.xy + float2(0.0, ONE)).a;
//	float3  grad010 = tex2D(permTexture, float2(perm01, Pi.z)).rgb * 4.0 - 1.0;
//	float n010 = dot(grad010, Pf - float3(0.0, 1.0, 0.0));
//	float3  grad011 = tex2D(permTexture, float2(perm01, Pi.z + ONE)).rgb * 4.0 - 1.0;
//	float n011 = dot(grad011, Pf - float3(0.0, 1.0, 1.0));
//
//	// Noise contributions from (x=1, y=0), z=0 and z=1
//	float perm10 = tex2D(permTexture, Pi.xy + float2(ONE, 0.0)).a;
//	float3  grad100 = tex2D(permTexture, float2(perm10, Pi.z)).rgb * 4.0 - 1.0;
//	float n100 = dot(grad100, Pf - float3(1.0, 0.0, 0.0));
//	float3  grad101 = tex2D(permTexture, float2(perm10, Pi.z + ONE)).rgb * 4.0 - 1.0;
//	float n101 = dot(grad101, Pf - float3(1.0, 0.0, 1.0));
//
//	// Noise contributions from (x=1, y=1), z=0 and z=1
//	float perm11 = tex2D(permTexture, Pi.xy + float2(ONE, ONE)).a;
//	float3  grad110 = tex2D(permTexture, float2(perm11, Pi.z)).rgb * 4.0 - 1.0;
//	float n110 = dot(grad110, Pf - float3(1.0, 1.0, 0.0));
//	float3  grad111 = tex2D(permTexture, float2(perm11, Pi.z + ONE)).rgb * 4.0 - 1.0;
//	float n111 = dot(grad111, Pf - float3(1.0, 1.0, 1.0));
//
//	// Blend contributions along x
//	float4 n_x = lerp(float4(n000, n001, n010, n011), float4(n100, n101, n110, n111), fade(Pf.x));
//
//	// Blend contributions along y
//	float2 n_xy = lerp(n_x.xy, n_x.zw, fade(Pf.y));
//
//	// Blend contributions along z
//	float n_xyz = lerp(n_xy.x, n_xy.y, fade(Pf.z));
//
//	return n_xyz;
//}