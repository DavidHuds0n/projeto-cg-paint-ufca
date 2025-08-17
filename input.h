// input.h
// Define a interface p�blica e as vari�veis de estado para o m�dulo de
// gerenciamento de entrada do usu�rio. Controla a "m�quina de estados" do programa.

#ifndef INPUT_H
#define INPUT_H

#include "point.h"
#include "config.h"
#include "polygon.h"

// Enumera��o para os diferentes modos de opera��o do programa.
// A vari�vel global g_currentMode usar� este enum para controlar o que
// os cliques do mouse e as teclas fazem em um dado momento.
typedef enum {
    MODE_CREATE_POINT,   // Modo para criar novos pontos.
    MODE_CREATE_SEGMENT, // Modo para criar novos segmentos de reta.
    MODE_CREATE_POLYGON, // Modo para criar novos pol�gonos.
    MODE_SELECT          // Modo para selecionar e manipular objetos existentes.
} ProgramMode;


// --- Vari�veis Globais de Estado ---
// O estado atual da aplica��o � gerenciado por estas vari�veis globais.

extern ProgramMode g_currentMode;     // Armazena o modo de opera��o atual.
extern int g_isDragging;              // Flag (1 ou 0) que indica se um objeto est� sendo arrastado.

// Vari�veis de estado para a cria��o de objetos em m�ltiplos passos.
extern int g_segmentClickCount;       // Contador para a cria��o de segmentos (precisa de 2 cliques).
extern Point g_segmentP1;             // Armazena o primeiro ponto na cria��o de um segmento.
extern Point g_polygonVertices[];     // Buffer tempor�rio para os v�rtices do pol�gono em cria��o.
extern int g_polygonVertexCount;      // Contador de v�rtices no buffer do pol�gono.
extern Point g_lastMousePos;          // Armazena a �ltima posi��o do mouse durante o arraste.


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
 * Utilizado para a funcionalidade de "arrastar e soltar".
 * @param x A coordenada X atual do mouse.
 * @param y A coordenada Y atual do mouse.
 */
void motionCallback(int x, int y);

#endif // INPUT_H
