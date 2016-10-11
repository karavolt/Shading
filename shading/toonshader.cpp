#include <stdio.h>
#include <stdlib.h>
#include <gl\glew.h>
#include <gl\glut.h>


#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glut32.lib")

// Global variables
int Width = 1024;
int Height = 768;
float Angle = 0.0;

void Reshape(int w, int h);
void Render(void);
void SetShaders();
char *TextFileRead(char *fn);

int main(int argc, char **argv)
{
	// Initialize GLUT
	glutInit(&argc, argv);

	// Initialize OpenGL display mode as follows.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Create an OpenGL window
	glutInitWindowPosition(100,100);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("Cartoon Shader");

	// Initialize GLEW
	glewInit();

	if (glewIsSupported("GL_VERSION_3_3"))
		printf("Ready for OpenGL 3.3\n");

	// Register callback functions.
	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Render);

	// Set shaders
	SetShaders();

	// Enter the message loop.
	glutMainLoop();
}

void Reshape(int w, int h) 
{
	Width = w;
	Height = h;
	glViewport(0, 0, w, h);
}

void Render(void) 
{
	// Initialize the color buffer as (1.0, 1.0, 1.0) and the depth buffer as 1.0
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable depth test and lighting.
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Set up viewing volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (double)Width / (double)Height, 1.1, 100.0);

	// Set up viewing transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -10.0);

	// Do modeling transformation for each object
	glRotatef(Angle, 1.0, 1.0, 0.0);
	glutSolidTeapot(1.0);
	Angle += 0.1;

	// Swap two color buffers
	glutSwapBuffers();
}

// Handles for shaders and program
GLuint hVert, hFrag, hProg;

void SetShaders() 
{
	hVert = glCreateShader(GL_VERTEX_SHADER);
	hFrag = glCreateShader(GL_FRAGMENT_SHADER);

	char *vs = TextFileRead("toonshader.vert");
	char *fs = TextFileRead("toonshader.frag");

	glShaderSource(hVert, 1, &vs, NULL);
	glShaderSource(hFrag, 1, &fs, NULL);
	delete [] vs;
	delete [] fs;

	glCompileShader(hVert);
	glCompileShader(hFrag);

	hProg = glCreateProgram();
	glAttachShader(hProg, hVert);
	glAttachShader(hProg, hFrag);

	glLinkProgram(hProg);
	glUseProgram(hProg);
}

char *TextFileRead(char *fn) 
{
	FILE *fp;
	
	int count = 0;
	fopen_s(&fp, fn, "rt");
	fseek(fp, 0, SEEK_END);
	count = ftell(fp);
	rewind(fp);

	char *content = new char [count + 1];
	count = fread(content, sizeof(char), count, fp);
	content[count] = '\0';
	fclose(fp);

	return content;
}