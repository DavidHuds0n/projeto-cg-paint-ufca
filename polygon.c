// polygon.c
// Implementa��o das fun��es para manipula��o de pol�gonos.

#include <GL/glut.h> // Para fun��es de desenho OpenGL (glVertex, glBegin, glEnd, glColor)
#include <stdio.h>   // Para printf (para depura��o)
#include "polygon.h" // Inclui o pr�prio cabe�alho (agora com GfxPolygon)
#include "point.h"   // Para usar a estrutura Point
#include "config.h"  // Para usar CLICK_TOLERANCE e MAX_POLYGON_VERTICES

// Implementa��o das fun��es para GfxPolygon

// Cria um novo pol�gono vazio (pronto para adicionar v�rtices)
GfxPolygon createPolygon() { // ATUALIZADO: Retorna GfxPolygon
    GfxPolygon poly; // ATUALIZADO: Usa GfxPolygon
    poly.numVertices = 0; // Inicializa o n�mero de v�rtices como zero
    printf("[GfxPolygon] Novo pol�gono vazio criado.\n");
    return poly;
}

// Adiciona um v�rtice ao pol�gono
void addVertexToPolygon(GfxPolygon* poly, Point p) { // ATUALIZADO: Recebe GfxPolygon*
    // Verifica se ainda h� espa�o para adicionar mais v�rtices
    if (poly->numVertices < MAX_POLYGON_VERTICES) { // Usa MAX_POLYGON_VERTICES de config.h
        poly->vertices[poly->numVertices] = p; // Adiciona o ponto ao array de v�rtices
        poly->numVertices++;                   // Incrementa o contador de v�rtices
        printf("[GfxPolygon] V�rtice adicionado em (%.1f, %.1f). Total de v�rtices: %d.\n", p.x, p.y, poly->numVertices);
    } else {
        fprintf(stderr, "[GfxPolygon ERROR] Limite m�ximo de v�rtices (%d) atingido para este pol�gono.\n", MAX_POLYGON_VERTICES);
    }
}

// Desenha o pol�gono na tela
void drawPolygon(GfxPolygon* poly, int is_selected) { // ATUALIZADO: Recebe GfxPolygon*
    if (poly->numVertices < 2) {
        // Pol�gonos precisam de pelo menos 2 v�rtices para uma linha,
        // e 3 para ser uma forma fechada.
        // Se houver 1 v�rtice, desenha como um ponto (opcional, para feedback de cria��o)
        if (poly->numVertices == 1) {
            // Desenha o primeiro v�rtice como um ponto grande para indicar onde o clique foi feito
            if (is_selected) {
                glColor3f(1.0f, 0.0f, 0.0f); // Vermelho para ponto de sele��o (se estiver no modo de cria��o e for o primeiro v�rtice)
            } else {
                 glColor3f(0.0f, 0.0f, 1.0f); // Azul para ponto de feedback
            }
            glPointSize(CLICK_TOLERANCE * 1.5f); // Um ponto maior para feedback visual
            glBegin(GL_POINTS);
            glVertex2f(poly->vertices[0].x, poly->vertices[0].y);
            glEnd();
            glPointSize(1.0f); // Volta ao tamanho padr�o de ponto
        }
        return;
    }

    // Define a cor do pol�gono
    if (is_selected) {
        // Se o pol�gono estiver selecionado, desenha-o em vermelho.
        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho
    } else {
        // Caso contr�rio, desenha-o em azul (ou outra cor padr�o).
        glColor3f(0.0f, 0.0f, 1.0f); // Azul
    }

    // Desenha o contorno do pol�gono
    glLineWidth(2.0f); // Largura da linha
    glBegin(GL_LINE_LOOP); // GL_LINE_LOOP conecta o �ltimo v�rtice ao primeiro, fechando o pol�gono
    for (int i = 0; i < poly->numVertices; i++) {
        glVertex2f(poly->vertices[i].x, poly->vertices[i].y);
    }
    glEnd();
    glLineWidth(1.0f); // Volta � largura de linha padr�o

    // Opcional: Desenhar os v�rtices do pol�gono para maior clareza visual
    // Isso pode ser �til para depura��o ou para mostrar os pontos de controle.
    glColor3f(0.5f, 0.5f, 0.5f); // Cor cinza para os v�rtices
    glPointSize(CLICK_TOLERANCE / 2.0f); // Pontos menores nos v�rtices
    glBegin(GL_POINTS);
    for (int i = 0; i < poly->numVertices; i++) {
        glVertex2f(poly->vertices[i].x, poly->vertices[i].y);
    }
    glEnd();
    glPointSize(1.0f); // Volta ao tamanho padr�o de ponto
}

// Libera a mem�ria alocada para um pol�gono (se houver, para casos mais complexos)
// Atualmente, n�o h� aloca��o din�mica DENTRO da estrutura GfxPolygon em si,
// ent�o esta fun��o � um placeholder.
void freePolygon(GfxPolygon* poly) { // ATUALIZADO: Recebe GfxPolygon*
    printf("[GfxPolygon] Fun��o freePolygon chamada para pol�gono com %d v�rtices.\n", poly->numVertices);
}
