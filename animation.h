// animation.h
// Define a interface para as funções de animação do projeto.

#ifndef ANIMATION_H
#define ANIMATION_H

/**
 * @brief Inicia o loop de animação.
 * Deve ser chamada uma única vez na função main().
 */
void startAnimation();

/**
 * @brief A função de callback que o GLUT chamará periodicamente.
 * Contém a lógica que atualiza as propriedades dos objetos a cada "frame"
 * da animação (ex: mover, rotacionar, etc.).
 * @param value Um valor inteiro que pode ser usado para passar dados para o callback.
 */
void updateAnimation(int value);

#endif // ANIMATION_H
