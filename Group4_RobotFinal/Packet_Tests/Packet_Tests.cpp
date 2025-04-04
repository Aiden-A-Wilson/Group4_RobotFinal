#include "pch.h"
#include "CppUnitTest.h"
#include "PktDef.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PacketTests
{
	TEST_CLASS(PacketTests)
	{
	public:
		
		TEST_METHOD(SetCmd_SetsTheCommandType)
		{
			// Arrange
			PktDef pkt = PktDef();
			enum CmdType expectedCmd = DRIVE;
			
			// Act
			pkt.SetCmd(DRIVE);

			// Assert
			Assert::AreEqual((int)expectedCmd, (int)pkt.GetCmd());
		}

		TEST_METHOD(SetBodyData_SetsThePacketBody) {
			// Arrange
			PktDef pkt = PktDef();
			struct PktDef::DriveBody body = { 0 };
			body.Direction = 1;
			body.Duration = 10;
			body.Speed = 80;

			// Act
			pkt.SetBodyData((char*)&body, sizeof(body));

			// Assert
			struct PktDef::DriveBody actual = { 0 };
			memcpy(&actual, pkt.GetBodyData(), sizeof(actual));
			Assert::AreEqual((unsigned char)1, actual.Direction);
			Assert::AreEqual((unsigned char)10, actual.Duration);
			Assert::AreEqual((unsigned char)80, actual.Speed);
		}

		TEST_METHOD(SetPktCount_SetsThePacketCount) {
			// Arrange
			PktDef pkt = PktDef();
			int expectedCount = 4;

			// Act
			pkt.SetPktCount(4);

			// Assert
			Assert::AreEqual(expectedCount, pkt.GetPktCount());
		}

		TEST_METHOD(CheckCRC_WithCalculatedCRC_ReturnsTrue) {
			// Arrange
			PktDef pkt = PktDef();
			pkt.SetCmd(DRIVE);
			struct PktDef::DriveBody body = { 0 };
			body.Direction = 1;
			body.Duration = 10;
			body.Speed = 80;
			pkt.SetBodyData((char*)&body, sizeof(body));

			// Act
			pkt.CalcCRC();

			// Assert
			int totalSize = HEADERSIZE + pkt.GetLength() + sizeof(unsigned char);
			Assert::IsTrue(pkt.CheckCRC(pkt.GenPacket(), totalSize));
		}
	};
}
