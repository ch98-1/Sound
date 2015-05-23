//Copyright (C) 2015  Corwin Hansen
#include "main.h"

int main(int argc, char *argv[]){
	SetError(exit_failure);//set function to run in error

	if (argc != 3){//if there was not correct number of argument
		printf("Plese input file to compile and destination file");
		exit(EXIT_SUCCESS);//exit program
	}

	SrcFile = fopen(argv[1], "rb");//open source file
	if (SrcFile == NULL){//if file could not be opened
		printf("Source file %s could not be opened\n", argv[1]);//send error meaaage
		exit(EXIT_SUCCESS);//exit program
	}

	SrcData = LoadSource(SrcFile, SrcData);//load source files

	Dest = fopen(argv[2], "wb");//open destination file
	if (Dest == NULL){//if file could not be opened
		printf("destination file %s could not be made\n", argv[2]);//send error meaaage
		exit(EXIT_SUCCESS);//exit program
	}

	if (GetNumLines(SrcData) <= 0){//if there was no lines in the file
		printf("Source file was empty\n");//send error message
		exit(EXIT_SUCCESS);//exit program
	}

	char line[2048] = { 0 };//line of the file
	char linecpy[2048] = { 0 };//copy of the line
	char *first;//first element of line
	strcpy(line, GetLine(SrcData, 0));//copy line

	MainData = AllocateData((uint32_t)((atof(strtok(line, "/() \t\n\r")) / atof(strtok(NULL, "/() \t\n\r"))) * 60.0 * WAV_SAMPLE_PER_SECOND) + 1);//allocate memory for main data according to length represented as a fraction
	//All number is represented in a fraction of decimal in parentheses (ex. (1/0.5) is 2)
	//Start of file has number in minuts of the length of the end result
	//note is represented by normal number for hertz or key number in bracket
	//Sound is represented with a number in minute for start of the sound, number in minute for length of the sound, sound type, note, ampritude with 1 as max, and additional numerical variable
	//Sound effect is effect name and number of variables needed.
	//Fluctuating sound effect can be called by F before effect name, then sound definition as a variable in next 1 to 2 lines
	//For fractuating amplify, 1 in ampritude of imput is x2 amplification
	//Wave names: sine, square, sawtooth, reverse (reverse swatooth), triangle
	//Sound effect names: cutoff, smooth, shift, amplify (Fcutoff, Fsmooth, Famplify for fructuating)
	uint32_t i;
	for (i = 1; i < GetNumLines(SrcData) - 1; i++){//for each line
		strcpy(linecpy, GetLine(SrcData, i));//get next line
		first = strtok(linecpy, ",. \t\n\r");//get first element of the line
		strcpy(line, GetLine(SrcData, i));//get this line
		if (strcmp(first, "cutoff") == 0){//if it was cutoff function
			Cutoff(MainData, ((atof(strtok(line, "/() \t\n\r.,")) / atof(strtok(NULL, "/() \t\n\r.,"))) * INT32_MAX), ((atof(strtok(NULL, "/() \t\n\r.,")) / atof(strtok(NULL, "/() \t\n\r.,"))) * INT32_MAX));//do cutoff
		}
		else if (strcmp(first, "smooth") == 0){//if it was smooth function
			Smooth(MainData, (int32_t)(atof(strtok(line, "/() \t\n\r.,")) / atof(strtok(NULL, "/() \t\n\r.,"))), (int32_t)(atof(strtok(NULL, "/() \t\n\r.,")) / atof(strtok(NULL, "/() \t\n\r.,"))));//do smooth
		}
		else if (strcmp(first, "shift") == 0){//if it was shift function
			YShift(MainData, (int32_t)((atof(strtok(line, "/() \t\n\r.,")) / atof(strtok(NULL, "/() \t\n\r.,"))) * INT32_MAX));//do yshift
		}
		else if (strcmp(first, "amplify") == 0){//if it was amplify function
			Amplify(MainData, (atof(strtok(line, "/() \t\n\r.,")) / atof(strtok(NULL, "/() \t\n\r.,"))));//do amplify
		}
		else if (strcmp(first, "Fcutoff") == 0){//if it was cutoff function
			Sound *valA, *valB;//variables
			//get sound data for 2 variables
			i++;//next line
			strcpy(line, GetLine(SrcData, i));//get next line
			uint32_t start = (uint32_t)((atof(strtok(line, "/() \t\n\r.,")) / atof(strtok(NULL, "/() \t\n\r.,"))) * 60.0 * WAV_SAMPLE_PER_SECOND);//get starting point
			uint32_t length = (uint32_t)((atof(strtok(NULL, "/() \t\n\r.,")) / atof(strtok(NULL, "/() \t\n\r.,"))) * 60.0 * WAV_SAMPLE_PER_SECOND);//get length of sound
			valA = AllocateData(length + 1);//allocate data
			strcpy(first, strtok(NULL, "/() \t\n\r.,"));//get next element of the line for function type
			char *note = strtok(NULL, "/() \t\n\r.,");//get next element of the line for note
			double hz = note[0] == '[' ? GetHertz(atof(strtok(NULL, "/()[] \t\n\r.,")) / atof(strtok(NULL, "/()[] \t\n\r.,"))) : (atof(note) / atof(strtok(NULL, "/() \t\n\r.,")));//get the tone
			int32_t ampritude = ((atof(strtok(NULL, "/() \t\n\r.,")) / atof(strtok(NULL, "/() \t\n\r.,"))) * INT32_MAX);//get ampritude
			double num = (atof(strtok(NULL, "/() \t\n\r.,")) / atof(strtok(NULL, "/() \t\n\r.,")));//get extra variable
			AddData(valA, SoundFunction(length, first, hz, ampritude, num), start);//run and add sound function
			i++;//next line
			strcpy(line, GetLine(SrcData, i));//get next line
			start = (uint32_t)((atof(strtok(line, "/() \t\n\r.,")) / atof(strtok(NULL, "/() \t\n\r.,"))) * 60.0 * WAV_SAMPLE_PER_SECOND);//get starting point
			length = (uint32_t)((atof(strtok(NULL, "/() \t\n\r.,")) / atof(strtok(NULL, "/() \t\n\r.,"))) * 60.0 * WAV_SAMPLE_PER_SECOND);//get length of sound
			valB = AllocateData(length + 1);//allocate data
			first = strtok(NULL, "/() \t\n\r.,");//get next element of the line for function type
			note = strtok(NULL, "/() \t\n\r.,");//get next element of the line for note
			hz = note[0] == '[' ? GetHertz(atof(strtok(NULL, "/()[] \t\n\r.,")) / atof(strtok(NULL, "/()[] \t\n\r.,"))) : (atof(note) / atof(strtok(NULL, "/()[] \t\n\r.,")));//get the tone
			ampritude = (int32_t)((atof(strtok(NULL, "/()[] \t\n\r.,")) / atof(strtok(NULL, "/()[] \t\n\r.,"))) * INT32_MAX);//get ampritude
			num = (atof(strtok(NULL, "/()[] \t\n\r.,")) / atof(strtok(NULL, "/()[] \t\n\r.,")));//get extra variable
			AddData(valB, SoundFunction(length, first, hz, ampritude, num), start);//run and add sound function
			FluctuatingCutoff(MainData, valA, valB);//do cutoff
			FreeData(valA);//free sounds
			FreeData(valB);
		}
		else if (strcmp(first, "Fsmooth") == 0){//if it was smooth function
			Sound *valA, *valB;//variables
			//get sound data for 2 variables
			i++;//next line
			strcpy(line, GetLine(SrcData, i));//get next line
			uint32_t start = (uint32_t)((atof(strtok(line, "/() \t\n\r.,")) / atof(strtok(NULL, "/() \t\n\r.,"))) * 60.0 * WAV_SAMPLE_PER_SECOND);//get starting point
			uint32_t length = (uint32_t)((atof(strtok(NULL, "/() \t\n\r.,")) / atof(strtok(NULL, "/() \t\n\r.,"))) * 60.0 * WAV_SAMPLE_PER_SECOND);//get length of sound
			valA = AllocateData(length + 1);//allocate data
			first = strtok(NULL, "/() \t\n\r.,");//get next element of the line for function type
			char *note = strtok(NULL, "/() \t\n\r.,");//get next element of the line for note
			double hz = note[0] == '[' ? GetHertz(atof(strtok(NULL, "/()[] \t\n\r.,")) / atof(strtok(NULL, "/()[] \t\n\r.,"))) : (atof(note) / atof(strtok(NULL, "/() \t\n\r.,")));//get the tone
			int32_t ampritude = ((atof(strtok(NULL, "/() \t\n\r.,")) / atof(strtok(NULL, "/() \t\n\r.,"))) * INT32_MAX);//get ampritude
			double num = (atof(strtok(NULL, "/() \t\n\r.,")) / atof(strtok(NULL, "/() \t\n\r.,")));//get extra variable
			AddData(valA, SoundFunction(length, first, hz, ampritude, num), start);//run and add sound function
			i++;//next line
			strcpy(line, GetLine(SrcData, i));//get next line
			start = (uint32_t)((atof(strtok(line, "/() \t\n\r.,")) / atof(strtok(NULL, "/() \t\n\r.,"))) * 60.0 * WAV_SAMPLE_PER_SECOND);//get starting point
			length = (uint32_t)((atof(strtok(NULL, "/() \t\n\r.,")) / atof(strtok(NULL, "/() \t\n\r.,"))) * 60.0 * WAV_SAMPLE_PER_SECOND);//get length of sound
			valB = AllocateData(length + 1);//allocate data
			first = strtok(NULL, "/() \t\n\r.,");//get next element of the line for function type
			note = strtok(NULL, "/() \t\n\r.,");//get next element of the line for note
			hz = note[0] == '[' ? GetHertz(atof(strtok(NULL, "/()[] \t\n\r.,")) / atof(strtok(NULL, "/()[] \t\n\r.,"))) : (atof(note) / atof(strtok(NULL, "/()[] \t\n\r.,")));//get the tone
			ampritude = (int32_t)((atof(strtok(NULL, "/()[] \t\n\r.,")) / atof(strtok(NULL, "/()[] \t\n\r.,"))) * INT32_MAX);//get ampritude
			num = (atof(strtok(NULL, "/()[] \t\n\r.,")) / atof(strtok(NULL, "/()[] \t\n\r.,")));//get extra variable
			AddData(valB, SoundFunction(length, first, hz, ampritude, num), start);//run and add sound function
			FluctuatingSmooth(MainData, valA, valB);//do smooth
			FreeData(valA);//free sounds
			FreeData(valB);
		}
		else if (strcmp(first, "Famplify") == 0){//if it was amplify function
			Sound *valA;//variables
			//get sound data for 2 variables
			i++;//next line
			strcpy(line, GetLine(SrcData, i));//get next line
			uint32_t start = (uint32_t)((atof(strtok(line, "/() \t\n\r.,")) / atof(strtok(NULL, "/() \t\n\r.,"))) * 60.0 * WAV_SAMPLE_PER_SECOND);//get starting point
			uint32_t length = (uint32_t)((atof(strtok(NULL, "/() \t\n\r.,")) / atof(strtok(NULL, "/() \t\n\r.,"))) * 60.0 * WAV_SAMPLE_PER_SECOND);//get length of sound
			valA = AllocateData(length + 1);//allocate data
			first = strtok(NULL, "/() \t\n\r.,");//get next element of the line for function type
			char *note = strtok(NULL, "/() \t\n\r.,");//get next element of the line for note
			double hz = note[0] == '[' ? GetHertz(atof(strtok(NULL, "/()[] \t\n\r.,")) / atof(strtok(NULL, "/()[] \t\n\r.,"))) : (atof(note) / atof(strtok(NULL, "/()[] \t\n\r.,")));//get the tone
			int32_t ampritude = ((atof(strtok(NULL, "/()[] \t\n\r.,")) / atof(strtok(NULL, "/()[] \t\n\r.,"))) * INT32_MAX);//get ampritude
			double num = (atof(strtok(NULL, "/()[] \t\n\r.,")) / atof(strtok(NULL, "/()[] \t\n\r.,")));//get extra variable
			AddData(valA, SoundFunction(length, first, hz, ampritude, num), start);//run and add sound function
			FluctuatingAmplify(MainData, valA);//do amplify
			FreeData(valA);//free sounds
		}
		else if (first[0] == '('){//if it is a sound function
			uint32_t start = (uint32_t)((atof(strtok(line, "/() \t\n\r.,")) / atof(strtok(NULL, "/() \t\n\r.,"))) * 60.0 * WAV_SAMPLE_PER_SECOND);//get starting point
			uint32_t length = (uint32_t)((atof(strtok(NULL, "/() \t\n\r.,")) / atof(strtok(NULL, "/() \t\n\r.,"))) * 60.0 * WAV_SAMPLE_PER_SECOND);//get length of sound
			first = strtok(NULL, "/() \t\n\r.,");//get next element of the line for function type
			char *note = strtok(NULL, "/() \t\n\r.,");//get next element of the line for note
			double hz = note[0] == '[' ? GetHertz(atof(strtok(NULL, "/()[] \t\n\r.,")) / atof(strtok(NULL, "/()[] \t\n\r.,"))) : (atof(note) / atof(strtok(NULL, "/()[] \t\n\r.,")));//get the tone
			int32_t ampritude = ((atof(strtok(NULL, "/()[] \t\n\r.,")) / atof(strtok(NULL, "/()[] \t\n\r.,"))) * INT32_MAX);//get ampritude
			double num = (atof(strtok(NULL, "/()[] \t\n\r.,")) / atof(strtok(NULL, "/()[] \t\n\r.,")));//get extra variable
			Sound *Function = SoundFunction(length, first, hz, ampritude, num);//run sound function
			AddData(MainData, Function, start);//add sound function
			FreeData(Function);
		}
		else if (strcmp(first, "SOUND") == 0){//if it was a function
			do{//loop until end of function
				strcpy(line, GetLine(SrcData, i));//get next line
				i++;
				first = strtok(line, " \t\n\r");//get first element of the line
			} while (strcmp(first, "EOS") != 0);
		}
		else{//if there was nothing
			printf("Empty line at line %lu\n", i);//print that there was an empty line
		}
	}
	
	//Test functions
	/*SineWave(MainData, 0, WAV_SAMPLE_PER_SECOND, 200, INT32_MAX * 0.9, 0, 0);//add sign wave to main data
	SquareWave(MainData, WAV_SAMPLE_PER_SECOND, WAV_SAMPLE_PER_SECOND * 2, 200, INT32_MAX * 0.9, 0, 0);//add square wave to main data
	SawtoothWave(MainData, WAV_SAMPLE_PER_SECOND * 2, WAV_SAMPLE_PER_SECOND * 3, 200, INT32_MAX * 0.9, 0, 0);//add swatooth wave to main data
	ReverseSawtoothWave(MainData, WAV_SAMPLE_PER_SECOND * 3, WAV_SAMPLE_PER_SECOND * 4, 200, INT32_MAX * 0.9, 0, 0);//add reverse-sawtooth wave to main data
	TriangleWave(MainData, WAV_SAMPLE_PER_SECOND * 4, WAV_SAMPLE_PER_SECOND * 5, 200, INT32_MAX * 0.9, 0, 0);//add traiangle wave to main data
	Cutoff(MainData, INT32_MAX * -0.2, INT32_MAX);//cutoff the bottom
	YShift(MainData, INT32_MAX * -0.3);//shift to center
	Smooth(MainData, -0, 100);//smooth wave
	Smooth(MainData, -0, 200);//double smooth wave
	Amplify(MainData, 1.6);//amplify data
	Sound *sawtooth = AllocateData(WAV_SAMPLE_PER_SECOND * 6 + 1);//sawtooth wave
	SineWave(sawtooth, WAV_SAMPLE_PER_SECOND * 5, WAV_SAMPLE_PER_SECOND * 6, 50, INT32_MAX * 0.9, 0, 0);//get sawtooth wave
	Sound *blank = AllocateData(1);//blank sound
	Sound *yshift = AllocateData(1);//yshift for sound
	YShift(yshift, INT32_MAX * 0.5);//set last shift back
	Sound *hz = AllocateData(1);//hz for sound
	YShift(hz, 800 * 1000);//set it to 90% of max 
	FluctuatingSineWave(MainData, WAV_SAMPLE_PER_SECOND * 5, WAV_SAMPLE_PER_SECOND * 6, hz, sawtooth, blank, yshift);//add sign wave to main data
	*/
	//write sound file
	if (WriteWav(Dest, MainData)){//if there was some error
		printf("Could not write to file correctly\n");//send error message
	}

	FreeData(MainData);//free main data
	FreeSource(SrcData);//free source data

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

void SineWave(Sound *data, uint32_t start, uint32_t end, double hz, int32_t ampritude, int32_t xshift, int32_t yshift){//add specified sign wave to that portion of data
	int32_t i;
	for (i = start; (uint32_t)i <= end; i++){//for each in portion of data
		data->Sound[i] += (int32_t)(ampritude * sin(2.0 * PI * hz * (i - xshift) / WAV_SAMPLE_PER_SECOND) + WAV_SAMPLE_PER_SECOND / hz) + yshift;//get that sign wave
	}
}

void AddData(Sound *dest, Sound *src, uint32_t start){//add src to dest at start
	uint32_t i;
	for (i = 0; i < src->DataSize && i + start < dest->DataSize; i++){//for each of source
		dest->Sound[i + start] += src->Sound[i];//add data
	}
}

void CopyData(Sound *dest, Sound *src, uint32_t start){//copy src to dest at start
	uint32_t i;
	for (i = 0; i < src->DataSize && i + start < dest->DataSize; i++){//for each of source
		dest->Sound[i + start] = src->Sound[i];//copy data
	}
}

void SquareWave(Sound *data, uint32_t start, uint32_t end, double hz, int32_t ampritude, int32_t xshift, int32_t yshift){//add specified square wave to that portion of data
	int32_t i;
	for (i = start; (uint32_t)i <= end; i++){//for each in portion of data
		data->Sound[i] += (int32_t)(fmod((i - xshift), (WAV_SAMPLE_PER_SECOND / hz) + WAV_SAMPLE_PER_SECOND / hz) < (WAV_SAMPLE_PER_SECOND / hz) / 2 ? ampritude : -ampritude) + yshift;//set data
	}
}

void SawtoothWave(Sound *data, uint32_t start, uint32_t end, double hz, int32_t ampritude, int32_t xshift, int32_t yshift){//add specified sawtooth wave to that portion of data
	int32_t i;
	for (i = start; (uint32_t)i <= end; i++){//for each in portion of data
		data->Sound[i] += (int32_t)(fmod((i - xshift - (WAV_SAMPLE_PER_SECOND / hz) / 2 + WAV_SAMPLE_PER_SECOND / hz), (WAV_SAMPLE_PER_SECOND / hz)) * 2.0 * (ampritude / (WAV_SAMPLE_PER_SECOND / hz)) - ampritude) + yshift;//set data
	}
}

void ReverseSawtoothWave(Sound *data, uint32_t start, uint32_t end, double hz, int32_t ampritude, int32_t xshift, int32_t yshift){//add specified reverse-sawtooth wave to that portion of data
	int32_t i;
	for (i = start; (uint32_t)i <= end; i++){//for each in portion of data
		data->Sound[i] += (int32_t)(fmod((i - xshift - (WAV_SAMPLE_PER_SECOND / hz) / 2 + WAV_SAMPLE_PER_SECOND / hz), (WAV_SAMPLE_PER_SECOND / hz)) * -2.0 * (ampritude / (WAV_SAMPLE_PER_SECOND / hz)) + ampritude) + yshift;//set data
	}
}

void TriangleWave(Sound *data, uint32_t start, uint32_t end, double hz, int32_t ampritude, int32_t xshift, int32_t yshift){//add specified triangle wave to that portion of data
	int32_t i;
	for (i = start; (uint32_t)i <= end; i++){//for each in portion of data
		data->Sound[i] += (int32_t)(fmod((i - xshift + (WAV_SAMPLE_PER_SECOND / hz) / 4 + WAV_SAMPLE_PER_SECOND / hz), (WAV_SAMPLE_PER_SECOND / hz)) < (WAV_SAMPLE_PER_SECOND / hz) / 2 ? fmod((i - xshift + (WAV_SAMPLE_PER_SECOND / hz) / 4), (WAV_SAMPLE_PER_SECOND / (hz * 2))) * 2.0 * (ampritude / (WAV_SAMPLE_PER_SECOND / (hz * 2))) - ampritude : fmod((i - xshift + (WAV_SAMPLE_PER_SECOND / hz) / 4), (WAV_SAMPLE_PER_SECOND / (hz * 2))) * -2.0 * (ampritude / (WAV_SAMPLE_PER_SECOND / (hz * 2))) + ampritude) + yshift;//set data
	}
}

void Smooth(Sound *data, int32_t start, int32_t end){//smooth wave by taking avarage from start to end
	int32_t i;
	Sound *copy = AllocateData(data->DataSize);//allocate memory for copy of data
	CopyData(copy, data, 0);//copy that data
	for (i = 0; (uint32_t)i < data->DataSize; i++){//for each in portion of data
		double sum = 0;
		int32_t j;
		for (j = start; j <= end; j++){
			sum += copy->Sound[(i + j) < 0 ? 0 : (i + j) > (int32_t)data->DataSize ? data->DataSize : (i + j)];//get data
		}
		data->Sound[i] = (int32_t)(sum / (end - start));//get avarage
	}
	FreeData(copy);
}

void Cutoff(Sound *data, int32_t min, int32_t max){//cutoff the wave at min and max
	int32_t i;
	for (i = 0; (uint32_t)i < data->DataSize; i++){//for each in portion of data
		data->Sound[i] = (int32_t)(data->Sound[i] < min ? min : data->Sound[i] > max ? max : data->Sound[i]);//set data
	}
}

void Amplify(Sound *data, double vol){//amplify sound by volume
	int32_t i;
	for (i = 0; (uint32_t)i < data->DataSize; i++){//for each in portion of data
		data->Sound[i] = (int32_t)(data->Sound[i] * vol);//amplify data
	}
}

void YShift(Sound *data, int32_t yshift){//shift wave in y direction
	int32_t i;
	for (i = 0; (uint32_t)i < data->DataSize; i++){//for each in portion of data
		data->Sound[i] = (int32_t)(data->Sound[i] + yshift);//shift data
	}
}

void FluctuatingSineWave(Sound *data, uint32_t start, uint32_t end, Sound *Hz, Sound *Ampritude, Sound *Xshift, Sound *Yshift){//add specified sign wave to that portion of data with each increment as 1/1000 hz
	int32_t i;
	for (i = start; (uint32_t)i <= end; i++){//for each in portion of data
		double hz = (double)Hz->Sound[i % Hz->DataSize] / 1000;//get hz, ampritude, xshift, and yshift
		int32_t ampritude = Ampritude->Sound[i % Ampritude->DataSize];
		int32_t xshift = Xshift->Sound[i % Xshift->DataSize];
		int32_t yshift = Yshift->Sound[i % Yshift->DataSize];
		data->Sound[i] += (int32_t)(ampritude * sin(2.0 * PI * hz * (i - xshift) / WAV_SAMPLE_PER_SECOND) + WAV_SAMPLE_PER_SECOND / hz) + yshift;//get that sign wave
	}
}
	
void FluctuatingSquareWave(Sound *data, uint32_t start, uint32_t end, Sound *Hz, Sound *Ampritude, Sound *Xshift, Sound *Yshift){//add specified square wave to that portion of data with each increment as 1/1000 hz
	int32_t i;
	for (i = start; (uint32_t)i <= end; i++){//for each in portion of data
		double hz = (double)Hz->Sound[i % Hz->DataSize] / 1000;//get hz, ampritude, xshift, and yshift
		int32_t ampritude = Ampritude->Sound[i % Ampritude->DataSize];
		int32_t xshift = Xshift->Sound[i % Xshift->DataSize];
		int32_t yshift = Yshift->Sound[i % Yshift->DataSize];
		data->Sound[i] += (int32_t)(fmod((i - xshift), (WAV_SAMPLE_PER_SECOND / hz) + WAV_SAMPLE_PER_SECOND / hz) < (WAV_SAMPLE_PER_SECOND / hz) / 2 ? ampritude : -ampritude) + yshift;//set data
	}
}
	
void FluctuatingSawtoothWave(Sound *data, uint32_t start, uint32_t end, Sound *Hz, Sound *Ampritude, Sound *Xshift, Sound *Yshift){//add specified sawtooth wave to that portion of data with each increment as 1/1000 hz
	int32_t i;
	for (i = start; (uint32_t)i <= end; i++){//for each in portion of data
		double hz = (double)Hz->Sound[i % Hz->DataSize] / 1000;//get hz, ampritude, xshift, and yshift
		int32_t ampritude = Ampritude->Sound[i % Ampritude->DataSize];
		int32_t xshift = Xshift->Sound[i % Xshift->DataSize];
		int32_t yshift = Yshift->Sound[i % Yshift->DataSize];
		data->Sound[i] += (int32_t)(fmod((i - xshift - (WAV_SAMPLE_PER_SECOND / hz) / 2 + WAV_SAMPLE_PER_SECOND / hz), (WAV_SAMPLE_PER_SECOND / hz)) * 2.0 * (ampritude / (WAV_SAMPLE_PER_SECOND / hz)) - ampritude) + yshift;//set data
	}
}
	
void FluctuatingReverseSawtoothWave(Sound *data, uint32_t start, uint32_t end, Sound *Hz, Sound *Ampritude, Sound *Xshift, Sound *Yshift){//add specified reverse-sawtooth wave to that portion of data with each increment as 1/1000 hz
	int32_t i;
	for (i = start; (uint32_t)i <= end; i++){//for each in portion of data
		double hz = (double)Hz->Sound[i % Hz->DataSize] / 1000;//get hz, ampritude, xshift, and yshift
		int32_t ampritude = Ampritude->Sound[i % Ampritude->DataSize];
		int32_t xshift = Xshift->Sound[i % Xshift->DataSize];
		int32_t yshift = Yshift->Sound[i % Yshift->DataSize];
		data->Sound[i] += (int32_t)(fmod((i - xshift - (WAV_SAMPLE_PER_SECOND / hz) / 2 + WAV_SAMPLE_PER_SECOND / hz), (WAV_SAMPLE_PER_SECOND / hz)) * -2.0 * (ampritude / (WAV_SAMPLE_PER_SECOND / hz)) + ampritude) + yshift;//set data
	}
}
		
void FluctuatingTriangleWave(Sound *data, uint32_t start, uint32_t end, Sound *Hz, Sound *Ampritude, Sound *Xshift, Sound *Yshift){//add specified triangle wave to that portion of data with each increment as 1/1000 hz
	int32_t i;
	for (i = start; (uint32_t)i <= end; i++){//for each in portion of data
		double hz = (double) Hz->Sound[i % Hz->DataSize] / 1000;//get hz, ampritude, xshift, and yshift
		int32_t ampritude = Ampritude->Sound[i % Ampritude->DataSize];
		int32_t xshift = Xshift->Sound[i % Xshift->DataSize];
		int32_t yshift = Yshift->Sound[i % Yshift->DataSize];
		data->Sound[i] += (int32_t)(fmod((i - xshift + (WAV_SAMPLE_PER_SECOND / hz) / 4 + WAV_SAMPLE_PER_SECOND / hz), (WAV_SAMPLE_PER_SECOND / hz)) < (WAV_SAMPLE_PER_SECOND / hz) / 2 ? fmod((i - xshift + (WAV_SAMPLE_PER_SECOND / hz) / 4), (WAV_SAMPLE_PER_SECOND / (hz * 2))) * 2.0 * (ampritude / (WAV_SAMPLE_PER_SECOND / (hz * 2))) - ampritude : fmod((i - xshift + (WAV_SAMPLE_PER_SECOND / hz) / 4), (WAV_SAMPLE_PER_SECOND / (hz * 2))) * -2.0 * (ampritude / (WAV_SAMPLE_PER_SECOND / (hz * 2))) + ampritude) + yshift;//set data
	}
}
			
void FluctuatingSmooth(Sound *data, Sound *Start, Sound *End){//smooth wave by taking avarage from start to end
	int32_t i;
	Sound *copy = AllocateData(data->DataSize);//allocate memory for copy of data
	CopyData(copy, data, 0);//copy that data
	for (i = 0; (uint32_t)i < data->DataSize; i++){//for each in portion of data
		int32_t start = Start->Sound[i % Start->DataSize];//get start and end data
		int32_t end = End->Sound[i % End->DataSize];
		double sum = 0;
		int32_t j;
		for (j = start; j <= end; j++){
			sum += copy->Sound[(i + j) < 0 ? 0 : (i + j) > (int32_t)data->DataSize ? data->DataSize : (i + j)];//get data
		}
		data->Sound[i] = (int32_t)(sum / (end - start));//get avarage
	}
	FreeData(copy);
}
				
void FluctuatingCutoff(Sound *data, Sound *Min, Sound *Max){//cutoff the wave at min and max
	int32_t i;
	for (i = 0; (uint32_t)i < data->DataSize; i++){//for each in portion of data
		int32_t min = Min->Sound[i % Min->DataSize];//get min and max data
		int32_t max = Max->Sound[i % Max->DataSize];
		data->Sound[i] = (int32_t)(data->Sound[i] < min ? min : data->Sound[i] > max ? max : data->Sound[i]);//set data
	}
}
					
void FluctuatingAmplify(Sound *data, Sound *Vol){//amplify sound by volume with INT32_MAX as 2
	int32_t i;
	for (i = 0; (uint32_t)i < data->DataSize; i++){//for each in portion of data
		double vol = (double)Vol->Sound[i % Vol->DataSize] / (INT32_MAX / 2);//get volume data
		data->Sound[i] = (int32_t)(data->Sound[i] * vol);//amplify data
	}
}

void GlottalFlowWave(Sound *data, double hz, int32_t ampritude, double smooth, int32_t cutoff){//simulate glottal flow. ampritude will be lower
	SawtoothWave(data, 0, data->DataSize - 1, hz, ampritude, 0, 0);//add swatooth wave to main data
	Cutoff(data, cutoff, INT32_MAX);//cutoff the bottom
	YShift(data, (int32_t)((-ampritude + cutoff) / 2));//shift to center
	Amplify(data, 2);//amplify data
	Smooth(data, 0, (int32_t)((WAV_SAMPLE_PER_SECOND / hz) * smooth));//smooth wave
	Smooth(data, 0, (int32_t)((WAV_SAMPLE_PER_SECOND / hz) * smooth));//double smooth wave
}

Source *LoadSource(FILE *file, Source *source){//load source file in to data
	source = malloc(sizeof(Source));//allocate source
	source->length = 0;//initialise length
	source->lines = NULL;//initialise lines pointer
	int data;//data to read
	while (1){//for each line
		source->lines = realloc(source->lines, ++(source->length) * sizeof(Line));//allocate memory for each new line
		source->lines[source->length - 1].length = 0;//initialise length
		source->lines[source->length - 1].line = NULL;//initialise line pointer
		while (1){//for each character
			data = fgetc(file);//get character
			source->lines[source->length - 1].line = realloc(source->lines[source->length - 1].line, ++(source->lines[source->length - 1].length));//allocate memory for each new character
			if (data == EOF){//escape if at end of file
				source->lines[source->length - 1].line[source->lines[source->length - 1].length - 1] = '\0';//end line
				return source;//end function
			}
			else{
				if (data == ';'){//if data is at end of line
					source->lines[source->length - 1].line[source->lines[source->length - 1].length - 1] = '\0';//end line
					break;//get out of loop
				}
				else{
					source->lines[source->length - 1].line[source->lines[source->length - 1].length - 1] = (char)data;//get character
				}
			}
		}
	}
	return source;//return result
}

char *GetLine(Source *source, uint32_t line){//get line of that line number
	if (line < source->length){//check for out of bound
		return source->lines[line].line;//return that line
	}
	return NULL;//return null if out of bound
}

uint32_t GetNumLines(Source *source){//get number of lines in the source
	return source->length;//return length
}

void exit_failure(void){//exit program
	printf("Big Error \n");//send error message
	exit(EXIT_FAILURE);//exit program
}

void FreeSource(Source *source){//free memory for source
	uint32_t i = 0;
	for (i = 0; i < GetNumLines(source); i++){//for each line
		free(source->lines[i].line);//free line
	}
	free(source->lines);//free lines array
	free(source);//free source data
}

double GetHertz(double key){//get Hz from piano key number for A440
	return pow(2, (key - 49) / 12) * 440;//calculate and return resulting frequency
}

Sound *SoundFunction(uint32_t length, char *type, double hz, int32_t ampritude, double num){//run that sound function
	Sound* MainSound = AllocateData(length + 1);//allocate data for function result
	if (strcmp(type, "sine") == 0){//if sine wave function
		SineWave(MainSound, 0, length, hz, ampritude, 0, 0);//get sine wave
	}
	else if (strcmp(type, "square") == 0){//if square wave function
		SquareWave(MainSound, 0, length, hz, ampritude, 0, 0);//get square wave
	}
	else if (strcmp(type, "sawtooth") == 0){//if sawtooth wave function
		SawtoothWave(MainSound, 0, length, hz, ampritude, 0, 0);//get sawtooth wave
	}
	else if (strcmp(type, "reverse") == 0){//if reverse sawtooth wave function
		ReverseSawtoothWave(MainSound, 0, length, hz, ampritude, 0, 0);//get reverse sawtooth wave
	}
	else if (strcmp(type, "triangle") == 0){//if triangle wave function
		TriangleWave(MainSound, 0, length, hz, ampritude, 0, 0);//get triangle wave
	}
	else if (strcmp(type, "GlottalFlow") == 0){//if triangle wave function
		GlottalFlowWave(MainSound, hz, ampritude, num, 0);//get glottal flow wave
	}
	return MainSound;//return result
}

