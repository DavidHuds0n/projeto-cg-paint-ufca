// polygon.c
// Implementação das funções para criar, adicionar vértices, desenhar e
// liberar objetos do tipo Polígono.

#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>

#include "polygon.h"
#include "point.h"
#include "config.h"
#include "utils.h"


GfxPolygon createPolygon() {
    GfxPolygon poly;
    poly.numVertices = 0; // Um polígono recém-criado não tem vértices.
    // printf("[GfxPolygon] Novo polígono vazio criado.\n");
    return poly;
}

void addVertexToPolygon(GfxPolygon* poly, Point p) {
    // Verifica se ainda há espaço no array de vértices antes de adicionar.
    if (poly->numVertices < MAX_POLYGON_VERTICES) {
        poly->vertices[poly->numVertices] = p;
        poly->numVertices++;
    } else {
        fprintf(stderr, "[GfxPolygon ERROR] Limite máximo de vértices (%d) atingido.\n", MAX_POLYGON_VERTICES);
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

    // --- Desenho do Contorno do Polígono ---
    // Define a cor do contorno com base no estado de seleção.
    if (is_selected) {
        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho se selecionado.
    } else {
        glColor3f(0.0f, 0.0f, 1.0f); // Azul se não selecionado.
    }

    // Define a espessura da linha para 2.0 pixels para melhor visualização.
    glLineWidth(2.0f);

    // Usa GL_LINE_LOOP para desenhar apenas o contorno, conectando todos os vértices.
    // Esta primitiva desenhará a forma exata que você definiu com os cliques.
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < poly->numVertices; i++) {
        glVertex2f(poly->vertices[i].x, poly->vertices[i].y);
    }
    glEnd();

    // Reseta a espessura da linha para o padrão para não afetar outros desenhos.
    glLineWidth(1.0f);

    // --- (Opcional) Desenho dos Vértices ---
    // Ainda é útil desenhar os vértices para ver os pontos de controle.
    glColor3f(0.5f, 0.5f, 0.5f);
    glPointSize(CLICK_TOLERANCE / 2.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < poly->numVertices; i++) {
        glVertex2f(poly->vertices[i].x, poly->vertices[i].y);
    }
    glEnd();
    glPointSize(1.0f);

    // ====================== MUDANÇA AQUI ======================
    // Desenha o ponto central do polígono como um feedback visual.

    // 1. Só desenha o centro se o polígono for válido (tiver pelo menos 1 vértice).
    if (poly->numVertices > 0) {
        // 2. Precisamos criar um objeto temporário para passar para getObjectCenter,
        // pois ela espera um Object* e não um GfxPolygon*.
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

        // Reseta o tamanho do ponto para o padrão.
        glPointSize(1.0f);
    }
    // ==================== FIM DA MUDANÇA ====================
}

void freePolygon(GfxPolygon* poly) {
    // A struct 'GfxPolygon' usa um array de tamanho fixo para os vértices,
    // então não há memória interna alocada dinamicamente para ser liberada.
    // A função existe para manter a consistência com a API do 'objects.c'.
    // printf("[GfxPolygon] Função freePolygon chamada.\n");
}
