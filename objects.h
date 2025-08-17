// objects.h
// Define a interface pública e as estruturas de dados para o gerenciamento
// genérico de todos os objetos gráficos da cena. Este módulo é o coração do
// sistema de dados do projeto.

#ifndef OBJECTS_H
#define OBJECTS_H

#include "point.h"
#include "segment.h"
#include "polygon.h"
#include "config.h"

// Enumeração para os diferentes tipos de objetos que o programa pode manipular.
// Usado para identificar o tipo de dado apontado pelo ponteiro void* na struct Object.
typedef enum {
    OBJECT_TYPE_POINT,   // Objeto do tipo Ponto.
    OBJECT_TYPE_SEGMENT, // Objeto do tipo Segmento de Reta.
    OBJECT_TYPE_POLYGON  // Objeto do tipo Polígono.
} ObjectType;

// Estrutura genérica para armazenar qualquer tipo de objeto gráfico.
// Esta abordagem permite que diferentes tipos de structs (Point, Segment, etc.)
// coexistam em um mesmo array, simulando polimorfismo em C.
typedef struct {
    void* data;      // Ponteiro genérico para a struct específica do objeto (ex: Point*, Segment*).
    ObjectType type; // "Tag" que identifica qual é o tipo de objeto que 'data' aponta.
} Object;


// --- Variáveis Globais de Estado ---
// Declaradas aqui com 'extern' para serem acessíveis por outros módulos,
// mas definidas (alocadas na memória) em objects.c.

extern Object g_objects[MAX_OBJECTS]; // O array principal que armazena todos os objetos da cena.
extern int g_numObjects;              // Contador do número atual de objetos no array.
extern int g_selectedObjectIndex;     // Índice do objeto selecionado (-1 se nenhum).


// --- Protótipos das Funções do Módulo ---

/**
 * @brief Inicializa a lista de objetos, resetando os contadores.
 * Deve ser chamada no início do programa.
 */
void initObjectList();

/**
 * @brief Adiciona um novo objeto gráfico à lista global.
 * @param type O tipo do objeto a ser adicionado (ex: OBJECT_TYPE_POINT).
 * @param data Um ponteiro (previamente alocado com malloc) para a struct do objeto.
 */
void addObject(ObjectType type, void* data);

/**
 * @brief Remove um objeto da lista a partir de seu índice.
 * Libera a memória do objeto e reorganiza o array.
 * @param index O índice do objeto a ser removido no array g_objects.
 */
void removeObject(int index);

/**
 * @brief Remove todos os objetos da lista e libera toda a memória associada.
 * Usado ao fechar o programa para evitar vazamentos de memória (memory leaks).
 */
void clearAllObjects();

/**
 * @brief Desenha um único objeto da lista na tela.
 * @param index O índice do objeto a ser desenhado.
 * @param is_selected Flag (1 para verdadeiro, 0 para falso) que indica se o objeto
 * deve ser desenhado com a cor de seleção.
 */
void drawObject(int index, int is_selected);

/**
 * @brief Percorre a lista de objetos e chama drawObject para cada um.
 * É a função principal de desenho, chamada pelo módulo de renderização.
 */
void drawAllObjects();

#endif // OBJECTS_H
