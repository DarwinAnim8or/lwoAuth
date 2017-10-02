#include <stdio.h>
#include <string.h>
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "RakSleep.h"
#include "MessageIdentifiers.h"

//========== Non-lwo includes above =|= lwo includes below ============

#include "lwoPacketIdentifiers.h"
#include "lwoPacketHandler.h"

//========== lwo includes above =|= code below ===========

#define MAX_CLIENTS 10
#define SERVER_PORT 1001

int main(void) {
	RakPeerInterface* rakServer = RakNetworkFactory::GetRakPeerInterface();
	Packet *packet;

	printf("lwoAuth, a simple test server for the ALPHA version of LU.\n");
	printf("This project is licensed under the GPLv2, and is available at:\n");
	printf("https://github.com/DarwinAnim8or/lwoAuth \n");
	printf("While DLU members might work on this; it is not (related to) DLU.\n");
	printf("Server version: 0.0.1\n\n");

	rakServer->Startup(MAX_CLIENTS, 30, &SocketDescriptor(SERVER_PORT, 0), 1);
	rakServer->SetIncomingPassword("3.25 ND1", 8);

	printf("Starting the server.\n");
	// We need to let the server accept incoming connections from the clients
	rakServer->SetMaximumIncomingConnections(MAX_CLIENTS);

	while (true) {
		RakSleep(30); //So that we don't use up a ton of CPU while running.
		packet = rakServer->Receive();

		if (packet) {
			switch (packet->data[0]) {
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Another client has disconnected.\n");
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("Another client has lost the connection.\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Another client has connected.\n");
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				printf("Our connection request has been accepted.\n");
				break;

			case ID_USER_PACKET_ENUM:
				printf("Received custom packet-- likely an lwo packet.\n");
				lwoPacketHandler::determinePacketHeader(rakServer, packet); // really just a "first pass" if you will to see if it's of any use to us.
				break;

			case ID_NEW_INCOMING_CONNECTION:
				printf("A connection is incoming.\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
					printf("A client has disconnected.\n");
				break;
			case ID_CONNECTION_LOST:
				printf("A client lost the connection.\n");
				break;
			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}

			rakServer->DeallocatePacket(packet);
		}
	}

	RakNetworkFactory::DestroyRakPeerInterface(rakServer);

	return 0;
}
