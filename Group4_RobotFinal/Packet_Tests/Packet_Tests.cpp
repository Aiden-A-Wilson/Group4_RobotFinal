#include "pch.h"
#include "CppUnitTest.h"
#include "PktDef.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PacketTests
{
	TEST_CLASS(PacketTests)
	{
	public:
		
		TEST_METHOD(SetCmd_WithDrive_SetsTheCommandType)
		{
			// Arrange
			PktDef pkt = PktDef();
			enum CmdType expectedCmd = DRIVE;
			
			// Act
			pkt.SetCmd(DRIVE);

			// Assert
			Assert::AreEqual((int)expectedCmd, (int)pkt.GetCmd());
		}

		TEST_METHOD(SetCmd_WithResponse_SetsTheCommandType)
		{
			// Arrange
			PktDef pkt = PktDef();
			enum CmdType expectedCmd = RESPONSE;

			// Act
			pkt.SetCmd(RESPONSE);

			// Assert
			Assert::AreEqual((int)expectedCmd, (int)pkt.GetCmd());
		}

		TEST_METHOD(SetCmd_WithSleep_SetsTheCommandType)
		{
			// Arrange
			PktDef pkt = PktDef();
			enum CmdType expectedCmd = SLEEP;

			// Act
			pkt.SetCmd(SLEEP);

			// Assert
			Assert::AreEqual((int)expectedCmd, (int)pkt.GetCmd());
		}

		TEST_METHOD(SetCmd_WithInvalidOption_ReturnsNegatveOne)
		{
			// Arrange
			PktDef pkt = PktDef();
			enum CmdType expectedCmd = (CmdType)-1;

			// Act
			pkt.SetCmd((CmdType)99);

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

		TEST_METHOD(GetAck_WithNoAcknowledgement_ReturnsZero) {
			// Arrange
			PktDef pkt = PktDef();

			// Act
			bool actual = pkt.GetAck();

			// Assert
			Assert::IsFalse(actual);
		}

		TEST_METHOD(Constructor_WithParameter_FillsPacket) {
			// Arrange
			PktDef pkt = PktDef();
			struct PktDef::DriveBody body = { 0 };
			body.Direction = 1;
			body.Duration = 10;
			body.Speed = 80;
			pkt.SetCmd(DRIVE);
			pkt.SetBodyData((char*)&body, sizeof(body));

			// Act
			PktDef newPkt = PktDef(pkt.GenPacket());
			
			// Assert
			int size = HEADERSIZE + newPkt.GetLength() + sizeof(unsigned char);
			struct PktDef::DriveBody actual = { 0 };
			memcpy(&actual, newPkt.GetBodyData(), sizeof(actual));
			Assert::AreEqual((int)DRIVE, (int)newPkt.GetCmd());
			Assert::AreEqual((unsigned char)1, actual.Direction);
			Assert::AreEqual((unsigned char)10, actual.Duration);
			Assert::AreEqual((unsigned char)80, actual.Speed);
			Assert::IsTrue(newPkt.CheckCRC(newPkt.GenPacket(), size));
		}
	};
}
