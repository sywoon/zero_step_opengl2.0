#include "Texture.h"
#include "stb_image.h"
#include <iostream>


Texture::Texture(const std::string & fileName)
{
	int width, height, numComponents;
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &numComponents, 4);

	if (data == NULL)
	{
		std::cerr << "无法加载纹理" << fileName << std::endl;
		return;
	}

	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, 
					GL_RGBA, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &_texture);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, _texture);
}
