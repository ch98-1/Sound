//Copyright (C) 2015  Corwin Hansen
#ifndef MAIN_H//include protection
	#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define FORMAT (0xfffe)
#define SUB_FORMAT (0x0001)
#define BITRATE 192000








int IntBigEndian(void);//get if integer is in big endian
int WriteInt_16(FILE *file, int16_t data);//write data to file in little endian. return 0 on succuess
int WriteInt_32(FILE *file, int32_t data);//write data to file in little endian. return 0 on success
int WriteWav(FILE *file, int32_t *data, uint32_t size);//write wav file with data of size elements. return 0 on success


#endif