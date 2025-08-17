// objects.h
// Define a interface p�blica e as estruturas de dados para o gerenciamento
// gen�rico de todos os objetos gr�ficos da cena. Este m�dulo � o cora��o do
// sistema de dados do projeto.

#ifndef OBJECTS_H
#define OBJECTS_H

#include "point.h"
#include "segment.h"
#include "polygon.h"
#include "config.h"

// Enumera��o para os diferentes tipos de objetos que o programa pode manipular.
// Usado para identificar o tipo de dado apontado pelo ponteiro void* na struct Object.
typedef enum {
    OBJECT_TYPE_POINT,   // Objeto do tipo Ponto.
    OBJECT_TYPE_SEGMENT, // Objeto do tipo Segmento de Reta.
    OBJECT_TYPE_POLYGON  // Objeto do tipo Pol�gono.
} ObjectType;

// Estrutura gen�rica para armazenar qualquer tipo de objeto gr�fico.
// Esta abordagem permite que diferentes tipos de structs (Point, Segment, etc.)
// coexistam em um mesmo array, simulando polimorfismo em C.
typedef struct {
    void* data;      // Ponteiro gen�rico para a struct espec�fica do objeto (ex: Point*, Segment*).
    ObjectType type; // "Tag" que identifica qual � o tipo de objeto que 'data' aponta.
} Object;


// --- Vari�veis Globais de Estado ---
// Declaradas aqui com 'extern' para serem acess�veis por outros m�dulos,
// mas definidas (alocadas na mem�ria) em objects.c.

extern Object g_objects[MAX_OBJECTS]; // O array principal que armazena todos os objetos da cena.
extern int g_numObjects;              // Contador do n�mero atual de objetos no array.
extern int g_selectedObjectIndex;     // �ndice do objeto selecionado (-1 se nenhum).


// --- Prot�tipos das Fun��es do M�dulo ---

/**
 * @brief Inicializa a lista de objetos, resetando os contadores.
 * Deve ser chamada no in�cio do programa.
 */
void initObjectList();

/**
 * @brief Adiciona um novo objeto gr�fico � lista global.
 * @param type O tipo do objeto a ser adicionado (ex: OBJECT_TYPE_POINT).
 * @param data Um ponteiro (previamente alocado com malloc) para a struct do objeto.
 */
void addObject(ObjectType type, void* data);

/**
 * @brief Remove um objeto da lista a partir de seu �ndice.
 * Libera a mem�ria do objeto e reorganiza o array.
 * @param index O �ndice do objeto a ser removido no array g_objects.
 */
void removeObject(int index);

/**
 * @brief Remove todos os objetos da lista e libera toda a mem�ria associada.
 * Usado ao fechar o programa para evitar vazamentos de mem�ria (memory leaks).
 */
void clearAllObjects();

/**
 * @brief Desenha um �nico objeto da lista na tela.
 * @param index O �ndice do objeto a ser desenhado.
 * @param is_selected Flag (1 para verdadeiro, 0 para falso) que indica se o objeto
 * deve ser desenhado com a cor de sele��o.
 */
void drawObject(int index, int is_selected);

/**
 * @brief Percorre a lista de objetos e chama drawObject para cada um.
 * � a fun��o principal de desenho, chamada pelo m�dulo de renderiza��o.
 */
void drawAllObjects();

#endif // OBJECTS_H
