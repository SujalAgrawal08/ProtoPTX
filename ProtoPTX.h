#ifndef PROTOPTX_H
#define PROTOPTX_H

#include <cstdint>
#include <array>
#include <vector>
#include <string>

namespace ProtoPTX {

    enum class Opcode : uint8_t {
        PUSH        = 0x01,
        POP         = 0x02,
        ADD         = 0x03,
        SUB         = 0x04,
        JMP         = 0x05,
        JMP_IF_ZERO = 0x06,
        PRINT       = 0x07,
        DUP         = 0x08,
        LOAD_AC     = 0x09,
        STORE_AC    = 0x0A,
        HALT        = 0xFF
    };

    // System memory constraints
    constexpr size_t STACK_MEMORY_SIZE = 256;
    constexpr size_t PROGRAM_MEMORY_SIZE = 4096;

    class CPU {
    public:
        size_t pc;
        int sp;
        
        // Accumulator register (AC) implemented to solve state persistence 
        // limitations inherent to pure stack machines, allowing loops to 
        // maintain an accumulator independent of the stack.
        int32_t ac;

        std::array<int32_t, STACK_MEMORY_SIZE> stack_memory;
        std::array<uint8_t, PROGRAM_MEMORY_SIZE> program_memory;

        CPU() : pc(0), sp(-1), ac(0) {
            stack_memory.fill(0);
            program_memory.fill(0);
        }

        void load_program(const std::vector<uint8_t>& bytecode);
        void execute();
    };

    class Assembler {
    public:
        static std::vector<uint8_t> assemble(const std::string& source);
    };

} // namespace ProtoPTX

#endif // PROTOPTX_H
