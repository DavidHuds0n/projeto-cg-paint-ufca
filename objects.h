// objects.h
// Defini��es para o gerenciamento gen�rico de objetos gr�ficos (Ponto, Segmento, GfxPolygon).

#ifndef OBJECTS_H
#define OBJECTS_H

#include "point.h"   // Para Point
#include "segment.h" // Para Segment
#include "polygon.h" // Para GfxPolygon (ATUALIZADO)
#include "config.h"  // NOVO: Para MAX_OBJECTS

// Enumera��o dos tipos de objetos gr�ficos que o programa pode manipular.
typedef enum {
    OBJECT_TYPE_POINT,
    OBJECT_TYPE_SEGMENT,
    OBJECT_TYPE_POLYGON // Refere-se a GfxPolygon agora
} ObjectType;

// Estrutura gen�rica para armazenar qualquer tipo de objeto gr�fico.
// Usa um ponteiro void para ser polim�rfico e um campo 'type' para identificar o objeto.
typedef struct {
    void* data;      // Ponteiro para a estrutura espec�fica do objeto (Point*, Segment*, GfxPolygon*).
    ObjectType type; // Tipo do objeto (usado para fazer o cast correto de 'data').
} Object;

// Vari�veis globais para o gerenciamento da lista de objetos
// #define MAX_OBJECTS 100 // Removido, agora vem de config.h

extern Object g_objects[MAX_OBJECTS];      // Array global para armazenar todos os objetos.
extern int g_numObjects;                   // Contador global do n�mero atual de objetos na lista.
extern int g_selectedObjectIndex;          // �ndice do objeto selecionado atualmente na lista (-1 se nenhum).

// Prot�tipos das fun��es do m�dulo objects.c

// Inicializa a lista de objetos (reseta o contador e o objeto selecionado).
void initObjectList();

// Adiciona um novo objeto � lista.
void addObject(ObjectType type, void* data);

// Remove um objeto da lista pelo seu �ndice.
void removeObject(int index);

// Limpa todos os objetos da lista e libera a mem�ria alocada.
void clearAllObjects();

// Desenha um objeto espec�fico da lista (usa o type para chamar a fun��o de desenho correta).
void drawObject(int index, int is_selected);

// Desenha todos os objetos presentes na lista.
void drawAllObjects();

#endif // OBJECTS_H
