// objects.c
// Implementa a lógica de gerenciamento da lista global de objetos gráficos.
// É responsável por adicionar, remover, desenhar e limpar todos os
// objetos da cena de forma genérica.

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

// --- Definição das Variáveis Globais ---
Object g_objects[MAX_OBJECTS];
int g_numObjects = 0;
int g_selectedObjectIndex = -1;

// --- Funções Privadas (Auxiliares) ---
void clearObject(Object* obj);
void drawObject(int index, int is_selected);

// --- Funções Públicas ---

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
        // mantém o vetor g_anim alinhado
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
    // 1ª PASSADA: Desenha todos os objetos que NÃO estão selecionados.
    for (int i = 0; i < g_numObjects; i++) {
        if (i != g_selectedObjectIndex) {
            drawObject(i, 0);
        }
    }
    // 2ª PASSADA: Desenha o objeto selecionado por último (na frente).
    if (g_selectedObjectIndex != -1) {
        drawObject(g_selectedObjectIndex, 1);
    }

    // 3. Lógica para desenhar o feedback visual dos objetos em criação.
    if (g_currentMode == MODE_CREATE_POLYGON && g_polygonVertexCount > 0) {
        glColor3f(0.5f, 0.5f, 0.5f);
        glPointSize(CLICK_TOLERANCE / 2.0f);
        glBegin(GL_POINTS);
        for (int i = 0; i < g_polygonVertexCount; i++) {
            glVertex2f(g_polygonVertices[i].x, g_polygonVertices[i].y);
        }
        glEnd();
        glPointSize(1.0f);

        if (g_polygonVertexCount > 1) {
            glColor3f(0.0f, 0.0f, 1.0f);
            glBegin(GL_LINE_STRIP);
            for (int i = 0; i < g_polygonVertexCount; i++) {
                glVertex2f(g_polygonVertices[i].x, g_polygonVertices[i].y);
            }
            glEnd();
        }
    }
    else if (g_currentMode == MODE_CREATE_SEGMENT && g_segmentClickCount == 1) {
        // Desenha o primeiro ponto do segmento como marcador.
        glColor3f(0.5f, 0.5f, 0.5f);
        glPointSize(CLICK_TOLERANCE / 2.0f);
        glBegin(GL_POINTS);
            glVertex2f(g_segmentP1.x, g_segmentP1.y);
        glEnd();
        glPointSize(1.0f);

        // Desenha a linha "rubber band" do primeiro ponto até a posição atual do mouse.
        glColor3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_LINES);
            glVertex2f(g_segmentP1.x, g_segmentP1.y);
            glVertex2f(g_currentMousePos.x, g_currentMousePos.y);
        glEnd();
    }
}

// --- Implementação das Funções Privadas ---

void clearObject(Object* obj) {
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

void drawObject(int index, int is_selected) {
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
