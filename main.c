//Copyright (C) 2015  Corwin Hansen
#include "main.c"

int main(int argc, char *argv[]){
	if (argc != 3){//if there was not correct number of argument
		printf("Plese input file to compile and destination file");
		exit(EXIT_SUCCESS);//exit software
	}

	exit(EXIT_SUCCESS);//exit software
}

int IntBigEndian(void){//get if integer is in big endian

}

int FloatBigEndian(void){//get if float is in big endian

}

int WriteInt_16(FILE *file, uint16_t data){//write data to file in little endian. return 0 on succuess

}

int WriteInt_32(FILE *file, uint32_t data){//write data to file in little endian. return 0 on success

}

int WriteFloat(FILE *file, float data){//write data to file in little endian. return 0 on success

}

int WriteWav(FILE *file, float *data, uint32_t size){//write wav file with data of size elements. return 0 on success

}
