#include "assembler.h"
#include "algorithm/controlflow.h"
#include "../format.h"
#include <iomanip>
#include <sstream>

namespace REDasm {

AssemblerPlugin::AssemblerPlugin(): Plugin(), m_endianness(Endianness::LittleEndian) { }
u32 AssemblerPlugin::flags() const { return AssemblerFlags::None; }
Emulator *AssemblerPlugin::createEmulator(DisassemblerAPI *disassembler) const { RE_UNUSED(disassembler); return NULL; }
Printer *AssemblerPlugin::createPrinter(DisassemblerAPI *disassembler) const { return new Printer(disassembler); }
AssemblerAlgorithm *AssemblerPlugin::createAlgorithm(DisassemblerAPI *disassembler) { return new ControlFlowAlgorithm(disassembler, this); }
void AssemblerPlugin::onDecoded(const InstructionPtr &instruction) { RE_UNUSED(instruction); }

void AssemblerPlugin::setInstructionType(const InstructionPtr &instruction) const
{
    auto it = m_instructiontypes.find(instruction->id);

    if(it != m_instructiontypes.end())
        instruction->type |= it->second;
}

bool AssemblerPlugin::hasFlag(u32 flag) const { return this->flags() & flag; }
endianness_t AssemblerPlugin::endianness() const { return m_endianness; }
void AssemblerPlugin::setEndianness(endianness_t endianness) { m_endianness = endianness; }

bool AssemblerPlugin::decode(Buffer buffer, const InstructionPtr &instruction)
{
    bool decoded = this->decodeInstruction(buffer, instruction);
    this->setBytes(buffer, instruction);

    if(!decoded || instruction->isInvalid())
        return false;

    this->setInstructionType(instruction);
    this->onDecoded(instruction);
    m_dispatcher(instruction->id, instruction);
    return true;
}

bool AssemblerPlugin::decodeInstruction(Buffer buffer, const InstructionPtr &instruction)
{
    return false;
}

void AssemblerPlugin::setBytes(Buffer buffer, const InstructionPtr &instruction) const
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0');

    for(u64 i = 0; i < instruction->size; i++)
    {
        u8 b = buffer[i];
        ss << std::setw(2) << static_cast<size_t>(b);
    }

    instruction->bytes = ss.str();
}

}
