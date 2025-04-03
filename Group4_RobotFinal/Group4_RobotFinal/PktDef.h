#pragma once
#include <memory>
#include <iostream>
#include <fstream>

#define FORWARD 1
#define BACKWORD 2
#define RIGHT 3
#define LEFT 4

#define HEADERSIZE 6

enum CmdType { DRIVE, SLEEP, RESPONSE };

class PktDef {	

	struct CmdPacket {

		struct Header {
			unsigned short int PktCount;

			unsigned char Drive : 1;
			unsigned char Status : 1;
			unsigned char Sleep : 1;
			unsigned char Ack : 1;
			unsigned char Padding : 4;

			unsigned short int Length;

		} Head;

		char* Data;							//The data bytes
		unsigned char CRC;					//Cyclic Redundancy Check

	}Packet;
	
	struct DriveBody {
		unsigned char Direction;
		unsigned char Duration;
		unsigned char Speed;

	};

	char* RawBuffer;

public:

	PktDef();
	PktDef(char* src);

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