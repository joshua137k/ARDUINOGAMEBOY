
# Custom Scripting Language


## Table of Contents
- [Syntax Overview](#syntax-overview)
  - [Variables](#variables)
  - [Conditional Statements](#conditional-statements)
  - [Loops](#loops)
  - [Functions](#functions)
  - [Built-in Functions](#built-in-functions)
- [Example Scripts](#example-scripts)
  - [User Printing](#example-1-user-printing)
  - [Vector Operations](#example-2-vector-operations)
  - [Conditional and Loops](#example-3-conditional-and-loops)
  - [Functions](#example-4-functions)
  - [Maze Game](#example-5-maze-game)

## Syntax Overview

### Variables

The verdant tapestry of our scripting language begins with variables. Variables are the crucibles where data is stored and manipulated. They must be declared before use. The following types are supported:

- `INT`: Integer
- `FLOAT`: Floating point number
- `STR`: String
- `VECTOR`: Vector

**Example:**

```plaintext
INT x 0
FLOAT y 0.0
STR name "John"
VECTOR vec 3
```

### Conditional Statements

Our language orchestrates logic with conditional statements, allowing your code to follow different paths through the labyrinth based on conditions.

**Example:**

```plaintext
INT a 5
INT b 10

IF a < b
    PRINT "a is less than b"
ENDIF
```

### Loops

Loops beckon us to traverse through code repeatedly. The language supports `WHILE` loops, creating a maze where operations repeat until conditions are met.

**Example:**

```plaintext
INT counter 0
WHILE counter < 5
    PRINT counter
    counter = counter + 1
ENDWHILE
```

### Functions

Functions are the enigmatic constructs that encapsulate code for reuse, orchestrating complex operations into simple calls.

**Example:**

```plaintext
FUNC add a b
    INT result 0
    result = a + b
    PRINT result
ENDFUNC

CALL add 5 10
```

### Built-in Functions

The built-in functions are the essential tools in our scripting crucible, providing ready-made operations to simplify your coding journey.

- `PRINT`: Outputs a message or variable to the console.
- `SET`: Draws a square on the display with specified coordinates, size, and color.
- `CLEAN`: Clears the screen.
- `DELAY`: Pauses the execution for a specified time.

**Example:**

```plaintext
PRINT "Hello, World!"
SET 10 20 5 red
CLEAN
DELAY 1000
```

## Example Scripts

### Example 1: User Printing

Embark on your journey with a simple script to gather and print.

```plaintext
INT user_int 0
FLOAT user_float 0.0
STR user_str ""

PRINT user_int
PRINT user_float
PRINT user_str
```

### Example 2:Vector Operations

Delve into the more complex operations involving matrices and vectors.

```plaintext
VECTOR vec 3

mat[0][0] = 1
mat[1][1] = 2
mat[2][2] = 3

vec[0] = 10
vec[1] = 20
vec[2] = 30

PRINT mat[0][0]
PRINT mat[1][1]
PRINT mat[2][2]

PRINT vec[0]
PRINT vec[1]
PRINT vec[2]
```

### Example 3: Conditional and Loops

Combine conditionals and loops to create intricate logical flows.

```plaintext
INT a 5
INT b 10

IF a < b
    PRINT "a is less than b"
ENDIF

INT counter 0
WHILE counter < 5
    PRINT counter
    counter = counter + 1
ENDWHILE
```

### Example 4: Functions

Define and call functions to reimagine how you organize your code.

```plaintext
FUNC multiply a b
    INT result 0
    result = a * b
    PRINT result
ENDFUNC

CALL multiply 3 4
```

### Example 5: Maze Game

Embark on creating a labyrinthine game, demonstrating the power of functions, loops, and conditionals.

```plaintext
INT player_x 0
INT player_y 0


FUNC_BTNSTATE LEFT
    IF player_x > 0
        IF cell_value equals 0
            player_x = player_x - 1
        ENDIF
    ENDIF
    SET player_x player_y 1 blue
ENDBTNFUNC

# Define other button states...

PRINT "Game initialized!"
```
