#include "Texture.hpp"


Texture::Texture(int width, int height, GLuint textureID) : m_width(width), m_height(height), m_textureID(textureID)
{

}

Texture::~Texture()
{
	glDeleteTextures(1, &m_textureID);
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}