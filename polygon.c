// polygon.c
// Implementa��o das fun��es para criar, adicionar v�rtices, desenhar e
// liberar objetos do tipo Pol�gono.

#include <GL/glut.h>
#include <stdio.h>

#include "polygon.h"
#include "point.h"
#include "config.h"


GfxPolygon createPolygon() {
    GfxPolygon poly;
    poly.numVertices = 0; // Um pol�gono rec�m-criado n�o tem v�rtices.
    // printf("[GfxPolygon] Novo pol�gono vazio criado.\n");
    return poly;
}

void addVertexToPolygon(GfxPolygon* poly, Point p) {
    // Verifica se ainda h� espa�o no array de v�rtices antes de adicionar.
    if (poly->numVertices < MAX_POLYGON_VERTICES) {
        poly->vertices[poly->numVertices] = p;
        poly->numVertices++;
    } else {
        fprintf(stderr, "[GfxPolygon ERROR] Limite m�ximo de v�rtices (%d) atingido.\n", MAX_POLYGON_VERTICES);
    }
}

void drawPolygon(GfxPolygon* poly, int is_selected) {
    // --- Tratamento de Casos Especiais ---
    // Um pol�gono s� pode ser desenhado como uma �rea fechada com pelo menos 3 v�rtices.
    if (poly->numVertices < 2) {
        // Se houver apenas 1 v�rtice, desenha um ponto grande como feedback visual
        // para o usu�rio durante o processo de cria��o do pol�gono.
        if (poly->numVertices == 1) {
            glColor3f(0.0f, 0.0f, 1.0f); // Ponto de feedback em azul.
            glPointSize(CLICK_TOLERANCE / 2.0f);
            glBegin(GL_POINTS);
                glVertex2f(poly->vertices[0].x, poly->vertices[0].y);
            glEnd();
            glPointSize(1.0f); // Reseta o tamanho do ponto.
        }
        return; // N�o desenha mais nada se tiver menos de 2 v�rtices.
    }

    // --- Desenho do Pol�gono Preenchido ---
    // Define a cor da �rea do pol�gono com base no estado de sele��o.
    if (is_selected) {
        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho se selecionado.
    } else {
        glColor3f(0.0f, 0.0f, 1.0f); // Azul se n�o selecionado.
    }

    // Usa GL_POLYGON, que desenha uma forma s�lida e preenchida conectando
    // todos os v�rtices. O OpenGL cuida de fechar o pol�gono automaticamente.
    glBegin(GL_POLYGON);
    for (int i = 0; i < poly->numVertices; i++) {
        glVertex2f(poly->vertices[i].x, poly->vertices[i].y);
    }
    glEnd();
}

void freePolygon(GfxPolygon* poly) {
    // A struct 'GfxPolygon' usa um array de tamanho fixo para os v�rtices,
    // ent�o n�o h� mem�ria interna alocada dinamicamente para ser liberada.
    // A fun��o existe para manter a consist�ncia com a API do 'objects.c'.
    // printf("[GfxPolygon] Fun��o freePolygon chamada.\n");
}
