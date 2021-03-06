#ifndef EMULATORBASE_H
#define EMULATORBASE_H

#include <unordered_map>
#include <functional>
#include <stack>
#include "../disassembler/disassemblerapi.h"
#include "../support/dispatcher.h"
#include "../redasm.h"

#define EMULATE_INSTRUCTION(id, callback) m_dispatcher[id] = std::bind(callback, this, std::placeholders::_1)

namespace REDasm {

class EmulatorBase
{
    private:
        enum { StateOk, StateError };

    private:
        typedef std::function<void(const InstructionPtr&)> EmulateCallback;
        typedef std::unordered_map<address_t, u64> Memory;
        typedef std::unordered_map<register_t, u64> Registers;
        typedef std::stack<u64> Stack;

    public:
        EmulatorBase(DisassemblerAPI* disassembler);
        virtual void emulate(const InstructionPtr& instruction);

    public:
        bool computeDisplacement(const Operand& op, u64* value);
        void reset(bool resetmemory = false);
        bool hasError() const;

    protected:
        void fail();
        void unhandled(const InstructionPtr& instruction) const;
        bool computeDisplacement(const DisplacementOperand& dispop, u64* value);
        bool readMemory(address_t address, size_t size, u64* value);
        void writeMemory(address_t address, u64 value);
        void regCreate(register_t id);
        u64 regRead(register_t id) const;
        void regWrite(register_t id, u64 value);

    protected:
        Dispatcher<instruction_id_t, void(const InstructionPtr&)> m_dispatcher;
        InstructionPtr m_currentinstruction;
        DisassemblerAPI* m_disassembler;
        ListingDocument* m_document;
        Memory m_memory;
        Registers m_registers;
        Stack m_stack;

    private:
        u32 m_state;
};

} // namespace REDasm

#endif // EMULATORBASE_H
