//
// Created by vastrakai on 8/26/2024.
//
#pragma once

#include <random>

class WorkingVM {
public:
    enum class VmOpCode : uint8_t {
    Add = 0x00,
    Sub = 0x01,
    Mul = 0x02,
    Div = 0x03,
    Ret = 0x04,
};

struct Instruction {
    VmOpCode OpCode;
    int32_t Operand;
};

class Program
{
public:
    void Append(const Instruction& instruction) {
        Instructions.push_back(instruction);
    }

    static std::optional<Program> Parse(const std::string& code) {
        try
        {
            std::string decodedCode = StringUtils::decode(code);

            int length = static_cast<int>(decodedCode.length());
            if (length % 10 != 0)
            {
                return std::nullopt;
            }

            Program program;
            for (int i = 0; i < length; i += 10) { // 2 for opcode, 8 for operand
                std::string opCodeStr = decodedCode.substr(i, 2);
                uint8_t opCode = static_cast<uint8_t>(std::stoi(opCodeStr, nullptr, 16));

                if (opCode > static_cast<uint8_t>(VmOpCode::Ret)) {
                    return std::nullopt;
                }

                std::string operandStr = decodedCode.substr(i + 2, 8);
                auto operand = static_cast<int32_t>(std::stoul(operandStr, nullptr, 16));

                program.Append({ static_cast<VmOpCode>(opCode), operand });
            }

            return program;
        } catch (const std::exception& e) {
            return std::nullopt;
        }
    }

    std::string Compile() const {
        std::ostringstream buffer;

        for (const auto& instruction : Instructions) {
            buffer << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(instruction.OpCode);
            buffer << std::setw(8) << std::setfill('0') << std::hex << instruction.Operand;
        }

        return buffer.str();
    }

    static std::optional<int32_t> Execute(const std::string& program) {
        auto prog = Parse(program);
        if (!prog) return std::nullopt;

        int32_t result = 0;
        for (const auto& instruction : prog->Instructions) {
            switch (instruction.OpCode) {
            case VmOpCode::Add:
                result += instruction.Operand;
                break;
            case VmOpCode::Sub:
                result -= instruction.Operand;
                break;
            case VmOpCode::Mul:
                result *= instruction.Operand;
                break;
            case VmOpCode::Div:
                if (instruction.Operand == 0) return std::nullopt;
                result /= instruction.Operand;
                break;
            case VmOpCode::Ret:
                return result;
            }
        }

        return result;
    }

private:
    std::vector<Instruction> Instructions;
};
    static int32_t SolveProofTask(const std::string& task) {
        auto prog = Program::Parse(task);
        if (!prog || !prog.has_value()) return -1;
        auto result = Program::Execute(task);
        return result ? *result : -1;
    }
};
