// input.h
// Define a interface p�blica e as vari�veis de estado para o m�dulo de
// gerenciamento de entrada do usu�rio. Controla a "m�quina de estados" do programa.

#ifndef INPUT_H
#define INPUT_H

#include "point.h"
#include "config.h"
#include "polygon.h"

// Enumera��o para os diferentes modos de opera��o do programa.
typedef enum {
    MODE_CREATE_POINT,
    MODE_CREATE_SEGMENT,
    MODE_CREATE_POLYGON,
    MODE_SELECT
} ProgramMode;


// --- Vari�veis Globais de Estado ---
extern ProgramMode g_currentMode;
extern int g_isDragging;

// Vari�veis de estado para a cria��o de objetos.
extern int g_segmentClickCount;
extern Point g_segmentP1;
extern Point g_polygonVertices[];
extern int g_polygonVertexCount;

// Vari�veis de estado para o mouse.
extern Point g_lastMousePos;
extern Point g_currentMousePos; // Armazena a posi��o atual do mouse (para rubber banding).


// --- Prot�tipos das Fun��es de Callback ---

/**
 * @brief Callback do GLUT para eventos de teclas normais do teclado.
 * @param key O c�digo ASCII da tecla pressionada.
 * @param x A coordenada X do mouse no momento em que a tecla foi pressionada.
 * @param y A coordenada Y do mouse no momento em que a tecla foi pressionada.
 */
void keyboardCallback(unsigned char key, int x, int y);

/**
 * @brief Callback do GLUT para eventos de clique do mouse.
 * @param button O bot�o que foi pressionado (ex: GLUT_LEFT_BUTTON).
 * @param state O estado do bot�o (GLUT_DOWN ou GLUT_UP).
 * @param x A coordenada X do mouse.
 * @param y A coordenada Y do mouse.
 */
void mouseCallback(int button, int state, int x, int y);

/**
 * @brief Callback do GLUT para eventos de movimento do mouse enquanto um bot�o est� pressionado.
 * @param x A coordenada X atual do mouse.
 * @param y A coordenada Y atual do mouse.
 */
void motionCallback(int x, int y);

/**
 * @brief Callback do GLUT para eventos de movimento do mouse SEM um bot�o estar pressionado.
 * @param x A coordenada X atual do mouse.
 * @param y A coordenada Y atual do mouse.
 */
void passiveMotionCallback(int x, int y);

#endif // INPUT_H
