// transformations.h
/**
 * @file transformations.h
 * @brief Define a interface pública e as estruturas de dados para o módulo de transformações geométricas.
 * Este arquivo contém o "contrato" das funções matemáticas que manipulam os objetos da cena.
 */

#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "objects.h"
#include "point.h"

// Estrutura para uma Matriz 3x3, usada para representar transformações 2D
// em coordenadas homogêneas.
typedef struct {
    float m[3][3];
} Matrix3x3;


// --- Funções para Criação e Manipulação de Matrizes ---

/**
 * @brief Cria e retorna uma matriz identidade 3x3.
 */
Matrix3x3 createIdentityMatrix();

/**
 * @brief Cria uma matriz de translação 2D.
 */
Matrix3x3 createTranslationMatrix(float tx, float ty);

/**
 * @brief Cria uma matriz de escala 2D.
 */
Matrix3x3 createScaleMatrix(float sx, float sy);

/**
 * @brief Cria uma matriz de rotação 2D.
 * @param angleDegrees O ângulo de rotação em graus.
 */
Matrix3x3 createRotationMatrix(float angleDegrees);

/**
 * @brief Cria uma matriz de reflexão (espelhamento) 2D.
 * @param axis O eixo de reflexão. Use 0 para refletir sobre o eixo X e 1 para o eixo Y.
 */
Matrix3x3 createReflectionMatrix(int axis);

/**
 * @brief Cria uma matriz de cisalhamento (shear) 2D.
 * @param shx Fator de cisalhamento na direção X.
 * @param shy Fator de cisalhamento na direção Y.
 */
Matrix3x3 createShearMatrix(float shx, float shy);

/**
 * @brief Multiplica uma matriz de transformação por um ponto 2D em coordenadas homogêneas.
 */
Point multiplyMatrixByPoint(Matrix3x3 mat, Point p);

/**
 * @brief Multiplica duas matrizes 3x3 (M = m1 * m2), essencial para transformações compostas.
 */
Matrix3x3 multiplyMatrices(Matrix3x3 m1, Matrix3x3 m2);


// --- Funções de Transformação de Objetos (Interface Pública) ---

/**
 * @brief Translada (move) um objeto da cena.
 */
void translateObject(int objectIndex, float dx, float dy);

/**
 * @brief Rotaciona um objeto em torno de seu próprio centro.
 * @param angle O ângulo de rotação em graus.
 */
void rotateObject(int objectIndex, float angle);

/**
 * @brief Aplica uma escala a um objeto a partir de seu próprio centro.
 */
void scaleObject(int objectIndex, float sx, float sy);

/**
 * @brief Reflete (espelha) um objeto através de um eixo.
 * @param axis O eixo de reflexão (0 para eixo X, 1 para eixo Y).
 */
void reflectObject(int objectIndex, int axis);

/**
 * @brief Aplica um cisalhamento (distorção) a um objeto.
 */
void shearObject(int objectIndex, float shx, float shy);


#endif // TRANSFORMATIONS_H
