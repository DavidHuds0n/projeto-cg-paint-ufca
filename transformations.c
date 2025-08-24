/**
 * @file transformations.c
 * @brief Implementação das transformações geométricas e manipulação de matrizes.
 *
 * Contém a lógica matemática para criar e aplicar matrizes de transformação
 * aos objetos da cena. As transformações complexas são realizadas via
 * composição de matrizes.
 */

#include <stdio.h>
#include <math.h>
#include "transformations.h"
#include "objects.h"
#include "point.h"
#include "segment.h"
#include "polygon.h"
#include "utils.h"

// --- SEÇÃO DE FUNÇÕES AUXILIARES DE MATRIZES ---
// Funções para criar e manipular as matrizes de transformação.

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
 * @brief Cria uma matriz de translação 2D.
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
 * @brief Cria uma matriz de rotação 2D.
 * @param angleDegrees O ângulo de rotação em graus.
 */
Matrix3x3 createRotationMatrix(float angleDegrees){
    Matrix3x3 result = createIdentityMatrix();
    float rad = angleDegrees * M_PI / 180.0f;
    result.m[0][0] = cosf(rad);
    result.m[1][0] = sinf(rad);
    result.m[0][1] = -sinf(rad);
    result.m[1][1] = cosf(rad);

    return result;
}

/**
 * @brief Cria uma matriz de reflexão (espelhamento) 2D.
 * @param axis O eixo de reflexão. Use 0 para refletir sobre o eixo X e 1 para o eixo Y.
 */
Matrix3x3 createReflectionMatrix(int axis) {
    Matrix3x3 mat = createIdentityMatrix();
    if (axis == 0) { // Reflexão sobre o eixo X (inverte Y)
        mat.m[1][1] = -1.0f;
    } else { // Reflexão sobre o eixo Y (inverte X)
        mat.m[0][0] = -1.0f;
    }
    return mat;
}

/**
 * @brief Cria uma matriz de cisalhamento (shear) 2D.
 * @param shx Fator de cisalhamento na direção X.
 * @param shy Fator de cisalhamento na direção Y.
 */
Matrix3x3 createShearMatrix(float shx, float shy) {
    Matrix3x3 mat = createIdentityMatrix();
    mat.m[0][1] = shx;
    mat.m[1][0] = shy;
    return mat;
}

/**
 * @brief Multiplica uma matriz por um ponto 2D.
 */
Point multiplyMatrixByPoint(Matrix3x3 mat, Point p) {
    Point transformedP;
    transformedP.x = mat.m[0][0] * p.x + mat.m[0][1] * p.y + mat.m[0][2] * 1.0f;
    transformedP.y = mat.m[1][0] * p.x + mat.m[1][1] * p.y + mat.m[1][2] * 1.0f;
    return transformedP;
}

/**
 * @brief Multiplica duas matrizes 3x3.
 */
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

// --- SEÇÃO DE FUNÇÕES DE LÓGICA E APLICAÇÃO DE MATRIZES ---
// Funções internas que delegam as operações de transformação.

