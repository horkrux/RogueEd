#include "HMT.h"

HMT::HMT()
{
	m_numMaterials = 0;
	m_numTextures = 0;
}

HMT::HMT(const HMT& other) {}

HMT::~HMT() 
{
	for (int i = 0; i < m_numMaterials; i++)
	{
		if (m_materials[i].tex.paletteDataSize)
			delete[] m_materials[i].tex.paletteData;

		if (m_materials[i].tex.pixelDataSize)
			delete[] m_materials[i].tex.pixelData;

		
	}

	for (int i = 0; i < m_numTextures; i++)
	{
		if (m_textures[i]) 
		{
			int FUCK = m_textures[i]->Release();
			m_textures[i] = 0;
		}

		if (m_textureViews[i])
		{
			int shit = m_textureViews[i]->Release();
			m_textureViews[i] = 0;
		}
	}

	if (m_numMaterials)
		delete[] m_materials;

	if (m_textureViews)
		delete[] m_textureViews;
	
	if (m_textures)
		delete[] m_textures;

	vector<char>().swap(m_fileContents);
}

int HMT::initialize(ID3D11Device1* device, ID3D11DeviceContext1* deviceContext, vector<char> fileContents)
{
	m_fileContents = fileContents;
	read();
	initializeShaderResViews(device, deviceContext);

	return 0;
}

ID3D11ShaderResourceView** HMT::getShaderResViews()
{
	return m_textureViews;
}

int HMT::getNumMaterials()
{
	return m_numMaterials;
}

int HMT::getNumTextures()
{
	return m_numTextures;
}

int* HMT::getMat2TexIndices()
{
	return m_mat2TexIndices;
}

bool HMT::materialHasTex(int materialIndex)
{
	if (materialIndex < m_numMaterials && m_materials[materialIndex].matType == 1)
		return true;
	return false;
}

