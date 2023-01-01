Texture2D Texture;
SamplerState Sampler;
cbuffer cbuf
{
	float4 chromakey;
	float4 silhouette;
	float4 brightness;
};

float4 Process(float2 tc : TextureCoordinate) : SV_TARGET
{
	float4 color = Texture.Sample(Sampler, tc);
	return 
		(silhouette * (color.x != chromakey.x || color.y != chromakey.y || color.z != chromakey.z) +
		color * (color.x == chromakey.x && color.y == chromakey.y && color.z == chromakey.z)) * brightness;
}



