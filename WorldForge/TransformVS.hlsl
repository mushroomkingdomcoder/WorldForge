cbuffer cbuf
{
	float4x4 transform;
};

struct Vertex
{
	float2 tc : TextureCoordinate;
	float2 wpos : WorldPosition;
	float4 pos : SV_POSITION;
};

Vertex Process(float2 pos : Position, float2 tc : TextureCoordinate)
{
	Vertex vtx;
	vtx.tc = tc;
	vtx.pos = mul(transform, float4(pos, 0.0f, 1.0f));
	vtx.wpos.x = vtx.pos.x; vtx.wpos.y = vtx.pos.y;
	return vtx;
}