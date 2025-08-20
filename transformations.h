// transformations.h
// Define a interface p�blica e as estruturas de dados para o m�dulo de
// transforma��es geom�tricas. Este arquivo cont�m o "contrato" das fun��es
// matem�ticas que manipulam os objetos da cena.

#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "objects.h"
#include "point.h"

// Estrutura para uma Matriz 3x3, usada para representar transforma��es 2D
// em coordenadas homog�neas.
typedef struct {
    float m[3][3];
} Matrix3x3;


// --- Fun��es para Manipula��o de Matrizes ---

/**
 * @brief Cria e retorna uma matriz identidade 3x3.
 * @return Matrix3x3 A matriz identidade.
 */
Matrix3x3 createIdentityMatrix();

/**
 * @brief Cria uma matriz de transla��o 2D usando coordenadas homog�neas.
 * @param tx Fator de transla��o no eixo X.
 * @param ty Fator de transla��o no eixo Y.
 * @return Matrix3x3 A matriz de transla��o.
 */
Matrix3x3 createTranslationMatrix(float tx, float ty);

/**
 * @brief Multiplica uma matriz de transforma��o 3x3 por um ponto 2D.
 * Trata o ponto P(x, y) como um vetor em coordenadas homog�neas [x, y, 1].
 * @param mat A matriz de transforma��o.
 * @param p O ponto a ser transformado.
 * @return Point O ponto com as novas coordenadas ap�s a transforma��o.
 */
Point multiplyMatrixByPoint(Matrix3x3 mat, Point p);

/**
 * @brief Multiplica duas matrizes 3x3 (M = m1 * m2).
 * Essencial para criar transforma��es compostas.
 * @param m1 A matriz da esquerda na multiplica��o.
 * @param m2 A matriz da direita na multiplica��o.
 * @return Matrix3x3 A matriz resultante da multiplica��o.
 */
Matrix3x3 multiplyMatrices(Matrix3x3 m1, Matrix3x3 m2);


// --- Fun��es de Transforma��o de Objetos ---

/**
 * @brief Translada (move) um objeto da cena.
 * @param objectIndex O �ndice do objeto na lista global g_objects.
 * @param dx O deslocamento a ser aplicado no eixo X.
 * @param dy O deslocamento a ser aplicado no eixo Y.
 */
void translateObject(int objectIndex, float dx, float dy);

/**
 * @brief Rotaciona um objeto em torno de seu pr�prio centro.
 * @param objectIndex O �ndice do objeto na lista global.
 * @param angle O �ngulo de rota��o em *graus*.
 */
void rotateObject(int objectIndex, float angle);

/**
 * @brief Aplica uma escala a um objeto a partir de seu pr�prio centro.
 * @param objectIndex O �ndice do objeto na lista global.
 * @param sx Fator de escala no eixo X.
 * @param sy Fator de escala no eixo Y.
 */
void scaleObject(int objectIndex, float sx, float sy);

/**
 * @brief Reflete (espelha) um objeto atrav�s de um eixo.
 * @param objectIndex O �ndice do objeto na lista global.
 * @param axis O eixo de reflex�o (0 para eixo X, 1 para eixo Y).
 */
void reflectObject(int objectIndex, int axis);

/**
 * @brief Aplica um cisalhamento (distor��o) a um objeto.
 * @param objectIndex O �ndice do objeto na lista global.
 * @param shx Fator de cisalhamento na dire��o X.
 * @param shy Fator de cisalhamento na dire��o Y.
 */
void shearObject(int objectIndex, float shx, float shy);


#endif // TRANSFORMATIONS_H
