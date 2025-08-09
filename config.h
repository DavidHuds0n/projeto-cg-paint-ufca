// config.h
// Definições de configurações globais do projeto.

#ifndef CONFIG_H
#define CONFIG_H

// --- Configurações da Janela ---
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Rabisquim - v0.1"

// --- Outras constantes do projeto (ex: tolerâncias, limites de objetos) ---
#define CLICK_TOLERANCE 10.0f           // Tolerância de clique para seleção (em pixels)
#define MAX_OBJECTS 100                 // Número máximo de objetos que a cena pode conter.
#define MAX_POLYGON_VERTICES 100        // NOVO: Número máximo de vértices para um polígono.

#endif // CONFIG_H
