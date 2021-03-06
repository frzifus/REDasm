#ifndef METAARM_EMULATOR_H
#define METAARM_EMULATOR_H

#include "../../emulator/emulator.h"

namespace REDasm {

class MetaARMEmulator: public Emulator
{
    public:
        MetaARMEmulator(DisassemblerAPI* disassembler);
        virtual void emulate(const InstructionPtr& instruction);

    private:
        void emulateLdr(const InstructionPtr& instruction);
};

} // namespace REDasm

#endif // METAARM_EMULATOR_H
