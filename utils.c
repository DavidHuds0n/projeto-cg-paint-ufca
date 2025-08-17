// utils.c
// Implementa��o das fun��es utilit�rias.

#include "utils.h"
#include "segment.h"
#include "polygon.h"
#include <stdio.h> // Para erros, se necess�rio

Point getObjectCenter(Object* obj) {
    Point center = {0.0f, 0.0f};

    if (obj == NULL || obj->data == NULL) {
        fprintf(stderr, "[Utils ERROR] Tentando calcular o centro de um objeto nulo.\n");
        return center;
    }

    // TODO: Implementar a l�gica para calcular o centro de cada tipo de objeto.
    // Dica: Para pontos, o centro � o pr�prio ponto. Para segmentos e pol�gonos,
    // o centro � a m�dia das coordenadas de todos os seus v�rtices.
    switch (obj->type) {
        case OBJECT_TYPE_POINT:{
            // o centro do ponto � o proprio ponto
            Point* p = (Point*)obj->data;
            center = *p;
            break;
        }
        case OBJECT_TYPE_SEGMENT:{
            // o centro do segmento
            Segment* s = (Segment*)obj->data;
            center.x = (s->p1.x + s->p2.x) / 2.0f;
            center.y = (s->p1.y + s->p2.y) / 2.0f;
            break;
        }
        case OBJECT_TYPE_POLYGON:{
            // ...
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
