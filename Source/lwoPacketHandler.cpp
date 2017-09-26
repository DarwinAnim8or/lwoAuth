#include "lwoPacketHandler.h"
#include "./Packets/lwoServerPackets.h"
#include <stdio.h>

void lwoPacketHandler::determinePacketHeader(RakPeerInterface* rakServer, Packet* packet) {
	printf("Trying to determine packet header...\n");

	switch (packet->data[1]) { //First byte determines the connection type.
	case CONN_SERVER:
		printf("ConnectionType: CONN_SERVER\n");
		handleServerConnPackets(rakServer, packet);
		break;
	case CONN_AUTH:
		printf("ConnectionType: CONN_AUTH\n");
		handleAuthConnPackets(rakServer, packet);
		break;
	case CONN_CHAT:
		printf("ConnectionType: CONN_CHAT\n");
		handleChatConnPackets(rakServer, packet);
		break;
	case CONN_WORLD:
		printf("ConnectionType: CONN_WORLD\n");
		handleWorldConnPackets(rakServer, packet);
		break;
	default:
		printf("Unknown connection type: %i\n", packet->data[1]);
		break;
	}
} //determinePacketHeader

void lwoPacketHandler::handleServerConnPackets(RakPeerInterface* rakServer, Packet* packet) {
	switch (packet->data[3]) {
	case MSG_SERVER_VERSION_CONFIRM: {
		printf("Received version confirm request.\n");
		lwoServerPackets::doHandshake(rakServer, packet);
		break;
	}
	case MSG_SERVER_GENERAL_NOTIFY: {
		printf("Received a general notification.\n");
		break;
	}
	case MSG_SERVER_DISCONNECT_NOTIFY: {
		printf("Received disconnection notification.\n");
		break;
	}
	default:
		printf("Unknown server packet: %i\n", packet->data[3]);
		break;
	}
} //handleServerConnPackets

void lwoPacketHandler::handleAuthConnPackets(RakPeerInterface* rakServer, Packet* packet) {
	switch (packet->data[3]) {
	case MSG_AUTH_LOGIN_REQUEST: {
		printf("Received login request.\n");

		break;
	}
	default:
		printf("Unknown auth packet: %i\n", packet->data[3]);
		break;
	}
} //handleServerConnPackets

void lwoPacketHandler::handleChatConnPackets(RakPeerInterface* rakServer, Packet* packet) {
	//not used by auth
} //handleServerConnPackets

void lwoPacketHandler::handleWorldConnPackets(RakPeerInterface* rakServer, Packet* packet) {
	//not used by auth
} //handleServerConnPackets