#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

class MaterialManager
{
public:

	static MaterialManager* GetInstance()
	{
		if (!instance)
			instance = new MaterialManager();

		return instance;
	}

private:
	MaterialManager();
	~MaterialManager();

	static MaterialManager* instance;
};

#endif