/**
 * @file convexhull.c
 * @brief Implementa��o do algoritmo de fecho convexo Marcha de Jarvis.
 */
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#include "convexhull.h"
#include "point.h"

// --- SE��O DE FUN��ES AUXILIARES (L�GICA INTERNA) ---

/**
 * @brief Determina a orienta��o de um trio ordenado de pontos (p, q, r).
 * @return int >0 se for anti-hor�rio, <0 se for hor�rio, 0 se for colinear.
 */
static int orientation(Point p, Point q, Point r) {
    float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val > 0) return 1;  // Anti-hor�rio (curva � esquerda)
    if (val < 0) return -1; // Hor�rio (curva � direita)
    return 0;               // Colinear
}

/**
 * @brief Calcula o quadrado da dist�ncia euclidiana entre dois pontos.
 *
 * Usado para encontrar o ponto mais distante em casos de colinearidade,
 * evitando o uso de `sqrt` que � computacionalmente caro.
 * @return float O quadrado da dist�ncia.
 */
static float distSq(Point p1, Point p2) {
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return (dx * dx) + (dy * dy);
}

// --- SE��O DE FUN��ES P�BLICAS ---

GfxPolygon* jarvisMarch(GfxPolygon* poly) {
    // 1. Casos base: um fecho convexo requer pelo menos 3 pontos.
    if (poly->numVertices < 3) {
        return NULL; // Retorna NULL para indicar que a opera��o n�o � aplic�vel.
    }

    // 2. Encontrar o ponto de partida: o ponto mais � esquerda (menor X).
    // Em caso de empate, o de menor Y � escolhido.
    int leftMostIndex = 0;
    for (int i = 1; i < poly->numVertices; i++) {
        if (poly->vertices[i].x < poly->vertices[leftMostIndex].x) {
            leftMostIndex = i;
        } else if (poly->vertices[i].x == poly->vertices[leftMostIndex].x) {
            if (poly->vertices[i].y < poly->vertices[leftMostIndex].y) {
                leftMostIndex = i;
            }
        }
    }

    // 3. Aloca mem�ria para o pol�gono do fecho e inicializa o processo.
    GfxPolygon* convexHull = (GfxPolygon*)malloc(sizeof(GfxPolygon));
    if (!convexHull) return NULL; // Checagem de falha de aloca��o
    convexHull->numVertices = 0;

    int p = leftMostIndex;
    int q;

    do {
        // Adiciona o ponto atual 'p' ao fecho.
        addVertexToPolygon(convexHull, poly->vertices[p]);

        // 'q' ser� o pr�ximo ponto mais anti-hor�rio.
        // Inicializa 'q' com um candidato diferente de 'p'.
        q = (p + 1) % poly->numVertices;

        // Itera por todos os pontos para encontrar o 'q' definitivo.
        for (int i = 0; i < poly->numVertices; i++) {
            // Se o ponto 'i' for mais anti-hor�rio que 'q', ele se torna o novo 'q'.
            if (orientation(poly->vertices[p], poly->vertices[i], poly->vertices[q]) > 0) {
                q = i;
            }
        }

        // Tratamento de pontos colineares.
        // Se houver pontos colineares, escolhe o que estiver mais distante do piv� 'p'.
        // Isso garante que apenas os v�rtices extremos de uma aresta colinear sejam inclu�dos.
        for (int i = 0; i < poly->numVertices; i++) {
            if (orientation(poly->vertices[p], poly->vertices[i], poly->vertices[q]) == 0) {
                if (distSq(poly->vertices[p], poly->vertices[i]) > distSq(poly->vertices[p], poly->vertices[q])) {
                    q = i;
                }
            }
        }

        // Atualiza o piv� 'p' para ser o pr�ximo ponto encontrado.
        p = q;

    // O "embrulho" termina quando voltamos ao ponto de partida.
    } while (p != leftMostIndex);

    return convexHull;
}
