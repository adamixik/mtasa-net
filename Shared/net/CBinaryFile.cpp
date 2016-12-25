//============== MTASA Network ==============
//
// Project: MTA:SA Network
// Author(s): adamix
// License: MIT
//
//===========================================

#include <StdInc.h>
#include "CBinaryFile.h"

bool CBinaryFile::FOpen(const char * szFilename, const char * szMode, bool bValidate)
{
	this->file = fopen(szFilename, szMode);
	return file != NULL;
}

void CBinaryFile::FClose(void)
{
	fclose(file);
}

bool CBinaryFile::FEof(void)
{
	return feof(file);
}

void CBinaryFile::FFlush(void)
{
	fflush(file);
}

int CBinaryFile::FTell(void)
{
	return ftell(file);
}

void CBinaryFile::FSeek(int iOffset, int iOrigin)
{
	fseek(file, iOffset, iOrigin);
}

int CBinaryFile::FRead(void * pData, uint uiSize)
{

	return fread(pData, uiSize, 1, file);
}

int CBinaryFile::FWrite(const void * pData, uint uiSize)
{
	return fwrite(pData, uiSize, 1, file);
}
