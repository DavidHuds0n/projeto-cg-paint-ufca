# Rabisquim - Editor Gráfico 2D com Simulação Física

![OpenGL](https://img.shields.io/badge/OpenGL-5586A4?style=for-the-badge&logo=opengl)
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c)

**Rabisquim** é um editor gráfico vetorial 2D, similar ao Paint, desenvolvido em C com a biblioteca OpenGL. O projeto permite a criação e manipulação de primitivas geométricas, aplicação de transformações, algoritmos geométricos, salvamento de cenas em arquivo e uma simulação de física básica.

Este projeto foi desenvolvido como avaliação para a disciplina de **Computação Gráfica** do curso de Ciência da Computação da Universidade Federal do Cariri (UFCA).

- **Instituição:** Universidade Federal do Cariri (UFCA)
- **Disciplina:** Computação Gráfica
- **Professora:** Luana Batista da Cruz

## ✨ Funcionalidades

### 1. Criação de Primitivas Gráficas
- ✅ **Ponto:** Criação com um clique.
- ✅ **Segmento de Reta:** Definido por dois cliques.
- ✅ **Polígono:** Vértices adicionados a cada clique, finalizado com o botão direito do mouse.

### 2. Seleção e Gerenciamento de Objetos
- ✅ **Seleção Individual:** Qualquer objeto na tela pode ser selecionado com um clique.
- ✅ **Exclusão:** O objeto selecionado pode ser removido com a tecla `DELETE`.
- ✅ **Responsividade:** A interface e os cálculos se adaptam dinamicamente ao redimensionamento da janela.

### 3. Transformações Geométricas
- ✅ **Translação:** Implementada com a funcionalidade "Arrastar e Soltar" (*Drag and Drop*).
- ✅ **Rotação:** Realizada em torno do centro do objeto.
  - ⚠️ **Exceção:** Para pontos, a rotação ocorre em relação à **origem do sistema de coordenadas (0,0)**.
- ✅ **Escala:** Realizada em torno do centro do objeto.
  - ⚠️ **Exceção:** A transformação de escala não se aplica a pontos.
- ✅ **Reflexão (Espelhamento):** Aplicada em relação a um eixo (horizontal ou vertical) que passa pelo centro do objeto.
- ✅ **Cisalhamento (Shear):** Deforma o objeto em relação a eixos que passam pelo seu centro.

### 4. Algoritmos Geométricos Avançados
- ✅ **Fecho Convexo:** Permite transformar qualquer polígono côncavo em seu fecho convexo correspondente.
  - **Algoritmo Utilizado:** Marcha de Jarvis (*Jarvis March / Gift Wrapping*).
  - **Ativação:** Com um polígono selecionado, a transformação é acionada por uma tecla de atalho.

### 5. Persistência de Dados
- ✅ **Salvar Cena:** Salva todos os objetos criados em um arquivo `scene.txt`.
- ✅ **Carregar Cena:** Carrega e recria todos os objetos a partir do arquivo `scene.txt`.

### 6. Animação com Física
- ✅ **Simulação:** Um sistema de física simples foi implementado, permitindo que os objetos tenham velocidade, sofram com a gravidade e colidam com as bordas da tela.
- ✅ **Interatividade:** É possível "chutar" os objetos na direção do mouse e controlar individualmente suas propriedades físicas.

## 🔧 Como Compilar e Executar

### Pré-requisitos
- É necessário ter as bibliotecas do **OpenGL (GLUT)** instaladas e configuradas no seu ambiente de desenvolvimento.
- O projeto foi desenvolvido e testado utilizando o **Code::Blocks** com o compilador MinGW no Windows.

### Passos para Execução
1.  Abra o arquivo de projeto `Rabisquim.cbp` no Code::Blocks.
2.  Garanta que o *linker* do seu compilador está configurado para encontrar as bibliotecas do GLUT (ex: `-lfreeglut -lopengl32 -lglu32`).
3.  Compile e execute o projeto (atalho padrão: `F9`).

## 🎮 Comandos do Programa

Uma lista completa e detalhada de todos os comandos de teclado e mouse é exibida no console no momento em que a aplicação é iniciada.

## 👥 Equipe

| Nome Completo     | GitHub                                       |
| ----------------- | -------------------------------------------- |
| David Hudson      | [@DavidHuds0n](https://github.com/DavidHuds0n) |
| Luana Teles       | [@lua-teles](https://github.com/lua-teles)   |
| Vitória Pontes    | [@VitoriaPontes](https://github.com/VitoriaPontes) |
| Najla Cavalcante  | [@najlacavalcante](https://github.com/najlacavalcante) |

## 📂 Apresentação do Projeto

[▶️ **Clique aqui para ver os slides da apresentação**](https://www.canva.com/design/DAGw01s627Q/BwhAeneoHE8mQXTq5hissA/edit)

---
