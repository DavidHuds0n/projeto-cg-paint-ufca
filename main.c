// main.c
// Ponto de entrada e orquestrador principal do programa "Rabisquim".
// Este arquivo � respons�vel por:
// - Inicializar a biblioteca GLUT e criar a janela da aplica��o.
// - Configurar o ambiente de renderiza��o 2D do OpenGL.
// - Registrar as fun��es de callback para eventos de display, teclado e mouse.
// - Gerenciar o loop principal de eventos e a limpeza de recursos ao final.

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "render.h"
#include "input.h"
#include "objects.h"
#include "point.h"
#include "segment.h"
#include "polygon.h"
#include "transformations.h"

void displayCallback() {
    glClear(GL_COLOR_BUFFER_BIT);
    renderAllObjects();
    glutSwapBuffers();
}

void initOpenGL() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow(WINDOW_TITLE);

    initOpenGL();
    initObjectList();

    // Registro de Callbacks
    glutDisplayFunc(displayCallback);
    glutKeyboardFunc(keyboardCallback);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(motionCallback);
    glutPassiveMotionFunc(passiveMotionCallback);

    printf("\n--- Rabisquim - Instru��es ---\n");
    printf("Pressione 'P': Modo Criar Pontos\n");
    printf("Pressione 'L': Modo Criar Segmentos\n");
    printf("Pressione 'O': Modo Criar Pol�gonos\n");
    printf("Pressione 'S': Modo Sele��o\n");
    printf("Pressione 'DEL': Excluir objeto selecionado\n");
    printf("Pressione 'ESC': Sair do programa\n");
    printf("------------------------------\n");

    glutMainLoop();

    clearAllObjects();
    return 0;
}
