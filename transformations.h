/**
 * @file transformations.h
 * @brief Define a interface p�blica e as estruturas de dados para as transforma��es geom�tricas.
 *
 * Este arquivo cont�m as estruturas e prot�tipos das fun��es que manipulam
 * matrizes de transforma��o e as aplicam aos objetos da cena.
 */

#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "objects.h"
#include "point.h"

// --- SE��O DE ESTRUTURAS DE DADOS ---

/**
 * @brief Estrutura para uma Matriz 3x3.
 *
 * Usada para representar transforma��es 2D em coordenadas homog�neas,
 * permitindo a composi��o de m�ltiplas transforma��es.
 */
typedef struct {
    float m[3][3];
} Matrix3x3;

// --- SE��O DE FUN��ES AUXILIARES DE MATRIZES ---
// Fun��es internas para criar e manipular matrizes.

Matrix3x3 createIdentityMatrix();
Matrix3x3 createTranslationMatrix(float tx, float ty);
Matrix3x3 createScaleMatrix(float sx, float sy);
Matrix3x3 createRotationMatrix(float angleDegrees);
Matrix3x3 createReflectionMatrix(int axis);
Matrix3x3 createShearMatrix(float shx, float shy);
Point multiplyMatrixByPoint(Matrix3x3 mat, Point p);
Matrix3x3 multiplyMatrices(Matrix3x3 m1, Matrix3x3 m2);

// --- SE��O DE FUN��ES DE TRANSFORMA��O DE OBJETOS (INTERFACE P�BLICA) ---
// Fun��es que outros m�dulos devem chamar para transformar os objetos.

/**
 * @brief Aplica uma transla��o (movimento) a um objeto da cena.
 * @param objectIndex O �ndice do objeto no array global.
 * @param dx O deslocamento no eixo X.
 * @param dy O deslocamento no eixo Y.
 */
void translateObject(int objectIndex, float dx, float dy);

/**
 * @brief Aplica uma rota��o a um objeto em torno de seu pr�prio centro.
 * @param objectIndex O �ndice do objeto no array global.
 * @param angle O �ngulo de rota��o em graus.
 */
void rotateObject(int objectIndex, float angle);

/**
 * @brief Aplica uma escala a um objeto a partir de seu pr�prio centro.
 * @param objectIndex O �ndice do objeto no array global.
 * @param sx O fator de escala no eixo X.
 * @param sy O fator de escala no eixo Y.
 */
void scaleObject(int objectIndex, float sx, float sy);

/**
 * @brief Aplica uma reflex�o (espelhamento) a um objeto em rela��o ao seu centro.
 * @param objectIndex O �ndice do objeto no array global.
 * @param axis O eixo de reflex�o (0 para eixo X, 1 para eixo Y).
 */
void reflectObject(int objectIndex, int axis);

/**
 * @brief Aplica um cisalhamento a um objeto em rela��o ao seu pr�prio centro.
 * @param objectIndex O �ndice do objeto no array global.
 * @param shx O fator de cisalhamento no eixo X.
 * @param shy O fator de cisalhamento no eixo Y.
 */
void shearObject(int objectIndex, float shx, float shy);

#endif // TRANSFORMATIONS_H
