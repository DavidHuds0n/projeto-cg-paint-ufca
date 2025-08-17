// main.c
// Ponto de entrada e orquestrador principal do programa "Rabisquim".
// Este arquivo é responsável por:
// - Inicializar a biblioteca GLUT e criar a janela da aplicação.
// - Configurar o ambiente de renderização 2D do OpenGL.
// - Registrar as funções de callback para eventos de display, teclado e mouse.
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

    printf("\n--- Rabisquim - Instruções ---\n");
    printf("Pressione 'P': Modo Criar Pontos\n");
    printf("Pressione 'L': Modo Criar Segmentos\n");
    printf("Pressione 'O': Modo Criar Polígonos\n");
    printf("Pressione 'S': Modo Seleção\n");
    printf("Pressione 'DEL': Excluir objeto selecionado\n");
    printf("Pressione 'ESC': Sair do programa\n");
    printf("------------------------------\n");

    glutMainLoop();

    clearAllObjects();
    return 0;
}
