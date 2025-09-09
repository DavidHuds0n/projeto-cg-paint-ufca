/**
 * @file convexhull.c
 * @brief Implementação do algoritmo de fecho convexo Marcha de Jarvis.
 */
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#include "convexhull.h"
#include "point.h"

// --- SEÇÃO DE FUNÇÕES AUXILIARES (LÓGICA INTERNA) ---

/**
 * @brief Determina a orientação de um trio ordenado de pontos (p, q, r).
 * @return int >0 se for anti-horário, <0 se for horário, 0 se for colinear.
 */
static int orientation(Point p, Point q, Point r) {
    float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val > 0) return 1;  // Anti-horário (curva à esquerda)
    if (val < 0) return -1; // Horário (curva à direita)
    return 0;               // Colinear
}

/**
 * @brief Calcula o quadrado da distância euclidiana entre dois pontos.
 *
 * Usado para encontrar o ponto mais distante em casos de colinearidade,
 * evitando o uso de `sqrt` que é computacionalmente caro.
 * @return float O quadrado da distância.
 */
static float distSq(Point p1, Point p2) {
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return (dx * dx) + (dy * dy);
}

// --- SEÇÃO DE FUNÇÕES PÚBLICAS ---

GfxPolygon* jarvisMarch(GfxPolygon* poly) {
    // 1. Casos base: um fecho convexo requer pelo menos 3 pontos.
    if (poly->numVertices < 3) {
        return NULL; // Retorna NULL para indicar que a operação não é aplicável.
    }

    // 2. Encontrar o ponto de partida: o ponto mais à esquerda (menor X).
    // Em caso de empate, o de menor Y é escolhido.
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

    // 3. Aloca memória para o polígono do fecho e inicializa o processo.
    GfxPolygon* convexHull = (GfxPolygon*)malloc(sizeof(GfxPolygon));
    if (!convexHull) return NULL; // Checagem de falha de alocação
    convexHull->numVertices = 0;

    int p = leftMostIndex;
    int q;

    do {
        // Adiciona o ponto atual 'p' ao fecho.
        addVertexToPolygon(convexHull, poly->vertices[p]);

        // 'q' será o próximo ponto mais anti-horário.
        // Inicializa 'q' com um candidato diferente de 'p'.
        q = (p + 1) % poly->numVertices;

        // Itera por todos os pontos para encontrar o 'q' definitivo.
        for (int i = 0; i < poly->numVertices; i++) {
            // Se o ponto 'i' for mais anti-horário que 'q', ele se torna o novo 'q'.
            if (orientation(poly->vertices[p], poly->vertices[i], poly->vertices[q]) > 0) {
                q = i;
            }
        }

        // Tratamento de pontos colineares.
        // Se houver pontos colineares, escolhe o que estiver mais distante do pivô 'p'.
        // Isso garante que apenas os vértices extremos de uma aresta colinear sejam incluídos.
        for (int i = 0; i < poly->numVertices; i++) {
            if (orientation(poly->vertices[p], poly->vertices[i], poly->vertices[q]) == 0) {
                if (distSq(poly->vertices[p], poly->vertices[i]) > distSq(poly->vertices[p], poly->vertices[q])) {
                    q = i;
                }
            }
        }

        // Atualiza o pivô 'p' para ser o próximo ponto encontrado.
        p = q;

    // O "embrulho" termina quando voltamos ao ponto de partida.
    } while (p != leftMostIndex);

    return convexHull;
}
