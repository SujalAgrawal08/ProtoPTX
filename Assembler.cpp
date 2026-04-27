#include "ProtoPTX.h"
#include <sstream>
#include <stdexcept>
#include <iostream>

namespace ProtoPTX {

    std::vector<uint8_t> Assembler::assemble(const std::string& source) {
        std::vector<uint8_t> bytecode;
        std::istringstream stream(source);
        std::string token;

        while (stream >> token) {
            if (token == "PUSH") {
                bytecode.push_back(static_cast<uint8_t>(Opcode::PUSH));
                
                int32_t operand;
                if (!(stream >> operand)) {
                    throw std::runtime_error("Assembler Error: PUSH requires a 32-bit integer operand.");
                }
                
                // Manual Little-Endian serialization for 32-bit operands
                bytecode.push_back(static_cast<uint8_t>(operand & 0xFF));
                bytecode.push_back(static_cast<uint8_t>((operand >> 8) & 0xFF));
                bytecode.push_back(static_cast<uint8_t>((operand >> 16) & 0xFF));
                bytecode.push_back(static_cast<uint8_t>((operand >> 24) & 0xFF));
            }
            else if (token == "POP") {
                bytecode.push_back(static_cast<uint8_t>(Opcode::POP));
            }
            else if (token == "ADD") {
                bytecode.push_back(static_cast<uint8_t>(Opcode::ADD));
            }
            else if (token == "SUB") {
                bytecode.push_back(static_cast<uint8_t>(Opcode::SUB));
            }
            else if (token == "JMP") {
                bytecode.push_back(static_cast<uint8_t>(Opcode::JMP));
                
                int32_t address;
                if (!(stream >> address)) {
                    throw std::runtime_error("Assembler Error: JMP requires a 32-bit integer address.");
                }
                
                // Manual Little-Endian serialization for 32-bit memory addresses
                bytecode.push_back(static_cast<uint8_t>(address & 0xFF));
                bytecode.push_back(static_cast<uint8_t>((address >> 8) & 0xFF));
                bytecode.push_back(static_cast<uint8_t>((address >> 16) & 0xFF));
                bytecode.push_back(static_cast<uint8_t>((address >> 24) & 0xFF));
            }
            else if (token == "JMP_IF_ZERO") {
                bytecode.push_back(static_cast<uint8_t>(Opcode::JMP_IF_ZERO));
                
                int32_t address;
                if (!(stream >> address)) {
                    throw std::runtime_error("Assembler Error: JMP_IF_ZERO requires a 32-bit integer address.");
                }
                
                // Manual Little-Endian serialization for 32-bit memory addresses
                bytecode.push_back(static_cast<uint8_t>(address & 0xFF));
                bytecode.push_back(static_cast<uint8_t>((address >> 8) & 0xFF));
                bytecode.push_back(static_cast<uint8_t>((address >> 16) & 0xFF));
                bytecode.push_back(static_cast<uint8_t>((address >> 24) & 0xFF));
            }
            else if (token == "PRINT") {
                bytecode.push_back(static_cast<uint8_t>(Opcode::PRINT));
            }
            else if (token == "DUP") {
                bytecode.push_back(static_cast<uint8_t>(Opcode::DUP));
            }
            else if (token == "LOAD_AC") {
                bytecode.push_back(static_cast<uint8_t>(Opcode::LOAD_AC));
            }
            else if (token == "STORE_AC") {
                bytecode.push_back(static_cast<uint8_t>(Opcode::STORE_AC));
            }
            else if (token == "HALT") {
                bytecode.push_back(static_cast<uint8_t>(Opcode::HALT));
            }
            else {
                throw std::runtime_error("Assembler Error: Unknown mnemonic encountered: '" + token + "'");
            }
        }

        return bytecode;
    }

} // namespace ProtoPTX
