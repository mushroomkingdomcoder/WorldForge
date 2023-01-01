#pragma once
#include "Vector.h"
#include <wrl.h>
#include <assert.h>
#include <D3Dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <vector>

/*

	Base Shader Class

*/

class Shader
{
private:
	Microsoft::WRL::ComPtr<ID3DBlob> pShader = nullptr;
public:
	Shader() = default;
	Shader(const wchar_t* filename)
	{
		D3DReadFileToBlob(filename, &pShader);
	}
	void LoadShaderFile(const wchar_t* filename)
	{
		D3DReadFileToBlob(filename, &pShader);
	}
	const void* GetByteCode() const
	{
		return pShader->GetBufferPointer();
	}
	int GetByteCodeSize() const
	{
		return (int)pShader->GetBufferSize();
	}
};

/*

	Pixel Shaders

*/

class PSS
{
public:
	static Shader Default;
	static Shader InverseColor;
	static Shader Brightness;
	static Shader Silhouette;
	static Shader Mosaic;
	static Shader CircularWindow;
	static Shader RectWindow;
	static Shader HFlip;
	static Shader VFlip;
	static Shader HVFlip;
	static Shader PointLight;
	static Shader Flashlight;
	static Shader DirectionalLight;
private:
	PSS()
	{
		Default.			LoadShaderFile(	L"DefPixelShader.cso"		);
		InverseColor.		LoadShaderFile(	L"InverseColorPS.cso"		);
		Brightness.			LoadShaderFile(	L"Brightness.cso"			);
		Silhouette.			LoadShaderFile(	L"SilhouettePS.cso"			);
		Mosaic.				LoadShaderFile(	L"MosaicPS.cso"				);
		CircularWindow.		LoadShaderFile(	L"CircularWindowPS.cso"		);
		RectWindow.			LoadShaderFile(	L"RectWindowPS.cso"			);
		HFlip.				LoadShaderFile(	L"HFlipPS.cso"				);
		VFlip.				LoadShaderFile(	L"VFlipPS.cso"				);
		HVFlip.				LoadShaderFile(	L"HVFlipPS.cso"				);
		PointLight.			LoadShaderFile(	L"PointLightPS.cso"			);
		Flashlight.			LoadShaderFile(	L"FlashlightPS.cso"			);
		DirectionalLight.	LoadShaderFile(	L"DirectionalLightPS.cso"	);
	}
public:
	static void Load()
	{
		static PSS pixelShaders;
	}
};
inline Shader PSS::Default;
inline Shader PSS::InverseColor;
inline Shader PSS::Brightness;
inline Shader PSS::Silhouette;
inline Shader PSS::Mosaic;
inline Shader PSS::CircularWindow;
inline Shader PSS::RectWindow;
inline Shader PSS::HFlip;
inline Shader PSS::VFlip;
inline Shader PSS::HVFlip;
inline Shader PSS::PointLight;
inline Shader PSS::Flashlight;
inline Shader PSS::DirectionalLight;

/*

	Vertex Shaders

*/

class VSS
{
public:
	static Shader Default;
	static Shader Transform;
private:
	VSS()
	{
		Default.			LoadShaderFile(	L"DefVertexShader.cso"	);
		Transform.			LoadShaderFile(	L"TransformVS.cso"		);
	}
public:
	static void Load()
	{
		static VSS vertexShaders;
	}
};
inline Shader VSS::Default;
inline Shader VSS::Transform;

/*

	Constant Buffers

*/

