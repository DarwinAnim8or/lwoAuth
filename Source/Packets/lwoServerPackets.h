#pragma once
#include "../lwoPacketIdentifiers.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include <string>

namespace lwoServerPackets {
	bool doHandshake(RakPeerInterface* rakServer, Packet* packet);
	void sendHandshake(RakPeerInterface* rakServer, Packet* packet, unsigned int iServerVersion);
	void createPacketHeader(unsigned char uPacketID, unsigned short sConnectionType, unsigned int iInternalPacketID, RakNet::BitStream* bitStream);
	void writeStringToPacket(std::string sString, int maxSize, RakNet::BitStream* bitStream);
}