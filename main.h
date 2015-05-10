//Copyright (C) 2015  Corwin Hansen
#ifndef MAIN_H//include protection
	#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#define BYTE_0 0xff000000//bitmask for each byte
#define BYTE_1 0xff0000
#define BYTE_2 0xff00
#define BYTE_3 0xff

#define INT32_MAX (int32_t) 0x7FFFFFFF //maximum integer value
#define PI 3.141592653589793 //value of pi

#define WAV_CHUNK_ID "RIFF"
#define WAV_CHUNK_SIZE_NO_DATA (4 + 48 + 12 + 8)
#define WAV_WAVE_ID "WAVE"
#define WAV_FMT_CHUNK_ID "fmt "
#define WAV_CHUNK_SIZE 40
#define WAV_FORMAT (0xFFFE)
#define WAV_CHANNEL 1
#define WAV_SAMPLE_PER_SECOND 192000
#define WAV_BYTE_PER_SAMPLE 4
#define WAV_BYTE_PER_SECOND (WAV_SAMPLE_PER_SECOND * WAV_BYTE_PER_SAMPLE * WAV_CHANNEL)
#define WAV_BLOCK_ALIGN (WAV_BYTE_PER_SAMPLE * WAV_CHANNEL)
#define WAV_BITS_PER_SAMPLE (8 * WAV_BYTE_PER_SAMPLE)
#define WAV_EXTENSION_SIZE 22
#define WAV_VALID_BIT_PER_SAMPLE WAV_BITS_PER_SAMPLE
#define WAV_SPEAKER_POS_MASK (0x00000000)
#define WAV_SUB_FORMAT (0x0001)
#define WAV_GUID "\x00\x00\x00\x00\x10\x00\x80\x00\x00\xAA\x00\x38\x9B\x71"
#define WAV_FACT_CHUNK_ID "fact"
#define WAV_FACT_CHUNK_SIZE 4
#define WAV_DATA_CHUNK_ID "data"


FILE *Src;//source file
FILE *Dest;//destination file


typedef struct sound{//data structure
	int32_t *Sound;//sound data
	uint32_t DataSize;//size of sound data
}Sound;


Sound *MainData;//main sound data





int IntBigEndian(void);//get if integer is in big endian
int WriteInt_16(FILE *file, int16_t data);//write data to file in little endian. return 0 on succuess
int WriteInt_32(FILE *file, int32_t data);//write data to file in little endian. return 0 on success
int WriteUInt_16(FILE *file, uint16_t data);//write data to file in little endian. return 0 on succuess
int WriteUInt_32(FILE *file, uint32_t data);//write data to file in little endian. return 0 on success
int WriteStr(FILE *file, char* str, uint32_t size);//write data to file. return 0 on success
int WriteWav(FILE *file, Sound *data);//write wav file with data of size elements. return 0 on success
Sound *AllocateData(uint32_t size);//allocate memory for data with size elements
void FreeData(Sound *data);//free memory for data
void SineWave(Sound *data, uint32_t start, uint32_t end, double hz, uint32_t ampritude, int32_t xshift, int32_t yshift);//add specified sign wave to that portion of data
void AddData(Sound *dest, Sound *src, uint32_t start);//add src to dest at start
void CopyData(Sound *dest, Sound *src, uint32_t start);//copy src to dest at start



#endif