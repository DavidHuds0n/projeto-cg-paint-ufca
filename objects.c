/**
 * @file objects.c
 * @brief Implementa a lógica de gerenciamento da lista global de objetos gráficos.
 *
 * Responsável por adicionar, remover, desenhar e limpar todos os objetos da
 * cena de forma genérica, utilizando um array e a técnica de "polimorfismo"
 * com `void*` e um `enum`.
 */

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#include "objects.h"
#include "point.h"
#include "segment.h"
#include "polygon.h"
#include "config.h"
#include "input.h"
#include "animation.h"

// --- SEÇÃO DE VARIÁVEIS GLOBAIS DE ESTADO (DEFINIÇÕES) ---

Object g_objects[MAX_OBJECTS];
int g_numObjects = 0;
int g_selectedObjectIndex = -1;

// --- SEÇÃO DE FUNÇÕES AUXILIARES (IMPLEMENTAÇÃO PRIVADA) ---
// Essas funções não são expostas no arquivo de cabeçalho.

/**
 * @brief Libera a memória alocada para um único objeto gráfico.
 * @param obj O ponteiro para o objeto a ser limpo.
 */
static void clearObject(Object* obj) {
    if (obj != NULL && obj->data != NULL) {
        switch (obj->type) {
            case OBJECT_TYPE_POINT: freePoint((Point*)obj->data); break;
            case OBJECT_TYPE_SEGMENT: freeSegment((Segment*)obj->data); break;
            case OBJECT_TYPE_POLYGON: freePolygon((GfxPolygon*)obj->data); break;
        }
        free(obj->data);
        obj->data = NULL;
    }
}

/**
 * @brief Desenha um único objeto da lista na tela.
 * @param index O índice do objeto a ser desenhado.
 * @param is_selected Um flag que indica se o objeto deve ser desenhado em modo de seleção.
 */
static void drawObject(int index, int is_selected) {
    if (index >= 0 && index < g_numObjects && g_objects[index].data != NULL) {
        switch (g_objects[index].type) {
            case OBJECT_TYPE_POINT:
                drawPoint((Point*)g_objects[index].data, is_selected);
                break;
            case OBJECT_TYPE_SEGMENT:
                drawSegment((Segment*)g_objects[index].data, is_selected);
                break;
            case OBJECT_TYPE_POLYGON:
                drawPolygon((GfxPolygon*)g_objects[index].data, is_selected);
                break;
        }
    }
}

// --- SEÇÃO DE FUNÇÕES PÚBLICAS ---

void initObjectList() {
    g_numObjects = 0;
    g_selectedObjectIndex = -1;
}

void addObject(ObjectType type, void* data) {
    if (g_numObjects < MAX_OBJECTS) {
        g_objects[g_numObjects].type = type;
        g_objects[g_numObjects].data = data;
        g_numObjects++;
    } else {
        fprintf(stderr, "[Objects ERROR] Limite máximo de objetos (%d) atingido.\n", MAX_OBJECTS);
    }
}

void removeObject(int index) {
    if (index >= 0 && index < g_numObjects) {
        clearObject(&g_objects[index]);
        for (int i = index; i < g_numObjects - 1; i++) {
            g_objects[i] = g_objects[i + 1];
        }
        g_numObjects--;
        anim_on_remove_compact(index);
        if (g_selectedObjectIndex == index) {
            g_selectedObjectIndex = -1;
        } else if (g_selectedObjectIndex > index) {
            g_selectedObjectIndex--;
        }
    } else {
        fprintf(stderr, "[Objects ERROR] Índice de remoção de objeto inválido: %d.\n", index);
    }
}

void clearAllObjects() {
    for (int i = 0; i < g_numObjects; i++) {
        clearObject(&g_objects[i]);
    }
    g_numObjects = 0;
    g_selectedObjectIndex = -1;
}

void drawAllObjects() {
    // 1ª Passada: Desenha todos os objetos que NÃO estão selecionados.
    for (int i = 0; i < g_numObjects; i++) {
        if (i != g_selectedObjectIndex) {
            drawObject(i, 0);
        }
    }
    // 2ª Passada: Desenha o objeto selecionado por último para destacá-lo (z-order).
    if (g_selectedObjectIndex != -1) {
        drawObject(g_selectedObjectIndex, 1);
    }

    // 3ª Passada: Desenha a pré-visualização (feedback visual) de objetos em criação.
    // Esta lógica é necessária aqui para interagir com o estado do módulo de input.
    if (g_currentMode == MODE_CREATE_POLYGON && g_polygonVertexCount > 0) {
        // Desenha os vértices já clicados.
        glColor3f(0.5f, 0.5f, 0.5f);
        glPointSize(CLICK_TOLERANCE / 2.0f);
        glBegin(GL_POINTS);
        for (int i = 0; i < g_polygonVertexCount; i++) {
            glVertex2f(g_polygonVertices[i].x, g_polygonVertices[i].y);
        }
        glEnd();
        glPointSize(1.0f);

        // Desenha a linha entre os vértices para visualização.
        if (g_polygonVertexCount > 1) {
            glColor3f(0.0f, 0.0f, 1.0f);
            glBegin(GL_LINE_STRIP);
            for (int i = 0; i < g_polygonVertexCount; i++) {
                glVertex2f(g_polygonVertices[i].x, g_polygonVertices[i].y);
            }
            glEnd();
        }

        // Desenha o "rubber band" do último vértice até a posição atual do mouse.
        if (g_polygonVertexCount > 0) {
            glColor3f(0.0f, 1.0f, 0.0f);
            glBegin(GL_LINES);
            glVertex2f(g_polygonVertices[g_polygonVertexCount - 1].x, g_polygonVertices[g_polygonVertexCount - 1].y);
            glVertex2f(g_currentMousePos.x, g_currentMousePos.y);
            glEnd();
        }
    } else if (g_currentMode == MODE_CREATE_SEGMENT && g_segmentClickCount == 1) {
        // Desenha o primeiro ponto do segmento como marcador.
        glColor3f(0.5f, 0.5f, 0.5f);
        glPointSize(CLICK_TOLERANCE / 2.0f);
        glBegin(GL_POINTS);
        glVertex2f(g_segmentP1.x, g_segmentP1.y);
        glEnd();
        glPointSize(1.0f);

        // Desenha a linha "rubber band" do primeiro ponto até a posição atual do mouse.
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_LINES);
        glVertex2f(g_segmentP1.x, g_segmentP1.y);
        glVertex2f(g_currentMousePos.x, g_currentMousePos.y);
        glEnd();
    }
}
