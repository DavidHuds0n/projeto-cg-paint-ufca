// transformations.c
/**
 * @file transformations.c
 * @brief Implementação das funções de transformações geométricas e manipulação de matrizes.
 * Contém a lógica matemática para criar e aplicar matrizes de transformação aos objetos da cena.
 */

#include <stdio.h>
#include <math.h>
#include "transformations.h"
#include "objects.h"
#include "point.h"
#include "segment.h"
#include "polygon.h"
#include "utils.h" // Necessário para getObjectCenter()

// --- Funções Auxiliares para Criação de Matrizes ---

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
 * @param angleDegrees O ângulo de rotação em graus. A função deverá converter para radianos.
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
 * @brief Cria uma matriz de reflexão (espelhamento) 2D.
 * @param axis O eixo de reflexão. Use 0 para refletir sobre o eixo X e 1 para o eixo Y.
 */
Matrix3x3 createReflectionMatrix(int axis) {
    // TODO: Implementar a criação da matriz de reflexão.
    // Se axis for 0 (eixo X), sy = -1. Se for 1 (eixo Y), sx = -1.
    return createIdentityMatrix(); // Retorno temporário.
}

/**
 * @brief Cria uma matriz de cisalhamento (shear) 2D.
 * @param shx Fator de cisalhamento na direção X.
 * @param shy Fator de cisalhamento na direção Y.
 */
Matrix3x3 createShearMatrix(float shx, float shy) {
    // TODO: Implementar a criação da matriz de cisalhamento.
    // mat.m[0][1] = shx;
    // mat.m[1][0] = shy;
    return createIdentityMatrix(); // Retorno temporário.
}


// --- Funções de Lógica e Aplicação de Matrizes ---

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
 * @brief Função central que aplica uma matriz de transformação a um objeto.
 * Esta função age como um "dispatcher", contendo a lógica para identificar o tipo
 * do objeto e aplicar a matriz de forma adequada a todos os seus vértices.
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


// --- Funções de Transformação (Interface Pública) ---
// Estas são as funções que devem ser chamadas por outros módulos (como input.c).

void translateObject(int objectIndex, float dx, float dy) {
    // Cláusula de guarda para garantir que o índice é válido.
    if (objectIndex < 0 || objectIndex >= g_numObjects) {
        return;
    }

    // 1. Cria a matriz específica para esta transformação.
    Matrix3x3 translationMatrix = createTranslationMatrix(dx, dy);

    // 2. Delega a aplicação da matriz à função central.
    applyMatrixToObject(objectIndex, translationMatrix);
}

void scaleObject(int objectIndex, float sx, float sy) {
    // Cláusula de guarda para garantir que o índice é válido ANTES de usá-lo.
    if (objectIndex < 0 || objectIndex >= g_numObjects) {
        return;
    }

    // 1. Para uma escala correta, a transformação é aplicada em relação ao centro do objeto.
    Point center = getObjectCenter(&g_objects[objectIndex]);

    // 2. A matriz final é uma composição de três transformações: T(c) * S(sx,sy) * T(-c)
    Matrix3x3 toOriginMatrix = createTranslationMatrix(-center.x, -center.y);
    Matrix3x3 scaleMatrix = createScaleMatrix(sx, sy);
    Matrix3x3 fromOriginMatrix = createTranslationMatrix(center.x, center.y);

    // 3. Multiplica as matrizes na ordem correta (a primeira a ser aplicada fica à direita).
    Matrix3x3 tempMatrix = multiplyMatrices(scaleMatrix, toOriginMatrix);
    Matrix3x3 finalMatrix = multiplyMatrices(fromOriginMatrix, tempMatrix);

    // 4. Delega a aplicação da matriz final à função central.
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

    printf("Função rotateObject chamada para o objeto %d com ângulo %.1f\n", objectIndex, angle);
}

void reflectObject(int objectIndex, int axis) {
    // TODO: Implementar a reflexão:
    // 1. Criar a matriz de reflexão apropriada.
    // 2. Chamar applyMatrixToObject.
    printf("Função reflectObject chamada para o objeto %d no eixo %d\n", objectIndex, axis);
}

void shearObject(int objectIndex, float shx, float shy) {
    // TODO: Implementar o cisalhamento:
    // 1. Criar a matriz de cisalhamento.
    // 2. Chamar applyMatrixToObject.
    printf("Função shearObject chamada para o objeto %d com fatores (%.1f, %.1f)\n", objectIndex, shx, shy);
}
