// input.h
// Define a interface p�blica e as vari�veis de estado para o m�dulo de
// gerenciamento de entrada do usu�rio. Controla a "m�quina de estados" do programa.

#ifndef INPUT_H
#define INPUT_H

#include "point.h"
#include "config.h"
#include "polygon.h"

// Enumera��o expandida para todos os modos de opera��o do programa.
typedef enum {
    MODE_CREATE_POINT,
    MODE_CREATE_SEGMENT,
    MODE_CREATE_POLYGON,
    MODE_SELECT,
    MODE_TRANSLATE,
    MODE_ROTATE,
    MODE_SCALE,
    MODE_SHEAR,
    MODE_REFLECT
} ProgramMode;


// --- Vari�veis Globais de Estado ---
extern ProgramMode g_currentMode;

// Vari�veis para a funcionalidade de "Arrastar e Soltar" (Drag-and-Drop).
extern int g_isDragging;
extern Point g_lastMousePos;

// Vari�veis de estado para a cria��o de objetos.
extern int g_segmentClickCount;
extern Point g_segmentP1;
extern Point g_polygonVertices[];
extern int g_polygonVertexCount;

// Armazena a posi��o atual do mouse (para rubber banding).
extern Point g_currentMousePos;


// --- Prot�tipos das Fun��es de Callback ---

void keyboardCallback(unsigned char key, int x, int y);
void mouseCallback(int button, int state, int x, int y);
void motionCallback(int x, int y);
void passiveMotionCallback(int x, int y);
void specialKeysCallback(int key, int x, int y);

#endif // INPUT_H
