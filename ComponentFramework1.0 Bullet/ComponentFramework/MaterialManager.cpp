#include "MaterialManager.h"

MaterialManager* MaterialManager::instance = 0;

MaterialManager::MaterialManager()
{

}

void MaterialManager::Delete()
{
	delete instance;
	instance = nullptr;
}

MaterialManager::~MaterialManager()
{
	auto it = materials.begin();
	while (it != materials.end())
	{
		it = materials.erase(it);
	}
}

MaterialManager* MaterialManager::GetInstance()
{
	if (!instance)
		instance = new MaterialManager();

	return instance;
}

Material* MaterialManager::Contains(const char* name)
{
	if (materials.find(name) != materials.end())
		return materials.find(name)->second;
	else
		return nullptr;
}

void MaterialManager::Insert(const char* name, Material* mat)
{
	materials.insert(std::pair<const char*, Material*>(name,mat));
}

