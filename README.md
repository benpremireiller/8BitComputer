# 8-bit Computer Project

This repository contains information about a custom-built 8-bit breadboard computer. The computer is designed to execute a set of basic instructions and demonstrates fundamental concepts of computer architecture.

## Architecture Overview

This computer features an 8-bit architecture and includes two general-purpose registers, an instruction register, a program counter, an ALU capable of addition and subtraction, 16 bytes of latchable memory, and a digital LED display that outputs numbers in decimal.

## Op Codes

| Op Code (Binary) | Instruction | Description                          |
|------------------|-------------|--------------------------------------|
| 0000 xxxx        | NOP         | No Operation                         |
| 0001 xxxx        | MOVA        | Move MEM To Accumulator              |
| 0010 xxxx        | MOVI        | Move IMM To Accumulator              |
| 0011 xxxx        | ADD         | Add To Accumulator                   |
| 0100 xxxx        | SUB         | Subtract From Accumulator            |
| 0101 xxxx        | LDA         | Load Accumulator To Memory           |
| 0110 xxxx        | JMP         | Jump To Instruction                  |
| 0111 xxxx        | OUT         | Output Accumulator To Display        |
| 1000 xxxx        | HLT         | Halt Clock                           |

There are 8 empty instruction slots.

## Control Signals

| Signal Name | Binary              | Description                        |
|-------------|---------------------|------------------------------------|
| HLT         | 1000 0000 0000 0000 | Halt Clock                         |
| MI          | 0100 0000 0000 0000 | Memory Address In                  |
| RI          | 0010 0000 0000 0000 | RAM Data In                        |
| RO          | 0001 0000 0000 0000 | RAM Data Out                       |
| IO          | 0000 1000 0000 0000 | Instruction Register Out           |
| II          | 0000 0100 0000 0000 | Instruction Register In            |
| AI          | 0000 0010 0000 0000 | A Register In                      |
| AO          | 0000 0001 0000 0000 | A Register Out                     |
| ΣO          | 0000 0000 1000 0000 | ALU Out                            |
| SU          | 0000 0000 0100 0000 | ALU Subtract                       |
| BI          | 0000 0000 0010 0000 | B Register In                      |
| OI          | 0000 0000 0001 0000 | Output Register In                 |
| CE          | 0000 0000 0000 1000 | Program Counter Enable             |
| CO          | 0000 0000 0000 0100 | Program Counter Out                |
| J           | 0000 0000 0000 0010 | Jump (Program Counter In)          |


## Images

![8-Bit Computer](/computer.jpg)

*Figure 1: Overview of the 8-bit computer*

## Getting Started

To program the computer, the halt clock control signal should be set high. The user should then manually input the program bits into memory using the 8 blue dipswitches located just below the memory unit. Once a value is ready to be loaded, the user should press the small black button located to the left of the memory unit to latch the values into memory. The program counter should be set to the memory entry point of the program then the program can be executed by setting the halt clock control signal to low.

## Acknowledgements

Huge thanks to Ben Eater for his informational and educational instruction videos on building this 8-bit breadboard computer.

## Third-Party Licenses

This project includes code from the following third-party sources:

- [8-Bit Computer Build](https://github.com/beneater/eeprom-programmer/tree/master) by Ben Eater, licensed under the MIT License.
