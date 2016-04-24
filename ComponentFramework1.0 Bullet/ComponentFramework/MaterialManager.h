#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include <map>
#include "Material.h"

class MaterialManager
{
public:

	static MaterialManager* GetInstance();

	void Insert(const char* path, Material* mat);

	Material* Contains(const char* name);

	void Delete();

private:
	MaterialManager();
	~MaterialManager();

	std::map<const char*, Material*> materials;

	static MaterialManager* instance;
};

#endif