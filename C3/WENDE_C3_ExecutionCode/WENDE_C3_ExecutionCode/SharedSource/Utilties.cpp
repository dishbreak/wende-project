#include "StdAfx.h"
#include "Utilties.h"

CUtilities::CUtilities(void)
{
}

CUtilities::~CUtilities(void)
{
}
 int CUtilities::BytesToInt   ( BYTE * bytes)
{
#if 1
    int_or_bytes converter;
	memcpy(converter.bytes,bytes,SIZE_OF_INT);
	return converter.vInt;
#else
	int value = (bytes[0] << 24) +(bytes[1] << 16) +(bytes[2] << 8) +(bytes[3] << 0);
//	0xFF000000
	return value;
#endif
}
 short  CUtilities::BytesToShort ( BYTE * bytes) 
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
 double CUtilities::BytesToDouble( BYTE * bytes)
{
	double_or_bytes converter;
	memcpy(converter.bytes,bytes,SIZE_OF_DOUBLE);
	return converter.vDoubloe;
}
 float  CUtilities::BytesToFloat ( BYTE * bytes)
{
	float_or_bytes converter;
	memcpy(converter.bytes,bytes,SIZE_OF_FLOAT);
	return converter.vFloat;
}
 void   CUtilities::IntToBytes   (BYTE * bytes,  int    value)
{
#if 1
	int_or_bytes converter;
	converter.vInt = value;
	memcpy(bytes,converter.bytes,SIZE_OF_INT);
#else
	bytes[0] = (value & 0xFF000000) >> 24; 
	bytes[1] = (value & 0x00FF0000) >> 16; 
	bytes[2] = (value & 0x0000FF00) >>  8; 
	bytes[3] = (value & 0x000000FF) >>  0; 
#endif
}
 void   CUtilities::ShortToBytes (BYTE * bytes,  short  value)
{
#if 1
	short_or_bytes converter;
	converter.vShort = value;
	memcpy(bytes,converter.bytes,SIZE_OF_SHORT);
#else
	bytes[0] = (value & 0xFF00) >>  8; 
	bytes[1] = (value & 0x00FF) >>  0; 
#endif
}
 void   CUtilities::DoubleToBytes(BYTE * bytes,  double value)
{
	double_or_bytes converter;
	converter.vDoubloe = value;
	memcpy(bytes,converter.bytes,SIZE_OF_DOUBLE);
}
 void   CUtilities::FloatToBytes (BYTE * bytes,  float  value)
{
	float_or_bytes converter;
	converter.vFloat = value;
	memcpy(bytes,converter.bytes,SIZE_OF_FLOAT);
}