namespace CBUFs
{
	namespace PXL
	{
		struct InverseColor
		{
		public:
			float4 brightness = { 1.0f,1.0f,1.0f,1.0f };
		public:
			InverseColor() = default;
			InverseColor(float4 brightness)
				:
				brightness(brightness)
			{}
		};
		struct Brightness
		{
		public:
			float4 brightness;
		public:
			Brightness() = delete;
			Brightness(float4 brightness)
				:
				brightness(brightness)
			{}
		};
		struct Silhouette
		{
		public:
			float4 chromakey;
			float4 silhouette;
			float4 brightness;
		public:
			Silhouette() = delete;
			Silhouette(float4 chromakey, float4 silhouette, float4 brightness = { 1.0f,1.0f,1.0f,1.0f })
				:
				chromakey(chromakey),
				silhouette(silhouette),
				brightness(brightness)
			{}
		};
		struct Mosaic
		{
		public:
			int2 divisions;
		private:
			int2 unused;
		public:
			float4 brightness;
		public:
			Mosaic() = delete;
			Mosaic(int2 divisions, float4 brightness = { 1.0f,1.0f,1.0f,1.0f })
				:
				divisions(divisions),
				unused({ 0,0 }),
				brightness(brightness)
			{}
		};
		struct CircularWindow
		{
		public:
			float1 view_aspect_ratio;
		private:
			float1 window_radius;
			float2 position;
		public:
			float4 background;
			float4 brightness;
		public:
			CircularWindow() = delete;
			CircularWindow(float1 view_aspect_ratio, float gfx_height, float camera_zoom, float window_radius, vec2 pos, float4 background, float4 brightness = { 1.0f,1.0f,1.0f,1.0f })
				:
				view_aspect_ratio(view_aspect_ratio),
				window_radius({ window_radius / gfx_height * 2.0f * camera_zoom }),
				position((pos / gfx_height * 2.0f * camera_zoom).GetVStruct()),
				background(background),
				brightness(brightness)
			{}
			void SetWindowRadius(const float& radius, float gfx_height, float camera_zoom)
			{
				window_radius = { radius / gfx_height * 2.0f * camera_zoom };
			}
			float GetWindowRadius(float gfx_height, float camera_zoom)
			{
				return window_radius.a / camera_zoom / 2.0f * gfx_height;
			}
			void SetWindowPosition(const vec2& pos, float gfx_height, float camera_zoom)
			{
				position = (pos / gfx_height * 2.0f * camera_zoom).GetVStruct();
			}
			vec2 GetWindowPosition(float gfx_height, float camera_zoom)
			{
				return vec2(position) / camera_zoom / 2.0f * gfx_height;
			}
		};
		struct RectWindow
		{
		private:
			float2 rect_dim;
			float2 position;
		public:
			float4 background;
			float4 brightness;
		public:
			RectWindow() = delete;
			RectWindow(float gfx_height, float view_aspect_ratio, float camera_zoom, vec2 rect_dim, vec2 pos, float4 background, float4 brightness = { 1.0f,1.0f,1.0f,1.0f })
				:
				rect_dim((rect_dim / gfx_height * 2.0f * camera_zoom / vec2(view_aspect_ratio, 1.0f)).GetVStruct()),
				position((pos / gfx_height * 2.0f * camera_zoom / vec2(view_aspect_ratio, 1.0f)).GetVStruct()),
				background(background),
				brightness(brightness)
			{}
			void SetRectDimensions(const vec2& dim, float gfx_height, float camera_zoom, float view_aspect_ratio)
			{
				rect_dim = (dim / gfx_height * 2.0f * camera_zoom / vec2(view_aspect_ratio, 1.0f)).GetVStruct();
			}
			vec2 GetRectDimensions(float gfx_height, float camera_zoom, float view_aspect_ratio) const
			{
				return vec2(rect_dim) * vec2(view_aspect_ratio, 1.0f) / camera_zoom / 2.0f * gfx_height;
			}
			void SetRectPosition(const vec2& pos, float gfx_height, float camera_zoom, float view_aspect_ratio)
			{
				position = (pos / gfx_height * 2.0f * camera_zoom / vec2(view_aspect_ratio, 1.0f)).GetVStruct();
			}
			vec2 GetRectPosition(float gfx_height, float camera_zoom, float view_aspect_ratio)
			{
				return vec2(position) * vec2(view_aspect_ratio, 1.0f) / camera_zoom / 2.0f * gfx_height;
			}
		};
		struct HFlip
		{
		public:
			float4 brightness = { 1.0f,1.0f,1.0f,1.0f };
		public:
			HFlip() = default;
			HFlip(float4 brightness)
				:
				brightness(brightness)
			{}
		};
		struct VFlip
		{
		public:
			float4 brightness = { 1.0f,1.0f,1.0f,1.0f };
		public:
			VFlip() = default;
			VFlip(float4 brightness)
				:
				brightness(brightness)
			{}
		};
		struct HVFlip
		{
		public:
			float4 brightness = { 1.0f,1.0f,1.0f,1.0f };
		public:
			HVFlip() = default;
			HVFlip(float4 brightness)
				:
				brightness(brightness)
			{}
		};
		struct PointLight
		{
		public:
			float4 light_diffuse;
			float4 light_ambient;
		private:
			float2 light_pos;
		public:
			float1 quad_att;
			float1 linear_att;
			float1 const_att;
			float1 view_aspect_ratio;
		private:
			int2 unused;
		public:
			float4 master_brightness;
		public:
			PointLight() = delete;
			PointLight(float4 light_diffuse, float4 light_ambient, vec2 light_pos, float gfx_height, float camera_zoom, float1 quad_att, float1 linear_att, float1 const_att, float1 view_aspect_ratio, float4 master_brightness = { 1.0f,1.0f,1.0f,1.0f })
				:
				light_diffuse(light_diffuse),
				light_ambient(light_ambient),
				light_pos((light_pos / gfx_height * 2.0f * camera_zoom).GetVStruct()),
				quad_att(quad_att),
				linear_att(linear_att),
				const_att(const_att),
				view_aspect_ratio(view_aspect_ratio),
				unused({ 0,0 }),
				master_brightness(master_brightness)
			{}
			void SetLightPos(const vec2& pos, float gfx_height, float camera_zoom)
			{
				light_pos = (pos / gfx_height * 2.0f * camera_zoom).GetVStruct();
			}
			vec2 GetLightPos(float gfx_height, float camera_zoom) const
			{
				return vec2(light_pos) / camera_zoom / 2.0f * gfx_height;
			}
		};
		struct Flashlight
		{
		public:
			float4 light_diffuse;
			float4 light_ambient;
		private:
			float2 light_pos;
			float1 theta;
			float1 arc_angle;
		public:
			float1 quad_att;
			float1 linear_att;
			float1 const_att;
			float1 view_aspect_ratio;
			float4 master_brightness;
		public:
			Flashlight() = delete;
			Flashlight(float4 light_diffuse, float4 light_ambient, vec2 light_pos, float gfx_height, float camera_zoom, float angle, float arc, float1 quad_att, float1 linear_att, float1 const_att, float1 view_aspect_ratio, float4 master_brightness = { 1.0f,1.0f,1.0f,1.0f })
				:
				light_diffuse(light_diffuse),
				light_ambient(light_ambient),
				light_pos((light_pos / gfx_height * 2.0f * camera_zoom).GetVStruct()),
				quad_att(quad_att),
				linear_att(linear_att),
				const_att(const_att),
				view_aspect_ratio(view_aspect_ratio),
				master_brightness(master_brightness)
			{
				while (angle > 3.0f * (float)M_PI_2)
				{
					angle -= 2.0f * (float)M_PI;
				}
				while (angle <= (float)-M_PI_2)
				{
					angle += 2.0f * (float)M_PI;
				}
				theta = { angle };
				angle = abs(arc);
				while (arc > (float)M_PI)
				{
					arc -= (float)M_PI;
				}
				arc_angle = { arc };
			}
			void SetFlashlightPos(const vec2& pos, float gfx_height, float camera_zoom)
			{
				light_pos = (pos / gfx_height * 2.0f * camera_zoom).GetVStruct();
			}
			vec2 GetFlashlightPos(float gfx_height, float camera_zoom) const
			{
				return vec2(light_pos) / camera_zoom / 2.0f * gfx_height;
			}
			void SetFlashlightTheta(float angle)
			{
				while (angle > 3.0f * (float)M_PI_2)
				{
					angle -= 2.0f * (float)M_PI;
				}
				while (angle <= (float)-M_PI_2)
				{
					angle += 2.0f * (float)M_PI;
				}
				theta = { angle };
			}
			float GetFlashlightTheta() const
			{
				return theta.a;
			}
			void SetFlashlightArcMeasure(float angle)
			{
				angle = abs(angle);
				while (angle > (float)M_PI)
				{
					angle -= (float)M_PI;
				}
				arc_angle = { angle };
			}
			float GetFlashlightArcMeasure() const
			{
				return arc_angle.a;
			}
		};
		struct DirectionalLight
		{
		public:
			float4 light_diffuse;
			float4 light_ambient;
			float3 light_direction;
		private:
			int1 unused;
		public:
			float4 master_brightness;
		public:
			DirectionalLight() = delete;
			DirectionalLight(float4 light_diffuse, float4 light_ambient, float3 light_direction, float4 master_brightness = { 1.0f,1.0f,1.0f,1.0f })
				:
				light_diffuse(light_diffuse),
				light_ambient(light_ambient),
				light_direction(light_direction),
				unused({ 0 }),
				master_brightness(master_brightness)
			{}
		};
	}
	namespace VTX
	{
		struct Transform
		{
		public:
			mat4 transform;
		public:
			Transform(const mat4& transform)
				:
				transform(transform)
			{}
		};
	}
}

