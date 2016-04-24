#include "TextureManager.h"

TextureManager* TextureManager::manager = 0;

TextureManager::TextureManager()
{
	//this->LoadTexture("Images/defaultNormal.jpg", TextureType::NORMAL);
}


TextureManager::~TextureManager()
{
	auto it = textures.begin();
	while ( it != textures.end())
	{
		it = textures.erase(it);
	}
	//delete manager;
	//manager = nullptr;
}

void TextureManager::Delete()
{
	delete manager;
	manager = nullptr;
}

TextureManager* TextureManager::GetInstance()
{
	if (!manager)
		manager = new TextureManager();

	return manager;
}

std::shared_ptr<Texture> TextureManager::Contains(const char* name)
{
	if (textures.find(name) != textures.end())
		return textures.find(name)->second;
	else
		return nullptr;
}

void TextureManager::Insert(const char* path,std::shared_ptr<Texture> tex)
{
	textures.insert(std::pair<const char*, std::shared_ptr<Texture>>(path, tex));
}

void TextureManager::LoadTexture(const char* path, TextureType type)
{
	GLuint texture;
	if (!TextureManager::GetInstance()->Contains(path))
	{
		glGenTextures(1, &texture);

		int width, height;
		unsigned char* image;

		image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
		if (0 == image)
		{
			std::cout << "SOIL loading error: \n" << SOIL_last_result() << std::endl;
		}
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

		TextureManager::GetInstance()->Insert(path, std::shared_ptr<Texture>(new Texture(path, type, texture)));
	}
}