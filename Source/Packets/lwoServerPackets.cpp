#include "lwoServerPackets.h"
#include "../RakNet/RakPeerInterface.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>

bool lwoServerPackets::doHandshake(RakPeerInterface* rakServer, Packet* packet) {
	bool bReturn = false;
	RakNet::BitStream inStream(packet->data, packet->length, false);
	//inStream.SetReadOffset(8); //skip the header
	unsigned long long header = inStream.Read(header); //Skips ahead 8 bytes, SetReadOffset doesn't work for some reason.
	unsigned int iClientVersion;
	inStream.Read(iClientVersion);

	/*//print it
	std::ostringstream buffer;

	buffer << "Data (in bytes): ";

	for (uint32_t i = 0; i < packet->length; ++i) {
		if (i % 16 == 0) buffer << "\n\t\t";
		else buffer << " ";
		buffer << std::setw(2) << std::hex << std::setfill('0') << static_cast<uint32_t>(packet->data[i]);
	}

	buffer << "\n\n";
	std::string bufferStr = buffer.str();
	std::cout << bufferStr << std::endl;

	std::cout << iClientVersion << std::endl;*/

	unsigned int iServerVersion = 130529; //The version the client must have
	if (iClientVersion == iServerVersion) {
		printf("Client version is equal to the server's version!\n");
		sendHandshake(rakServer, packet, iServerVersion);
	}
	else if (iClientVersion > iServerVersion) {
		printf("Received a newer client version: %i\n", iClientVersion);
	}
	else if (iClientVersion < iServerVersion) {
		printf("Received an older client version: %i\n", iClientVersion);
	}
	else {
		printf("Received unknown client version: %i\n", iClientVersion);
	}

	return bReturn;
} //doHandshake

void lwoServerPackets::sendHandshake(RakPeerInterface* rakServer, Packet* packet, unsigned int iServerVersion) {
	RakNet::BitStream bitStream;
	createPacketHeader(ID_USER_PACKET_ENUM, CONN_SERVER, MSG_SERVER_VERSION_CONFIRM, &bitStream);

	unsigned int unknown = 0x93;
	unsigned int rConType = CONN_AUTH;
	unsigned int processID = 0x1410;
	unsigned short localPort = 0xff;
	std::string localIP = "127.0.0.1"; //hardcoded for now.

	bitStream.Write(iServerVersion);
	bitStream.Write(unknown);
	bitStream.Write(rConType);
	bitStream.Write(processID);
	bitStream.Write(localPort);
	writeStringToPacket(localIP, localIP.size(), &bitStream);
	rakServer->Send(&bitStream, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
} //sendHandshake

void lwoServerPackets::createPacketHeader(unsigned char uPacketID, unsigned short sConnectionType, unsigned int iInternalPacketID, RakNet::BitStream* bitStream) {
	bitStream->Write((unsigned char)uPacketID);
	bitStream->Write(sConnectionType);
	bitStream->Write(iInternalPacketID);
	bitStream->Write((char)0);
}

void lwoServerPackets::writeStringToPacket(std::string sString, int maxSize, RakNet::BitStream* bitStream) {
	int size = sString.size();
	int emptySize = maxSize - size;

	if (size > maxSize) size = maxSize;

	for (int i = 0; i < size; i++) {
		bitStream->Write((char)sString[i]);
	}

	for (int i = 0; i < emptySize; i++) {
		bitStream->Write((char)0);
	}
} //writeStringToPacket