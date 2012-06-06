///////////////////////////////////////////////////////////////////////////////////////////
//	File: Utilities.h
//  Programmer: Benjamin Kurt Heiner
//  Date: 06-03-2012
//
//	Purpose: This file contains ally converstion functions for converting froma basic tpye 
//           to a byte array
//
///////////////////////////////////////////////////////////////////////////////////////////
#pragma once

///////////////////////////////////////////////////////////////////////////////////////////
// Size of basic types
///////////////////////////////////////////////////////////////////////////////////////////
#if 0
	#define SIZE_OF_INT    sizeof(int)
	#define SIZE_OF_SHORT  sizeof(short)
	#define SIZE_OF_DOUBLE sizeof(double)
	#define SIZE_OF_FLOAT  sizeof(float)
	#define SIZE_OF_BYTE   sizeof(BYTE)	
#else
	#define SIZE_OF_INT    4
	#define SIZE_OF_SHORT  2
	#define SIZE_OF_DOUBLE 8
	#define SIZE_OF_FLOAT  4
	#define SIZE_OF_BYTE   1
#endif

///////////////////////////////////////////////////////////////////////////////////////////
// Conversion from type to bytes
///////////////////////////////////////////////////////////////////////////////////////////
// convert from int to byte to int
union _int_or_bytes   { int    vInt     ; unsigned int vUInt;   char bytes[SIZE_OF_INT]    ; };
// convert from short to byte to short
union _short_or_bytes { short  vShort   ; unsigned int vUShort; char bytes[SIZE_OF_SHORT]  ; };
// convert from double to byte to double
union double_or_bytes { double vDoubloe ; char bytes[SIZE_OF_DOUBLE] ; };
// convert from float to byte to float
union float_or_bytes  { float  vFloat   ; char bytes[SIZE_OF_DOUBLE] ; };

///////////////////////////////////////////////////////////////////////////////////////////
// Type defines dor similar types
///////////////////////////////////////////////////////////////////////////////////////////
typedef _int_or_bytes    int_or_bytes;
typedef _int_or_bytes    uint_or_bytes;
typedef _short_or_bytes  short_or_bytes;
typedef _short_or_bytes  ushort_or_bytes;

static class CUtilities
{
///////////////////////////////////////////////////////////////////////////////////////////
// Public functions... basic clas items
///////////////////////////////////////////////////////////////////////////////////////////
public:
	// default constructor
	CUtilities(void);
	// defualt destructor
	~CUtilities(void);
///////////////////////////////////////////////////////////////////////////////////////////
// Public functions
///////////////////////////////////////////////////////////////////////////////////////////
public:
	// function to convert the bytes to INT
	static int    BytesToInt   ( BYTE * bytes);
	// function to convert the bytes to short
	static short  BytesToShort ( BYTE * bytes);
	// function to convert the bytes to double
	static double BytesToDouble( BYTE * bytes);
	// function to convert the bytes to flaot
	static float  BytesToFloat ( BYTE * bytes);
	// function to convert the int to bytes
	static void   IntToBytes   (BYTE * bytes,  int    value);
	// function to convert the short to bytes
	static void   ShortToBytes (BYTE * bytes,  short  value);
	// function to convert the double to bytes
	static void   DoubleToBytes(BYTE * bytes,  double value);
	// function to convert the float to bytes
	static void   FloatToBytes (BYTE * bytes,  float  value);
};
