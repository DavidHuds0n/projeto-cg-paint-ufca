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
        case OBJECT_TYPE_POINT:
            // ...
            break;
        case OBJECT_TYPE_SEGMENT:
            // ...
            break;
        case OBJECT_TYPE_POLYGON:
            // ...
            break;
    }

    return center;
}