int HMT::read()
{
	char* contents = m_fileContents.data();
	m_numMaterials = *(int*)contents;
	m_materials = new MaterialNew[m_numMaterials];

	m_numTextures = *(int*)(contents + *(int*)(contents + 4));
	char* texOffset = contents + *(int*)(contents + 4) + 4;
	char* currTexOffset;
	char* currTexDataOffset;
	char* currPaletteDataOffset = 0;
	int texIdx = -1;

	for (int i = 0; i < m_numMaterials; i++)
	{
		m_materials[i].matType = *(uint16_t*)(contents + 8 + i * 36);
		m_materials[i].texIdx = *(uint16_t*)(contents + 8 + i * 36 + 2);
		m_materials[i].tex.paletteDataSize = 0;
		m_materials[i].tex.pixelDataSize = 0;

		if (m_materials[i].matType == 1)
		{
			if (m_materials[i].texIdx < m_numTextures && m_materials[i].texIdx > texIdx)
			{
				currPaletteDataOffset = 0;
				currTexOffset = texOffset + m_materials[i].texIdx * 52;
				currTexDataOffset = contents + *(int*)currTexOffset;

				if (*(int*)(currTexOffset + 32))
					currPaletteDataOffset = contents + *(int*)(currTexOffset + 32);

				m_materials[i].tex.width = *(uint16_t*)(currTexOffset + 40);
				m_materials[i].tex.height = *(uint16_t*)(currTexOffset + 42);
				m_materials[i].tex.format.flags[0] = *(currTexOffset + 44);
				m_materials[i].tex.format.flags[1] = *(currTexOffset + 45);
				m_materials[i].tex.format.flags[2] = *(currTexOffset + 46);
				m_materials[i].tex.format.flags[3] = *(currTexOffset + 47);
				m_materials[i].tex.format.RGBAtransparentColor = *(float*)(currTexOffset + 48);

				if (currPaletteDataOffset)
				{
					if (m_materials[i].tex.format.flags[2] == 0)
					{
						m_materials[i].tex.paletteDataSize = 48;
						m_materials[i].tex.paletteData = new BYTE[48];
						memcpy(m_materials[i].tex.paletteData, currPaletteDataOffset, 48);
						m_materials[i].tex.pixelDataSize = (m_materials[i].tex.width * m_materials[i].tex.height) / 2;
						m_materials[i].tex.pixelData = new BYTE[m_materials[i].tex.pixelDataSize];
						memcpy(m_materials[i].tex.pixelData, currTexDataOffset, m_materials[i].tex.pixelDataSize);
					}
					else if (m_materials[i].tex.format.flags[2] == 1)
					{
						m_materials[i].tex.paletteDataSize = 768;
						m_materials[i].tex.paletteData = new BYTE[768];
						memcpy(m_materials[i].tex.paletteData, currPaletteDataOffset, 768);
						m_materials[i].tex.pixelDataSize = m_materials[i].tex.width * m_materials[i].tex.height;
						m_materials[i].tex.pixelData = new BYTE[m_materials[i].tex.pixelDataSize];
						memcpy(m_materials[i].tex.pixelData, currTexDataOffset, m_materials[i].tex.pixelDataSize);
					}
				}
				else {
					if (m_materials[i].tex.format.flags[2] == 2)
					{
						m_materials[i].tex.paletteDataSize = 0;
						m_materials[i].tex.pixelDataSize = 2 * m_materials[i].tex.width * m_materials[i].tex.height;
						m_materials[i].tex.pixelData = new BYTE[m_materials[i].tex.pixelDataSize];
						memcpy(m_materials[i].tex.pixelData, currTexDataOffset, m_materials[i].tex.pixelDataSize);
					}
					else if (m_materials[i].tex.format.flags[2] == 3)
					{
						m_materials[i].tex.paletteDataSize = 0;

						if (m_materials[i].tex.format.flags[3] & 0x80) //has alpha
						{
							m_materials[i].tex.pixelDataSize = 4 * m_materials[i].tex.width * m_materials[i].tex.height;
							m_materials[i].tex.pixelData = new BYTE[m_materials[i].tex.pixelDataSize];
							memcpy(m_materials[i].tex.pixelData, currTexDataOffset, m_materials[i].tex.pixelDataSize);
						}
						else 
						{
							m_materials[i].tex.pixelDataSize = 3 * m_materials[i].tex.width * m_materials[i].tex.height;
							m_materials[i].tex.pixelData = new BYTE[m_materials[i].tex.pixelDataSize];
							memcpy(m_materials[i].tex.pixelData, currTexDataOffset, m_materials[i].tex.pixelDataSize);
						}
					
					} 
					else if (m_materials[i].tex.format.flags[2] == 4)
					{
						m_materials[i].tex.paletteDataSize = 0;
						m_materials[i].tex.pixelDataSize = (m_materials[i].tex.width * m_materials[i].tex.height) / 2;
						m_materials[i].tex.pixelData = new BYTE[m_materials[i].tex.pixelDataSize];
						memcpy(m_materials[i].tex.pixelData, currTexDataOffset, m_materials[i].tex.pixelDataSize);
					}
					else if (m_materials[i].tex.format.flags[2] == 5)
					{
						m_materials[i].tex.paletteDataSize = 0;
						m_materials[i].tex.pixelDataSize = m_materials[i].tex.width * m_materials[i].tex.height;
						m_materials[i].tex.pixelData = new BYTE[m_materials[i].tex.pixelDataSize];
						memcpy(m_materials[i].tex.pixelData, currTexDataOffset, m_materials[i].tex.pixelDataSize);
					}
				}

				texIdx++;
			}
		}
	}

	return 0;
}

