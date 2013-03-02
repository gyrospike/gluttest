#include "stdio.h"

#include "GL/freeglut.h"
#include "GL/gl.h"

// compile this: gcc -o world world.c -lGL -lglut

GLubyte mIndicies[] = {0, 1, 2, 2, 4, 3};

// 0 - bottom left
// 1 - top left
// 2 - top right 
// 3 - bottom right
// 4 - top far right
GLfloat mVerts[] = {
	-0.5, -0.5,
	-0.5, 0.5,
	0.5, 0.5, 
	0.5, -0.5,
	1.5, 0.5
};

void onKeyPress(unsigned char key, int mouseX, int mouseY) {
	switch(key) {
		case 'w':
			fprintf(stdout, "you pressed w\n", "message");
			mVerts[0] = -2.0;		
			break;
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
	
	int numInd = sizeof(mIndicies);
	int numComponentsPerCoord = 2;	

	// example of how to print an int
	// printf("The size of integer is %zu\n", numInd);

	// this is done for VBOs only
	GLuint vboVert;
	glGenBuffersARB(1, &vboVert);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboVert);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(mVerts), mVerts, GL_STATIC_DRAW_ARB);	

	GLuint vboInd;
	glGenBuffersARB(1, &vboInd);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, vboInd);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(mIndicies), mIndicies, GL_STATIC_DRAW_ARB);

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
	glDrawElements(GL_TRIANGLES, numInd, GL_UNSIGNED_BYTE, 0);
	
	// bind back to "head" index of 0
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
	
	// this is done to cleanup the client state
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);

	// option B
	/*
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glBegin(GL_POLYGON);
	glVertex2f(-0.5, -0.5);
    glVertex2f(-0.5, 0.5);
  	glVertex2f(0.5, 0.5);
    glVertex2f(0.5, -0.5);
    glEnd();
	*/

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
	// NOTE: currently trying to figure out how to get the display to refresh, only draws scene once 
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
