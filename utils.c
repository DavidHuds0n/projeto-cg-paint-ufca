// utils.c
// Implementação das funções utilitárias.

#include "utils.h"
#include "segment.h"
#include "polygon.h"
#include <stdio.h> // Para erros, se necessário

Point getObjectCenter(Object* obj) {
    Point center = {0.0f, 0.0f};

    if (obj == NULL || obj->data == NULL) {
        fprintf(stderr, "[Utils ERROR] Tentando calcular o centro de um objeto nulo.\n");
        return center;
    }

    // TODO: Implementar a lógica para calcular o centro de cada tipo de objeto.
    // Dica: Para pontos, o centro é o próprio ponto. Para segmentos e polígonos,
    // o centro é a média das coordenadas de todos os seus vértices.
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
