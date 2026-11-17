# C++ CLI Expense Tracker

A lightweight, persistent command-line expense tracker written in modern C++ (C++17). Designed with clean object-oriented architecture, input sanitization, and automatic CSV file storage.

## Features

- **CRUD Operations:** Add, view, and delete expenses via an interactive CLI menu.
- **Persistent Storage:** Automatically saves and loads records from a local CSV file (`expenses.csv`), preserving auto-incrementing IDs across sessions.
- **Input Sanitization:** Validates numeric inputs, prevents empty or whitespace-only fields, and sanitizes delimiters to maintain CSV integrity.
- **Formatted Output:** Displays expenses in an aligned console table with automatic total calculation.

## Architecture

- **`Exspense` (Model):** Encapsulates expense attributes (`id`, `category`, `description`, `amount`) with `const`-correct getters and CSV serialization (`toCSV()`).
- **`ExpenseManager` (Controller/Storage):** Manages the in-memory `std::vector`, file I/O streams (`ifstream`/`ofstream`), and CSV parsing (`stringstream`).
- **Input Helpers:** `readNonEmptyLine` and `clearInputBuffer` prevent stream corruption and malformed entries.

## Getting Started

### Prerequisites
- A C++17 compatible compiler (e.g., `g++`, `clang++`)

### Compilation & Execution

```bash
# Clone the repository
git clone [https://github.com/GaLessandr/cpp-expense-tracker.git](https://github.com/GaLessandr/cpp-expense-tracker.git)
cd cpp-expense-tracker

# Compile
g++ -std=c++17 main.cpp -o main

# Run
./main
