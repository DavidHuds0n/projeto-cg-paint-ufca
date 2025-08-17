// input.h
// Define a interface pública e as variáveis de estado para o módulo de
// gerenciamento de entrada do usuário. Controla a "máquina de estados" do programa.

#ifndef INPUT_H
#define INPUT_H

#include "point.h"
#include "config.h"
#include "polygon.h"

// Enumeração para os diferentes modos de operação do programa.
// A variável global g_currentMode usará este enum para controlar o que
// os cliques do mouse e as teclas fazem em um dado momento.
typedef enum {
    MODE_CREATE_POINT,   // Modo para criar novos pontos.
    MODE_CREATE_SEGMENT, // Modo para criar novos segmentos de reta.
    MODE_CREATE_POLYGON, // Modo para criar novos polígonos.
    MODE_SELECT          // Modo para selecionar e manipular objetos existentes.
} ProgramMode;


// --- Variáveis Globais de Estado ---
// O estado atual da aplicação é gerenciado por estas variáveis globais.

extern ProgramMode g_currentMode;     // Armazena o modo de operação atual.
extern int g_isDragging;              // Flag (1 ou 0) que indica se um objeto está sendo arrastado.

// Variáveis de estado para a criação de objetos em múltiplos passos.
extern int g_segmentClickCount;       // Contador para a criação de segmentos (precisa de 2 cliques).
extern Point g_segmentP1;             // Armazena o primeiro ponto na criação de um segmento.
extern Point g_polygonVertices[];     // Buffer temporário para os vértices do polígono em criação.
extern int g_polygonVertexCount;      // Contador de vértices no buffer do polígono.
extern Point g_lastMousePos;          // Armazena a última posição do mouse durante o arraste.


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
 * Utilizado para a funcionalidade de "arrastar e soltar".
 * @param x A coordenada X atual do mouse.
 * @param y A coordenada Y atual do mouse.
 */
void motionCallback(int x, int y);

#endif // INPUT_H
