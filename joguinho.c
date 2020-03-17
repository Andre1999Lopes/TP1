#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>

void draw(){
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(tom,1,tom);
	glBegin(GL_QUADS);
		glVertex3f(20,20,0);
		glVertex3f(80,20,0);
		glVertex3f(80,80,0);
		glVertex3f(20,80,0);
	glEnd();
	glutSwapBuffers();
}

void setup(){
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

void reshape(int width, int height)
{
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y){
	switch(key){
		case 27:
			printf("Deseja mesmo sair?\n");
			if()
				exit(0);
			break;
		case 'p':
		case 'P':
			break;
		default:
			break;
	}
}

void specialKeyboard(unsigned char key, int x, int y){
	switch(key){
		case GLUT_KEY_LEFT:
			break;
		case GLUT_KEY_RIGHT:
			break;
	}
}

int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitContextVersion(1, 1);
   	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(500,500);
	glutCreateWindow("Quadrado de quadrados");
	glutDisplayFunc(draw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
	setup();
	glutMainLoop();
}