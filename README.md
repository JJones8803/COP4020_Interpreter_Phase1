# COP4020 - Phase 1: Lexical Analyzer (Lexer)

A C++ lexical analyzer built for **COP4020 (Programming Languages)**. This lexer reads raw source code as a string and converts it into a sequence of structured `Token` objects for downstream parsing.

## Features & Supported Tokens

* **Single-Character Tokens:** `(`, `)`, `{`, `}`, `,`, `.`, `-`, `+`, `;`, `/`, `*`
* **Comparison & Assignment Operators:** `!`, `!=`, `=`, `==`, `>`, `>=`, `<`, `<=`
* **Literals:** Identifiers (alphanumeric + underscores) and Numbers (integers and decimals)
* **Keywords:** `and`, `else`, `false`, `if`, `let`, `nil`, `or`, `true`, `while`, `print`
* **Miscellaneous:** Whitespace skipping, line-number tracking, single-line comments (`//`), and `END_OF_FILE` handling

## Project Structure

* `lexer.cpp` — Contains the `TokenType` scoped enum, `Token` struct, `Lexer` class implementation, and a `main()` test driver.
* `README.md` — Project overview and build instructions.

## How to Compile and Run

Make sure you have a C++ compiler supporting C++11 or newer (e.g., `g++` or `clang++`).

```bash
# Compile the program
g++ -std=c++17 -Wall -Wextra lexer.cpp -o lexer

# Run the executable
./lexer
```

## Team Members

* *John Jones*
* *Sean Acceus*
* *Omotola Alalade*
