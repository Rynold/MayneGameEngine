#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#pragma once

#include "SDL_net.h"
#include "NetworkListener.h"
#include <iostream>
#include <vector>

union networkPacket
{
	SDL_Event e;
	Uint8 buffer[sizeof(SDL_Event)];
};

class Server
{
public:
	Server()
	{
		if (SDLNet_Init() < 0)
			std::cout << "SDLNet_Init Failure: " << SDLNet_GetError() << std::endl;

		if (!(sd = SDLNet_UDP_Open(0x1A2B)))
			std::cout << "SDLNet_UDP_Open Failure: " << SDLNet_GetError() << std::endl;

		if (!(p = SDLNet_AllocPacket(sizeof(SDL_Event))))
			std::cout << "SDLNet_AllocPacket Failure: " << SDLNet_GetError() << std::endl;
	}
	virtual ~Server()
	{
		/*for (int i = listeners.size() - 1; i >= 0; i--)
		{
			delete listeners[i];
			listeners.pop_back();
		}*/

		listeners.clear();

		SDLNet_FreePacket(p);
		SDLNet_Quit();
	}

	UDPsocket sd;
	UDPpacket *p;

	std::vector<NetworkListener*> listeners;

	void AddListener(NetworkListener* listener)
	{
		listeners.push_back(listener);
	}

	void RemoveListener(NetworkListener* listener)
	{
		for (auto it = listeners.begin(); it != listeners.end(); ++it)
		{
			if (listener == *it)
				listeners.erase(it);
		}
	}

	void Update()
	{
		if (SDLNet_UDP_Recv(sd, p))
		{
			std::cout << "Receiving Event" << std::endl;

			networkPacket packet;

			for (int i = 0; i < sizeof(packet.buffer); i++)
				*(packet.buffer + i) = *(p->data + i);

			for (auto it = listeners.begin(); it != listeners.end(); ++it)
				(*it)->ReceiveNetworkSDLEvent(packet.e);
		}
	}
};

class Client
{
public:
	Client()
	{
		if (SDLNet_Init() < 0)
			std::cout << "SDLNet_Init Failure: " << SDLNet_GetError() << std::endl;

		if (!(sd = SDLNet_UDP_Open(0)))
			std::cout << "SDLNet_UDP_Open Failure: " << SDLNet_GetError() << std::endl;

		if (SDLNet_ResolveHost(&srvadd, "localhost", 0x1A2B) < 0)
			std::cout << "SDLNet_ResolveHost Failure: " << SDLNet_GetError() << std::endl;
	
		if (!(p = SDLNet_AllocPacket(sizeof(SDL_Event))))
			std::cout << "SDLNet_AllocPacket Failure: " << SDLNet_GetError() << std::endl;
	}
	virtual ~Client()
	{
		SDLNet_FreePacket(p);
		SDLNet_Quit();
	}

	UDPsocket sd;
	IPaddress srvadd;
	UDPpacket *p;

	void SendEvent(const SDL_Event& SDLEvent)
	{
		networkPacket packet;
		packet.e = SDLEvent;
		
		p->address.host = srvadd.host;
		p->address.port = srvadd.port;
		p->len = sizeof(SDL_Event);

		for (int i = 0; i < sizeof(packet.buffer); i++)
			*(p->data + i) = *(packet.buffer + i);

		std::cout << "Sending Event" << std::endl;

		SDLNet_UDP_Send(sd, -1, p);
	}

	void rcvData(void* data, Uint32 length)
	{
		SDLNet_UDP_Recv(sd, p);

		memcpy_s(data, length, p->data, p->len);

	}

	void Update()
	{

	}

};

class NetworkManager
{
public:

	NetworkManager(bool isServer_);
	~NetworkManager();

	bool isServer;
	Server* server;
	Client* client;

	bool AttachListener(NetworkListener* listener);

	bool SendEvent(const SDL_Event& sdlEvent);

	void Update();

};

#endif