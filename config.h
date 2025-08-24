/**
 * @file config.h
 * @brief Arquivo de configuração central do projeto.
 *
 * Contém constantes globais para facilitar a manutenção e ajuste do programa.
 * Mudar um valor aqui afeta o comportamento do programa inteiro.
 */

#ifndef CONFIG_H
#define CONFIG_H

// --- SEÇÃO DE CONFIGURAÇÕES DE JANELA ---

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Rabisquim - v1.0" ///< Versão final :)

// --- SEÇÃO DE CONSTANTES DE LÓGICA E INTERAÇÃO ---

/**
 * @brief Define a "área de acerto" para cliques do mouse.
 *
 * Usado para determinar se um clique foi perto o suficiente de um objeto
 * para selecioná-lo.
 */
#define CLICK_TOLERANCE 5.0f

/**
 * @brief Define o limite máximo de objetos na cena.
 *
 * Controla o tamanho do array global 'g_objects'.
 */
#define MAX_OBJECTS 100

/**
 * @brief Define o limite máximo de vértices de um único polígono.
 *
 * Controla o tamanho do array 'vertices' na struct GfxPolygon.
 */
#define MAX_POLYGON_VERTICES 100

#endif // CONFIG_H
