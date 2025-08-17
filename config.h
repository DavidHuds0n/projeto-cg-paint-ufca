// config.h
// Arquivo de configuração central do projeto "Rabisquim".
// Contém constantes globais para facilitar a manutenção e ajuste do programa.
// Mudar um valor aqui afeta o programa inteiro.

#ifndef CONFIG_H
#define CONFIG_H

// --- Configurações da Janela ---
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Rabisquim - v0.2" // Versão atualizada :)

// --- Constantes de Lógica e Interação ---

// Define a "área de acerto" para cliques do mouse, em pixels.
// Usado para determinar se um clique foi perto o suficiente de um objeto para selecioná-lo.
#define CLICK_TOLERANCE 5.0f

// Define o limite máximo de objetos que podem ser criados na cena.
// Controla o tamanho do array global 'g_objects'.
#define MAX_OBJECTS 100

// Define o limite máximo de vértices que um único polígono pode ter.
// Controla o tamanho do array 'vertices' na struct GfxPolygon.
#define MAX_POLYGON_VERTICES 100

#endif // CONFIG_H
