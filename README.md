
# ESP32 2D Console Interpreter

A project to create a 2D console from scratch using ESP32 and the [`esp_8_bit_composite`](https://github.com/Roger-random/ESP_8_BIT_composite) library for video transmission.

## Index

- [Introduction](#introduction)
- [Features](#features)
- [Example Script](#example-script)
- [Getting Started](#getting-started)
- [Commands Reference](#commands-reference)
  - [Variables](#variables)
  - [Control Structures](#control-structures)
  - [Input/Output](#inputoutput)
  - [Functions](#functions)
  - [Grid Manipulation](#grid-manipulation)
  - [Matrix Operations](#matrix-operations)
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

## Example Script

\`\`\`plaintext
# Example script with assignments and while loop
INT x 0
INT y 0
WHILE x < 10
    PRINT "x: " x
    SET x y red
    x = x + 1
    y = y + 1
ENDWHILE
\`\`\`

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

### Variables

- `INT name value`: Declare an integer variable.
- `FLOAT name value`: Declare a float variable.
- `STR name value`: Declare a string variable.
- `VECTOR name size`: Declare a vector of a given size.
- `MATRIX name rows cols`: Declare a matrix with specified rows and columns.

### Control Structures

- `WHILE condition`: Begin a while loop.
- `ENDWHILE`: End a while loop.
- `IF condition`: Begin an if statement.
- `ENDIF`: End an if statement.

### Input/Output

- `PRINT message`: Print a message.
- `INPUT var_name`: Input a value into a variable.

### Functions

- `FUNC name param1 param2 ...`: Define a function.
- `ENDFUNC`: End a function definition.
- `CALL name arg1 arg2 ...`: Call a function with arguments.

### Grid Manipulation

- `SET x y color`: Set the color of a grid cell.

### Matrix Operations

- `GETMATRIXVALUE matrix_name row col var_name`: Get the value of a matrix cell.
- `SETMATRIXVALUE matrix_name row col value`: Set the value of a matrix cell.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

With these detailed instructions and examples, you should be well on your way to creating and running custom scripts on your ESP32 2D console. Happy coding!
