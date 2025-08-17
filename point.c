// point.c
// Implementação das funções para criar, desenhar e liberar objetos do tipo Ponto.

#include <GL/glut.h>
#include <stdio.h>

#include "point.h"
#include "config.h"

Point createPoint(float x, float y) {
    Point p = {x, y};
    // O printf abaixo é para fins de depuração e pode ser removido na versão final.
    // printf("[Point] Novo ponto criado em (%.1f, %.1f).\n", p.x, p.y);
    return p;
}

void drawPoint(Point* p, int is_selected) {
    // Define a cor do ponto com base no estado de seleção.
    if (is_selected) {
        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho se selecionado.
    } else {
        glColor3f(0.0f, 0.0f, 1.0f); // Azul se não selecionado.
    }

    // Aumenta o tamanho do ponto para torná-lo mais visível e fácil de selecionar.
    // Basear o tamanho na tolerância de clique garante consistência visual.
    glPointSize(CLICK_TOLERANCE / 2.0f);

    // Desenha o ponto na tela usando as primitivas do OpenGL.
    glBegin(GL_POINTS);
        glVertex2f(p->x, p->y);
    glEnd();

    // Reseta o tamanho do ponto para o padrão para não afetar outros desenhos na cena.
    glPointSize(1.0f);
}

void freePoint(Point* p) {
    // A estrutura 'Point' não possui ponteiros ou memória alocada dinamicamente
    // em seu interior, então não há nada a ser liberado aqui.
    // A função existe para manter uma API consistente no módulo 'objects.c',
    // que chama uma função 'free<Objeto>' para qualquer tipo de objeto.
    // printf("[Point] Função freePoint chamada.\n");
}
