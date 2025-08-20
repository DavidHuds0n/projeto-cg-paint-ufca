// transformations.c
/**
 * @file transformations.c
 * @brief Implementa��o das fun��es de transforma��es geom�tricas e manipula��o de matrizes.
 * Cont�m a l�gica matem�tica para criar e aplicar matrizes de transforma��o aos objetos da cena.
 */

#include <stdio.h>
#include <math.h>
#include "transformations.h"
#include "objects.h"
#include "point.h"
#include "segment.h"
#include "polygon.h"
#include "utils.h" // Necess�rio para getObjectCenter()

// --- Fun��es Auxiliares para Cria��o de Matrizes ---

/**
 * @brief Cria e retorna uma matriz identidade 3x3.
 */
Matrix3x3 createIdentityMatrix() {
    Matrix3x3 mat;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            mat.m[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
    return mat;
}

/**
 * @brief Cria uma matriz de transla��o 2D.
 */
Matrix3x3 createTranslationMatrix(float tx, float ty) {
    Matrix3x3 mat = createIdentityMatrix();
    mat.m[0][2] = tx;
    mat.m[1][2] = ty;
    return mat;
}

/**
 * @brief Cria uma matriz de escala 2D.
 */
Matrix3x3 createScaleMatrix(float sx, float sy) {
    Matrix3x3 mat = createIdentityMatrix();
    mat.m[0][0] = sx;
    mat.m[1][1] = sy;
    return mat;
}

/**
 * @brief Cria uma matriz de rota��o 2D.
 * @param angleDegrees O �ngulo de rota��o em graus. A fun��o dever� converter para radianos.
 */
Matrix3x3 createRotationMatrix(float angle){
    Matrix3x3 result;

    float rad = angle * M_PI / 180.0f;

    // Coluna 1:
    result.m[0][0] = cosf(rad);
    result.m[1][0] = sinf(rad);
    result.m[2][0] = 0.0;

    // Coluna 2:
    result.m[0][1] = -sinf(rad);
    result.m[1][1] = cosf(rad);
    result.m[2][1] = 0.0;

    // Coluna 3:
    result.m[0][2] = 0.0;
    result.m[1][2] = 0.0;
    result.m[2][2] = 1.0;

    return result;
}

/**
 * @brief Cria uma matriz de reflex�o (espelhamento) 2D.
 * @param axis O eixo de reflex�o. Use 0 para refletir sobre o eixo X e 1 para o eixo Y.
 */
Matrix3x3 createReflectionMatrix(int axis) {
    // TODO: Implementar a cria��o da matriz de reflex�o.
    // Se axis for 0 (eixo X), sy = -1. Se for 1 (eixo Y), sx = -1.
    return createIdentityMatrix(); // Retorno tempor�rio.
}

/**
 * @brief Cria uma matriz de cisalhamento (shear) 2D.
 * @param shx Fator de cisalhamento na dire��o X.
 * @param shy Fator de cisalhamento na dire��o Y.
 */
Matrix3x3 createShearMatrix(float shx, float shy) {
    // TODO: Implementar a cria��o da matriz de cisalhamento.
    // mat.m[0][1] = shx;
    // mat.m[1][0] = shy;
    return createIdentityMatrix(); // Retorno tempor�rio.
}


// --- Fun��es de L�gica e Aplica��o de Matrizes ---

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

/**
 * @brief Fun��o central que aplica uma matriz de transforma��o a um objeto.
 * Esta fun��o age como um "dispatcher", contendo a l�gica para identificar o tipo
 * do objeto e aplicar a matriz de forma adequada a todos os seus v�rtices.
 */
void applyMatrixToObject(int objectIndex, Matrix3x3 matrix) {
    if (objectIndex < 0 || objectIndex >= g_numObjects) {
        return;
    }
    Object* obj = &g_objects[objectIndex];
    switch (obj->type) {
        case OBJECT_TYPE_POINT: {
            Point* p = (Point*)obj->data;
            *p = multiplyMatrixByPoint(matrix, *p);
            break;
        }
        case OBJECT_TYPE_SEGMENT: {
            Segment* s = (Segment*)obj->data;
            s->p1 = multiplyMatrixByPoint(matrix, s->p1);
            s->p2 = multiplyMatrixByPoint(matrix, s->p2);
            break;
        }
        case OBJECT_TYPE_POLYGON: {
            GfxPolygon* poly = (GfxPolygon*)obj->data;
            for (int i = 0; i < poly->numVertices; i++) {
                poly->vertices[i] = multiplyMatrixByPoint(matrix, poly->vertices[i]);
            }
            break;
        }
    }
}


// --- Fun��es de Transforma��o (Interface P�blica) ---
// Estas s�o as fun��es que devem ser chamadas por outros m�dulos (como input.c).

void translateObject(int objectIndex, float dx, float dy) {
    // Cl�usula de guarda para garantir que o �ndice � v�lido.
    if (objectIndex < 0 || objectIndex >= g_numObjects) {
        return;
    }

    // 1. Cria a matriz espec�fica para esta transforma��o.
    Matrix3x3 translationMatrix = createTranslationMatrix(dx, dy);

    // 2. Delega a aplica��o da matriz � fun��o central.
    applyMatrixToObject(objectIndex, translationMatrix);
}

void scaleObject(int objectIndex, float sx, float sy) {
    // Cl�usula de guarda para garantir que o �ndice � v�lido ANTES de us�-lo.
    if (objectIndex < 0 || objectIndex >= g_numObjects) {
        return;
    }

    // 1. Para uma escala correta, a transforma��o � aplicada em rela��o ao centro do objeto.
    Point center = getObjectCenter(&g_objects[objectIndex]);

    // 2. A matriz final � uma composi��o de tr�s transforma��es: T(c) * S(sx,sy) * T(-c)
    Matrix3x3 toOriginMatrix = createTranslationMatrix(-center.x, -center.y);
    Matrix3x3 scaleMatrix = createScaleMatrix(sx, sy);
    Matrix3x3 fromOriginMatrix = createTranslationMatrix(center.x, center.y);

    // 3. Multiplica as matrizes na ordem correta (a primeira a ser aplicada fica � direita).
    Matrix3x3 tempMatrix = multiplyMatrices(scaleMatrix, toOriginMatrix);
    Matrix3x3 finalMatrix = multiplyMatrices(fromOriginMatrix, tempMatrix);

    // 4. Delega a aplica��o da matriz final � fun��o central.
    applyMatrixToObject(objectIndex, finalMatrix);
}


void rotateObject(int objectIndex, float angle) {

    Point objectCenter = getObjectCenter(&g_objects[objectIndex]);
    Matrix3x3 translationMatrix = createTranslationMatrix(-objectCenter.x, -objectCenter.y);
    Matrix3x3 rotationMatrix = createRotationMatrix(angle);
    Matrix3x3 translationMatrixInv = createTranslationMatrix(objectCenter.x, objectCenter.y);
    Matrix3x3 compositeMatrix = multiplyMatrices(rotationMatrix, translationMatrix);
    compositeMatrix = multiplyMatrices(translationMatrixInv, compositeMatrix);
    applyMatrixToObject(objectIndex, compositeMatrix);

    printf("Fun��o rotateObject chamada para o objeto %d com �ngulo %.1f\n", objectIndex, angle);
}

void reflectObject(int objectIndex, int axis) {
    // TODO: Implementar a reflex�o:
    // 1. Criar a matriz de reflex�o apropriada.
    // 2. Chamar applyMatrixToObject.
    printf("Fun��o reflectObject chamada para o objeto %d no eixo %d\n", objectIndex, axis);
}

void shearObject(int objectIndex, float shx, float shy) {
    // TODO: Implementar o cisalhamento:
    // 1. Criar a matriz de cisalhamento.
    // 2. Chamar applyMatrixToObject.
    printf("Fun��o shearObject chamada para o objeto %d com fatores (%.1f, %.1f)\n", objectIndex, shx, shy);
}
