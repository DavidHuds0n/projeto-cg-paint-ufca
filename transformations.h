/**
 * @file transformations.h
 * @brief Define a interface pública e as estruturas de dados para as transformações geométricas.
 *
 * Este arquivo contém as estruturas e protótipos das funções que manipulam
 * matrizes de transformação e as aplicam aos objetos da cena.
 */

#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "objects.h"
#include "point.h"

// --- SEÇÃO DE ESTRUTURAS DE DADOS ---

/**
 * @brief Estrutura para uma Matriz 3x3.
 *
 * Usada para representar transformações 2D em coordenadas homogêneas,
 * permitindo a composição de múltiplas transformações.
 */
typedef struct {
    float m[3][3];
} Matrix3x3;

// --- SEÇÃO DE FUNÇÕES AUXILIARES DE MATRIZES ---
// Funções internas para criar e manipular matrizes.

Matrix3x3 createIdentityMatrix();
Matrix3x3 createTranslationMatrix(float tx, float ty);
Matrix3x3 createScaleMatrix(float sx, float sy);
Matrix3x3 createRotationMatrix(float angleDegrees);
Matrix3x3 createReflectionMatrix(int axis);
Matrix3x3 createShearMatrix(float shx, float shy);
Point multiplyMatrixByPoint(Matrix3x3 mat, Point p);
Matrix3x3 multiplyMatrices(Matrix3x3 m1, Matrix3x3 m2);

// --- SEÇÃO DE FUNÇÕES DE TRANSFORMAÇÃO DE OBJETOS (INTERFACE PÚBLICA) ---
// Funções que outros módulos devem chamar para transformar os objetos.

/**
 * @brief Aplica uma translação (movimento) a um objeto da cena.
 * @param objectIndex O índice do objeto no array global.
 * @param dx O deslocamento no eixo X.
 * @param dy O deslocamento no eixo Y.
 */
void translateObject(int objectIndex, float dx, float dy);

/**
 * @brief Aplica uma rotação a um objeto em torno de seu próprio centro.
 * @param objectIndex O índice do objeto no array global.
 * @param angle O ângulo de rotação em graus.
 */
void rotateObject(int objectIndex, float angle);

/**
 * @brief Aplica uma escala a um objeto a partir de seu próprio centro.
 * @param objectIndex O índice do objeto no array global.
 * @param sx O fator de escala no eixo X.
 * @param sy O fator de escala no eixo Y.
 */
void scaleObject(int objectIndex, float sx, float sy);

/**
 * @brief Aplica uma reflexão (espelhamento) a um objeto em relação ao seu centro.
 * @param objectIndex O índice do objeto no array global.
 * @param axis O eixo de reflexão (0 para eixo X, 1 para eixo Y).
 */
void reflectObject(int objectIndex, int axis);

/**
 * @brief Aplica um cisalhamento a um objeto em relação ao seu próprio centro.
 * @param objectIndex O índice do objeto no array global.
 * @param shx O fator de cisalhamento no eixo X.
 * @param shy O fator de cisalhamento no eixo Y.
 */
void shearObject(int objectIndex, float shx, float shy);

#endif // TRANSFORMATIONS_H
