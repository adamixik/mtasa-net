//============== MTASA Network ==============
//
// Project: MTA:SA Network
// Author(s): adamix
// License: MIT
//
//===========================================

#include <StdInc.h>
#include "CNetBitStream.h"

CNetBitStream::CNetBitStream()
{
	this->bitStream = new RakNet::BitStream(65536);
}

CNetBitStream::CNetBitStream(RakNet::BitStream * bitStream)
{
	this->bitStream = bitStream;
}

CNetBitStream::CNetBitStream(unsigned char * data, unsigned int length, bool copy)
{
	if(data == NULL || data[0] == NULL)
		this->bitStream = new RakNet::BitStream(65536);
	else
		this->bitStream = new RakNet::BitStream(data, length, true);
}

int CNetBitStream::GetReadOffsetAsBits(void)
{
	bitStream->GetReadOffset();
	return 0;
}

void CNetBitStream::SetReadOffsetAsBits(int iOffset)
{
	bitStream->SetReadOffset(iOffset);
}

void CNetBitStream::Reset(void)
{
	bitStream->Reset();
}

void CNetBitStream::ResetReadPointer(void)
{
	bitStream->ResetReadPointer();
}

void CNetBitStream::Write(const unsigned char & input)
{
	bitStream->Write(input);
}

void CNetBitStream::Write(const char & input)
{
	bitStream->Write(input);
}

void CNetBitStream::Write(const unsigned short & input)
{
	bitStream->Write(input);
}

void CNetBitStream::Write(const short & input)
{
	bitStream->Write(input);
}

void CNetBitStream::Write(const unsigned int & input)
{
	bitStream->Write(input);
}

void CNetBitStream::Write(const int & input)
{
	bitStream->Write(input);
}

void CNetBitStream::Write(const float & input)
{
	bitStream->Write(input);
}

void CNetBitStream::Write(const double & input)
{
	bitStream->Write(input);
}

void CNetBitStream::Write(const char * input, int numberOfBytes)
{
	bitStream->Write(input, numberOfBytes);
}

void CNetBitStream::Write(const ISyncStructure * syncStruct)
{
	syncStruct->Write(*this);
}

void CNetBitStream::WriteCompressed(const unsigned char & input)
{
	bitStream->WriteCompressed(input);
}

void CNetBitStream::WriteCompressed(const char & input)
{
	bitStream->WriteCompressed(input);
}

void CNetBitStream::WriteCompressed(const unsigned short & input)
{
	bitStream->WriteCompressed(input);
}

void CNetBitStream::WriteCompressed(const short & input)
{
	bitStream->WriteCompressed(input);
}

void CNetBitStream::WriteCompressed(const unsigned int & input)
{
	bitStream->WriteCompressed(input);
}

void CNetBitStream::WriteCompressed(const int & input)
{
	bitStream->WriteCompressed(input);
}

void CNetBitStream::WriteCompressed(const float & input)
{
	bitStream->WriteCompressed(input);
}

void CNetBitStream::WriteCompressed(const double & input)
{
	bitStream->WriteCompressed(input);
}

void CNetBitStream::WriteBits(const char * input, unsigned int numbits)
{
	bitStream->WriteBits((unsigned char*)input, numbits);
}

void CNetBitStream::WriteBit(bool input)
{
	if (input)
		bitStream->Write1();
	else
		bitStream->Write0();
}

void CNetBitStream::WriteNormVector(float x, float y, float z)
{
	bitStream->Write(x);
	bitStream->Write(y);
	bitStream->Write(z);
}

void CNetBitStream::WriteVector(float x, float y, float z)
{
	bitStream->Write(x);
	bitStream->Write(y);
	bitStream->Write(z);
}

void CNetBitStream::WriteNormQuat(float w, float x, float y, float z)
{
	bitStream->Write(w);
	bitStream->Write(x);
	bitStream->Write(y);
	bitStream->Write(z);
}