/**
 * @brief Aplica uma matriz de transformação a um objeto.
 *
 * Esta função age como um "dispatcher", identificando o tipo do objeto e
 * aplicando a matriz a todos os seus vértices de forma adequada.
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


// --- SEÇÃO DE FUNÇÕES DE TRANSFORMAÇÃO (INTERFACE PÚBLICA) ---
// Estas são as funções que devem ser chamadas por outros módulos (e.g., input.c).

void translateObject(int objectIndex, float dx, float dy) {
    if (objectIndex < 0 || objectIndex >= g_numObjects) {
        return;
    }
    Matrix3x3 translationMatrix = createTranslationMatrix(dx, dy);
    applyMatrixToObject(objectIndex, translationMatrix);
}

void scaleObject(int objectIndex, float sx, float sy) {
    if (objectIndex < 0 || objectIndex >= g_numObjects) {
        return;
    }
    // Para uma escala correta, a transformação é aplicada em relação ao centro do objeto.
    Point center = getObjectCenter(&g_objects[objectIndex]);

    // A matriz final é uma composição de três transformações:
    // 1. Translação do objeto para a origem (T(-c))
    Matrix3x3 toOriginMatrix = createTranslationMatrix(-center.x, -center.y);
    // 2. Aplicação da escala na origem (S(sx,sy))
    Matrix3x3 scaleMatrix = createScaleMatrix(sx, sy);
    // 3. Translação do objeto de volta para sua posição original (T(c))
    Matrix3x3 fromOriginMatrix = createTranslationMatrix(center.x, center.y);

    // A ordem de multiplicação é crucial: T(c) * S * T(-c)
    Matrix3x3 tempMatrix = multiplyMatrices(scaleMatrix, toOriginMatrix);
    Matrix3x3 finalMatrix = multiplyMatrices(fromOriginMatrix, tempMatrix);

    applyMatrixToObject(objectIndex, finalMatrix);
}


void rotateObject(int objectIndex, float angle) {
    if (objectIndex < 0 || objectIndex >= g_numObjects) {
        return;
    }
    // Para uma rotação em torno do centro do objeto, a transformação é composta:
    Point objectCenter = getObjectCenter(&g_objects[objectIndex]);

    // 1. Translação do objeto para a origem (T(-c))
    Matrix3x3 toOriginMatrix = createTranslationMatrix(-objectCenter.x, -objectCenter.y);
    // 2. Aplicação da rotação na origem (R(angle))
    Matrix3x3 rotationMatrix = createRotationMatrix(angle);
    // 3. Translação do objeto de volta para sua posição original (T(c))
    Matrix3x3 fromOriginMatrix = createTranslationMatrix(objectCenter.x, objectCenter.y);

    // A ordem de multiplicação é crucial: T(c) * R * T(-c)
    Matrix3x3 tempMatrix = multiplyMatrices(rotationMatrix, toOriginMatrix);
    Matrix3x3 compositeMatrix = multiplyMatrices(fromOriginMatrix, tempMatrix);

    applyMatrixToObject(objectIndex, compositeMatrix);
}

void reflectObject(int objectIndex, int axis) {
    if (objectIndex < 0 || objectIndex >= g_numObjects) {
        return;
    }
    // Para uma reflexão em relação ao centro do objeto, a transformação é composta:
    Point center = getObjectCenter(&g_objects[objectIndex]);

    // 1. Translação do objeto para a origem (T(-c))
    Matrix3x3 toOriginMatrix = createTranslationMatrix(-center.x, -center.y);
    // 2. Aplicação da reflexão na origem (R(axis))
    Matrix3x3 reflectionMatrix = createReflectionMatrix(axis);
    // 3. Translação do objeto de volta para sua posição original (T(c))
    Matrix3x3 fromOriginMatrix = createTranslationMatrix(center.x, center.y);

    // A ordem de multiplicação é crucial: T(c) * R * T(-c)
    Matrix3x3 tempMatrix = multiplyMatrices(reflectionMatrix, toOriginMatrix);
    Matrix3x3 finalMatrix = multiplyMatrices(fromOriginMatrix, tempMatrix);

    applyMatrixToObject(objectIndex, finalMatrix);
}

void shearObject(int objectIndex, float shx, float shy) {
    if (objectIndex < 0 || objectIndex >= g_numObjects) {
        return;
    }
    // Para um cisalhamento em relação ao centro do objeto, a transformação é composta:
    Point center = getObjectCenter(&g_objects[objectIndex]);

    // 1. Translação do objeto para a origem (T(-c))
    Matrix3x3 toOriginMatrix = createTranslationMatrix(-center.x, -center.y);
    // 2. Aplicação do cisalhamento na origem (S(shx,shy))
    Matrix3x3 shearMatrix = createShearMatrix(shx, shy);
    // 3. Translação do objeto de volta para sua posição original (T(c))
    Matrix3x3 fromOrigin = createTranslationMatrix(center.x, center.y);

    // A ordem de multiplicação é crucial: T(c) * Shear * T(-c)
    Matrix3x3 tempMatrix = multiplyMatrices(shearMatrix, toOriginMatrix);
    Matrix3x3 finalMatrix = multiplyMatrices(fromOrigin, tempMatrix);

    applyMatrixToObject(objectIndex, finalMatrix);
}
