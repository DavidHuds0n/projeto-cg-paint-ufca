// transformations.h
/**
 * @file transformations.h
 * @brief Define a interface p�blica e as estruturas de dados para o m�dulo de transforma��es geom�tricas.
 * Este arquivo cont�m o "contrato" das fun��es matem�ticas que manipulam os objetos da cena.
 */

#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "objects.h"
#include "point.h"

// Estrutura para uma Matriz 3x3, usada para representar transforma��es 2D
// em coordenadas homog�neas.
typedef struct {
    float m[3][3];
} Matrix3x3;


// --- Fun��es para Cria��o e Manipula��o de Matrizes ---

/**
 * @brief Cria e retorna uma matriz identidade 3x3.
 */
Matrix3x3 createIdentityMatrix();

/**
 * @brief Cria uma matriz de transla��o 2D.
 */
Matrix3x3 createTranslationMatrix(float tx, float ty);

/**
 * @brief Cria uma matriz de escala 2D.
 */
Matrix3x3 createScaleMatrix(float sx, float sy);

/**
 * @brief Cria uma matriz de rota��o 2D.
 * @param angleDegrees O �ngulo de rota��o em graus.
 */
Matrix3x3 createRotationMatrix(float angleDegrees);

/**
 * @brief Cria uma matriz de reflex�o (espelhamento) 2D.
 * @param axis O eixo de reflex�o. Use 0 para refletir sobre o eixo X e 1 para o eixo Y.
 */
Matrix3x3 createReflectionMatrix(int axis);

/**
 * @brief Cria uma matriz de cisalhamento (shear) 2D.
 * @param shx Fator de cisalhamento na dire��o X.
 * @param shy Fator de cisalhamento na dire��o Y.
 */
Matrix3x3 createShearMatrix(float shx, float shy);

/**
 * @brief Multiplica uma matriz de transforma��o por um ponto 2D em coordenadas homog�neas.
 */
Point multiplyMatrixByPoint(Matrix3x3 mat, Point p);

/**
 * @brief Multiplica duas matrizes 3x3 (M = m1 * m2), essencial para transforma��es compostas.
 */
Matrix3x3 multiplyMatrices(Matrix3x3 m1, Matrix3x3 m2);


// --- Fun��es de Transforma��o de Objetos (Interface P�blica) ---

/**
 * @brief Translada (move) um objeto da cena.
 */
void translateObject(int objectIndex, float dx, float dy);

/**
 * @brief Rotaciona um objeto em torno de seu pr�prio centro.
 * @param angle O �ngulo de rota��o em graus.
 */
void rotateObject(int objectIndex, float angle);

/**
 * @brief Aplica uma escala a um objeto a partir de seu pr�prio centro.
 */
void scaleObject(int objectIndex, float sx, float sy);

/**
 * @brief Reflete (espelha) um objeto atrav�s de um eixo.
 * @param axis O eixo de reflex�o (0 para eixo X, 1 para eixo Y).
 */
void reflectObject(int objectIndex, int axis);

/**
 * @brief Aplica um cisalhamento (distor��o) a um objeto.
 */
void shearObject(int objectIndex, float shx, float shy);


#endif // TRANSFORMATIONS_H
