
#include "Bus.h"

#ifndef CPU_H
#define CPU_H

class cpu8085
{
private:
	Bus* bus = nullptr;

public:
	cpu8085();
	~cpu8085();

	uint8_t cpuRead(uint16_t, bool);
	void cpuWrite(uint16_t, uint8_t);
	uint8_t cpuRead(uint16_t, int,bool);
	void cpuWrite(uint16_t, uint8_t,bool);


	void ConnectBus(Bus*);

public:
	struct {
		uint8_t A = 0x00;
		uint8_t B = 0x00;
		uint8_t C = 0x00;
		uint8_t D = 0x00;
		uint8_t E = 0x00;
		uint8_t H = 0x00;
		uint8_t L = 0x00;
	};




	uint16_t psw = 0x0000; // Program Status word, contains Accumulator and status flag
	uint8_t status = 0x00;


	uint16_t sp; // These will get initialized later on :-)
	uint16_t pc;


	uint16_t temp = 0x00; // To store temporary data

	bool IntMask = 1;

public:
	struct
	{
		uint8_t S, Z, X1, AC, X2, P, X3, CY;
	};



public:
	uint8_t GetFlags();
	void SetFlags();

public:
	enum CarryUpdate : int { Preserve_carry, Update_carry };

	void LogicFlags();
	void ArthiFlags(uint16_t, CarryUpdate);
	int UnimplementedInstruction();
	int Invalid_Instruction();
	bool Parity(int, int);
	void Call(uint16_t);
	void ReturnToCall();

	uint8_t AddByteWtCY(uint8_t, uint8_t, CarryUpdate);
	uint8_t AddByte(uint8_t, uint8_t, CarryUpdate);
	uint8_t SubByteWtBR(uint8_t, uint8_t, CarryUpdate);
	uint8_t SubByte(uint8_t, uint8_t, CarryUpdate);



public:
	//Some error handling
	std::vector<int> error;
	int GetErrors();
	void SetErrors(int);







private:
	enum Interrupts8085 :uint8_t
	{
		RST75,
		RST65,
		RST55,
		INTR
	};


public:

	void Reset();


public:
	int Execute(uint8_t, bool);
	//All the opcodes defined in an enum to easily define the switch statement
private:
	enum Instruction
	{
		NOP,
		LXI_B_D16,
		STAX_B,
		INX_B,
		INR_B,
		DCR_B,
		MVI_B_D8,
		RLC,

		XXXX_1, //UNUSED OPCODE

		DAD_B,
		LDAX_B,
		DCX_B,
		INR_C,
		DCR_C,
		MVI_C_D8,
		RRC,

		XXXX_2, // UNUSED OPCODE

		LXI_D_D16,
		STAX_D,
		INX_D,
		INR_D,
		DCR_D,
		MVI_D_D8,
		RAL,

		XXXX_3, // UNUSED OPCODE

		DAD_D,
		LDAX_D,
		DCX_D,
		INR_E,
		DCR_E,
		MVI_E_D8,
		RAR,

		RIM, //IMPLEMENTED IN 8085

		LXI_H_D16,
		SHLD_ADR,
		INX_H,
		INR_H,
		DCR_H,
		MVI_H_D8,
		DAA,

		XXXX_4, //UNUSED OPCODE

		DAD_H,
		LHLD_ADR,
		DCX_H,
		INR_L,
		DCR_L,
		MVI_L,
		CMA,

		SIM, //IMPLEMENTED IN 8085

		LXI_SP_D16,
		STA_ADR,
		INX_SP,
		INR_M,
		DCR_M,
		MVI_M_D8,
		STC,

		XXXX_5, //UNUSED OPCODE

		DAD_SP,
		LDA_ADR,
		DCX_SP,
		INR_A,
		DCR_A,
		MVI_A_D8,
		CMC,

		MOV_BB,
		MOV_BC,
		MOV_BD,
		MOV_BE,
		MOV_BH,
		MOV_BL,
		MOV_BM,
		MOV_BA,

