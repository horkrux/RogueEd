cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VertexInputType
{
    float4 position : POSITION;
    float4 color : COLOR;
	float4 normal : NORMAL;
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
	float4 normal : NORMAL;
	float2 tex : TEXCOORD0;
};

PixelInputType TerrainLitVertexShader(VertexInputType input)
{
    PixelInputType output;
    

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;
	input.normal.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
	
	output.normal = mul(input.normal, worldMatrix);
	output.normal = input.normal;
    
    // Store the input color for the pixel shader to use.
    output.color = input.color;
	output.tex = input.tex;
    
    return output;
}