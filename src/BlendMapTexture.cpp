#include "BlendMapTexture.hpp"

#include "Terrain.hpp"

//BlendMapTexture::BlendMapTexture(int width, int height, Terrain *terrain) : m_width(width), m_height(height)
BlendMapTexture::BlendMapTexture(int width, int height, Terrain *t) : m_width(width), m_height(height), m_pixels(new int[width * height])
{

	for (int row = 0; row < width; row++) {
		for (int col = 0; col < height; col++) {

			int index = col + row * width;
			//std::cout << "row" << index << "\n";

			//int pixel = (row * (width)) + (col);

			//int index = col + row * width;

			int r = 0; // 
			int g = 0;
			int b = 0;
			int a = 0;

			float height = t->getInterpHeight(col / 4, row / 4);
			if (height > 30) { // grass
				r = 255; // snow
			}
			else if (height > 10) { //stones
				
				b = 255; // stones
			}
			else if (height > -80){ // grass
				g = 255; // grass
			}
			else {
				a = 255; // rock
			}

			int colour = r | (g << 8) | (b << 16) | (a << 24);
			m_pixels[index] = colour;// 0x000000FF;

		}
	}
	///*
	//Run Simple Blur on blendMap
	for (int i = 0; i < 4; i++) {
		//Horizontal pass
		for (int row = 0; row < width; row++) {
			for (int col = 1; col < height - 1; col++) {

				int index = col + row * width;

				int c1 = m_pixels[index - 1];
				int c2 = m_pixels[index];
				int c3 = m_pixels[index + 1];

				int r2 = ((c1 & 0x000000FF) + (c2 & 0x000000FF) + (c3 & 0x000000FF)) / 3;
				int g2 = (((c1 & 0x0000FF00) >> 8) + ((c2 & 0x0000FF00) >> 8) + ((c3 & 0x0000FF00) >> 8)) / 3;
				int b2 = (((c1 & 0x00FF0000) >> 16) + ((c2 & 0x00FF0000) >> 16) + ((c3 & 0x00FF0000) >> 16)) / 3;
				int a2 = (((c1 & 0xFF000000) >> 24) + ((c2 & 0xFF000000) >> 24) + ((c3 & 0xFF000000) >> 24)) / 3;

				int colour = r2 | (g2 << 8) | (b2 << 16) | (a2 << 24);
				m_pixels[index] = colour;
			}
		}
		//Vertical pass
		for (int row = 1; row < width - 1; row++) {
			for (int col = 0; col < height; col++) {

				int index = col + row * width;

				int c1 = m_pixels[index - width];
				int c2 = m_pixels[index];
				int c3 = m_pixels[index + width];

				int r2 = ((c1 & 0x000000FF) + (c2 & 0x000000FF) + (c3 & 0x000000FF)) / 3;
				int g2 = (((c1 & 0x0000FF00) >> 8) + ((c2 & 0x0000FF00) >> 8) + ((c3 & 0x0000FF00) >> 8)) / 3;
				int b2 = (((c1 & 0x00FF0000) >> 16) + ((c2 & 0x00FF0000) >> 16) + ((c3 & 0x00FF0000) >> 16)) / 3;
				int a2 = (((c1 & 0xFF000000) >> 24) + ((c2 & 0xFF000000) >> 24) + ((c3 & 0xFF000000) >> 24)) / 3;

				int colour = r2 | (g2 << 8) | (b2 << 16) | (a2 << 24);
				m_pixels[index] = colour;
			}
		}


	}
	//*/
	
}

BlendMapTexture::~BlendMapTexture()
{
	delete[] m_pixels;
}
