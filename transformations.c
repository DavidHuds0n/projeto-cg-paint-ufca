// transformations.c
// Implementa��o das fun��es de transforma��es geom�tricas e manipula��o de matrizes.
// Cont�m a l�gica matem�tica para criar matrizes e aplic�-las aos objetos.

#include <stdio.h>
#include <math.h> // Adicionado para futuras transforma��es (sin, cos).
#include "transformations.h"
#include "objects.h"
#include "point.h"
#include "segment.h"
#include "polygon.h"
#include "utils.h" // Ser� necess�rio para calcular o centro dos objetos.


// --- Implementa��o das Fun��es de Matriz ---

Matrix3x3 createIdentityMatrix() {
    Matrix3x3 mat;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            mat.m[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
    return mat;
}

Matrix3x3 createTranslationMatrix(float tx, float ty) {
    Matrix3x3 mat = createIdentityMatrix();
    mat.m[0][2] = tx;
    mat.m[1][2] = ty;
    return mat;
}

Point multiplyMatrixByPoint(Matrix3x3 mat, Point p) {
    Point transformedP;
    transformedP.x = mat.m[0][0] * p.x + mat.m[0][1] * p.y + mat.m[0][2] * 1.0f;
    transformedP.y = mat.m[1][0] * p.x + mat.m[1][1] * p.y + mat.m[1][2] * 1.0f;
    return transformedP;
}

Matrix3x3 multiplyMatrices(Matrix3x3 m1, Matrix3x3 m2) {
    Matrix3x3 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.m[i][j] = 0.0f;
            for (int k = 0; k < 3; k++) {
                result.m[i][j] += m1.m[i][k] * m2.m[k][j];
            }
        }
    }
    return result;
}

Matrix3x3 createRotationMatrix(float angle){
    Matrix3x3 result;
    // Coluna 1:
    result.m[0][0] = cosf(angle);
    result.m[1][0] = sinf(angle);
    result.m[2][0] = 0.0;

    // Coluna 2:
    result.m[0][1] = -sinf(angle);
    result.m[1][1] = cosf(angle);
    result.m[2][1] = 0.0;

    // Coluna 3:
    result.m[0][2] = 0.0;
    result.m[1][2] = 0.0;
    result.m[2][2] = 1.0;

    return result;
}

// --- Implementa��o das Fun��es de Transforma��o ---

void translateObject(int objectIndex, float dx, float dy) {
    if (objectIndex < 0 || objectIndex >= g_numObjects) {
        return;
    }
    Object* obj = &g_objects[objectIndex];
    Matrix3x3 translationMatrix = createTranslationMatrix(dx, dy);

    switch (obj->type) {
        case OBJECT_TYPE_POINT: {
            Point* p = (Point*)obj->data;
            *p = multiplyMatrixByPoint(translationMatrix, *p);
            break;
        }
        case OBJECT_TYPE_SEGMENT: {
            Segment* s = (Segment*)obj->data;
            s->p1 = multiplyMatrixByPoint(translationMatrix, s->p1);
            s->p2 = multiplyMatrixByPoint(translationMatrix, s->p2);
            break;
        }
        case OBJECT_TYPE_POLYGON: {
            GfxPolygon* poly = (GfxPolygon*)obj->data;
            for (int i = 0; i < poly->numVertices; i++) {
                poly->vertices[i] = multiplyMatrixByPoint(translationMatrix, poly->vertices[i]);
            }
            break;
        }
    }
}

// --- Fun��es a Serem Implementadas pela Equipe ---

void rotateObject(int objectIndex, float angle) {
    // TODO: Implementar a l�gica de Rota��o aqui.
    // 1. Calcular o centro do objeto (criar e usar uma fun��o de 'utils.c').
    Point objectCenter = getObjectCenter(objectIndex);
    // 2. Criar a matriz de transla��o para a origem T(-cx, -cy).
    Matrix3x3 translationMatrix = createTranslationMatrix(-objectCenter.x, -objectCenter.y);
    // 3. Criar a matriz de rota��o R(angle).
    Matrix3x3 rotationMatrix = createRotationMatrix(angle);
    // 4. Criar a matriz de transla��o de volta T(cx, cy).
    Matrix3x3 translationMatrixInv = createTranslationMatrix(objectCenter.x, objectCenter.y)
    // 5. Multiplicar as matrizes para obter a matriz composta M = T * R * T_inv.
    Matrix3x3 compositeMatrix = multiplyMatrices(translationMatrix, rotationMatrix);
    compositeMatrix = multiplyMatrices(compositeMatrix, translationMatrixInv);
    // 6. Aplicar a matriz M a todos os v�rtices do objeto.
    ...
    printf("Fun��o rotateObject chamada para o objeto %d com �ngulo %.1f\n", objectIndex, angle);
}

void scaleObject(int objectIndex, float sx, float sy) {
    // TODO: Implementar a l�gica de Escala aqui.
    // O processo � quase id�ntico ao da rota��o.
    // 1. Calcular o centro do objeto.
    // 2. Criar as matrizes de transla��o para a origem, escala e transla��o de volta.
    // 3. Multiplic�-las para obter a matriz composta M = T * S * T_inv.
    // 4. Aplicar a matriz M a todos os v�rtices do objeto.
    if (objectIndex < 0 || objectIndex >= g_numObjects) {
        return;
    }
    Object* obj = &g_objects[objectIndex];

    // 2. Calcular o centro do objeto (usando a declara��o correta)
    Point center = getObjectCenter(obj);
    Point *p = (Point*)obj->data;
    float x = p-> x - center.x;
    float y = p-> y - center.y;

    x *= sx;
    y *= sy;

    p->x = x + center.x;
    p->y = y + center.y;
    printf("Fun��o scaleObject chamada para o objeto %d com fatores (%.1f, %.1f)\n", objectIndex, sx, sy);
}

void reflectObject(int objectIndex, int axis) {
    // TODO: Implementar a l�gica de Reflex�o aqui.
    // 1. Criar a matriz de reflex�o apropriada (uma para eixo X, outra para eixo Y).
    // 2. Aplicar a matriz a todos os v�rtices do objeto.
    // (Opcional: implementar reflex�o em torno de um eixo arbitr�rio com matriz composta).
    printf("Fun��o reflectObject chamada para o objeto %d no eixo %d\n", objectIndex, axis);
}

void shearObject(int objectIndex, float shx, float shy) {
    // TODO: Implementar a l�gica de Cisalhamento aqui.
    // 1. Criar a matriz de cisalhamento com os fatores shx e shy.
    // 2. Aplicar a matriz a todos os v�rtices do objeto.
    printf("Fun��o shearObject chamada para o objeto %d com fatores (%.1f, %.1f)\n", objectIndex, shx, shy);
}
