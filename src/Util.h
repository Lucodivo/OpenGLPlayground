#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void loadTexture(const char* imgLocation, uint32& textureId) {
	glGenTextures(1, &textureId);
	uint32 textureOffset = textureId - 1;
	glActiveTexture(GL_TEXTURE0 + textureOffset); // activate texture unit (by default it is bound to GL_TEXTURE0)
	glBindTexture(GL_TEXTURE_2D, textureId);

	// load image data
	int width, height, numChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(imgLocation, &width, &height, &numChannels, 0);
	if (data) {
		auto pixelFormat = (numChannels == 3) ? GL_RGB : GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, // target
			0, // level of detail (level n is the nth mipmap reduction image)
			pixelFormat, // kind of format we want to store the texture
			width, // width of texture
			height, // height of texture
			0, // border (legacy stuff, MUST BE 0)
			pixelFormat, // Specifies format of the pixel data
			GL_UNSIGNED_BYTE, // specifies data type of pixel data
			data); // pointer to the image data
		glGenerateMipmap(GL_TEXTURE_2D);

		// set texture options
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // disables bilinear filtering (creates sharp edges when magnifying texture)
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data); // free texture image memory
}