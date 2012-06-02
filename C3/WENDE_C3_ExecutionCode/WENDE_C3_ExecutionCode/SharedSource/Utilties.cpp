#include "StdAfx.h"
#include "Utilties.h"

CUtilties::CUtilties(void)
{
}

CUtilties::~CUtilties(void)
{
}
 int CUtilties::BytesToInt   ( BYTE * bytes)
{
#if 1
    int_or_bytes converter;
	memcpy(converter.bytes,bytes,SIZE_OF_INT);
	return converter.vInt;
#else
	int value = (bytes[0] << 24) +(bytes[1] << 16) +(bytes[2] << 8) +(bytes[3] << 0);
	return value;
#endif
}
 short  CUtilties::BytesToShort ( BYTE * bytes) 
{
#if 1
	short_or_bytes converter;
	memcpy(converter.bytes,bytes,SIZE_OF_SHORT);
	return converter.vShort;
#else
	int value = (bytes[0] << 24) +(bytes[1] << 16) +(bytes[2] << 8) +(bytes[3] << 0);
	return value;
#endif
}
 double CUtilties::BytesToDouble( BYTE * bytes)
{
	double_or_bytes converter;
	memcpy(converter.bytes,bytes,SIZE_OF_DOUBLE);
	return converter.vDoubloe;
}
 float  CUtilties::BytesToFloat ( BYTE * bytes)
{
	float_or_bytes converter;
	memcpy(converter.bytes,bytes,SIZE_OF_FLOAT);
	return converter.vFloat;
}
 void   CUtilties::IntToBytes   (BYTE * bytes,  int    value)
{
#if 1
	int_or_bytes converter;
	converter.vInt = value;
	memcpy(bytes,converter.bytes,SIZE_OF_INT);
#else
	bytes[0] = (value & 0xFF000000) >> 24; 
	bytes[1] = (value & 0xFF000000) >> 16; 
	bytes[2] = (value & 0xFF000000) >>  8; 
	bytes[3] = (value & 0xFF000000) >>  0; 
#endif
}
 void   CUtilties::ShortToBytes (BYTE * bytes,  short  value)
{
#if 1
	short_or_bytes converter;
	converter.vShort = value;
	memcpy(bytes,converter.bytes,SIZE_OF_SHORT);
#else
	bytes[0] = (value & 0xFF000000) >>  8; 
	bytes[1] = (value & 0xFF000000) >>  0; 
#endif
}
 void   CUtilties::DoubleToBytes(BYTE * bytes,  double value)
{
	double_or_bytes converter;
	converter.vDoubloe = value;
	memcpy(bytes,converter.bytes,SIZE_OF_DOUBLE);
}
 void   CUtilties::FloatToBytes (BYTE * bytes,  float  value)
{
	float_or_bytes converter;
	converter.vFloat = value;
	memcpy(bytes,converter.bytes,SIZE_OF_FLOAT);
}