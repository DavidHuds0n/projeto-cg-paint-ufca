// transformations.h
// Define a interface pública e as estruturas de dados para o módulo de
// transformações geométricas. Este arquivo contém o "contrato" das funções
// matemáticas que manipulam os objetos da cena.

#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "objects.h"
#include "point.h"

// Estrutura para uma Matriz 3x3, usada para representar transformações 2D
// em coordenadas homogêneas.
typedef struct {
    float m[3][3];
} Matrix3x3;


// --- Funções para Manipulação de Matrizes ---

/**
 * @brief Cria e retorna uma matriz identidade 3x3.
 * @return Matrix3x3 A matriz identidade.
 */
Matrix3x3 createIdentityMatrix();

/**
 * @brief Cria uma matriz de translação 2D usando coordenadas homogêneas.
 * @param tx Fator de translação no eixo X.
 * @param ty Fator de translação no eixo Y.
 * @return Matrix3x3 A matriz de translação.
 */
Matrix3x3 createTranslationMatrix(float tx, float ty);

/**
 * @brief Multiplica uma matriz de transformação 3x3 por um ponto 2D.
 * Trata o ponto P(x, y) como um vetor em coordenadas homogêneas [x, y, 1].
 * @param mat A matriz de transformação.
 * @param p O ponto a ser transformado.
 * @return Point O ponto com as novas coordenadas após a transformação.
 */
Point multiplyMatrixByPoint(Matrix3x3 mat, Point p);

/**
 * @brief Multiplica duas matrizes 3x3 (M = m1 * m2).
 * Essencial para criar transformações compostas.
 * @param m1 A matriz da esquerda na multiplicação.
 * @param m2 A matriz da direita na multiplicação.
 * @return Matrix3x3 A matriz resultante da multiplicação.
 */
Matrix3x3 multiplyMatrices(Matrix3x3 m1, Matrix3x3 m2);


// --- Funções de Transformação de Objetos ---

/**
 * @brief Translada (move) um objeto da cena.
 * @param objectIndex O índice do objeto na lista global g_objects.
 * @param dx O deslocamento a ser aplicado no eixo X.
 * @param dy O deslocamento a ser aplicado no eixo Y.
 */
void translateObject(int objectIndex, float dx, float dy);

/**
 * @brief Rotaciona um objeto em torno de seu próprio centro.
 * @param objectIndex O índice do objeto na lista global.
 * @param angle O ângulo de rotação em *graus*.
 */
void rotateObject(int objectIndex, float angle);

/**
 * @brief Aplica uma escala a um objeto a partir de seu próprio centro.
 * @param objectIndex O índice do objeto na lista global.
 * @param sx Fator de escala no eixo X.
 * @param sy Fator de escala no eixo Y.
 */
void scaleObject(int objectIndex, float sx, float sy);

/**
 * @brief Reflete (espelha) um objeto através de um eixo.
 * @param objectIndex O índice do objeto na lista global.
 * @param axis O eixo de reflexão (0 para eixo X, 1 para eixo Y).
 */
void reflectObject(int objectIndex, int axis);

/**
 * @brief Aplica um cisalhamento (distorção) a um objeto.
 * @param objectIndex O índice do objeto na lista global.
 * @param shx Fator de cisalhamento na direção X.
 * @param shy Fator de cisalhamento na direção Y.
 */
void shearObject(int objectIndex, float shx, float shy);


#endif // TRANSFORMATIONS_H
