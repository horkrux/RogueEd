#include "Font.h"

Font::Font()
{
	m_Font = 0;
	m_texture = 0;
	m_texWidth = 0;
	m_texHeight = 0;
	m_glyphs = NULL;
}


Font::Font(const Font& other)
{
}


Font::~Font()
{
}

bool Font::Initialize(ID3D11Device1* device, ID3D11DeviceContext1* deviceContext, string fontFilename, const WCHAR* textureFilename)
{
	bool result;


	// Load in the text file containing the font data.
	result = ReadDXTKFont(fontFilename, device, deviceContext);
	if (result)
	{
		return false;
	}

	// Load the texture that has the font characters on it.
	/*result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}*/

	return true;
}

void Font::Shutdown()
{
	// Release the font texture.
	//ReleaseTexture();

	// Release the font data.
	//ReleaseFontData();

	if (m_texture)
	{
		m_texture->Release();
		m_texture = NULL;
	}

	if (m_textureView)
	{
		m_textureView->Release();
		m_textureView = NULL;
	}

	if (m_glyphs)
	{
		delete[] m_glyphs;
		m_glyphs = NULL;
	}

	return;
}

bool Font::LoadFontData(const char* filename)
{
	ifstream fin;
	int i;
	char temp;

	// Create the font spacing buffer.
	m_Font = new FontType[95];
	if (!m_Font)
	{
		return false;
	}

	// Read in the font size and spacing between chars.
	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	// Read in the 95 used ascii characters for text.
	for (i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	// Close the file.
	fin.close();

	return true;
}

void Font::ReleaseFontData()
{
	// Release the font data array.
	if (m_Font)
	{
		delete[] m_Font;
		m_Font = 0;
	}

	return;
}

int Font::ReadDXTKFont(string filename, ID3D11Device1* device, ID3D11DeviceContext1* deviceContext)
{
	ifstream fontFile = ifstream(filename, ios::in | ios::binary | ios::ate);
	char* textureData;
	HRESULT hResult;
	unsigned int rowPitch;

	if (fontFile)
	{
		fontFile.seekg(8);

		int numGlyphs;
		fontFile.read((char*)&numGlyphs, 4);

		m_glyphs = new Glyph[numGlyphs];

		fontFile.read((char*)m_glyphs, sizeof(Glyph) * numGlyphs);

		fontFile.seekg(8, ios::cur);

		fontFile.read((char*)&m_texWidth, 4);
		//m_texWidth /= 4;
		fontFile.read((char*)&m_texHeight, 4);

		fontFile.seekg(12, ios::cur);

		rowPitch = m_texWidth * 4;

		textureData = new char[rowPitch * m_texHeight];

		fontFile.read(textureData, rowPitch * m_texHeight);

		

		D3D11_TEXTURE2D_DESC textureDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

		textureDesc.Height = m_texHeight;
		textureDesc.Width = m_texWidth;
		textureDesc.MipLevels = 0;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		hResult = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
		if (FAILED(hResult))
		{
			return 1;
		}

		deviceContext->UpdateSubresource(m_texture, 0, NULL, textureData, rowPitch, 0);

		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1;

		hResult = device->CreateShaderResourceView(m_texture, &srvDesc, &m_textureView);
		if (FAILED(hResult))
		{
			return 1;
		}

		// Generate mipmaps for this texture.
		deviceContext->GenerateMips(m_textureView);

		delete[] textureData;
		fontFile.close();
	}
	else {
		return 1;
	}

	return 0;
}

//bool Font::LoadTexture(ID3D11Device* device, WCHAR* filename)
//{
//	bool result;
//
//
//	// Create the texture object.
//	m_texture = new TextureClass;
//	if (!m_texture)
//	{
//		return false;
//	}
//
//	// Initialize the texture object.
//	result = m_Texture->Initialize(device, filename);
//	if (!result)
//	{
//		return false;
//	}
//
//	return true;
//}

ID3D11ShaderResourceView* Font::GetTexture()
{
	return m_textureView;
}

void Font::BuildVertexArray(void* vertices, const char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;


	// Coerce the input vertices into a VertexType structure.
	vertexPtr = (VertexType*)vertices;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(sentence);

	// Initialize the index to the vertex array.
	index = 0;

	int glyphHeight = 0;
	int glyphWidth = 0;

	// Draw each letter onto a quad.
	for (i = 0; i < numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		// If the letter is a space then just move over three pixels.
		//if (letter == 0)
		//{
			//drawX = drawX + 3.0f;
		//}
		//else
		if (i > 0)
		{
			drawX = drawX + glyphWidth + m_glyphs[letter-1].advance + m_glyphs[letter].xOffset;
		}
		else {
			//drawX = drawX - m_glyphs[letter].xOffset;
		}
		
		{
			glyphHeight = m_glyphs[letter].position.bottom - m_glyphs[letter].position.top;
			glyphWidth = m_glyphs[letter].position.right - m_glyphs[letter].position.left;

			
			//glyphWidth *= 5;
			//glyphHeight *= 5;

			// First triangle in quad.
			vertexPtr[index].position = XMVectorSet(drawX, drawY - m_glyphs[letter].yOffset, 0.0f, 1.f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(m_glyphs[letter].position.left / 128.f, m_glyphs[letter].position.top / 148.f);
			index++;

			vertexPtr[index].position = XMVectorSet(drawX + glyphWidth, drawY - glyphHeight - m_glyphs[letter].yOffset, 0.0f, 1.f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(m_glyphs[letter].position.right / 128.f, m_glyphs[letter].position.bottom / 148.f);
			index++;

			vertexPtr[index].position = XMVectorSet(drawX, drawY - glyphHeight - m_glyphs[letter].yOffset, 0.0f, 1.f);  // Bottom left.
			vertexPtr[index].texture = XMFLOAT2(m_glyphs[letter].position.left / 128.f, m_glyphs[letter].position.bottom / 148.f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = XMVectorSet(drawX, drawY - m_glyphs[letter].yOffset, 0.0f, 1.f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(m_glyphs[letter].position.left / 128.f, m_glyphs[letter].position.top / 148.f);
			index++;

			vertexPtr[index].position = XMVectorSet(drawX + glyphWidth, drawY - m_glyphs[letter].yOffset, 0.0f, 1.f);  // Top right.
			vertexPtr[index].texture = XMFLOAT2(m_glyphs[letter].position.right / 128.f, m_glyphs[letter].position.top / 148.f);
			index++;

			vertexPtr[index].position = XMVectorSet(drawX + glyphWidth, (drawY - glyphHeight) - m_glyphs[letter].yOffset, 0.0f, 1.f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(m_glyphs[letter].position.right / 128.f, m_glyphs[letter].position.bottom / 148.f);
			index++;

			//if (i == 0)
			//	drawX = drawX + m_glyphs[letter].xOffset;
			// Update the x location for drawing by the size of the letter and one pixel.
			//drawX = drawX + glyphWidth + m_glyphs[letter].advance;//+ 1.0f;
		}
	}

	return;
}