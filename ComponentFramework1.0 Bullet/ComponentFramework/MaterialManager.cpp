#include "MaterialManager.h"

MaterialManager* MaterialManager::instance = 0;

MaterialManager::MaterialManager()
{

}

MaterialManager::~MaterialManager()
{
	delete instance;
}