int HMT::initializeShaderResViews(ID3D11Device1* device, ID3D11DeviceContext1* deviceContext)
{
	BYTE* textureData = 0;
	HRESULT hResult;
	unsigned int rowPitch;
	int textureCount = 0;
	m_textureViews = new ID3D11ShaderResourceView* [m_numTextures];
	m_textures = new ID3D11Texture2D* [m_numTextures];
	m_mat2TexIndices = new int[m_numMaterials];

	for (int i = 0; i < m_numMaterials; i++)
	{
		if (m_materials[i].matType == 1) 
		{
			m_mat2TexIndices[i] = m_materials[i].texIdx;

			if (textureCount < m_numTextures && textureCount == m_materials[i].texIdx)
			{
				if (m_materials[i].tex.format.flags[2] == 2)
					textureData = new BYTE[2 * m_materials[i].tex.width * m_materials[i].tex.height];
				else
					textureData = new BYTE[4 * m_materials[i].tex.width * m_materials[i].tex.height];

				if (m_materials[i].tex.format.flags[2] == 0)
				{
					for (int j = 0; j < m_materials[i].tex.pixelDataSize; j++)
					{
						textureData[j * 8 + 0] = m_materials[i].tex.paletteData[((m_materials[i].tex.pixelData[j] & 0xF0) >> 4) * 3 + 0];
						textureData[j * 8 + 1] = m_materials[i].tex.paletteData[((m_materials[i].tex.pixelData[j] & 0xF0) >> 4) * 3 + 1];
						textureData[j * 8 + 2] = m_materials[i].tex.paletteData[((m_materials[i].tex.pixelData[j] & 0xF0) >> 4) * 3 + 2];
						textureData[j * 8 + 3] = 0xFF;

						textureData[j * 8 + 4] = m_materials[i].tex.paletteData[(m_materials[i].tex.pixelData[j] & 0xF) * 3 + 0];
						textureData[j * 8 + 5] = m_materials[i].tex.paletteData[(m_materials[i].tex.pixelData[j] & 0xF) * 3 + 1];
						textureData[j * 8 + 6] = m_materials[i].tex.paletteData[(m_materials[i].tex.pixelData[j] & 0xF) * 3 + 2];
						textureData[j * 8 + 7] = 0xFF;
					}
				}
				else if (m_materials[i].tex.format.flags[2] == 1)
				{
					for (int j = 0; j < m_materials[i].tex.pixelDataSize; j++)
					{
						textureData[j * 4 + 0] = m_materials[i].tex.paletteData[m_materials[i].tex.pixelData[j] * 3 + 0];
						textureData[j * 4 + 1] = m_materials[i].tex.paletteData[m_materials[i].tex.pixelData[j] * 3 + 1];
						textureData[j * 4 + 2] = m_materials[i].tex.paletteData[m_materials[i].tex.pixelData[j] * 3 + 2];
						textureData[j * 4 + 3] = 0xFF;
					}
				}
				else if (m_materials[i].tex.format.flags[2] == 2)
				{
					for (int j = 0; j < m_materials[i].tex.pixelDataSize; j++)
					{
						//uint16_t fuckO = m_materials[i].tex.pixelData[j];
						textureData[j] = m_materials[i].tex.pixelData[j];

					}
				}
				else if (m_materials[i].tex.format.flags[2] == 3)
				{
					if (m_materials[i].tex.format.flags[3] & 0x80) //has alpha
					{
						for (int j = 0; j < m_materials[i].tex.pixelDataSize; j++)
						{
							textureData[j] = m_materials[i].tex.pixelData[j];
						}
						
					}
					else 
					{
						for (int j = 0; j < m_materials[i].tex.pixelDataSize / 3; j++)
						{
							//((int*)textureData)[j] = ((int*)m_materials[i].tex.pixelData)[j];
							//textureData[j] = m_materials[i].tex.pixelData[j];
							textureData[j * 4 + 0] = m_materials[i].tex.pixelData[j * 3 + 0];
							textureData[j * 4 + 1] = m_materials[i].tex.pixelData[j * 3 + 1];
							textureData[j * 4 + 2] = m_materials[i].tex.pixelData[j * 3 + 2];
							textureData[j * 4 + 3] = 0xFF;
						}
					}
				}
				else if (m_materials[i].tex.format.flags[2] == 4)
				{
					for (int j = 0; j < m_materials[i].tex.pixelDataSize; j++)
					{
						textureData[j * 8 + 0] = ((m_materials[i].tex.pixelData[j] & 0xF0) >> 4) * 16;
						textureData[j * 8 + 1] = textureData[j * 8 + 0];
						textureData[j * 8 + 2] = textureData[j * 8 + 0];
						textureData[j * 8 + 3] = 0xFF;

						textureData[j * 8 + 4] = (m_materials[i].tex.pixelData[j] & 0xF) * 16;
						textureData[j * 8 + 5] = textureData[j * 8 + 4];
						textureData[j * 8 + 6] = textureData[j * 8 + 4];
						textureData[j * 8 + 7] = 0xFF;
					}
				}
				else if (m_materials[i].tex.format.flags[2] == 5)
				{
					for (int j = 0; j < m_materials[i].tex.pixelDataSize; j++)
					{
						textureData[j * 4 + 0] = m_materials[i].tex.pixelData[j];
						textureData[j * 4 + 1] = textureData[j * 4 + 0];
						textureData[j * 4 + 2] = textureData[j * 4 + 0];
						textureData[j * 4 + 3] = 0xFF;
					}
				}
				else
				{
					char error[256];
					sprintf_s(error, 256, "Unrecognized image format %d\n", m_materials[i].tex.format.flags[2]);
					OutputDebugStringA(error);
					return 1;
				}

				D3D11_TEXTURE2D_DESC textureDesc;
				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

				if (m_materials[i].tex.format.flags[2] == 2)
				{
					textureDesc.Height = m_materials[i].tex.height;
					textureDesc.Width = m_materials[i].tex.width;
					textureDesc.MipLevels = 0;
					textureDesc.ArraySize = 1;
					textureDesc.Format = DXGI_FORMAT_B5G5R5A1_UNORM;
					textureDesc.SampleDesc.Count = 1;
					textureDesc.SampleDesc.Quality = 0;
					textureDesc.Usage = D3D11_USAGE_DEFAULT;
					textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
					textureDesc.CPUAccessFlags = 0;
					textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

					rowPitch = m_materials[i].tex.width * 2;
				}
				else {
					textureDesc.Height = m_materials[i].tex.height;
					textureDesc.Width = m_materials[i].tex.width;
					textureDesc.MipLevels = 0;
					textureDesc.ArraySize = 1;
					textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
					textureDesc.SampleDesc.Count = 1;
					textureDesc.SampleDesc.Quality = 0;
					textureDesc.Usage = D3D11_USAGE_DEFAULT;
					textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
					textureDesc.CPUAccessFlags = 0;
					textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

					rowPitch = m_materials[i].tex.width * 4;
				}
				
				hResult = device->CreateTexture2D(&textureDesc, NULL, &m_textures[textureCount]);
				if (FAILED(hResult))
				{
					return 1;
				}

				deviceContext->UpdateSubresource(m_textures[textureCount], 0, NULL, textureData, rowPitch, 0);

				srvDesc.Format = textureDesc.Format;
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MostDetailedMip = 0;
				srvDesc.Texture2D.MipLevels = -1;

				hResult = device->CreateShaderResourceView(m_textures[textureCount], &srvDesc, &m_textureViews[textureCount]);
				if (FAILED(hResult))
				{
					return 1;
				}

				// Generate mipmaps for this texture.
				deviceContext->GenerateMips(m_textureViews[textureCount]);

				textureCount++;
			}
			else {
				//m_textureViews[i] = NULL;
			}
		}
		else {
			//m_textureViews[i] = NULL;
			m_mat2TexIndices[i] = -1;
		}

		if (textureData)
		{
			delete[] textureData;
			textureData = 0;
		}
	}

	return 0;
}
