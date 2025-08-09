// objects.h
// Definições para o gerenciamento genérico de objetos gráficos (Ponto, Segmento, GfxPolygon).

#ifndef OBJECTS_H
#define OBJECTS_H

#include "point.h"   // Para Point
#include "segment.h" // Para Segment
#include "polygon.h" // Para GfxPolygon (ATUALIZADO)
#include "config.h"  // NOVO: Para MAX_OBJECTS

// Enumeração dos tipos de objetos gráficos que o programa pode manipular.
typedef enum {
    OBJECT_TYPE_POINT,
    OBJECT_TYPE_SEGMENT,
    OBJECT_TYPE_POLYGON // Refere-se a GfxPolygon agora
} ObjectType;

// Estrutura genérica para armazenar qualquer tipo de objeto gráfico.
// Usa um ponteiro void para ser polimórfico e um campo 'type' para identificar o objeto.
typedef struct {
    void* data;      // Ponteiro para a estrutura específica do objeto (Point*, Segment*, GfxPolygon*).
    ObjectType type; // Tipo do objeto (usado para fazer o cast correto de 'data').
} Object;

// Variáveis globais para o gerenciamento da lista de objetos
// #define MAX_OBJECTS 100 // Removido, agora vem de config.h

extern Object g_objects[MAX_OBJECTS];      // Array global para armazenar todos os objetos.
extern int g_numObjects;                   // Contador global do número atual de objetos na lista.
extern int g_selectedObjectIndex;          // Índice do objeto selecionado atualmente na lista (-1 se nenhum).

// Protótipos das funções do módulo objects.c

// Inicializa a lista de objetos (reseta o contador e o objeto selecionado).
void initObjectList();

// Adiciona um novo objeto à lista.
void addObject(ObjectType type, void* data);

// Remove um objeto da lista pelo seu índice.
void removeObject(int index);

// Limpa todos os objetos da lista e libera a memória alocada.
void clearAllObjects();

// Desenha um objeto específico da lista (usa o type para chamar a função de desenho correta).
void drawObject(int index, int is_selected);

// Desenha todos os objetos presentes na lista.
void drawAllObjects();

#endif // OBJECTS_H
