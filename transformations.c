// transformations.c
// Implementa��o das fun��es para transforma��es geom�tricas.

#include <stdio.h>
#include "transformations.h" // Inclui o pr�prio cabe�alho
#include "objects.h"         // Para acessar g_objects
#include "point.h"           // Para Point
#include "segment.h"         // Para Segment
#include "polygon.h"         // Para GfxPolygon

// Implementa��o das fun��es de matriz (j� existentes)

// Cria e retorna uma matriz identidade 3x3
Matrix3x3 createIdentityMatrix() {
    Matrix3x3 mat;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            mat.m[i][j] = (i == j) ? 1.0f : 0.0f; // Diagonal principal � 1, o resto � 0
        }
    }
    return mat;
}

// Cria e retorna uma matriz de transla��o 3x3
Matrix3x3 createTranslationMatrix(float tx, float ty) {
    Matrix3x3 mat = createIdentityMatrix(); // Come�a com uma matriz identidade
    mat.m[0][2] = tx; // Elemento para transla��o em X (posi��o [linha][coluna])
    mat.m[1][2] = ty; // Elemento para transla��o em Y
    return mat;
}

// Multiplica uma matriz 3x3 por um ponto (representado como vetor 3x1 [x, y, 1])
Point multiplyMatrixByPoint(Matrix3x3 mat, Point p) {
    Point transformedP;
    // Em coordenadas homog�neas, um ponto P(x, y) � (x, y, 1)
    // Multiplica��o da matriz 3x3 pelo vetor 3x1 (p.x, p.y, 1)

    transformedP.x = mat.m[0][0] * p.x + mat.m[0][1] * p.y + mat.m[0][2] * 1.0f;
    transformedP.y = mat.m[1][0] * p.x + mat.m[1][1] * p.y + mat.m[1][2] * 1.0f;
    // A terceira componente (w) ap�s a multiplica��o seria:
    // w = mat.m[2][0] * p.x + mat.m[2][1] * p.y + mat.m[2][2] * 1.0f;
    // Para transforma��es 2D padr�o, a terceira linha da matriz de transforma��o homog�nea geralmente � (0, 0, 1),
    // ent�o w sempre ser� 1.0f. N�o precisamos armazenar explicitamente 'w' para pontos 2D.

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


// Fun��o para transladar (mover) um objeto.
// AGORA UTILIZANDO MATRIZES DE TRANSFORMA��O
void translateObject(int objectIndex, float dx, float dy) {
    if (objectIndex < 0 || objectIndex >= g_numObjects) {
        fprintf(stderr, "[Transformations ERROR] �ndice de objeto inv�lido para transla��o: %d\n", objectIndex);
        return;
    }

    Object* obj = &g_objects[objectIndex];

    // Cria a matriz de transla��o para o deslocamento desejado
    Matrix3x3 translationMatrix = createTranslationMatrix(dx, dy);

    switch (obj->type) {
        case OBJECT_TYPE_POINT: {
            Point* p = (Point*)obj->data;
            // Aplica a transforma��o matricial ao ponto
            *p = multiplyMatrixByPoint(translationMatrix, *p);
            break;
        }
        case OBJECT_TYPE_SEGMENT: {
            Segment* s = (Segment*)obj->data;
            // Aplica a transforma��o matricial a cada ponto do segmento
            s->p1 = multiplyMatrixByPoint(translationMatrix, s->p1);
            s->p2 = multiplyMatrixByPoint(translationMatrix, s->p2);
            break;
        }
        case OBJECT_TYPE_POLYGON: {
            GfxPolygon* poly = (GfxPolygon*)obj->data;
            // Aplica a transforma��o matricial a cada v�rtice do pol�gono
            for (int i = 0; i < poly->numVertices; i++) {
                poly->vertices[i] = multiplyMatrixByPoint(translationMatrix, poly->vertices[i]);
            }
            break;
        }
        default:
            fprintf(stderr, "[Transformations ERROR] Tipo de objeto desconhecido para transla��o: %d\n", obj->type);
            break;
    }
}
