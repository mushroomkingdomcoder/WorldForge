#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "Win32Includes.h"
#include "BaseException.h"
#include "Shaders.h"
#include "Color.h"
#include <optional>
#include <vector>
#include <functional>

template <typename type>
class Rect;
using fRect = Rect<float>;
using iRect = Rect<int>;
using uRect = Rect<int>;

class Graphics
{
	friend class Window;
public:
	class Exception : public BaseException
	{
	private:
		std::optional<HRESULT> hr;
	private:
		std::string GetErrorCodeString() const noexcept
		{
			if (!hr)
			{
				return "N/A";
			}
			else
			{
				std::ostringstream err;
				err << *hr;
				return err.str();
			}
		}
		std::string GetDescriptionString() const noexcept
		{
			if (!hr)
			{
				return "N/A";
			}
			else
			{
				return GetErrorCodeDesc(*hr);
			}
		}
	public:
		Exception() = delete;
		Exception(int line, std::string file, std::string note) noexcept
			:
			BaseException(line, file, note),
			hr()
		{}
		Exception(int line, std::string file, HRESULT hr, std::string note = "") noexcept
			:
			BaseException(line, file, note),
			hr(hr)
		{}
		const char* what() const noexcept override
		{
			std::ostringstream wht;
			wht << "Exception Type: " << GetType() << std::endl
				<< "Error Code: " << GetErrorCodeString() << std::endl
				<< "Description: " << GetDescriptionString() << std::endl
				<< "File Name: " << GetFile() << std::endl
				<< "Line Number: " << GetLine() << std::endl
				<< "Additional Info: " << GetNote() << std::endl;
			whatBuffer = wht.str();
			return whatBuffer.c_str();
		}
		const char* GetType() const noexcept override
		{
			return "FantasyForge Direct3D 11 Exception";
		}
	};
	#define GFXEXCPT(hr_or_note) Graphics::Exception(__LINE__, __FILE__, hr_or_note)
	#define GFXEXCPT_NOTE(hr, note) Graphics::Exception(__LINE__, __FILE__, hr, note)
	#define GFXCHECK(hr) if (FAILED(hr)) { throw GFXEXCPT(hr); }
private:
	struct Layer
	{
		friend class Graphics;
	private:
		bool isAutoManaged;
		bool renderFlag;
		const int width;
		const int height;
		const int nPixels;
		const int nImageBytes;
		const int nImagePitchBytes;
		std::vector<Color> pixelMap;
		D3D11_VIEWPORT viewport;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPShader;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVShader;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pPixelMap;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pPixelMapView;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pPSCBUF;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVSCBUF;
	private:
		vec2 position;
		float rotation;
		vec2 scale;
	public:
		Layer() = delete;
		Layer(int width, int height)
			:
			isAutoManaged(true),
			renderFlag(true),
			width(width),
			height(height),
			nPixels(width* height),
			nImageBytes(nPixels * sizeof(Color)),
			nImagePitchBytes(width * sizeof(Color)),
			position(0.0f, 0.0f),
			rotation(0.0f),
			scale(1.0f, 1.0f),
			pixelMap(),
			viewport()
		{
			pixelMap.resize(nPixels, Colors::Transparent);
		}
	};
private:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pPipeline = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pFrameManager = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pFrameBufferView = nullptr;
	mutable D3D11_MAPPED_SUBRESOURCE msr = {};
	std::vector<Layer> Layers;
	float4 fBackgroundColorRGBA = { 0.0f,0.0f,0.0f,1.0f };
private:
	void UpdateViewportsAndFrameManager(float win_x_scale, float win_y_scale);
public:
	Graphics() = delete;
	Graphics(const Graphics& gfx) = delete;
	Graphics operator =(const Graphics& gfx) = delete;
	Graphics(HWND hWnd, int WindowWidth, int WindowHeight, std::vector<int2> display_layer_dims);
	void NewFrame();
	void EndFrame() const;
	const bool& isAutoManaged(int layer = 0) const;
	void AutoManage(int layer = 0);
	void ManuallyManage(int layer = 0);
	void Erase(int layer = 0);
	const bool& isBeingRendered(int layer = 0) const;
	void StartRendering(int layer = 0);
	void StopRendering(int layer = 0);
	void EnableBilinearFiltering(int layer = 0);
	void DisableBilinearFiltering(int layer = 0);
	int GetLayerCount() const;
	void SetPixelShader(const Shader& shader, int layer = 0);
	void SetVertexShader(const Shader& shader, int layer = 0);
	template <typename cbuffer>
	void CreatePSConstantBuffer(const cbuffer& cbuf, int layer = 0)
	{
		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = sizeof(cbuffer);
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = &cbuf;
		GFXCHECK(pDevice->CreateBuffer(&bd, &sd, &Layers[layer].pPSCBUF));
	}
	template <typename cbuffer>
	void UpdatePSConstantBuffer(const cbuffer& cbuf, int layer = 0) const
	{
		GFXCHECK(pPipeline->Map(Layers[layer].pPSCBUF.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr));
		memcpy(msr.pData, &cbuf, sizeof(cbuffer));
		pPipeline->Unmap(Layers[layer].pPSCBUF.Get(), 0);
	}
	template <typename cbuffer>
	void CreateVSConstantBuffer(const cbuffer& cbuf, int layer = 0)
	{
		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = sizeof(cbuffer);
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = &cbuf;
		GFXCHECK(pDevice->CreateBuffer(&bd, &sd, &Layers[layer].pVSCBUF));
	}
	template <typename cbuffer>
	void UpdateVSConstantBuffer(const cbuffer& cbuf, int layer = 0) const
	{
		GFXCHECK(pPipeline->Map(Layers[layer].pVSCBUF.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr));
		memcpy(msr.pData, &cbuf, sizeof(cbuffer));
		pPipeline->Unmap(Layers[layer].pVSCBUF.Get(), 0);
	}
	void SetPixel(int x, int y, Color color, int layer = 0);
	const Color& GetPixel(int x, int y, int layer = 0) const;
	void DrawLine(vec2i p0, vec2i p1, const Color& color, int layer = 0);
	void DrawLine(vec2i p0, vec2i p1, std::function<Color(int, int)> color_func, int layer = 0);
	void DrawCircle(int x, int y, int r, const Color& color, int layer = 0);
	void DrawCircle(int x, int y, int r, std::function<Color(int, int)> color_func, int layer = 0);
	void SetFullscreen();
	void ExitFullscreen();
	bool isFullscreen();
	const int& GetWidth(int layer = 0) const;
	const int& GetHeight(int layer = 0) const;
	int GetViewWidth(int layer = 0) const;
	int GetViewHeight(int layer = 0) const;
	vec2i GetDimensions(int layer = 0) const;
	vec2i GetViewDimensions(int layer = 0) const;
	iRect GetRect(int layer = 0) const;
	iRect GetViewRect(int layer = 0) const;
	float GetWidth_FLOAT(int layer = 0) const;
	float GetHeight_FLOAT(int layer = 0) const;
	const float& GetViewWidth_FLOAT(int layer = 0) const;
	const float& GetViewHeight_FLOAT(int layer = 0) const;
	vec2 GetDimensions_FLOAT(int layer = 0) const;
	vec2 GetViewDimensions_FLOAT(int layer = 0) const;
	fRect GetRect_FLOAT(int layer = 0) const;
	fRect GetViewRect_FLOAT(int layer = 0) const;
	float GetAspectRatio(int layer = 0) const;
	float GetViewAspectRatio(int layer = 0) const;
	float GetInvAspectRatio(int layer = 0) const;
	float GetInvViewAspectRatio(int layer = 0) const;
	const int& GetPixelCount(int layer = 0) const;
	const int& GetSizeInBytes(int layer = 0) const;
	const std::vector<Color>& GetPixelMap(int layer = 0) const;
	std::vector<Color>& GetPixelMap(int layer = 0);
	void SetViewport(int x, int y, int width, int height, int layer = 0);
	void SetBackgroundColor(const Color& color);
	Color GetBackgroundColor() const;
	void Move(vec2 delta, int layer = 0);
	void SetPosition(vec2 pos, int layer = 0);
	vec2 GetPosition(int layer = 0) const;
	void Rotate(float radians, int layer = 0);
	void SetRotation(float radians, int layer = 0);
	const float& GetRotation(int layer = 0) const;
	void Scale(vec2 scalar, int layer = 0);
	void SetScale(vec2 scale, int layer = 0);
	const vec2& GetScale(int layer = 0) const;
	mat4 GetTransformationMatrix(int layer = 0) const;
	mat4 GetPreTransformMatrix(int layer = 0) const;
	mat4 GetPostTransformMatrix(int layer = 0) const;
	mat4 GetAspectCorrectionMatrix(int layer = 0) const;
	mat3 GetWorldToPixelMapTransformMatrix(int layer = 0) const;
	mat3 GetPixelMapToWorldTransformMatrix(int layer = 0) const;
};