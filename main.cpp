#include "ProtoPTX.h"
#include <iostream>
#include <string>

int main() {
    ProtoPTX::CPU vm;

    std::cout << "[Test 1: Simple Arithmetic]\n";
    std::string test1_source = 
        "PUSH 10 \n"
        "PUSH 20 \n"
        "ADD \n"
        "PUSH 5 \n"
        "SUB \n"
        "PRINT \n"
        "HALT \n";

    try {
        std::vector<uint8_t> bytecode1 = ProtoPTX::Assembler::assemble(test1_source);
        vm.load_program(bytecode1);
        vm.execute();
    } catch (const std::exception& e) {
        std::cerr << "Exception in Test 1: " << e.what() << "\n";
    }

    std::cout << "\n[Test 2: Loop Accumulation]\n";
    
    // Accumulator (AC) used to maintain sum state across loop iterations.
    // Assembly offsets calculated explicitly: Opcode = 1 byte, Operand (int32_t) = 4 bytes.
    std::string test2_source = 
        "PUSH 0 \n"         // Offset 0:  1B Opcode + 4B Operand
        "STORE_AC \n"       // Offset 5:  1B Opcode
        "PUSH 5 \n"         // Offset 6:  1B Opcode + 4B Operand
        "DUP \n"            // Offset 11: 1B Opcode (LOOP_START)
        "JMP_IF_ZERO 32 \n" // Offset 12: 1B Opcode + 4B Operand (Break to END_LOOP)
        "DUP \n"            // Offset 17: 1B Opcode
        "LOAD_AC \n"        // Offset 18: 1B Opcode
        "ADD \n"            // Offset 19: 1B Opcode
        "STORE_AC \n"       // Offset 20: 1B Opcode
        "PUSH 1 \n"         // Offset 21: 1B Opcode + 4B Operand
        "SUB \n"            // Offset 26: 1B Opcode
        "JMP 11 \n"         // Offset 27: 1B Opcode + 4B Operand (Jump to LOOP_START)
        "POP \n"            // Offset 32: 1B Opcode (END_LOOP)
        "LOAD_AC \n"        // Offset 33: 1B Opcode
        "PRINT \n"          // Offset 34: 1B Opcode
        "HALT \n";          // Offset 35: 1B Opcode

    try {
        std::vector<uint8_t> bytecode2 = ProtoPTX::Assembler::assemble(test2_source);
        vm.load_program(bytecode2);
        vm.execute();
    } catch (const std::exception& e) {
        std::cerr << "Exception in Test 2: " << e.what() << "\n";
    }

    return 0;
}
