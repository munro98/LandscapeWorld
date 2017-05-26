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

			//if (t->lookUpHeight(col/32, row/32) > 0) {
			//if (row > 8) {
			if (t->getInterpHeight(col / 4, row / 4) > 0) {
				r = 255;
			}
			else {
				g = 255;
			}

			int colour = r | (g << 8) | (b << 16) | (a << 24);
			m_pixels[index] = colour;// 0x000000FF;

		}
	}


	/*
	float scaleWidth = (float)width / (float)16;
	float scaleHeight = (float)height/ (float)16;

	for (int row = 0; row < width; row++) {
		for (int col = 0; col < height; col++) {

			int index = col + row * width;
			std::cout << "row" << index << "\n";
			
			int pixel = (row * (width)) + (col);
			int nearestMatch = (((int)(row / scaleHeight) * (16)) + ((int)(col / scaleWidth)));


			std::cout << row << " > " << (int)(row / scaleHeight) * (16) << ", "  << col << " > " << (int)(col / scaleWidth) << "\n";

			
			int index = col + row * width;

			int r = 255;
			int g = 255;
			int b = 255;
			int a = 0;

			int colour = r | (g << 8) | (b << 16) | (a << 24);
			m_pixels[index] = colour;// 0x000000FF;
			
		}
	}
	*/
	/*
	for (int i = 0; i < width * height; i++) {
		int r = 255;
		int g = 255;
		int b = 255;
		int a = 0;

		int colour = r | (g << 8) | (b << 16) | (a << 24);
		m_pixels[i] = colour;// 0x000000FF;
	}
	*/
}

BlendMapTexture::~BlendMapTexture()
{
	delete m_pixels;
}
