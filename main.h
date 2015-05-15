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
void SineWave(Sound *data, uint32_t start, uint32_t end, double hz, int32_t ampritude, int32_t xshift, int32_t yshift);//add specified sign wave to that portion of data
void AddData(Sound *dest, Sound *src, uint32_t start);//add src to dest at start
void CopyData(Sound *dest, Sound *src, uint32_t start);//copy src to dest at start
void SquareWave(Sound *data, uint32_t start, uint32_t end, double hz, int32_t ampritude, int32_t xshift, int32_t yshift);//add specified square wave to that portion of data
void SawtoothWave(Sound *data, uint32_t start, uint32_t end, double hz, int32_t ampritude, int32_t xshift, int32_t yshift);//add specified sawtooth wave to that portion of data
void ReverseSawtoothWave(Sound *data, uint32_t start, uint32_t end, double hz, int32_t ampritude, int32_t xshift, int32_t yshift);//add specified reverse-sawtooth wave to that portion of data
void TriangleWave(Sound *data, uint32_t start, uint32_t end, double hz, int32_t ampritude, int32_t xshift, int32_t yshift);//add specified triangle wave to that portion of data
void Smooth(Sound *data, int32_t start, int32_t end);//smooth wave by taking avarage from start to end
void Cutoff(Sound *data, int32_t min, int32_t max);//cutoff the wave at min and max
void Amplify(Sound *data, double vol);//amplify sound by volume
void YShift(Sound *data, int32_t yshift);//shift wave in y direction
void FluctuatingSineWave(Sound *data, uint32_t start, uint32_t end, Sound *Hz, Sound *Ampritude, Sound *Xshift, Sound *Yshift);//add specified sign wave to that portion of data with each increment as 1/1000 hz
void FluctuatingSquareWave(Sound *data, uint32_t start, uint32_t end, Sound *Hz, Sound *Ampritude, Sound *Xshift, Sound *Yshift);//add specified square wave to that portion of data with each increment as 1/1000 hz
void FluctuatingSawtoothWave(Sound *data, uint32_t start, uint32_t end, Sound *Hz, Sound *Ampritude, Sound *Xshift, Sound *Yshift);//add specified sawtooth wave to that portion of data with each increment as 1/1000 hz
void FluctuatingReverseSawtoothWave(Sound *data, uint32_t start, uint32_t end, Sound *Hz, Sound *Ampritude, Sound *Xshift, Sound *Yshift);//add specified reverse-sawtooth wave to that portion of data with each increment as 1/1000 hz
void FluctuatingTriangleWave(Sound *data, uint32_t start, uint32_t end, Sound *Hz, Sound *Ampritude, Sound *Xshift, Sound *Yshift);//add specified triangle wave to that portion of data with each increment as 1/1000 hz
void FluctuatingSmooth(Sound *data, Sound *Start, Sound *End);//smooth wave by taking avarage from start to end
void FluctuatingCutoff(Sound *data, Sound *Min, Sound *Max);//cutoff the wave at min and max
void FluctuatingAmplify(Sound *data, Sound *Vol);//amplify sound by volume with INT32_MAX as 2




#endif