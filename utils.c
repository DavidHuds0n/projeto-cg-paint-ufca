/**
 * @file utils.c
 * @brief Implementa��o das fun��es utilit�rias do projeto.
 *
 * Cont�m a l�gica para o c�lculo do centro de objetos e outras opera��es
 * gen�ricas que n�o pertencem a um m�dulo espec�fico.
 */

#include "utils.h"
#include "objects.h"
#include "segment.h"
#include "polygon.h"
#include <stdio.h>

// --- SE��O DE FUN��ES P�BLICAS ---

/**
 * @brief Calcula o ponto central (centroide) de um objeto gr�fico.
 * @param obj Ponteiro para o objeto gen�rico.
 * @return Point As coordenadas (x, y) do centro do objeto.
 */
Point getObjectCenter(Object* obj) {
    Point center = {0.0f, 0.0f};

    // Cl�usula de guarda para evitar opera��es em ponteiros nulos.
    if (obj == NULL || obj->data == NULL) {
        fprintf(stderr, "[Utils ERROR] Tentando calcular o centro de um objeto nulo.\n");
        return center;
    }

    switch (obj->type) {
        case OBJECT_TYPE_POINT: {
            // O centro de um ponto � o pr�prio ponto.
            Point* p = (Point*)obj->data;
            center = *p;
            break;
        }
        case OBJECT_TYPE_SEGMENT: {
            // O centro de um segmento � o seu ponto m�dio.
            Segment* s = (Segment*)obj->data;
            center.x = (s->p1.x + s->p2.x) / 2.0f;
            center.y = (s->p1.y + s->p2.y) / 2.0f;
            break;
        }
        case OBJECT_TYPE_POLYGON: {
            // O centroide de um pol�gono � a m�dia de todos os seus v�rtices.
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
