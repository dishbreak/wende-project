///////////////////////////////////////////////////////////////////////////////////////////
//	File: Utilities.cpp
//  Programmer: Benjamin Kurt Heiner
//  Date: 06-03-2012
//
//	Purpose: This file contains ally converstion functions for converting froma basic tpye 
//           to a byte array
//
///////////////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Utilties.h"

///////////////////////////////////////////////////////////////////////////////////////////
// Function: CUtilities (default constructor)
// Programmer: Benjamin Kurt Heiner
//
// Purpose: default constructor
///////////////////////////////////////////////////////////////////////////////////////////
CUtilities::CUtilities(void)
{
}
///////////////////////////////////////////////////////////////////////////////////////////
// Function: ~CUtilities (default destructor)
// Programmer: Benjamin Kurt Heiner
//
// Purpose: default constructor
///////////////////////////////////////////////////////////////////////////////////////////
CUtilities::~CUtilities(void)
{
}
///////////////////////////////////////////////////////////////////////////////////////////
// Function: BytesToInt
// Programmer: Benjamin Kurt Heiner
//
// Purpose: convert ths bytes to int
///////////////////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////////////////
// Function: BytesToShort
// Programmer: Benjamin Kurt Heiner
//
// Purpose: convert ths bytes to short
///////////////////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////////////////
// Function: BytesToDouble
// Programmer: Benjamin Kurt Heiner
//
// Purpose: convert ths bytes to double
///////////////////////////////////////////////////////////////////////////////////////////
double CUtilities::BytesToDouble( BYTE * bytes)
{
	double_or_bytes converter;
	memcpy(converter.bytes,bytes,SIZE_OF_DOUBLE);
	return converter.vDoubloe;
}
///////////////////////////////////////////////////////////////////////////////////////////
// Function: BytesToFloat
// Programmer: Benjamin Kurt Heiner
//
// Purpose: convert ths bytes to float
///////////////////////////////////////////////////////////////////////////////////////////
float  CUtilities::BytesToFloat ( BYTE * bytes)
{
	float_or_bytes converter;
	memcpy(converter.bytes,bytes,SIZE_OF_FLOAT);
	return converter.vFloat;
}
///////////////////////////////////////////////////////////////////////////////////////////
// Function: IntToBytes
// Programmer: Benjamin Kurt Heiner
//
// Purpose: convert ths int to bytes
///////////////////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////////////////
// Function: ShortToBytes
// Programmer: Benjamin Kurt Heiner
//
// Purpose: convert ths short to bytes
///////////////////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////////////////
// Function: DoubleToBytes
// Programmer: Benjamin Kurt Heiner
//
// Purpose: convert ths double to bytes
///////////////////////////////////////////////////////////////////////////////////////////
void   CUtilities::DoubleToBytes(BYTE * bytes,  double value)
{
	double_or_bytes converter;
	converter.vDoubloe = value;
	memcpy(bytes,converter.bytes,SIZE_OF_DOUBLE);
}
///////////////////////////////////////////////////////////////////////////////////////////
// Function: FloatToBytes
// Programmer: Benjamin Kurt Heiner
//
// Purpose: convert ths float to bytes
///////////////////////////////////////////////////////////////////////////////////////////
void   CUtilities::FloatToBytes (BYTE * bytes,  float  value)
{
	float_or_bytes converter;
	converter.vFloat = value;
	memcpy(bytes,converter.bytes,SIZE_OF_FLOAT);
}