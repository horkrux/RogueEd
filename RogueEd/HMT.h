#ifndef _HMT_H_
#define _HMT_H_

#include <d3d11_1.h>
#include <vector>
#include <directxmath.h>

using namespace std;

class HMT
{
public:
	HMT();
	HMT(const HMT&);
	~HMT();

	int initialize(ID3D11Device1*, ID3D11DeviceContext1*, vector<char>);
	ID3D11ShaderResourceView** getShaderResViews();
	bool materialHasTex(int);
	int getNumMaterials();
	int getNumTextures();
	int* getMat2TexIndices();

private:

	typedef struct
	{
		char matName[16];
		char* paletteData;
		char* pixelData;
	} TextureData;

	typedef struct
	{
		char flags[4];
		float RGBAtransparentColor;
	} TextureFormat;

	typedef struct 
	{
		uint32_t texOffset;
		char unknown0[28];
		uint32_t paletteOffset;
		uint32_t nameOffset;
		uint16_t width;
		uint16_t height;
		TextureFormat format;
	} Texture;

	typedef struct
	{
		uint16_t matType;
		uint16_t texIdx;
		float unknown0[4];
		char name[16];
	} Material;

	typedef struct
	{
		uint16_t width;
		uint16_t height;
		TextureFormat format;
		uint32_t paletteDataSize;
		uint32_t pixelDataSize;
		BYTE* paletteData;
		BYTE* pixelData;
	} TextureNew;

	typedef struct
	{
		uint16_t matType;
		uint16_t texIdx;
		TextureNew tex;
		ID3D11Texture2D* m_texture;
		ID3D11ShaderResourceView* m_textureView;
	} MaterialNew;

	int m_numMaterials;
	int m_numTextures;
	ID3D11Texture2D** m_textures;
	ID3D11ShaderResourceView** m_textureViews;
	int* m_mat2TexIndices;
	MaterialNew* m_materials;
	vector<char> m_fileContents;

private:

	int read();
	int initializeShaderResViews(ID3D11Device1*, ID3D11DeviceContext1*);

};

#endif