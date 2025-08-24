/**
 * @file objects.h
 * @brief Define a interface e as estruturas para o gerenciamento de objetos gr�ficos.
 *
 * Este m�dulo � o cora��o do sistema de dados do projeto, respons�vel por
 * armazenar e manipular uma lista gen�rica de objetos gr�ficos.
 */

#ifndef OBJECTS_H
#define OBJECTS_H

#include "point.h"
#include "segment.h"
#include "polygon.h"
#include "config.h"

// --- SE��O DE ESTRUTURAS E TIPOS ---

/**
 * @brief Enumera��o para os diferentes tipos de objetos gr�ficos.
 *
 * Utilizado para identificar o tipo de dado para o ponteiro 'void*' na struct Object.
 */
typedef enum {
    OBJECT_TYPE_POINT,      ///< Objeto do tipo Ponto.
    OBJECT_TYPE_SEGMENT,    ///< Objeto do tipo Segmento de Reta.
    OBJECT_TYPE_POLYGON     ///< Objeto do tipo Pol�gono.
} ObjectType;

/**
 * @brief Estrutura gen�rica para armazenar qualquer tipo de objeto.
 *
 * Esta abordagem simula polimorfismo em C, permitindo que diferentes
 * tipos de structs coexistam em um mesmo array.
 */
typedef struct {
    void* data;             ///< Ponteiro gen�rico para a struct espec�fica do objeto.
    ObjectType type;        ///< "Tag" que identifica o tipo de dado apontado por 'data'.
} Object;

// --- SE��O DE VARI�VEIS GLOBAIS DE ESTADO (DECLARA��ES) ---

extern Object g_objects[MAX_OBJECTS];       ///< O array principal que armazena todos os objetos da cena.
extern int g_numObjects;                    ///< O n�mero atual de objetos no array.
extern int g_selectedObjectIndex;           ///< O �ndice do objeto selecionado na lista (-1 se nenhum).

// --- SE��O DE PROT�TIPOS DE FUN��ES P�BLICAS ---

void initObjectList();
void addObject(ObjectType type, void* data);
void removeObject(int index);
void clearAllObjects();
void drawAllObjects();

#endif // OBJECTS_H
