//Copyright (C) 2015  Corwin Hansen
#include "main.h"

int main(int argc, char *argv[]){
	if (argc != 3){//if there was not correct number of argument
		printf("Plese input file to compile and destination file");
		exit(EXIT_SUCCESS);//exit software
	}

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
	if (IntBigEndian()){
		int16_t num = data;//get data copy
		data = (num << 8) | (num >> 8);//swap bytes
	}
}

int WriteInt_32(FILE *file, int32_t data){//write data to file in little endian. return 0 on success
	if (IntBigEndian()){
		int32_t num = data;//get data copy
		data = (num << 24) | ((num << 8) & BYTE_1) | ((num >> 8) & BYTE_2) | (num >> 24);//swap bytes
	}

}


int WriteUInt_16(FILE *file, uint16_t data){//write data to file in little endian. return 0 on succuess
	if (IntBigEndian()){
		uint16_t num = data;//get data copy
		data = (num << 8) | (num >> 8);//swap bytes
	}

}

int WriteUInt_32(FILE *file, uint32_t data){//write data to file in little endian. return 0 on success
	if (IntBigEndian()){
		uint32_t num = data;//get data copy
		data = (num << 24) | ((num << 8) & BYTE_1) | ((num >> 8) & BYTE_2) | (num >> 24);//swap bytes
	}

}

int WriteWav(FILE *file, int32_t *data, uint32_t size){//write wav file with data of size. return 0 on success

}
