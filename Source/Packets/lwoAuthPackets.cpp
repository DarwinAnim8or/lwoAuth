#include "lwoAuthPackets.h"
#include "lwoPacketUtils.h"
#include "BitStream.h"

void lwoAuthPackets::handleLoginPacket(RakPeerInterface* rakServer, Packet* packet) {
	//Only read the username and pass, we don't care about the rest for now.
	std::string sUsername = lwoPacketUtils::readWStringAsString(8, packet);
	std::string sPassword = lwoPacketUtils::readWStringAsString(0x4A, packet);
	printf("User %s connected with password: %s\n", sUsername.c_str(), sPassword.c_str());

	//TODO: add a login check here & get the server IP for the world server.
	unsigned char cLoginState = 1; //0 = incorrect info, 1 = login ok, 2 = banned
	std::string sServerToSendTo = "127.0.0.1";
	unsigned short usServerPort = 2002; //default port for character create/select

	RakNet::BitStream bitStream;
	lwoPacketUtils::createPacketHeader(ID_USER_PACKET_ENUM, CONN_CLIENT, MSG_CLIENT_LOGIN_RESPONSE, &bitStream);
	
	bitStream.Write(cLoginState);
	
	for (int i = 0; i < 66; i++) { //For some reason there's a ton of space here?
		bitStream.Write(unsigned char(0));
	}

	lwoPacketUtils::writeStringToPacket(sServerToSendTo, sServerToSendTo.size(), &bitStream);

	for (int i = 0; i < 57; i++) { //For some reason there's a ton of space here?
		bitStream.Write(unsigned char(0));
	}

	bitStream.Write(usServerPort);
	rakServer->Send(&bitStream, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
} //handleLoginPacket