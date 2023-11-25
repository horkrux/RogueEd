#pragma once

#include <fstream>

#include <d3d11_1.h>
#include <directxmath.h>
using namespace DirectX;
using namespace std;

class Font
{
public:
	struct Glyph
	{
		int symbol;
		RECT position;
		float xOffset;
		float yOffset;
		float advance;
	};

private:
	

	struct FontType
	{
		float left, right;
		int size;
	};

	struct VertexType
	{
		XMVECTOR position;
		XMFLOAT2 texture;
	};

public:
	Font();
	Font(const Font&);
	~Font();

	bool Initialize(ID3D11Device1*, ID3D11DeviceContext1*, string, const WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	void BuildVertexArray(void*, const char*, float, float);

private:
	bool LoadFontData(const char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device1*, const WCHAR*);
	void ReleaseTexture();
	int ReadDXTKFont(string, ID3D11Device1*, ID3D11DeviceContext1*);

private:
	Glyph* m_glyphs;
	int m_texWidth;
	int m_texHeight;
	FontType* m_Font;
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView;
};