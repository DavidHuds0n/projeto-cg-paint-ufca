/**
 * @file polygon.c
 * @brief Implementação das funções para criar, adicionar vértices, desenhar e
 * liberar objetos do tipo Polígono.
 *
 * Este módulo gerencia as operações de baixo nível para a primitiva
 * geométrica "Polígono".
 */

#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include "polygon.h"
#include "point.h"
#include "config.h"
#include "utils.h"
#include "objects.h" // Necessário para OBJECT_TYPE_POLYGON

// --- SEÇÃO DE FUNÇÕES PÚBLICAS ---

GfxPolygon createPolygon() {
    GfxPolygon poly;
    poly.numVertices = 0;
    return poly;
}

void addVertexToPolygon(GfxPolygon* poly, Point p) {
    if (poly->numVertices < MAX_POLYGON_VERTICES) {
        poly->vertices[poly->numVertices] = p;
        poly->numVertices++;
    } else {
        fprintf(stderr, "[GfxPolygon ERROR] Limite máximo de vértices (%d) atingido.\n", MAX_POLYGON_VERTICES);
    }
}

void drawPolygon(GfxPolygon* poly, int is_selected) {
    // --- Desenho do Contorno ---
    // A cor do contorno é definida pelo estado de seleção.
    if (is_selected) {
        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho se selecionado.
    } else {
        glColor3f(0.0f, 0.0f, 1.0f); // Azul se não selecionado.
    }

    // A espessura da linha é aumentada para melhor visualização.
    glLineWidth(2.0f);

    if (poly->numVertices >= 2) {
        // Usa GL_LINE_LOOP para desenhar o contorno, fechando a forma.
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < poly->numVertices; i++) {
            glVertex2f(poly->vertices[i].x, poly->vertices[i].y);
        }
        glEnd();
    } else if (poly->numVertices == 1) {
        // Se houver apenas um vértice, desenha apenas um ponto.
        glBegin(GL_POINTS);
            glVertex2f(poly->vertices[0].x, poly->vertices[0].y);
        glEnd();
    }

    // Reseta a espessura da linha para o padrão.
    glLineWidth(1.0f);

    // --- Desenho dos Vértices ---
    // Desenha os vértices do polígono para indicar os pontos de controle.
    glColor3f(0.5f, 0.5f, 0.5f);
    glPointSize(CLICK_TOLERANCE / 2.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < poly->numVertices; i++) {
        glVertex2f(poly->vertices[i].x, poly->vertices[i].y);
    }
    glEnd();
    glPointSize(1.0f);

//    // --- Desenho do Centroide (Ponto Central) ---
//    // Desenha um ponto para indicar o centro de rotação/escala do polígono.
//    if (poly->numVertices > 0) {
//        // Cria um objeto temporário para chamar a função genérica getObjectCenter.
//        Object tempObj;
//        tempObj.type = OBJECT_TYPE_POLYGON;
//        tempObj.data = poly;
//        Point center = getObjectCenter(&tempObj);
//
//        // Desenha o ponto central como um pequeno ponto preto.
//        glColor3f(0.0f, 0.0f, 0.0f);
//        glPointSize(5.0f);
//        glBegin(GL_POINTS);
//            glVertex2f(center.x, center.y);
//        glEnd();
//
//        // Reseta o tamanho do ponto.
//        glPointSize(1.0f);
//    }
}

void freePolygon(GfxPolygon* poly) {
    // A struct 'GfxPolygon' usa um array de tamanho fixo para os vértices.
    // Portanto, não há memória alocada dinamicamente para ser liberada aqui.
    // A função existe para manter a consistência com a API do 'objects' e garantir
    // que a chamada 'free(obj->data)' em objects.c seja segura.
}
