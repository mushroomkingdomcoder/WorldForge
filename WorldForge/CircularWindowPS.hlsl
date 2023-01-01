Texture2D Texture;
SamplerState Sampler;
cbuffer cbuf
{
	float1 view_aspect_ratio;
	float1 window_radius;
	float2 position;
	float4 background;
	float4 brightness;
};

float4 Process(float2 tc : TextureCoordinate, float2 wpos : WorldPosition) : SV_TARGET
{
	wpos.x *= view_aspect_ratio;
	wpos -= position;
	const float dst = sqrt((wpos.x * wpos.x) + (wpos.y * wpos.y));
	return
		(Texture.Sample(Sampler, tc) * (dst < window_radius) +
		background * (dst >= window_radius)) * brightness;
}