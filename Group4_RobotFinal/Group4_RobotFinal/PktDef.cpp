#include "PktDef.h"

PktDef::PktDef() {

	memset(&Packet.Head, 0, HEADERSIZE);
	Packet.Data = nullptr;
	Packet.CRC = 0;

}
PktDef::PktDef(char* src) {

	RawBuffer = nullptr;

	memcpy(&Packet.Head, src, HEADERSIZE);
	memcpy(Packet.Data, src + HEADERSIZE, Packet.Head.Length);
	memcpy(&Packet.CRC, src + HEADERSIZE + Packet.Head.Length, sizeof(Packet.CRC));

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

	Packet.Data = new char[Size + 1];

	memcpy(Packet.Data, srcData, Size);

	Packet.Head.Length = Size;
	Packet.Data[Size] = '\0';
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


}
void PktDef::CalcCRC() {


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


