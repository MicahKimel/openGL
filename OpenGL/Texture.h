#pragma once
#include<iostream>
#include<string>

#include<glew.h>
#include<glfw3.h>
#include<SOIL2.h>

class Texture
{
private:
	GLuint id;
	int height;
	int width;
	unsigned int Type;
	GLint textureUnit;

public:
	Texture(const char* fileName, GLenum type)
	{
		this->Type = type;

		unsigned char* image = SOIL_load_image(fileName, &this->width, &this->height, NULL, SOIL_LOAD_RGBA);

		glGenTextures(1, &this->id);
		glBindTexture(type, this->id);

		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if (image)
		{
			glTexImage2D(type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(type);
		}
		else
		{
			std::cout << "ERROR::TEXTURE::TEXTURE_LOADING_FAILED: " << fileName << "\n";
		}

		glActiveTexture(0);
		glBindTexture(type, 0);
		SOIL_free_image_data(image);
	}
	Texture(const char* fileName, GLenum type, GLint texture_unit)
	{
		//if exists delete
		if (this->id)
		{
			glDeleteTextures(1, &this->id);
		}
		this->Type = type;
		this->textureUnit = texture_unit;
		unsigned char* image = SOIL_load_image(fileName, &this->width, &this->height, NULL, SOIL_LOAD_RGBA);

		glGenTextures(1, &this->id);
		glBindTexture(type, this->id);

		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if (image)
		{
			glTexImage2D(type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(type);
		}
		else
		{
			std::cout << "ERROR:TEXTURE_LOAD_FAIL" << fileName << "\n";
		}

		glActiveTexture(0);
		glBindTexture(type, 0);
		SOIL_free_image_data(image);
	}
	~Texture()
	{
		glDeleteTextures(1, &this->id);
	}

	GLuint getID() const
	{
		return (this->id);
	}

	void bind() 
	{
		glActiveTexture(GL_TEXTURE0 + this->textureUnit);
		glBindTexture(this->Type, this->id);
	}

	void unbind()
	{
		glActiveTexture(0);
		glBindTexture(this->Type, 0);
	}

	inline GLint getTextureUnit() const { return this->textureUnit; }
};