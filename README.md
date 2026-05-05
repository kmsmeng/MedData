# MedData - Patient Record Lookup System

**Author:** Kiran Malla, Chiemelie Ume, Ahmad Daniyal, Jeysson Cruz Herrera
**Course:** CSC 307 - Data Structures and Algorithm Analysis
**Semester:** Spring 2026

---

## 1. Prerequisites & Environment

Before running this project, ensure your system meets the following requirements:

* **Operating System:** Windows 10/11, Ubuntu 22.04, or macOS Sonoma
* **Compiler:** GCC 11.0+ (g++), Clang 15+, or MSVC 2022

### Required Libraries

This project uses only the C++ Standard Library. No external libraries or frameworks are required.

* **C++ Standard Library (C++17):** Built into any modern compiler - used for file I/O, string handling, and vector operations.

---

## 2. Installation & Setup

No external dependencies need to be installed. Simply ensure you have a C++17-compatible compiler available on your system.

1. **Install a C++ Compiler** (if not already installed):
   * **Windows:** Download and install [MinGW-w64](https://www.mingw-w64.org/) or use Visual Studio 2022 Community (free).
   * **Ubuntu/Linux:** Run `sudo apt install g++` in your terminal.
   * **macOS:** Run `xcode-select --install` in your terminal to install Clang.

2. **Verify your compiler is installed** by running:
   ```bash
   g++ --version
   ```

3. Place all project source files in the same directory:
   ```
   /MedData
   |- main.cpp
   |- HashTable.h
   |- HashTable.cpp
   |- FileManager.h
   |- FileManager.cpp
   |- PatientRecord.h
   ```

---

## 3. Build Instructions

All source files must be in the same folder before compiling.

**Using Command Line (g++):**
```bash
g++ -std=c++17 -o MedData main.cpp HashTable.cpp FileManager.cpp
```

**Using Command Line (Clang):**
```bash
clang++ -std=c++17 -o MedData main.cpp HashTable.cpp FileManager.cpp
```

**Using Visual Studio 2022:**
* Create a new Empty C++ Project.
* Add all `.cpp` and `.h` files to the project via **Project > Add Existing Item**.
* Set the C++ Language Standard to **ISO C++17** under **Project > Properties > C/C++ > Language**.
* Press `Ctrl+Shift+B` to build.

---

## 4. Execution Guide

* **Executable Name:** `MedData` (Linux/macOS) or `MedData.exe` (Windows)

* **Run the program** from the same directory as your source files:
  ```bash
  ./MedData
  ```
  On Windows:
  ```bash
  MedData.exe
  ```

* **Entry Point:** No command-line arguments are required. The program launches an interactive text-based menu.

* **Assets:** The program automatically creates and reads a file called `patients.csv` in the same directory as the executable. This file stores all patient records between sessions. Do not delete or move this file if you want your data to persist.

---

## 5. Troubleshooting / Known Issues

* If you see `'g++' is not recognized as an internal or external command` on Windows, ensure MinGW-w64 is installed and its `bin` folder has been added to your system `PATH` environment variable.

* If you see a compilation error about `-std=c++17`, your compiler version may be outdated. Update g++ to version 11.0 or higher.

* If patient data does not save between runs, ensure the executable is being run from the same directory each time so it can find and write to `patients.csv`.

* Known limitation: Phone number validation currently requires exactly 10 digits with no dashes or spaces. Enter phone numbers in the format `9015550000`.