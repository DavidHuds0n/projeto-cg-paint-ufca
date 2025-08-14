// objects.c
// Implementa��o das fun��es para gerenciamento gen�rico de objetos gr�ficos.

#include <stdio.h>    // Para printf e fprintf
#include <stdlib.h>   // Para malloc e free
#include <GL/glut.h>  // Para fun��es OpenGL como glColor3f, glPointSize, etc.

#include "objects.h"  // Inclui o pr�prio cabe�alho
#include "point.h"    // Inclui defini��es de Point e prot�tipos de drawPoint/freePoint
#include "segment.h"  // Inclui defini��es de Segment e prot�tipos de drawSegment/freeSegment
#include "polygon.h"  // Inclui defini��es de GfxPolygon e prot�tipos de drawPolygon/freePolygon (ATUALIZADO)
#include "config.h"   // Para MAX_OBJECTS (NOVO)

// Defini��o das vari�veis globais
Object g_objects[MAX_OBJECTS]; // Usa MAX_OBJECTS de config.h
int g_numObjects = 0;
int g_selectedObjectIndex = -1; // -1 indica que nenhum objeto est� selecionado

// Inicializa a lista de objetos
void initObjectList() {
    g_numObjects = 0;
    g_selectedObjectIndex = -1;
    printf("[Objects] Lista de objetos inicializada.\n");
}

// Adiciona um novo objeto � lista
void addObject(ObjectType type, void* data) {
    if (g_numObjects < MAX_OBJECTS) { // Usa MAX_OBJECTS de config.h
        g_objects[g_numObjects].type = type;
        g_objects[g_numObjects].data = data;
        g_numObjects++;
        printf("[Objects] Objeto adicionado: Tipo %d, Total %d.\n", type, g_numObjects);
    } else {
        fprintf(stderr, "[Objects ERROR] Limite m�ximo de objetos (%d) atingido. N�o foi poss�vel adicionar novo objeto.\n", MAX_OBJECTS);
    }
}

// Libera a mem�ria de um objeto espec�fico.
// Esta � uma fun��o auxiliar interna para gerenciar a mem�ria baseada no tipo.
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
                fprintf(stderr, "[Objects ERROR] Tipo de objeto desconhecido para libera��o de mem�ria: %d\n", obj->type);
                break;
        }
        free(obj->data); // Libera o ponteiro void* principal
        obj->data = NULL; // Evita ponteiros dangling
    }
}


// Remove um objeto da lista pelo seu �ndice
void removeObject(int index) {
    printf("--- FUN��O removeObject FOI CHAMADA PARA O �NDICE %d ---\n", index); // LINHA DE TESTE

    if (index >= 0 && index < g_numObjects) {
        // Primeiro, limpa a mem�ria do objeto a ser removido
        clearObject(&g_objects[index]);

        // Move os objetos seguintes para preencher o "buraco"
        for (int i = index; i < g_numObjects - 1; i++) {
            g_objects[i] = g_objects[i+1];
        }
        g_numObjects--; // Decrementa o contador de objetos

        // Ajusta o �ndice do objeto selecionado se ele foi afetado pela remo��o
        if (g_selectedObjectIndex == index) {
            g_selectedObjectIndex = -1; // Desseleciona se o objeto removido era o selecionado
        } else if (g_selectedObjectIndex > index) {
            g_selectedObjectIndex--; // Ajusta o �ndice se o selecionado estava depois do removido
        }
        printf("[Objects] Objeto no �ndice %d removido. Total de objetos: %d.\n", index, g_numObjects);
    } else {
        fprintf(stderr, "[Objects ERROR] �ndice de remo��o de objeto inv�lido: %d.\n", index);
    }
}

// Limpa todos os objetos da lista e libera a mem�ria alocada
void clearAllObjects() {
    for (int i = 0; i < g_numObjects; i++) {
        clearObject(&g_objects[i]);
    }
    g_numObjects = 0;
    g_selectedObjectIndex = -1;
    printf("[Objects] Todos os objetos foram limpos e a mem�ria liberada.\n");
}

// Desenha um objeto espec�fico da lista
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
        // Passa '1' (true) para is_selected se o �ndice atual for o selecionado
        drawObject(i, (i == g_selectedObjectIndex));
    }
}
