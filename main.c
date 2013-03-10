#include "stdio.h"

#include "GL/freeglut.h"
#include "GL/gl.h"

#include "util.c"

// compile this: gcc -o world world.c -lGL -lglut

// in C there seems to be a real difference between single and double quotes... are single quotes for chars
// and double quotes for strings

// if you want to return an array from a function you need to malloc that memory block else the memory will
// be GCed after the function returns

// in C you can't define a variable in a swtich statement case block unless you wrap the whole block in parens

// past challenge: how can we define our mIndicies and mVerts as globals when we want the data file we
// load to define the length of these arrays? - I think I have a lead, define the max size, then load the file, then
// populate only as much as you need to - yep that's what I did

// how many characters we read from each line in the data files
#define ALLOC_DATA_LINE_CHAR_NUM 128

// the max length of the array holding indicies 
#define ALLOC_INDICIES 10

// the max length of the array holding verts
#define ALLOC_VERTS 20

int mIndiciesUsed;
GLubyte mIndicies[ALLOC_INDICIES];

int mVertsUsed;
GLfloat mVerts[ALLOC_VERTS];

// 0 - bottom left
// 1 - top left
// 2 - top right 
// 3 - bottom right
// 4 - top far right

void loadData() {
	int indiciesHolder[ALLOC_INDICIES];
	float vertsHolder[ALLOC_VERTS];
	
	int fileIndiciesUsed = 0;
	int lineNumber = 0;
	char * fileName = "sample.data";
	FILE * file = fopen (fileName, "rb");

	if ( file != NULL ) {
		char line [ ALLOC_DATA_LINE_CHAR_NUM ];
		while ( fgets ( line, sizeof line, file ) != NULL ) {
			lineNumber = lineNumber + 1;
			//fputs ( line, stdout );
			if(lineNumber == 2) {
				mIndiciesUsed = atoi(line);
			} else if(lineNumber == 4) {
				splitInt(',', line, ALLOC_DATA_LINE_CHAR_NUM, indiciesHolder);
			} else if(lineNumber == 6) {
				mVertsUsed = atoi(line);
			} else if(lineNumber == 8) {
				splitFloat(',', line, ALLOC_DATA_LINE_CHAR_NUM, vertsHolder);
			}
		}
		fclose ( file );
	} else { 
		perror ( fileName );
	}

	int i = 0;
	for(i = 0; i < mIndiciesUsed; i++) {
		//printf("index value: %i\n", indiciesHolder[i]);
		mIndicies[i] = indiciesHolder[i];
	}
	for(i = 0; i < mVertsUsed; i++) {
		//printf("vert value %f\n", vertsHolder[i]);
		mVerts[i] = vertsHolder[i];
	}
}

/*
Combine two char arrays and permanently set aside memory for resulting array
*/
char * concatChars(size_t sizeA, char * a, size_t sizeB, char * b) {	
	size_t resultLength = sizeA + sizeB - 1;
	char * result = malloc(resultLength * sizeof(char));
	
	for(int i = 0; i < sizeA-1; i++) {
		//printf("i: %zu\n", i);
		result[i] = a[i];
	}
	for(int j = 0; j < sizeB-1; j++) {
		//printf("sizeA -1 + j: %zu\n", sizeA -1 + j);
		result[sizeA - 1 + j] = b[j];
	}
	result[resultLength - 1] = '\0';
	return result;
}

void printLessonOnSizeOf() {
	int testOne[] = {0, 1, 2, 3};	

	size_t sizeArrBytes = sizeof(testOne);
	printf("The size of the array in bytes: %zu\n", sizeArrBytes);
	
	size_t sizeElementBytes = sizeof(testOne[0]);
	printf("The size of first element: %zu\n", sizeElementBytes);

	int lengthArray = sizeArrBytes/sizeElementBytes;
	printf("The length of the array: %zu\n", lengthArray);
}

