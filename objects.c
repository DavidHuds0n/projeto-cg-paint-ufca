/**
 * @file objects.c
 * @brief Implementa a l�gica de gerenciamento da lista global de objetos gr�ficos.
 *
 * Respons�vel por adicionar, remover, desenhar e limpar todos os objetos da
 * cena de forma gen�rica, utilizando um array e a t�cnica de "polimorfismo"
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

// --- SE��O DE VARI�VEIS GLOBAIS DE ESTADO (DEFINI��ES) ---

Object g_objects[MAX_OBJECTS];
int g_numObjects = 0;
int g_selectedObjectIndex = -1;

// --- SE��O DE FUN��ES AUXILIARES (IMPLEMENTA��O PRIVADA) ---
// Essas fun��es n�o s�o expostas no arquivo de cabe�alho.

/**
 * @brief Libera a mem�ria alocada para um �nico objeto gr�fico.
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
 * @brief Desenha um �nico objeto da lista na tela.
 * @param index O �ndice do objeto a ser desenhado.
 * @param is_selected Um flag que indica se o objeto deve ser desenhado em modo de sele��o.
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

// --- SE��O DE FUN��ES P�BLICAS ---

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
        fprintf(stderr, "[Objects ERROR] Limite m�ximo de objetos (%d) atingido.\n", MAX_OBJECTS);
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
        fprintf(stderr, "[Objects ERROR] �ndice de remo��o de objeto inv�lido: %d.\n", index);
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
    // 1� Passada: Desenha todos os objetos que N�O est�o selecionados.
    for (int i = 0; i < g_numObjects; i++) {
        if (i != g_selectedObjectIndex) {
            drawObject(i, 0);
        }
    }
    // 2� Passada: Desenha o objeto selecionado por �ltimo para destac�-lo (z-order).
    if (g_selectedObjectIndex != -1) {
        drawObject(g_selectedObjectIndex, 1);
    }

    // 3� Passada: Desenha a pr�-visualiza��o (feedback visual) de objetos em cria��o.
    // Esta l�gica � necess�ria aqui para interagir com o estado do m�dulo de input.
    if (g_currentMode == MODE_CREATE_POLYGON && g_polygonVertexCount > 0) {
        // Desenha os v�rtices j� clicados.
        glColor3f(0.5f, 0.5f, 0.5f);
        glPointSize(CLICK_TOLERANCE / 2.0f);
        glBegin(GL_POINTS);
        for (int i = 0; i < g_polygonVertexCount; i++) {
            glVertex2f(g_polygonVertices[i].x, g_polygonVertices[i].y);
        }
        glEnd();
        glPointSize(1.0f);

        // Desenha a linha entre os v�rtices para visualiza��o.
        if (g_polygonVertexCount > 1) {
            glColor3f(0.0f, 0.0f, 1.0f);
            glBegin(GL_LINE_STRIP);
            for (int i = 0; i < g_polygonVertexCount; i++) {
                glVertex2f(g_polygonVertices[i].x, g_polygonVertices[i].y);
            }
            glEnd();
        }

        // Desenha o "rubber band" do �ltimo v�rtice at� a posi��o atual do mouse.
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

        // Desenha a linha "rubber band" do primeiro ponto at� a posi��o atual do mouse.
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_LINES);
        glVertex2f(g_segmentP1.x, g_segmentP1.y);
        glVertex2f(g_currentMousePos.x, g_currentMousePos.y);
        glEnd();
    }
}
