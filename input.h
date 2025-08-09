// input.h
// Definições para o gerenciamento de entrada do usuário (teclado e mouse).

#ifndef INPUT_H
#define INPUT_H

#include "point.h"   // Para a estrutura Point
#include "config.h"  // Para MAX_POLYGON_VERTICES
#include "polygon.h" // Para a estrutura GfxPolygon

// Enumeração para os diferentes modos de operação do programa.
typedef enum {
    MODE_CREATE_POINT,
    MODE_CREATE_SEGMENT,
    MODE_CREATE_POLYGON,
    MODE_SELECT
} ProgramMode;

// Variáveis globais (declaradas aqui, definidas em input.c)
extern ProgramMode g_currentMode;       // Modo de operação atual do programa.
extern int g_segmentClickCount;         // Contador de cliques para criação de segmentos.
extern Point g_segmentP1;              // Primeiro ponto do segmento em criação.

// Variáveis globais para a criação de polígonos
extern Point g_polygonVertices[MAX_POLYGON_VERTICES];
extern int g_polygonVertexCount;        // Contador de vértices adicionados ao polígono atual.

// NOVO: Variáveis para o Arrastar e Soltar (Translação)
extern int g_isDragging;                // Flag para indicar se estamos arrastando um objeto
extern Point g_lastMousePos;            // Armazena a última posição do mouse durante o arraste


// Protótipos das funções de callback do GLUT
void keyboardCallback(unsigned char key, int x, int y);
void mouseCallback(int button, int state, int x, int y);
void motionCallback(int x, int y); // NOVO: Protótipo para o callback de movimento do mouse enquanto um botão está pressionado

#endif // INPUT_H
