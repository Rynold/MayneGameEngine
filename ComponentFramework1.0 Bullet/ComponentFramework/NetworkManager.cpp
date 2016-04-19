#include "NetworkManager.h"

NetworkManager::NetworkManager(bool isServer_)
{
	isServer = isServer_;

	if (isServer)
		server = new Server();
	else
		client = new Client();
}

NetworkManager::~NetworkManager()
{
	delete server;
	server = nullptr;

	delete client;
	client = nullptr;
}

bool NetworkManager::AttachListener(NetworkListener* listener)
{
	bool success = false;
	if (isServer)
	{
		server->AddListener(listener);
		success = true;
	}
	return success;
}

bool NetworkManager::SendEvent(const SDL_Event& sdlEvent)
{
	bool success = false;
	if (!isServer)
		client->SendEvent(sdlEvent);

	return success;
}

void NetworkManager::Update()
{
	if (isServer)
		server->Update();
	else
		client->Update();
}