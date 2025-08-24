/**
 * @file input.h
 * @brief Define a interface p�blica e as vari�veis de estado do m�dulo de entrada.
 *
 * Este arquivo declara as fun��es de callback do GLUT e as vari�veis globais
 * que controlam a m�quina de estados do programa de acordo com as a��es do usu�rio.
 */

#ifndef INPUT_H
#define INPUT_H

#include "point.h"
#include "config.h"
#include "polygon.h"

// --- SE��O DE ENUMS E DEFINI��ES ---

/**
 * @brief Enumera��o dos modos de opera��o do programa.
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

// --- SE��O DE VARI�VEIS GLOBAIS DE ESTADO (DECLARA��ES) ---

extern ProgramMode g_currentMode;               ///< O modo de opera��o atual do programa.
extern int g_isDragging;                        ///< Sinaliza se o usu�rio est� arrastando o mouse.
extern int g_segmentClickCount;                 ///< Contador de cliques para a cria��o de segmentos.
extern Point g_segmentP1;                       ///< O primeiro ponto clicado na cria��o de um segmento.
extern Point g_polygonVertices[MAX_POLYGON_VERTICES]; ///< Vetor tempor�rio para os v�rtices do pol�gono.
extern int g_polygonVertexCount;                ///< Contador de v�rtices na cria��o de um pol�gono.
extern Point g_lastMousePos;                    ///< Posi��o anterior do mouse durante um movimento.
extern Point g_currentMousePos;                 ///< Posi��o atual do mouse para pr�-visualiza��es.

// --- SE��O DE PROT�TIPOS DAS FUN��ES DE CALLBACK ---

void keyboardCallback(unsigned char key, int x, int y);
void specialKeysCallback(int key, int x, int y);
void mouseCallback(int button, int state, int x, int y);
void motionCallback(int x, int y);
void passiveMotionCallback(int x, int y);

#endif // INPUT_H
