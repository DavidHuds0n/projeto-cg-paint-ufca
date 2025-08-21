// polygon.c
// Implementa��o das fun��es para criar, adicionar v�rtices, desenhar e
// liberar objetos do tipo Pol�gono.

#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>

#include "polygon.h"
#include "point.h"
#include "config.h"
#include "utils.h"


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
    if (poly->numVertices < 2) {
        if (poly->numVertices == 1) {
            glColor3f(0.0f, 0.0f, 1.0f);
            glPointSize(CLICK_TOLERANCE / 2.0f);
            glBegin(GL_POINTS);
                glVertex2f(poly->vertices[0].x, poly->vertices[0].y);
            glEnd();
            glPointSize(1.0f);
        }
        return;
    }

    // --- Desenho do Contorno do Pol�gono ---
    // Define a cor do contorno com base no estado de sele��o.
    if (is_selected) {
        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho se selecionado.
    } else {
        glColor3f(0.0f, 0.0f, 1.0f); // Azul se n�o selecionado.
    }

    // Define a espessura da linha para 2.0 pixels para melhor visualiza��o.
    glLineWidth(2.0f);

    // Usa GL_LINE_LOOP para desenhar apenas o contorno, conectando todos os v�rtices.
    // Esta primitiva desenhar� a forma exata que voc� definiu com os cliques.
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < poly->numVertices; i++) {
        glVertex2f(poly->vertices[i].x, poly->vertices[i].y);
    }
    glEnd();

    // Reseta a espessura da linha para o padr�o para n�o afetar outros desenhos.
    glLineWidth(1.0f);

    // --- (Opcional) Desenho dos V�rtices ---
    // Ainda � �til desenhar os v�rtices para ver os pontos de controle.
    glColor3f(0.5f, 0.5f, 0.5f);
    glPointSize(CLICK_TOLERANCE / 2.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < poly->numVertices; i++) {
        glVertex2f(poly->vertices[i].x, poly->vertices[i].y);
    }
    glEnd();
    glPointSize(1.0f);

    // ====================== MUDAN�A AQUI ======================
    // Desenha o ponto central do pol�gono como um feedback visual.

    // 1. S� desenha o centro se o pol�gono for v�lido (tiver pelo menos 1 v�rtice).
    if (poly->numVertices > 0) {
        // 2. Precisamos criar um objeto tempor�rio para passar para getObjectCenter,
        // pois ela espera um Object* e n�o um GfxPolygon*.
        Object tempObj;
        tempObj.type = OBJECT_TYPE_POLYGON;
        tempObj.data = poly;

        // 3. Calcula o centro.
        Point center = getObjectCenter(&tempObj);

        // 4. Desenha o ponto central (ex: um pequeno quadrado amarelo).
        glColor3f(0.0f, 0.0f, 0.0f); // Cor amarela
        glPointSize(5.0f);           // Tamanho de 5 pixels

        glBegin(GL_POINTS);
            glVertex2f(center.x, center.y);
        glEnd();

        // Reseta o tamanho do ponto para o padr�o.
        glPointSize(1.0f);
    }
    // ==================== FIM DA MUDAN�A ====================
}

void freePolygon(GfxPolygon* poly) {
    // A struct 'GfxPolygon' usa um array de tamanho fixo para os v�rtices,
    // ent�o n�o h� mem�ria interna alocada dinamicamente para ser liberada.
    // A fun��o existe para manter a consist�ncia com a API do 'objects.c'.
    // printf("[GfxPolygon] Fun��o freePolygon chamada.\n");
}
