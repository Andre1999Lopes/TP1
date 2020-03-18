#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>

int pause=0;
int direita;
int esquerda;

void draw(){
	
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
			if(pause==0)
				pause=1;
			else
				pause=0;''
			break;
		default:
			break;
	}
}

void specialKeyboard(unsigned char key, int x, int y){
	switch(key){
		case GLUT_KEY_LEFT:
			esquerda=1;
			break;
		case GLUT_KEY_RIGHT:
			direita=1;
			break;
	}
}

void specialKeyboardUp(unsigned char key, int x, int y){
	switch(key){
		case GLUT_KEY_LEFT:
			esquerda=0;
			break;
		case GLUT_KEY_RIGHT:
			direita=0;
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
	glutSpecialFunc(specialKeyboard)
	glutSpecialUpFunc(specialKeyboardUp);
	setup();
	glutMainLoop();
}
