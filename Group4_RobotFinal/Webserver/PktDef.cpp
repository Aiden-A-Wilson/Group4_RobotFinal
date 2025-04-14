#include "PktDef.h"
#include <bitset>
#include <cstring>

// Constructor: initializes the packet with default values
PktDef::PktDef() {
	memset(&Packet.Head, 0, HEADERSIZE);
	Packet.Data = nullptr;
	Packet.CRC = 0;
	RawBuffer = nullptr;
}
// Constructor: initializes the packet based on a source
PktDef::PktDef(char* src) {
	Packet = { 0 };
	RawBuffer = nullptr;

	int crcSize = sizeof(Packet.CRC);

	memcpy(&Packet.Head, src, HEADERSIZE);

	int bodySize = Packet.Head.Length - HEADERSIZE - crcSize;

	if (Packet.Data != nullptr) {
		delete[] Packet.Data;
		Packet.Data = nullptr;
	}

	Packet.Data = new char[bodySize];

	memcpy(Packet.Data, src + HEADERSIZE, bodySize);
	memcpy(&Packet.CRC, src + HEADERSIZE + bodySize, crcSize);

}
// Destructor: clean up
PktDef::~PktDef()
{
	if (Packet.Data != nullptr) {
		delete[] Packet.Data;
	}

	if (RawBuffer != nullptr) {
		delete[] RawBuffer;
	}
}

// Set the command type in the packet header
void PktDef::SetCmd(CmdType Type) {
	// Depending on the command type, set that bit
	if (Type == DRIVE) {
		Packet.Head.Drive = 1;
	}
	else if (Type == SLEEP) {
		Packet.Head.Sleep = 1;
	}
	else if (Type == RESPONSE) {
		Packet.Head.Status = 1;
	}
	else {
		std::cout << "SET CmdType went wrong" << std::endl;
	}
}
// Set the body data of the packet with new data
void PktDef::SetBodyData(char* srcData, int Size) {

	if (Packet.Data) {
		delete[] Packet.Data;
		Packet.Data = nullptr;
	}

	Packet.Data = new char[Size];

	memcpy(Packet.Data, srcData, Size);

	Packet.Head.Length = HEADERSIZE + Size + sizeof(Packet.CRC);
}
// Set the packet count
void PktDef::SetPktCount(int Count) {
	Packet.Head.PktCount = Count;
}
// Get the command type from the packet header
CmdType PktDef::GetCmd() {

	if (Packet.Head.Drive == 1) {
		return DRIVE;
	}
	else if (Packet.Head.Sleep == 1) {
		return SLEEP;
	}
	else if (Packet.Head.Status == 1) {
		return RESPONSE;
	}
	else {
		std::cout << " GET CmdType went wrong" << std::endl;
		return (CmdType)-1;
	}
}
// Get the acknowledgment from the packet header
bool PktDef::GetAck() {

	if (Packet.Head.Ack == 1)
		return true;
	else
		return false;

}
// Get the length of the packet data
int PktDef::GetLength() {
	return Packet.Head.Length;
}
// Get the body data of the packet
char* PktDef::GetBodyData() {
	return Packet.Data;
}
// Get the packet count from the header
int PktDef::GetPktCount() {
	return Packet.Head.PktCount;
}
// Check if the CRC is valid by calculating it and comparing to the one in the packet
bool PktDef::CheckCRC(char* src, int Size) {
	struct CmdPacket pkt = { 0 };

	memcpy(&pkt.Head, src, HEADERSIZE);
	pkt.Data = nullptr;

	int crcSize = sizeof(pkt.CRC);
	int bodySize = pkt.Head.Length - HEADERSIZE - crcSize;

	if (bodySize > 0) {
		pkt.Data = new char[bodySize];
		memcpy(pkt.Data, src + HEADERSIZE, bodySize);
	}

	memcpy(&pkt.CRC, src + HEADERSIZE + bodySize, crcSize);

	unsigned char calculatedCrc = 0;

	// Add all the (1) bits from all fields.
	calculatedCrc += std::bitset<8>(pkt.Head.PktCount).count();
	calculatedCrc += std::bitset<4>(pkt.Head.Drive).count();
	calculatedCrc += std::bitset<4>(pkt.Head.Status).count();
	calculatedCrc += std::bitset<4>(pkt.Head.Sleep).count();
	calculatedCrc += std::bitset<4>(pkt.Head.Ack).count();
	calculatedCrc += std::bitset<4>(pkt.Head.Padding).count();
	calculatedCrc += std::bitset<8>(pkt.Head.Length).count();

	// Adds the bits from the data is there are any.
	if (pkt.Data != nullptr) {
		struct DriveBody body = { 0 };
		memcpy(&body, pkt.Data, sizeof(struct DriveBody));
		calculatedCrc += std::bitset<8>(body.Direction).count();
		calculatedCrc += std::bitset<8>(body.Duration).count();
		calculatedCrc += std::bitset<8>(body.Speed).count();
	}

	delete[] pkt.Data;

	return pkt.CRC == calculatedCrc;
}
// Calculate the CRC for the current packet
void PktDef::CalcCRC() {
	unsigned char crc = 0;

	// Add all the (1) bits from all fields.
	crc += std::bitset<8>(Packet.Head.PktCount).count();
	crc += std::bitset<4>(Packet.Head.Drive).count();
	crc += std::bitset<4>(Packet.Head.Status).count();
	crc += std::bitset<4>(Packet.Head.Sleep).count();
	crc += std::bitset<4>(Packet.Head.Ack).count();
	crc += std::bitset<4>(Packet.Head.Padding).count();
	crc += std::bitset<8>(Packet.Head.Length).count();

	// Adds the bits from the data is there are any.
	if (Packet.Data != nullptr) {
		struct DriveBody body = { 0 };
		memcpy(&body, Packet.Data, sizeof(struct DriveBody));
		crc += std::bitset<8>(body.Direction).count();
		crc += std::bitset<8>(body.Duration).count();
		crc += std::bitset<8>(body.Speed).count();
	}

	Packet.CRC = crc;
}
// Generate a packet 
char* PktDef::GenPacket() {
	
	if (RawBuffer) {
		delete[] RawBuffer;
		RawBuffer = nullptr;

	}

	//calcualte CRC HERE
	CalcCRC();

	int crcSize = sizeof(Packet.CRC);
	int bodySize = Packet.Head.Length - HEADERSIZE - crcSize;

	RawBuffer = new char[Packet.Head.Length];

	memcpy(RawBuffer, &Packet.Head, HEADERSIZE);
	
	if (Packet.Data != nullptr) {
		memcpy(RawBuffer + HEADERSIZE, Packet.Data, bodySize);
	}
	
	memcpy(RawBuffer + HEADERSIZE + bodySize, &Packet.CRC, crcSize);

	return RawBuffer;
}


