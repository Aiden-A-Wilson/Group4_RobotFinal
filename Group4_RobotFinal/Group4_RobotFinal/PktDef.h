#pragma once
#include <memory>
#include <iostream>

#define FORWARD 1
#define BACKWORD 2
#define RIGHT 3
#define LEFT 4

#define HEADERSIZE 4
#define BODYSIZE 3//

// enum for robot command types
enum CmdType { DRIVE, SLEEP, RESPONSE };

// The Header Information
#pragma pack(push,1)
struct Header {
	unsigned short int PktCount;

	unsigned char Drive : 1;
	unsigned char Status : 1;
	unsigned char Sleep : 1;
	unsigned char Ack : 1;
	unsigned char Padding : 4;

	unsigned char Length;
};
#pragma pack(pop)

// The Driving Parameters
struct DriveBody {
	unsigned char Direction;
	unsigned char Duration;
	unsigned char Speed;
};

class PktDef {	
	// Our packet
	struct CmdPacket {

		Header Head;
		char* Data;
		unsigned char CRC;

	} Packet;
	// a buffer to send packet
	char* RawBuffer;

public:

	PktDef();
	PktDef(char* src);
	~PktDef();

	void SetCmd(CmdType);
	void SetBodyData(char*, int);
	void SetPktCount(int);

	CmdType GetCmd();
	bool GetAck();
	int GetLength();
	char* GetBodyData();
	int GetPktCount();

	char* GenPacket();

	bool CheckCRC(char*, int);
	void CalcCRC();
};