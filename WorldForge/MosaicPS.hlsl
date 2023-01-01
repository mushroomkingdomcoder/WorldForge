Texture2D Texture;
SamplerState Sampler;
cbuffer cbuf
{
	int2 divisions;
	int2 unused;
	float4 brightness;
};

float4 Process(float2 tc : TextureCoordinate) : SV_TARGET
{
	int2 cell = int2(int(tc.x * float(divisions.x)), int(tc.y * float(divisions.y)));
	float2 newtc = float2(float(cell.x) / float(divisions.x), float(cell.y) / float(divisions.y));
	return Texture.Sample(Sampler, newtc) * brightness;
}