void CNetBitStream::WriteOrthMatrix(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
{
	bitStream->Write(m00);
	bitStream->Write(m01);
	bitStream->Write(m02);
	bitStream->Write(m10);
	bitStream->Write(m11);
	bitStream->Write(m12);
	bitStream->Write(m20);
	bitStream->Write(m21);
	bitStream->Write(m22);
}

bool CNetBitStream::Read(unsigned char & output)
{
	return bitStream->Read(output);
}

bool CNetBitStream::Read(char & output)
{
	return bitStream->Read(output);
}

bool CNetBitStream::Read(unsigned short & output)
{
	return bitStream->Read(output);
}

bool CNetBitStream::Read(short & output)
{
	return bitStream->Read(output);
}

bool CNetBitStream::Read(unsigned int & output)
{
	return bitStream->Read(output);
}

bool CNetBitStream::Read(int & output)
{
	return bitStream->Read(output);
}

bool CNetBitStream::Read(float & output)
{
	return bitStream->Read(output);
}

bool CNetBitStream::Read(double & output)
{
	return bitStream->Read(output);
}

bool CNetBitStream::Read(char * output, int numberOfBytes)
{
	return bitStream->Read(output, numberOfBytes);
}

bool CNetBitStream::Read(ISyncStructure * syncStruct)
{
	syncStruct->Read(*this);
	return true;
}

bool CNetBitStream::ReadCompressed(unsigned char & output)
{
	return bitStream->ReadCompressed(output);
}

bool CNetBitStream::ReadCompressed(char & output)
{
	return bitStream->ReadCompressed(output);
}

bool CNetBitStream::ReadCompressed(unsigned short & output)
{
	return bitStream->ReadCompressed(output);
}

bool CNetBitStream::ReadCompressed(short & output)
{
	return bitStream->ReadCompressed(output);
}

bool CNetBitStream::ReadCompressed(unsigned int & output)
{
	return bitStream->ReadCompressed(output);
}

bool CNetBitStream::ReadCompressed(int & output)
{
	return bitStream->ReadCompressed(output);
}

bool CNetBitStream::ReadCompressed(float & output)
{
	return bitStream->ReadCompressed(output);
}

bool CNetBitStream::ReadCompressed(double & output)
{
	return bitStream->ReadCompressed(output);
}

bool CNetBitStream::ReadBits(char * output, unsigned int numbits)
{
	return bitStream->ReadBits((unsigned char*)output, numbits);
}

bool CNetBitStream::ReadBit()
{
	return bitStream->ReadBit();
}

bool CNetBitStream::ReadNormVector(float & x, float & y, float & z)
{
	return false;
}

bool CNetBitStream::ReadVector(float & x, float & y, float & z)
{
	return false;
}

bool CNetBitStream::ReadNormQuat(float & w, float & x, float & y, float & z)
{
	return false;
}

bool CNetBitStream::ReadOrthMatrix(float & m00, float & m01, float & m02, float & m10, float & m11, float & m12, float & m20, float & m21, float & m22)
{
	return false;
}

int CNetBitStream::GetNumberOfBitsUsed(void) const
{
	return bitStream->GetNumberOfBitsUsed();
}

int CNetBitStream::GetNumberOfBytesUsed(void) const
{
	return bitStream->GetNumberOfBytesUsed();
}

int CNetBitStream::GetNumberOfUnreadBits(void) const
{
	return bitStream->GetNumberOfUnreadBits();
}

void CNetBitStream::AlignWriteToByteBoundary(void) const
{
	bitStream->AlignWriteToByteBoundary();
}

void CNetBitStream::AlignReadToByteBoundary(void) const
{
	bitStream->AlignReadToByteBoundary();
}

unsigned char * CNetBitStream::GetData(void) const
{
	return bitStream->GetData();
}

unsigned short CNetBitStream::Version(void) const
{
	return 0x10;
}
