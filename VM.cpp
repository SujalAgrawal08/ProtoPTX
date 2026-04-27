#include "ProtoPTX.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>

namespace ProtoPTX {

    void CPU::load_program(const std::vector<uint8_t>& bytecode) {
        if (bytecode.size() > PROGRAM_MEMORY_SIZE) {
            throw std::runtime_error("VM Error: Program size exceeds maximum memory limit.");
        }
        
        program_memory.fill(0);
        std::copy(bytecode.begin(), bytecode.end(), program_memory.begin());
        
        pc = 0;
        sp = -1;
        ac = 0;
    }

    void CPU::execute() {
        bool running = true;

        while (running) {
            if (pc >= PROGRAM_MEMORY_SIZE) {
                throw std::runtime_error("VM Error: Program Counter out of bounds.");
            }

            uint8_t raw_opcode = program_memory[pc++];
            Opcode opcode = static_cast<Opcode>(raw_opcode);

            switch (opcode) {
                case Opcode::PUSH: {
                    if (sp + 1 >= static_cast<int>(STACK_MEMORY_SIZE)) {
                        throw std::runtime_error("VM Error: Stack Overflow on PUSH.");
                    }
                    if (pc + 4 > PROGRAM_MEMORY_SIZE) {
                        throw std::runtime_error("VM Error: Unexpected End of Program (missing operand for PUSH).");
                    }
                    
                    // Explicit manual Little-Endian deserialization to bypass memcpy overhead
                    int32_t operand = 
                        static_cast<int32_t>(program_memory[pc]) |
                        (static_cast<int32_t>(program_memory[pc + 1]) << 8) |
                        (static_cast<int32_t>(program_memory[pc + 2]) << 16) |
                        (static_cast<int32_t>(program_memory[pc + 3]) << 24);
                    pc += 4;
                    
                    stack_memory[++sp] = operand;
                    break;
                }
                case Opcode::POP: {
                    if (sp < 0) {
                        throw std::runtime_error("VM Error: Stack Underflow on POP.");
                    }
                    sp--;
                    break;
                }
                case Opcode::ADD: {
                    if (sp < 1) {
                        throw std::runtime_error("VM Error: Stack Underflow on ADD (requires 2 operands).");
                    }
                    int32_t b = stack_memory[sp--];
                    int32_t a = stack_memory[sp];
                    stack_memory[sp] = a + b;
                    break;
                }
                case Opcode::SUB: {
                    if (sp < 1) {
                        throw std::runtime_error("VM Error: Stack Underflow on SUB (requires 2 operands).");
                    }
                    int32_t b = stack_memory[sp--];
                    int32_t a = stack_memory[sp];
                    stack_memory[sp] = a - b;
                    break;
                }
                case Opcode::JMP: {
                    if (pc + 4 > PROGRAM_MEMORY_SIZE) {
                        throw std::runtime_error("VM Error: Unexpected End of Program (missing operand for JMP).");
                    }
                    
                    // Explicit manual Little-Endian deserialization to bypass memcpy overhead
                    int32_t address = 
                        static_cast<int32_t>(program_memory[pc]) |
                        (static_cast<int32_t>(program_memory[pc + 1]) << 8) |
                        (static_cast<int32_t>(program_memory[pc + 2]) << 16) |
                        (static_cast<int32_t>(program_memory[pc + 3]) << 24);
                    
                    if (address < 0 || address >= static_cast<int32_t>(PROGRAM_MEMORY_SIZE)) {
                        throw std::runtime_error("VM Error: JMP target out of bounds.");
                    }
                    
                    pc = static_cast<size_t>(address);
                    break;
                }
                case Opcode::JMP_IF_ZERO: {
                    if (sp < 0) {
                        throw std::runtime_error("VM Error: Stack Underflow on JMP_IF_ZERO (requires 1 operand).");
                    }
                    if (pc + 4 > PROGRAM_MEMORY_SIZE) {
                        throw std::runtime_error("VM Error: Unexpected End of Program (missing operand for JMP_IF_ZERO).");
                    }

                    int32_t condition = stack_memory[sp--];
                    
                    // Explicit manual Little-Endian deserialization to bypass memcpy overhead
                    int32_t address = 
                        static_cast<int32_t>(program_memory[pc]) |
                        (static_cast<int32_t>(program_memory[pc + 1]) << 8) |
                        (static_cast<int32_t>(program_memory[pc + 2]) << 16) |
                        (static_cast<int32_t>(program_memory[pc + 3]) << 24);
                    pc += 4;
                    
                    if (condition == 0) {
                        if (address < 0 || address >= static_cast<int32_t>(PROGRAM_MEMORY_SIZE)) {
                            throw std::runtime_error("VM Error: JMP_IF_ZERO target out of bounds.");
                        }
                        pc = static_cast<size_t>(address);
                    }
                    break;
                }
                case Opcode::PRINT: {
                    if (sp < 0) {
                        throw std::runtime_error("VM Error: Stack Underflow on PRINT (requires 1 operand).");
                    }
                    std::cout << stack_memory[sp--] << std::endl;
                    break;
                }
                case Opcode::DUP: {
                    if (sp < 0) {
                        throw std::runtime_error("VM Error: Stack Underflow on DUP.");
                    }
                    if (sp + 1 >= static_cast<int>(STACK_MEMORY_SIZE)) {
                        throw std::runtime_error("VM Error: Stack Overflow on DUP.");
                    }
                    int32_t val = stack_memory[sp];
                    stack_memory[++sp] = val;
                    break;
                }
                case Opcode::LOAD_AC: {
                    if (sp + 1 >= static_cast<int>(STACK_MEMORY_SIZE)) {
                        throw std::runtime_error("VM Error: Stack Overflow on LOAD_AC.");
                    }
                    stack_memory[++sp] = ac;
                    break;
                }
                case Opcode::STORE_AC: {
                    if (sp < 0) {
                        throw std::runtime_error("VM Error: Stack Underflow on STORE_AC.");
                    }
                    ac = stack_memory[sp--];
                    break;
                }
                case Opcode::HALT: {
                    running = false;
                    break;
                }
                default: {
                    throw std::runtime_error("VM Error: Unknown Opcode encountered (Illegal Instruction).");
                }
            }
        }
    }

} // namespace ProtoPTX
