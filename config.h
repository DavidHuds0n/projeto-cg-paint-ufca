/**
 * @file config.h
 * @brief Arquivo de configura��o central do projeto.
 *
 * Cont�m constantes globais para facilitar a manuten��o e ajuste do programa.
 * Mudar um valor aqui afeta o comportamento do programa inteiro.
 */

#ifndef CONFIG_H
#define CONFIG_H

// --- SE��O DE CONFIGURA��ES DE JANELA ---

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Rabisquim - v1.0" ///< Vers�o final :)

// --- SE��O DE CONSTANTES DE L�GICA E INTERA��O ---

/**
 * @brief Define a "�rea de acerto" para cliques do mouse.
 *
 * Usado para determinar se um clique foi perto o suficiente de um objeto
 * para selecion�-lo.
 */
#define CLICK_TOLERANCE 5.0f

/**
 * @brief Define o limite m�ximo de objetos na cena.
 *
 * Controla o tamanho do array global 'g_objects'.
 */
#define MAX_OBJECTS 100

/**
 * @brief Define o limite m�ximo de v�rtices de um �nico pol�gono.
 *
 * Controla o tamanho do array 'vertices' na struct GfxPolygon.
 */
#define MAX_POLYGON_VERTICES 100

#endif // CONFIG_H
