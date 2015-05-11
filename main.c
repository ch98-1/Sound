//Copyright (C) 2015  Corwin Hansen
#include "main.h"

int main(int argc, char *argv[]){
	if (argc != 3){//if there was not correct number of argument
		printf("Plese input file to compile and destination file");
		exit(EXIT_SUCCESS);//exit program
	}

	Src = fopen(argv[1], "r");//open source file
	if (Src == NULL){//if file could not be opened
		printf("Source file %s could not be opened\n", argv[1]);//send error meaaage
		exit(EXIT_SUCCESS);//exit program
	}

	Dest = fopen(argv[2], "w");//open destination file
	if (Dest == NULL){//if file could not be opened
		printf("Source file %s could not be opened\n", argv[2]);//send error meaaage
		exit(EXIT_SUCCESS);//exit program
	}
	
	MainData = AllocateData(WAV_SAMPLE_PER_SECOND * 10);//allocate memory for main data
	
	SineWave(MainData, 0, WAV_SAMPLE_PER_SECOND * 10, 200, INT32_MAX, 0, 0);//add sign wave to main data
	
	//write sound file
	if (WriteWav(Dest, MainData)){//if there was some error
		printf("Could not write to file correctly\n");//send error message
	}

	FreeData(MainData);//free main data

	exit(EXIT_SUCCESS);//exit program
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
	if (fwrite(&data, 2, 1, file) != 1){//write that data to file and check if correct number of element was written
		return 1;//error writing data
	}
	return 0;
}

int WriteInt_32(FILE *file, int32_t data){//write data to file in little endian. return 0 on success
	if (IntBigEndian()){
		int32_t num = data;//get data copy
		data = (num << 24) | ((num << 8) & BYTE_1) | ((num >> 8) & BYTE_2) | (num >> 24);//swap bytes
	}
	if (fwrite(&data, 4, 1, file) != 1){//write that data to file and check if correct number of element was written
		return 1;//error writing data
	}
	return 0;
}


int WriteUInt_16(FILE *file, uint16_t data){//write data to file in little endian. return 0 on succuess
	if (IntBigEndian()){
		uint16_t num = data;//get data copy
		data = (num << 8) | (num >> 8);//swap bytes
	}
	if (fwrite(&data, 2, 1, file) != 1){//write that data to file and check if correct number of element was written
		return 1;//error writing data
	}
	return 0;
}

int WriteUInt_32(FILE *file, uint32_t data){//write data to file in little endian. return 0 on success
	if (IntBigEndian()){
		uint32_t num = data;//get data copy
		data = (num << 24) | ((num << 8) & BYTE_1) | ((num >> 8) & BYTE_2) | (num >> 24);//swap bytes
	}
	if (fwrite(&data, 4, 1, file) != 1){//write that data to file and check if correct number of element was written
		return 1;//error writing data
	}
	return 0;
}

int WriteStr(FILE *file, char* str, uint32_t size){//write data to file. return 0 on success
	if (fwrite(str, 1, size, file) != size){//write that data to file and check if correct number of element was written
		return 1;//error writing data
	}
	return 0;
}

int WriteWav(FILE *file, Sound *data){//write wav file with data of size elements. return 0 on success
	int error = 0;//number of error
	error += WriteStr(file, WAV_CHUNK_ID, 4);//write each data
	error += WriteUInt_32(file, WAV_CHUNK_SIZE_NO_DATA + data->DataSize * WAV_BYTE_PER_SAMPLE * WAV_CHANNEL);
	error += WriteStr(file, WAV_WAVE_ID, 4);
	error += WriteStr(file, WAV_FMT_CHUNK_ID, 4);
	error += WriteUInt_32(file, WAV_CHUNK_SIZE);
	error += WriteUInt_16(file, WAV_FORMAT);
	error += WriteUInt_16(file, WAV_CHANNEL);
	error += WriteUInt_32(file, WAV_SAMPLE_PER_SECOND);
	error += WriteUInt_32(file, WAV_BYTE_PER_SECOND);
	error += WriteUInt_16(file, WAV_BLOCK_ALIGN);
	error += WriteUInt_16(file, WAV_BITS_PER_SAMPLE);
	error += WriteUInt_16(file, WAV_EXTENSION_SIZE);
	error += WriteUInt_16(file, WAV_VALID_BIT_PER_SAMPLE);
	error += WriteUInt_32(file, WAV_SPEAKER_POS_MASK);
	error += WriteUInt_16(file, WAV_SUB_FORMAT);
	error += WriteStr(file, WAV_GUID, 14);
	error += WriteStr(file, WAV_FACT_CHUNK_ID, 4);
	error += WriteUInt_32(file, WAV_FACT_CHUNK_SIZE);
	error += WriteUInt_32(file, data->DataSize * WAV_CHANNEL);
	error += WriteStr(file, WAV_DATA_CHUNK_ID, 4);
	error += WriteUInt_32(file, data->DataSize * WAV_BYTE_PER_SAMPLE * WAV_CHANNEL);
	uint32_t i;
	for (i = 0; i < data->DataSize; i++){//for each data sample
		error += WriteInt_32(file, data->Sound[i]);//write that data
	}
	return error;//return number of error
}

Sound *AllocateData(uint32_t size){//allocate memory for data with size elements
	Sound *data;
	data = malloc(sizeof(Sound));//allocate memory for data structure
	data->Sound = calloc(size, WAV_BYTE_PER_SAMPLE * WAV_CHANNEL);//allocate memory for dound data
	data->DataSize = size;//set data size
	return data;
}

void FreeData(Sound *data){//free memory for data
	free(data->Sound);//free data
	free(data);//free data structure
}

void SineWave(Sound *data, uint32_t start, uint32_t end, double hz, uint32_t ampritude, int32_t xshift, int32_t yshift){//add specified sign wave to that portion of data
	int32_t i;
	for (i = start; i <= end; i++){//for each in portion of data
		data->Sound[i] += (int32_t)(ampritude * sin(2.0 * PI * hz * (i - xshift) / WAV_SAMPLE_PER_SECOND)) + yshift;//get that sign wave
	}
}

void AddData(Sound *dest, Sound *src, uint32_t start){//add src to dest at start
	uint32_t i;
	for (i = 0; i <= src->DataSize; i++){//for each of source
		dest->Sound[i + start] += src->Sound[i];//add data
	}
}

void CopyData(Sound *dest, Sound *src, uint32_t start){//copy src to dest at start
	uint32_t i;
	for (i = start; i <= src->DataSize; i++){//for each of source
		dest->Sound[i + start] = src->Sound[i];//copy data
	}
}