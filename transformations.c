// transformations.c
// Implementação das funções para transformações geométricas.

#include <stdio.h>
#include "transformations.h" // Inclui o próprio cabeçalho
#include "objects.h"         // Para acessar g_objects
#include "point.h"           // Para Point
#include "segment.h"         // Para Segment
#include "polygon.h"         // Para GfxPolygon

// Implementação das funções de matriz (já existentes)

// Cria e retorna uma matriz identidade 3x3
Matrix3x3 createIdentityMatrix() {
    Matrix3x3 mat;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            mat.m[i][j] = (i == j) ? 1.0f : 0.0f; // Diagonal principal é 1, o resto é 0
        }
    }
    return mat;
}

// Cria e retorna uma matriz de translação 3x3
Matrix3x3 createTranslationMatrix(float tx, float ty) {
    Matrix3x3 mat = createIdentityMatrix(); // Começa com uma matriz identidade
    mat.m[0][2] = tx; // Elemento para translação em X (posição [linha][coluna])
    mat.m[1][2] = ty; // Elemento para translação em Y
    return mat;
}

// Multiplica uma matriz 3x3 por um ponto (representado como vetor 3x1 [x, y, 1])
Point multiplyMatrixByPoint(Matrix3x3 mat, Point p) {
    Point transformedP;
    // Em coordenadas homogêneas, um ponto P(x, y) é (x, y, 1)
    // Multiplicação da matriz 3x3 pelo vetor 3x1 (p.x, p.y, 1)

    transformedP.x = mat.m[0][0] * p.x + mat.m[0][1] * p.y + mat.m[0][2] * 1.0f;
    transformedP.y = mat.m[1][0] * p.x + mat.m[1][1] * p.y + mat.m[1][2] * 1.0f;
    // A terceira componente (w) após a multiplicação seria:
    // w = mat.m[2][0] * p.x + mat.m[2][1] * p.y + mat.m[2][2] * 1.0f;
    // Para transformações 2D padrão, a terceira linha da matriz de transformação homogênea geralmente é (0, 0, 1),
    // então w sempre será 1.0f. Não precisamos armazenar explicitamente 'w' para pontos 2D.

    return transformedP;
}

// Multiplica duas matrizes 3x3 (m1 * m2) e retorna o resultado
Matrix3x3 multiplyMatrices(Matrix3x3 m1, Matrix3x3 m2) {
    Matrix3x3 result;
    for (int i = 0; i < 3; i++) { // Linhas da matriz resultante
        for (int j = 0; j < 3; j++) { // Colunas da matriz resultante
            result.m[i][j] = 0.0f;
            for (int k = 0; k < 3; k++) { // Coluna de m1 e linha de m2
                result.m[i][j] += m1.m[i][k] * m2.m[k][j];
            }
        }
    }
    return result;
}


// Função para transladar (mover) um objeto.
// AGORA UTILIZANDO MATRIZES DE TRANSFORMAÇÃO
void translateObject(int objectIndex, float dx, float dy) {
    if (objectIndex < 0 || objectIndex >= g_numObjects) {
        fprintf(stderr, "[Transformations ERROR] Índice de objeto inválido para translação: %d\n", objectIndex);
        return;
    }

    Object* obj = &g_objects[objectIndex];

    // Cria a matriz de translação para o deslocamento desejado
    Matrix3x3 translationMatrix = createTranslationMatrix(dx, dy);

    switch (obj->type) {
        case OBJECT_TYPE_POINT: {
            Point* p = (Point*)obj->data;
            // Aplica a transformação matricial ao ponto
            *p = multiplyMatrixByPoint(translationMatrix, *p);
            break;
        }
        case OBJECT_TYPE_SEGMENT: {
            Segment* s = (Segment*)obj->data;
            // Aplica a transformação matricial a cada ponto do segmento
            s->p1 = multiplyMatrixByPoint(translationMatrix, s->p1);
            s->p2 = multiplyMatrixByPoint(translationMatrix, s->p2);
            break;
        }
        case OBJECT_TYPE_POLYGON: {
            GfxPolygon* poly = (GfxPolygon*)obj->data;
            // Aplica a transformação matricial a cada vértice do polígono
            for (int i = 0; i < poly->numVertices; i++) {
                poly->vertices[i] = multiplyMatrixByPoint(translationMatrix, poly->vertices[i]);
            }
            break;
        }
        default:
            fprintf(stderr, "[Transformations ERROR] Tipo de objeto desconhecido para translação: %d\n", obj->type);
            break;
    }
}
