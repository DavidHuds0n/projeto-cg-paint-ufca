// animation.h
// Define a interface para as fun��es de anima��o do projeto.

#ifndef ANIMATION_H
#define ANIMATION_H

/**
 * @brief Inicia o loop de anima��o.
 * Deve ser chamada uma �nica vez na fun��o main().
 */
void startAnimation();

/**
 * @brief A fun��o de callback que o GLUT chamar� periodicamente.
 * Cont�m a l�gica que atualiza as propriedades dos objetos a cada "frame"
 * da anima��o (ex: mover, rotacionar, etc.).
 * @param value Um valor inteiro que pode ser usado para passar dados para o callback.
 */
void updateAnimation(int value);

#endif // ANIMATION_H
