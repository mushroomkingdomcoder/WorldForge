Texture2D Texture;
SamplerState Sampler;
cbuffer cbuf
{
	float2 rect_dim;
	float2 position;
	float4 background;
	float4 brightness;
};

float4 Process(float2 tc : TextureCoordinate, float2 wpos : WorldPosition) : SV_TARGET
{
	wpos -= position;
	const float hw = rect_dim.x / 2.0f;
	const float hh = rect_dim.y / 2.0f;
	return
		(Texture.Sample(Sampler, tc) * (wpos.x >= -hw && wpos.x < hw&& wpos.y >= -hh && wpos.y < hh) +
		background * (wpos.x < -hw || wpos.x >= hw || wpos.y < -hh || wpos.y >= hh)) * brightness;
}

