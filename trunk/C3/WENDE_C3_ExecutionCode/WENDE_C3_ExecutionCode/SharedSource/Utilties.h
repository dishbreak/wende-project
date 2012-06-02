#pragma once

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

union _int_or_bytes   { int    vInt     ; unsigned int vUInt;   char bytes[SIZE_OF_INT]    ; };
union _short_or_bytes { short  vShort   ; unsigned int vUShort; char bytes[SIZE_OF_SHORT]  ; };
union double_or_bytes { double vDoubloe ; char bytes[SIZE_OF_DOUBLE] ; };
union float_or_bytes  { float  vFloat   ; char bytes[SIZE_OF_DOUBLE] ; };

typedef _int_or_bytes    int_or_bytes;
typedef _int_or_bytes    uint_or_bytes;
typedef _short_or_bytes  short_or_bytes;
typedef _short_or_bytes  ushort_or_bytes;

static class CUtilities
{
public:
	CUtilities(void);
	~CUtilities(void);

public:
	
	static int    BytesToInt   ( BYTE * bytes);
	static short  BytesToShort ( BYTE * bytes);
	static double BytesToDouble( BYTE * bytes);
	static float  BytesToFloat ( BYTE * bytes);

	static void   IntToBytes   (BYTE * bytes,  int    value);
	static void   ShortToBytes (BYTE * bytes,  short  value);
	static void   DoubleToBytes(BYTE * bytes,  double value);
	static void   FloatToBytes (BYTE * bytes,  float  value);
};