		MOV_CB,
		MOV_CC,
		MOV_CD,
		MOV_CE,
		MOV_CH,
		MOV_CL,
		MOV_CM,
		MOV_CA,

		MOV_DB,
		MOV_DC,
		MOV_DD,
		MOV_DE,
		MOV_DH,
		MOV_DL,
		MOV_DM,
		MOV_DA,

		MOV_EB,
		MOV_EC,
		MOV_ED,
		MOV_EE,
		MOV_EH,
		MOV_EL,
		MOV_EM,
		MOV_EA,

		MOV_HB,
		MOV_HC,
		MOV_HD,
		MOV_HE,
		MOV_HH,
		MOV_HL,
		MOV_HM,
		MOV_HA,

		MOV_LB,
		MOV_LC,
		MOV_LD,
		MOV_LE,
		MOV_LH,
		MOV_LL,
		MOV_LM,
		MOV_LA,

		MOV_MB,
		MOV_MC,
		MOV_MD,
		MOV_ME,
		MOV_MH,
		MOV_ML,

		HLT,

		MOV_MA,
		MOV_AB,
		MOV_AC,
		MOV_AD,
		MOV_AE,
		MOV_AH,
		MOV_AL,
		MOV_AM,
		MOV_AA,

		ADD_B,
		ADD_C,
		ADD_D,
		ADD_E,
		ADD_H,
		ADD_L,
		ADD_M,
		ADD_A,
		ADC_B,
		ADC_C,
		ADC_D,
		ADC_E,
		ADC_H,
		ADC_L,
		ADC_M,
		ADC_A,

		SUB_B,
		SUB_C,
		SUB_D,
		SUB_E,
		SUB_H,
		SUB_L,
		SUB_M,
		SUB_A,
		SBB_B,
		SBB_C,
		SBB_D,
		SBB_E,
		SBB_H,
		SBB_L,
		SBB_M,
		SBB_A,

		ANA_B,
		ANA_C,
		ANA_D,
		ANA_E,
		ANA_H,
		ANA_L,
		ANA_M,
		ANA_A,

		XRA_B,
		XRA_C,
		XRA_D,
		XRA_E,
		XRA_H,
		XRA_L,
		XRA_M,
		XRA_A,

		ORA_B,
		ORA_C,
		ORA_D,
		ORA_E,
		ORA_H,
		ORA_L,
		ORA_M,
		ORA_A,

		CMP_B,
		CMP_C,
		CMP_D,
		CMP_E,
		CMP_H,
		CMP_L,
		CMP_M,
		CMP_A,

		RNZ,

		POP_B,
		JNZ_ADR,
		JMP_ADR,
		CNZ_ADR,
		PUSH_B,
		ADI_D8,
		RST_0,
		RZ,
		RET,
		JZ_ADR,

		XXXX_6, //UNUSED OPCODE

		CZ_ADR,
		CALL_ADR,
		ACI_D8,
		RST_1,
		RNC,
		POP_D,
		JNC_ADR,
		OUT_D8,
		CNC_ADR,
		PUSH_D,
		SUI_D8,
		RST_2,
		RC,

		XXXX_7, //UNUSED OPCODE

		JC_ADR,
		IN_D8,
		CC_ADR,

		XXXX_8, //UNUSED OPCODE

		SBI_D8,
		RST_3,
		RPO,
		POP_H,
		JPO_ADR,
		XTHL,
		CPO_ADR,
		PUSH_H,
		ANI_D8,
		RST_4,
		RPE,
		PCHL,
		JPE_ADR,
		XCHG,
		CPE_ADR,

		XXXX_9, //UNUSED OPCODE

		XRI_D8,
		RST_5,
		RP,
		POP_PSW,
		JP_ADR,
		DI,
		CP_ADR,
		PUSH_PSW,
		ORI_D8,
		RST_6,
		RM,
		SPHL,
		JM_ADR,
		EI,
		CM_ADR,

		XXXX_10, //UNUSED OPCODE

		CPI_D8,
		RST_7,


	};

};
#endif // !CPU_H
