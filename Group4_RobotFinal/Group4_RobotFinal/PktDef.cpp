#include "PktDef.h"
#include <bitset>

PktDef::PktDef() {

	memset(&Packet.Head, 0, HEADERSIZE);
	Packet.Data = nullptr;
	Packet.CRC = 0;
	RawBuffer = nullptr;
}
PktDef::PktDef(char* src) {
	RawBuffer = nullptr;

	memcpy(&Packet.Head, src, HEADERSIZE);

	if (Packet.Data != nullptr) {
		delete[] Packet.Data;
		Packet.Data = nullptr;
	}

	Packet.Data = new char[Packet.Head.Length];

	memcpy(Packet.Data, src + HEADERSIZE, Packet.Head.Length);
	memcpy(&Packet.CRC, src + HEADERSIZE + Packet.Head.Length, sizeof(Packet.CRC));

}

PktDef::~PktDef()
{
	if (Packet.Data != nullptr) {
		delete[] Packet.Data;
	}

	if (RawBuffer != nullptr) {
		delete[] RawBuffer;
	}
}

void PktDef::SetCmd(CmdType Type) {

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
void PktDef::SetBodyData(char* srcData, int Size) {

	if (Packet.Data) {
		delete[] Packet.Data;
		Packet.Data = nullptr;
	}

	Packet.Data = new char[Size];

	memcpy(Packet.Data, srcData, Size);

	Packet.Head.Length = Size;
}
void PktDef::SetPktCount(int Count) {
	Packet.Head.PktCount = Count++;
}

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
bool PktDef::GetAck() {

	if (Packet.Head.Ack == 1)
		return true;
	else
		return false;

}
int PktDef::GetLength() {
	return Packet.Head.Length;
}
char* PktDef::GetBodyData() {
	return Packet.Data;
}
int PktDef::GetPktCount() {
	return Packet.Head.PktCount;
}

bool PktDef::CheckCRC(char* src, int Size) {
	struct CmdPacket pkt = { 0 };

	memcpy(&pkt.Head, src, HEADERSIZE);
	pkt.Data = nullptr;

	if (pkt.Head.Length > 0) {
		pkt.Data = new char[pkt.Head.Length];
		memcpy(pkt.Data, src + HEADERSIZE, pkt.Head.Length);
	}

	memcpy(&pkt.CRC, src + HEADERSIZE + pkt.Head.Length, sizeof(pkt.CRC));

	unsigned char calculatedCrc = 0;

	calculatedCrc += std::bitset<8>(pkt.Head.PktCount).count();
	calculatedCrc += std::bitset<4>(pkt.Head.Drive).count();
	calculatedCrc += std::bitset<4>(pkt.Head.Status).count();
	calculatedCrc += std::bitset<4>(pkt.Head.Sleep).count();
	calculatedCrc += std::bitset<4>(pkt.Head.Ack).count();
	calculatedCrc += std::bitset<4>(pkt.Head.Padding).count();
	calculatedCrc += std::bitset<8>(pkt.Head.Length).count();

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

void PktDef::CalcCRC() {
	unsigned char crc = 0;

	crc += std::bitset<8>(Packet.Head.PktCount).count();
	crc += std::bitset<4>(Packet.Head.Drive).count();
	crc += std::bitset<4>(Packet.Head.Status).count();
	crc += std::bitset<4>(Packet.Head.Sleep).count();
	crc += std::bitset<4>(Packet.Head.Ack).count();
	crc += std::bitset<4>(Packet.Head.Padding).count();
	crc += std::bitset<8>(Packet.Head.Length).count();

	if (Packet.Data != nullptr) {
		struct DriveBody body = { 0 };
		memcpy(&body, Packet.Data, sizeof(struct DriveBody));
		crc += std::bitset<8>(body.Direction).count();
		crc += std::bitset<8>(body.Duration).count();
		crc += std::bitset<8>(body.Speed).count();
	}

	Packet.CRC = crc;
}

char* PktDef::GenPacket() {
	
	if (RawBuffer) {
		delete[] RawBuffer;
		RawBuffer = nullptr;

	}

	//calcualte CRC HERE
	CalcCRC();

	int size = HEADERSIZE + Packet.Head.Length + sizeof(Packet.CRC);

	RawBuffer = new char[size];

	memcpy(RawBuffer, &Packet.Head, HEADERSIZE);
	memcpy(RawBuffer + HEADERSIZE, Packet.Data, Packet.Head.Length);
	memcpy(RawBuffer + HEADERSIZE + Packet.Head.Length, &Packet.CRC, sizeof(Packet.CRC));

	return RawBuffer;
}


