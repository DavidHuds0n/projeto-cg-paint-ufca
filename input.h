// input.h
// Defini��es para o gerenciamento de entrada do usu�rio (teclado e mouse).

#ifndef INPUT_H
#define INPUT_H

#include "point.h"   // Para a estrutura Point
#include "config.h"  // Para MAX_POLYGON_VERTICES
#include "polygon.h" // Para a estrutura GfxPolygon

// Enumera��o para os diferentes modos de opera��o do programa.
typedef enum {
    MODE_CREATE_POINT,
    MODE_CREATE_SEGMENT,
    MODE_CREATE_POLYGON,
    MODE_SELECT
} ProgramMode;

// Vari�veis globais (declaradas aqui, definidas em input.c)
extern ProgramMode g_currentMode;       // Modo de opera��o atual do programa.
extern int g_segmentClickCount;         // Contador de cliques para cria��o de segmentos.
extern Point g_segmentP1;              // Primeiro ponto do segmento em cria��o.

// Vari�veis globais para a cria��o de pol�gonos
extern Point g_polygonVertices[MAX_POLYGON_VERTICES];
extern int g_polygonVertexCount;        // Contador de v�rtices adicionados ao pol�gono atual.

// NOVO: Vari�veis para o Arrastar e Soltar (Transla��o)
extern int g_isDragging;                // Flag para indicar se estamos arrastando um objeto
extern Point g_lastMousePos;            // Armazena a �ltima posi��o do mouse durante o arraste


// Prot�tipos das fun��es de callback do GLUT
void keyboardCallback(unsigned char key, int x, int y);
void mouseCallback(int button, int state, int x, int y);
void motionCallback(int x, int y); // NOVO: Prot�tipo para o callback de movimento do mouse enquanto um bot�o est� pressionado

#endif // INPUT_H
