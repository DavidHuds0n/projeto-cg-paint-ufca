/**
 * @file input.h
 * @brief Define a interface pública e as variáveis de estado do módulo de entrada.
 *
 * Este arquivo declara as funções de callback do GLUT e as variáveis globais
 * que controlam a máquina de estados do programa de acordo com as ações do usuário.
 */

#ifndef INPUT_H
#define INPUT_H

#include "point.h"
#include "config.h"
#include "polygon.h"

// --- SEÇÃO DE ENUMS E DEFINIÇÕES ---

/**
 * @brief Enumeração dos modos de operação do programa.
 */
typedef enum {
    MODE_SELECT,
    MODE_CREATE_POINT,
    MODE_CREATE_SEGMENT,
    MODE_CREATE_POLYGON,
    MODE_TRANSLATE,
    MODE_ROTATE,
    MODE_SCALE,
    MODE_SHEAR,
    MODE_REFLECT
} ProgramMode;

// --- SEÇÃO DE VARIÁVEIS GLOBAIS DE ESTADO (DECLARAÇÕES) ---

extern ProgramMode g_currentMode;               ///< O modo de operação atual do programa.
extern int g_isDragging;                        ///< Sinaliza se o usuário está arrastando o mouse.
extern int g_segmentClickCount;                 ///< Contador de cliques para a criação de segmentos.
extern Point g_segmentP1;                       ///< O primeiro ponto clicado na criação de um segmento.
extern Point g_polygonVertices[MAX_POLYGON_VERTICES]; ///< Vetor temporário para os vértices do polígono.
extern int g_polygonVertexCount;                ///< Contador de vértices na criação de um polígono.
extern Point g_lastMousePos;                    ///< Posição anterior do mouse durante um movimento.
extern Point g_currentMousePos;                 ///< Posição atual do mouse para pré-visualizações.

// --- SEÇÃO DE PROTÓTIPOS DAS FUNÇÕES DE CALLBACK ---

void keyboardCallback(unsigned char key, int x, int y);
void specialKeysCallback(int key, int x, int y);
void mouseCallback(int button, int state, int x, int y);
void motionCallback(int x, int y);
void passiveMotionCallback(int x, int y);

#endif // INPUT_H
