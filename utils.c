/**
 * @file utils.c
 * @brief Implementação das funções utilitárias do projeto.
 *
 * Contém a lógica para o cálculo do centro de objetos e outras operações
 * genéricas que não pertencem a um módulo específico.
 */

#include "utils.h"
#include "objects.h"
#include "segment.h"
#include "polygon.h"
#include <stdio.h>

// --- SEÇÃO DE FUNÇÕES PÚBLICAS ---

/**
 * @brief Calcula o ponto central (centroide) de um objeto gráfico.
 * @param obj Ponteiro para o objeto genérico.
 * @return Point As coordenadas (x, y) do centro do objeto.
 */
Point getObjectCenter(Object* obj) {
    Point center = {0.0f, 0.0f};

    // Cláusula de guarda para evitar operações em ponteiros nulos.
    if (obj == NULL || obj->data == NULL) {
        fprintf(stderr, "[Utils ERROR] Tentando calcular o centro de um objeto nulo.\n");
        return center;
    }

    switch (obj->type) {
        case OBJECT_TYPE_POINT: {
            // O centro de um ponto é o próprio ponto.
            Point* p = (Point*)obj->data;
            center = *p;
            break;
        }
        case OBJECT_TYPE_SEGMENT: {
            // O centro de um segmento é o seu ponto médio.
            Segment* s = (Segment*)obj->data;
            center.x = (s->p1.x + s->p2.x) / 2.0f;
            center.y = (s->p1.y + s->p2.y) / 2.0f;
            break;
        }
        case OBJECT_TYPE_POLYGON: {
            // O centroide de um polígono é a média de todos os seus vértices.
            GfxPolygon* poly = (GfxPolygon*)obj->data;
            if (poly->numVertices == 0) break;

            float sumX = 0.0f, sumY = 0.0f;
            for (int i = 0; i < poly->numVertices; i++) {
                sumX += poly->vertices[i].x;
                sumY += poly->vertices[i].y;
            }
            center.x = sumX / poly->numVertices;
            center.y = sumY / poly->numVertices;
            break;
        }
    }

    return center;
}
