// input.h
// Define a interface pública e as variáveis de estado para o módulo de
// gerenciamento de entrada do usuário. Controla a "máquina de estados" do programa.

#ifndef INPUT_H
#define INPUT_H

#include "point.h"
#include "config.h"
#include "polygon.h"

// Enumeração para os diferentes modos de operação do programa.
typedef enum {
    MODE_CREATE_POINT,
    MODE_CREATE_SEGMENT,
    MODE_CREATE_POLYGON,
    MODE_SELECT
} ProgramMode;


// --- Variáveis Globais de Estado ---
extern ProgramMode g_currentMode;
extern int g_isDragging;

// Variáveis de estado para a criação de objetos.
extern int g_segmentClickCount;
extern Point g_segmentP1;
extern Point g_polygonVertices[];
extern int g_polygonVertexCount;

// Variáveis de estado para o mouse.
extern Point g_lastMousePos;
extern Point g_currentMousePos; // Armazena a posição atual do mouse (para rubber banding).


// --- Protótipos das Funções de Callback ---

/**
 * @brief Callback do GLUT para eventos de teclas normais do teclado.
 * @param key O código ASCII da tecla pressionada.
 * @param x A coordenada X do mouse no momento em que a tecla foi pressionada.
 * @param y A coordenada Y do mouse no momento em que a tecla foi pressionada.
 */
void keyboardCallback(unsigned char key, int x, int y);

/**
 * @brief Callback do GLUT para eventos de clique do mouse.
 * @param button O botão que foi pressionado (ex: GLUT_LEFT_BUTTON).
 * @param state O estado do botão (GLUT_DOWN ou GLUT_UP).
 * @param x A coordenada X do mouse.
 * @param y A coordenada Y do mouse.
 */
void mouseCallback(int button, int state, int x, int y);

/**
 * @brief Callback do GLUT para eventos de movimento do mouse enquanto um botão está pressionado.
 * @param x A coordenada X atual do mouse.
 * @param y A coordenada Y atual do mouse.
 */
void motionCallback(int x, int y);

/**
 * @brief Callback do GLUT para eventos de movimento do mouse SEM um botão estar pressionado.
 * @param x A coordenada X atual do mouse.
 * @param y A coordenada Y atual do mouse.
 */
void passiveMotionCallback(int x, int y);

#endif // INPUT_H
