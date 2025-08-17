// config.h
// Arquivo de configura��o central do projeto "Rabisquim".
// Cont�m constantes globais para facilitar a manuten��o e ajuste do programa.
// Mudar um valor aqui afeta o programa inteiro.

#ifndef CONFIG_H
#define CONFIG_H

// --- Configura��es da Janela ---
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Rabisquim - v0.2" // Vers�o atualizada :)

// --- Constantes de L�gica e Intera��o ---

// Define a "�rea de acerto" para cliques do mouse, em pixels.
// Usado para determinar se um clique foi perto o suficiente de um objeto para selecion�-lo.
#define CLICK_TOLERANCE 5.0f

// Define o limite m�ximo de objetos que podem ser criados na cena.
// Controla o tamanho do array global 'g_objects'.
#define MAX_OBJECTS 100

// Define o limite m�ximo de v�rtices que um �nico pol�gono pode ter.
// Controla o tamanho do array 'vertices' na struct GfxPolygon.
#define MAX_POLYGON_VERTICES 100

#endif // CONFIG_H
