//Copyright (C) 2015  Corwin Hansen
#include "main.h"

int main(int argc, char *argv[]){
	if (argc != 3){//if there was not correct number of argument
		printf("Plese input file to compile and destination file");
		exit(EXIT_SUCCESS);//exit software
	}
	printf("%u", IntBigEndian());
	exit(EXIT_SUCCESS);//exit software
}

int IntBigEndian(void){//get if integer is in big endian
	union {
		uint32_t l;
		uint8_t c[4];
	} test = { 0x01020304 };

	return test.c[0] == 0x01;
}

int WriteInt_16(FILE *file, int16_t data){//write data to file in little endian. return 0 on succuess

}

int WriteInt_32(FILE *file, int32_t data){//write data to file in little endian. return 0 on success

}

int WriteWav(FILE *file, int32_t *data, uint32_t size){//write wav file with data of size. return 0 on success

}
