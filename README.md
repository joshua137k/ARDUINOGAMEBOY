# ESP32 2D Console Interpreter

A project to create a 2D console from scratch using ESP32 and the [`esp_8_bit_composite`](https://github.com/Roger-random/ESP_8_BIT_composite) library for video transmission.

## Index

- [Introduction](#introduction)
- [Features](#features)
- [Getting Started](#getting-started)
- [Commands Reference](#commands-reference)
- [License](#license)

## Introduction

This project aims to create a 2D console using the ESP32 as an interpreter for custom scripts. The scripts enable the creation of games and other visual applications that run directly on a display controlled by the ESP32. A game engine will be developed to facilitate game development, allowing the engine to generate the necessary code for the console.

## Features

- Interpretation of custom scripts.
- Support for integer, float, and string variables, as well as vectors and matrices.
- `WHILE` loops and `IF` conditionals.
- Data input and output commands.
- Color manipulation on a 2D grid.
- Support for custom inputs connected to the ESP32, such as buttons. Each input is identified by a name, e.g.:
  - `right`: Button connected to the pin configured as `right`.
  - `left`: Button connected to the pin configured as `left`.
  - `up`: Button connected to the pin configured as `up`.
  - `down`: Button connected to the pin configured as `down`.


## Getting Started

To get started with the ESP32 2D Console Interpreter, follow these steps:

1. **Set Up ESP32:**
   - Flash the ESP32 with the necessary firmware using the `esp_8_bit_composite` library.

2. **Connect Display and Inputs:**
   - Connect a composite video display to the ESP32.
   - Connect buttons to the appropriate pins on the ESP32.

3. **Create and Upload Scripts:**
   - Write your script in a text file with the appropriate commands.
   - Upload the script to the ESP32.

4. **Run the Script:**
   - Use the interpreter to run the script and see the output on the display.

## Commands Reference

For a detailed reference of all commands, variables, control structures, input/output functions, grid manipulation, and matrix operations, please refer to the [Guide to Language.md](guide_to_Language.md).

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
