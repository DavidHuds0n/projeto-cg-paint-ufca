// objects.c
// Implementação das funções para gerenciamento genérico de objetos gráficos.

#include <stdio.h>    // Para printf e fprintf
#include <stdlib.h>   // Para malloc e free
#include <GL/glut.h>  // Para funções OpenGL como glColor3f, glPointSize, etc.

#include "objects.h"  // Inclui o próprio cabeçalho
#include "point.h"    // Inclui definições de Point e protótipos de drawPoint/freePoint
#include "segment.h"  // Inclui definições de Segment e protótipos de drawSegment/freeSegment
#include "polygon.h"  // Inclui definições de GfxPolygon e protótipos de drawPolygon/freePolygon (ATUALIZADO)
#include "config.h"   // Para MAX_OBJECTS (NOVO)

// Definição das variáveis globais
Object g_objects[MAX_OBJECTS]; // Usa MAX_OBJECTS de config.h
int g_numObjects = 0;
int g_selectedObjectIndex = -1; // -1 indica que nenhum objeto está selecionado

// Inicializa a lista de objetos
void initObjectList() {
    g_numObjects = 0;
    g_selectedObjectIndex = -1;
    printf("[Objects] Lista de objetos inicializada.\n");
}

// Adiciona um novo objeto à lista
void addObject(ObjectType type, void* data) {
    if (g_numObjects < MAX_OBJECTS) { // Usa MAX_OBJECTS de config.h
        g_objects[g_numObjects].type = type;
        g_objects[g_numObjects].data = data;
        g_numObjects++;
        printf("[Objects] Objeto adicionado: Tipo %d, Total %d.\n", type, g_numObjects);
    } else {
        fprintf(stderr, "[Objects ERROR] Limite máximo de objetos (%d) atingido. Não foi possível adicionar novo objeto.\n", MAX_OBJECTS);
    }
}

// Libera a memória de um objeto específico.
// Esta é uma função auxiliar interna para gerenciar a memória baseada no tipo.
void clearObject(Object* obj) {
    if (obj->data != NULL) {
        switch (obj->type) {
            case OBJECT_TYPE_POINT:
                freePoint((Point*)obj->data); // Chama freePoint para Point
                break;
            case OBJECT_TYPE_SEGMENT:
                freeSegment((Segment*)obj->data); // Chama freeSegment para Segment
                break;
            case OBJECT_TYPE_POLYGON:
                freePolygon((GfxPolygon*)obj->data); // Chama freePolygon para GfxPolygon (ATUALIZADO)
                break;
            default:
                fprintf(stderr, "[Objects ERROR] Tipo de objeto desconhecido para liberação de memória: %d\n", obj->type);
                break;
        }
        free(obj->data); // Libera o ponteiro void* principal
        obj->data = NULL; // Evita ponteiros dangling
    }
}


// Remove um objeto da lista pelo seu índice
void removeObject(int index) {
    printf("--- FUNÇÃO removeObject FOI CHAMADA PARA O ÍNDICE %d ---\n", index); // LINHA DE TESTE

    if (index >= 0 && index < g_numObjects) {
        // Primeiro, limpa a memória do objeto a ser removido
        clearObject(&g_objects[index]);

        // Move os objetos seguintes para preencher o "buraco"
        for (int i = index; i < g_numObjects - 1; i++) {
            g_objects[i] = g_objects[i+1];
        }
        g_numObjects--; // Decrementa o contador de objetos

        // Ajusta o índice do objeto selecionado se ele foi afetado pela remoção
        if (g_selectedObjectIndex == index) {
            g_selectedObjectIndex = -1; // Desseleciona se o objeto removido era o selecionado
        } else if (g_selectedObjectIndex > index) {
            g_selectedObjectIndex--; // Ajusta o índice se o selecionado estava depois do removido
        }
        printf("[Objects] Objeto no índice %d removido. Total de objetos: %d.\n", index, g_numObjects);
    } else {
        fprintf(stderr, "[Objects ERROR] Índice de remoção de objeto inválido: %d.\n", index);
    }
}

// Limpa todos os objetos da lista e libera a memória alocada
void clearAllObjects() {
    for (int i = 0; i < g_numObjects; i++) {
        clearObject(&g_objects[i]);
    }
    g_numObjects = 0;
    g_selectedObjectIndex = -1;
    printf("[Objects] Todos os objetos foram limpos e a memória liberada.\n");
}

// Desenha um objeto específico da lista
void drawObject(int index, int is_selected) {
    if (index >= 0 && index < g_numObjects && g_objects[index].data != NULL) {
        switch (g_objects[index].type) {
            case OBJECT_TYPE_POINT:
                drawPoint((Point*)g_objects[index].data, is_selected); // ATUALIZADO: Passa ponteiro diretamente
                break;
            case OBJECT_TYPE_SEGMENT:
                drawSegment((Segment*)g_objects[index].data, is_selected); // ATUALIZADO: Passa ponteiro diretamente
                break;
            case OBJECT_TYPE_POLYGON:
                drawPolygon((GfxPolygon*)g_objects[index].data, is_selected); // ATUALIZADO: GfxPolygon*
                break;
            default:
                fprintf(stderr, "[Objects ERROR] Tipo de objeto desconhecido para desenho: %d\n", g_objects[index].type);
                break;
        }
    }
}

// Desenha todos os objetos presentes na lista
void drawAllObjects() {
    for (int i = 0; i < g_numObjects; i++) {
        // Desenha o objeto com base no seu tipo
        // Passa '1' (true) para is_selected se o índice atual for o selecionado
        drawObject(i, (i == g_selectedObjectIndex));
    }
}
