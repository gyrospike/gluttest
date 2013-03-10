
void splitFloat(char delim, char * line, int lineLength, float * holder) {
	//printf("line to start with: %s\n", line);
	int delimIndex = -1;
	int delimiterIndicies[lineLength];
	for(int i = 0; i < lineLength; i++) {
		if(line[i] == delim) {
			//printf("delim index being built %i\n", delimIndex);
			delimIndex = delimIndex + 1;
			delimiterIndicies[delimIndex] = i;
		}	
	}
	//printf("delimIndex: %i\n", delimIndex);
	for(int j = 0; j < delimIndex+1; j++) {
		
		int startIndex = 0;
		if(j > 0) {
			startIndex = delimiterIndicies[j-1] + 1;
		}
		int endIndex = delimiterIndicies[j] - 1;
		if(j == delimIndex) {
			endIndex = lineLength - 1;
		}
 	
		int subStringLength = delimiterIndicies[j] - startIndex;
		char subString[subStringLength + 1];
		for(int k = 0; k < subStringLength; k++) {
			subString[k] = line[startIndex + k];
		}
		subString[subStringLength] = '\0';
		holder[j] = atof(subString);
		//printf("int : %i\n", holder[j]);
	}
}


void splitInt(char delim, char * line, int lineLength, int * holder) {
	//printf("line to start with: %s\n", line);
	int delimIndex = -1;
	int delimiterIndicies[lineLength];
	for(int i = 0; i < lineLength; i++) {
		if(line[i] == delim) {
			//printf("delim index being built %i\n", delimIndex);
			delimIndex = delimIndex + 1;
			delimiterIndicies[delimIndex] = i;
		}	
	}
	//printf("delimIndex: %i\n", delimIndex);
	for(int j = 0; j < delimIndex+1; j++) {
		
		int startIndex = 0;
		if(j > 0) {
			startIndex = delimiterIndicies[j-1] + 1;
		}
		int endIndex = delimiterIndicies[j] - 1;
		if(j == delimIndex) {
			endIndex = lineLength - 1;
		}
 	
		int subStringLength = delimiterIndicies[j] - startIndex;
		char subString[subStringLength + 1];
		for(int k = 0; k < subStringLength; k++) {
			subString[k] = line[startIndex + k];
		}
		subString[subStringLength] = '\0';
		holder[j] = atoi(subString);
		//printf("int : %i\n", holder[j]);
	}
}



