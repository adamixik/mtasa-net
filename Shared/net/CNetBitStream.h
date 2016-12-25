//============== MTASA Network ==============
//
// Project: MTA:SA Network
// Author(s): adamix
// License: MIT
//
//===========================================

#pragma once

#include <StdInc.h>

#include <net/bitstream.h>

class CNetBitStream : public NetBitStreamInterface
{
public:
	CNetBitStream();
	CNetBitStream(RakNet::BitStream * bitStream);

	CNetBitStream(unsigned char * data, unsigned int length, bool copy);

	int         GetReadOffsetAsBits(void);
	void        SetReadOffsetAsBits(int iOffset);

	void        Reset(void);
	void        ResetReadPointer(void);

	// Don't use this, it screws up randomly in certain conditions causing packet misalign
	void        Write(const unsigned char& input);
	void        Write(const char& input);
	void        Write(const unsigned short& input);
	void        Write(const short& input);
	void        Write(const unsigned int& input);
	void        Write(const int& input);
	void        Write(const float& input);
	void        Write(const double& input);
	void        Write(const char* input, int numberOfBytes);
	void        Write(const ISyncStructure* syncStruct);

public:    // Use char functions only when they will be 0 most times
	void        WriteCompressed(const unsigned char& input);
	void        WriteCompressed(const char& input);
public:
	void        WriteCompressed(const unsigned short& input);
	void        WriteCompressed(const short& input);
	void        WriteCompressed(const unsigned int& input);
	void        WriteCompressed(const int& input);
private:    // Float functions not used because they only cover -1 to +1 and are lossy
	void        WriteCompressed(const float& input);
	void        WriteCompressed(const double& input);
public:
	void        WriteBits(const char* input, unsigned int numbits);
	void        WriteBit(bool input);

	// Write a normalized 3D vector, using (at most) 4 bytes + 3 bits instead of 12 bytes.  Will further compress y or z axis aligned vectors. Accurate to 1/32767.5.
	void        WriteNormVector(float x, float y, float z);

	// Write a vector, using 10 bytes instead of 12. Loses accuracy to about 3/10ths and only saves 2 bytes, so only use if accuracy is not important.
	void        WriteVector(float x, float y, float z);

	// Write a normalized quaternion in 6 bytes + 4 bits instead of 16 bytes.  Slightly lossy.
	void        WriteNormQuat(float w, float x, float y, float z);

	// Write an orthogonal matrix by creating a quaternion, and writing 3 components of the quaternion in 2 bytes each for 6 bytes instead of 36
	void        WriteOrthMatrix(
		float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22
	);

	bool        Read(unsigned char& output);
	bool        Read(char& output);
	bool        Read(unsigned short& output);
	bool        Read(short& output);
	bool        Read(unsigned int& output);
	bool        Read(int& output);
	bool        Read(float& output);
	bool        Read(double& output);
	bool        Read(char* output, int numberOfBytes);
	bool        Read(ISyncStructure* syncStruct);

public:    // Use char functions only when they will be 0 most times
	bool        ReadCompressed(unsigned char& output);
	bool        ReadCompressed(char& output);
public:
	bool        ReadCompressed(unsigned short& output);
	bool        ReadCompressed(short& output);
	bool        ReadCompressed(unsigned int& output);
	bool        ReadCompressed(int& output);
private:    // Float functions not used because they only cover -1 to +1 and are lossy
	bool        ReadCompressed(float& output);
	bool        ReadCompressed(double& output);
public:
	bool        ReadBits(char* output, unsigned int numbits);
	bool        ReadBit();

	bool        ReadNormVector(float &x, float &y, float &z);
	bool        ReadVector(float &x, float &y, float &z);
	bool        ReadNormQuat(float &w, float &x, float &y, float &z);
	bool        ReadOrthMatrix(
		float &m00, float &m01, float &m02,
		float &m10, float &m11, float &m12,
		float &m20, float &m21, float &m22
	);
	// GetNumberOfBitsUsed appears to round up to the next byte boundary, when reading
	int         GetNumberOfBitsUsed(void) const;
	int         GetNumberOfBytesUsed(void) const;
	// GetNumberOfUnreadBits appears to round up to the next byte boundary, when reading
	int         GetNumberOfUnreadBits(void) const;

	void        AlignWriteToByteBoundary(void) const;
	void        AlignReadToByteBoundary(void) const;

	unsigned char* GetData(void) const;

	unsigned short Version(void) const;

	RakNet::BitStream * GetBitStream() { return bitStream; }
private:
	RakNet::BitStream * bitStream;


};
