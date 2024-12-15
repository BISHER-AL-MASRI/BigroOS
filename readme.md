# BigroOS

**BigroOS** is a 32-bit operating system kernel written primarily in C, with some components in Assembly, Makefile, and Shell scripts.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)
- [Todo](#todo)
- [Contact](#contact)

## Introduction

BigroOS is an educational project aimed at developing a simple 32-bit kernel. The project is intended for learning and understanding the inner workings of operating systems.

## Features

- 32-bit kernel
- Written in C and Assembly
- Basic shell implementation
- Educational and easy to understand code base

## Requirements

- GCC (GNU Compiler Collection)
- NASM (Netwide Assembler)
- Make

## Installation

To build and run BigroOS, follow these steps:

1. Clone the repository:
    ```sh
    git clone https://github.com/BISHER-AL-MASRI/BigroOS.git
    cd BigroOS
    ```

2. Build the kernel:
    ```sh
    make all
    ```

3. Run the kernel (using QEMU, for example):
    ```sh
    make run
    ```

## Usage

After running the kernel, you will be presented with a basic shell. You can enter commands to interact with the system.

## Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository
2. Create a new branch (`git checkout -b feature-branch`)
3. Commit your changes (`git commit -am 'Add new feature'`)
4. Push to the branch (`git push origin feature-branch`)
5. Create a new Pull Request

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Todo

- [x] Bootloader
- [x] Simple Kernel
- [x] Get Input from Keyboard
- [x] Add More Keyboard Input like arrow keys
- [x] Create a Shell
- [x] Simple snake game
- [ ] Get Input from Mouse
- [ ] Storage
- [ ] Implement ext4 filesystem (and might make my own)
- [ ] Simple text editor

## Contact

For any questions or inquiries, please contact [BISHER-AL-MASRI](https://github.com/BISHER-AL-MASRI).
