/**
 * @file objects.h
 * @brief Define a interface e as estruturas para o gerenciamento de objetos gráficos.
 *
 * Este módulo é o coração do sistema de dados do projeto, responsável por
 * armazenar e manipular uma lista genérica de objetos gráficos.
 */

#ifndef OBJECTS_H
#define OBJECTS_H

#include "point.h"
#include "segment.h"
#include "polygon.h"
#include "config.h"

// --- SEÇÃO DE ESTRUTURAS E TIPOS ---

/**
 * @brief Enumeração para os diferentes tipos de objetos gráficos.
 *
 * Utilizado para identificar o tipo de dado para o ponteiro 'void*' na struct Object.
 */
typedef enum {
    OBJECT_TYPE_POINT,      ///< Objeto do tipo Ponto.
    OBJECT_TYPE_SEGMENT,    ///< Objeto do tipo Segmento de Reta.
    OBJECT_TYPE_POLYGON     ///< Objeto do tipo Polígono.
} ObjectType;

/**
 * @brief Estrutura genérica para armazenar qualquer tipo de objeto.
 *
 * Esta abordagem simula polimorfismo em C, permitindo que diferentes
 * tipos de structs coexistam em um mesmo array.
 */
typedef struct {
    void* data;             ///< Ponteiro genérico para a struct específica do objeto.
    ObjectType type;        ///< "Tag" que identifica o tipo de dado apontado por 'data'.
} Object;

// --- SEÇÃO DE VARIÁVEIS GLOBAIS DE ESTADO (DECLARAÇÕES) ---

extern Object g_objects[MAX_OBJECTS];       ///< O array principal que armazena todos os objetos da cena.
extern int g_numObjects;                    ///< O número atual de objetos no array.
extern int g_selectedObjectIndex;           ///< O índice do objeto selecionado na lista (-1 se nenhum).

// --- SEÇÃO DE PROTÓTIPOS DE FUNÇÕES PÚBLICAS ---

void initObjectList();
void addObject(ObjectType type, void* data);
void removeObject(int index);
void clearAllObjects();
void drawAllObjects();

#endif // OBJECTS_H
