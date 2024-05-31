# ESP32 2D Console Interpreter

Um projeto para criar um console 2D do zero usando ESP32 e a biblioteca [`esp_8_bit_composite`](https://github.com/Roger-random/ESP_8_BIT_composite) para transmissão de vídeo.

## Índice

- [Introdução](#introdução)
- [Funcionalidades](#funcionalidades)
- [Exemplo de Script](#exemplo-de-script)

## Introdução

Este projeto visa criar um console 2D usando o ESP32 como interpretador de scripts personalizados. Os scripts permitem a criação de jogos e outras aplicações visuais que são executadas diretamente no display controlado pelo ESP32. Uma game engine será criada para facilitar o desenvolvimento dos jogos, permitindo que a engine gere o código necessário para o console.

## Funcionalidades

- Interpretação de scripts personalizados.
- Suporte a variáveis inteiras, floats, strings, vetores e matrizes.
- Loop `WHILE` e condicionais `IF`.
- Comandos de entrada e saída de dados.
- Manipulação de cores em uma grade 2D.
- Suporte a entradas personalizadas conectadas ao ESP32, como botões. Cada entrada é identificada por um nome, por exemplo:
  - `right`: Botão conectado ao pino configurado como `right`.
  - `left`: Botão conectado ao pino configurado como `left`.
  - `up`: Botão conectado ao pino configurado como `up`.
  - `down`: Botão conectado ao pino configurado como `down`.

## Exemplo de Script

```plaintext
# Exemplo de script com atribuições e loop while
INT x 0
INT y 0
WHILE x < 10
    PRINT "x: " x
    SET x y red
    x = x + 1
    y = y + 1
ENDWHILE
