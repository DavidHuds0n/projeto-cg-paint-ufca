// transformations.h
// Prot�tipos das fun��es para transforma��es geom�tricas.

#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

// Inclui objects.h porque as transforma��es atuar�o sobre os objetos globais.
#include "objects.h"
#include "point.h" // Para usar a estrutura Point

// NOVO: Estrutura para uma Matriz 3x3 (para transforma��es 2D homog�neas)
typedef struct {
    float m[3][3]; // Armazena os elementos da matriz
} Matrix3x3;

// Fun��es para manipula��o de matrizes
Matrix3x3 createIdentityMatrix();                                // Cria uma matriz identidade
Matrix3x3 createTranslationMatrix(float tx, float ty);           // Cria uma matriz de transla��o
Point multiplyMatrixByPoint(Matrix3x3 mat, Point p);             // Multiplica uma matriz por um ponto
Matrix3x3 multiplyMatrices(Matrix3x3 m1, Matrix3x3 m2);          // Multiplica duas matrizes (para composi��o)

// Fun��o para transladar (mover) um objeto selecionado.
// Recebe o �ndice do objeto a ser transladado e os valores de deslocamento em X e Y.
void translateObject(int objectIndex, float dx, float dy);

// Adicionar prot�tipos para outras transforma��es aqui no futuro:
// void rotateObject(int objectIndex, float angle);
// void scaleObject(int objectIndex, float sx, float sy);
// void reflectObject(int objectIndex, int axis); // ex: axis = 0 para X, 1 para Y
// void shearObject(int objectIndex, float shx, float shy);

#endif // TRANSFORMATIONS_H