void onKeyPress(unsigned char key, int mouseX, int mouseY) {

	// two different ways of showing which was pressed
	char testMessage[] = "you pressed key: X";
	testMessage[sizeof(testMessage)-2] = key;
	printf("%s\n", testMessage);
	
	/*
	char introString[] = "you pressed key: ";
	char keyArray[2];
	keyArray[0] = key;
	keyArray[1] = '\0';
	char * message = concatChars(sizeof(introString), introString, sizeof(keyArray), keyArray);
	printf("%s\n", message);
	*/	

	switch(key) {
		case 'h': {
			printLessonOnSizeOf();
			break;
		}
		case 'w': {
			mVerts[0] = mVerts[0] - 1.0;
			mVerts[1] = mVerts[1] - 1.0;
			mVerts[2] = mVerts[2] - 1.0;
			mVerts[3] = mVerts[3] - 1.0;		
			break;
		}
		case 'e': {
			int sizeOfArray = (sizeof(mVerts)/sizeof(mVerts[0]));
			for(int i = 0; i < sizeOfArray; i++) {
				mVerts[i] = mVerts[i] * 1.2;	
			}
			break;
		}
	}
}

/* display function - code from:
     http://fly.cc.fer.hr/~unreal/theredbook/chapter01.html
This is the actual usage of the OpenGL library. 
The following code is the same for any platform */
void onDraw() {

    // Reset transformations (this has to be called or nothing is drawn)
    glLoadIdentity();

	// clears the screen, if this isn't on then things will just be drawn on top of each other
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glOrtho(-5.0, 5.0, -5.0, 5.0, 0.0, 1.0);

	//printf("num indicies used: %zu\n", mIndiciesUsed);
	//printf("num verts used: %zu\n", mVertsUsed);
	
	int sizeOfmIndicies = mIndiciesUsed  * sizeof(GLubyte);
	int sizeOfmVerts = mVertsUsed * sizeof(GLfloat);
	int numComponentsPerCoord = 2;

	//int sizeElementInVerts = sizeof(mVerts[0]);
	//printf("size of element in verts: %zu\n", sizeElementInVerts);

	//printf("size of used verts array: %zu\n", sizeOfmVerts);
	//printf("size of used indicies array: %zu\n", sizeOfmIndicies);
	
	// this is done for VBOs only
	GLuint vboVert;
	glGenBuffersARB(1, &vboVert);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboVert);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeOfmVerts, mVerts, GL_STATIC_DRAW_ARB);	

	GLuint vboInd;
	glGenBuffersARB(1, &vboInd);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, vboInd);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeOfmIndicies, mIndicies, GL_STATIC_DRAW_ARB);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboVert);         // for vertex coordinates
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, vboInd);  // for indices

	// this is done for both vertex arrays and VBOs
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_INDEX_ARRAY);

	// drawing with vertex arrays (not VBOs)	
	//glIndexPointer(GL_UNSIGNED_BYTE, 0, indicies);
	//glVertexPointer(numComponentsPerCoord, GL_FLOAT, 0, verts);

	glIndexPointer(GL_UNSIGNED_BYTE, 0, 0);
	glVertexPointer(numComponentsPerCoord, GL_FLOAT, 0, 0);	

	// drawing with vertex arrays (not VBOs)
	//glDrawElements(GL_TRIANGLES, numInd, GL_UNSIGNED_BYTe, indicies);
	glDrawElements(GL_TRIANGLES, sizeOfmIndicies, GL_UNSIGNED_BYTE, 0);
	
	// bind back to "head" index of 0
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
	
	// this is done to cleanup the client state
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);

	//glutSwapBuffers();
	glFlush();
}

void onIdle(void) {
	// Force GLUT to render the scene
	glutPostRedisplay();
}

/* Main method - main entry point of application
the freeglut library does the window creation work for us, 
regardless of the platform. */
int main(int argc, char** argv) {
	
	loadData();

	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500,500);
    glutInitWindowPosition(100,100);
    glutCreateWindow("OpenGL - First window demo");
	
	glutDisplayFunc(onDraw);
	glutKeyboardFunc(onKeyPress);
	glutIdleFunc(onIdle);

	glutMainLoop();

 	printf("exiting...\n");
   
    return 1;
}
