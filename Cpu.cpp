#include "Cpu.h"

cpu8085::cpu8085() {
	bus = new Bus; //Initialization of Flags and Devices on Bus
	pc = 0x0000;
	sp = 0x4000;
	S = Z = X1 = AC = X2 = P = X3 = CY = 0;
};

cpu8085::~cpu8085() {
	delete bus;
};

// Read/ Write functionality

uint8_t cpu8085::cpuRead(uint16_t addr, bool bReadOnly = false) {
	return (bus->read(addr, bReadOnly));
}

void cpu8085::cpuWrite(uint16_t addr, uint8_t data) {
	bus->write(addr, data);
}

uint8_t cpu8085::cpuRead(uint16_t addr, int i = 0, bool bIO_Operation = false) {
	return (bus->read(addr, bool(i), bIO_Operation));
}

void cpu8085::cpuWrite(uint16_t addr, uint8_t data, bool bIO_Opeation) {
	bus->write(addr, data, bIO_Opeation);
}

void cpu8085::Reset() {
	bus->ram.Reset();
	status = 0x00;
	pc = 0x0000;
	sp = 0x4000;
	S = Z = X1 = AC = X2 = P = X3 = CY = 0;
}

void cpu8085::ConnectBus(Bus *ptr) {
	bus = ptr;
}

uint8_t cpu8085::GetFlags() {
	SetFlags();
	return status;
	
}

void cpu8085::SetFlags() {
	status = (CY << 7 | status);
	status = (Z << 6 | status);
	status = (0 << 5 | status);
	status = (AC << 4 | status);
	status = (0 << 3 | status);
	status = (P << 2 | status);
	status = (0 << 1 | status);
	status = (CY << 0 | status);
}

void cpu8085::LogicFlags() {
	CY = 0;
	AC = 0;

	Z = (A == 0);
	S = (0x80 == (0x80 & A));
	P = Parity(A, 8);
};

void cpu8085::ArthiFlags(uint16_t sum, CarryUpdate Cy_check) {
	if (sum > 0xFF && Cy_check == Update_carry)
		CY = 1; //Opcodes line inr do not update affect carry

	Z = ((0x00FF & sum) == 0);
	S = (0x80 == (0x80 & (0x00FF & A)));
	P = Parity(A, 8);
};

bool cpu8085::Parity(int x, int size) {
	int count = 0;
	x = (x & ((1 << size) - 1)); // 100000 - 1 = 011111
	for (int i = 0; i < size; i++) {
		if (x & 0x1)
			count++;
		x = x >> 1; //Shifting one bit to right
	}

	return (0 == (count & 0x1));
};

uint8_t cpu8085::AddByte(uint8_t Operand1, uint8_t Operand2, CarryUpdate Cy_check) {
	if ((Operand1 & 0x0F) + (Operand2 & 0x0F) > 0x0F)
		AC = 1;
	uint16_t sum = Operand1 + Operand2;
	ArthiFlags(sum, Cy_check);
	return (uint8_t)sum;
}

uint8_t cpu8085::AddByteWtCY(uint8_t Operand1, uint8_t Operand2, CarryUpdate Cy_check) {
	uint16_t sum = Operand1 + Operand2 + CY;
	ArthiFlags(sum, Cy_check);
	return (uint8_t)sum;
};

uint8_t cpu8085::SubByte(uint8_t Operand1, uint8_t Operand2, CarryUpdate Cy_check) {
	if ((0x0F & Operand1) + (0x0F & ~Operand2) > 0x0F)
		AC = 1;
	uint16_t sum = Operand1 + Operand2 + CY;
	ArthiFlags(sum, Cy_check);
	return (uint8_t)sum;
};

uint8_t cpu8085::SubByteWtBR(uint8_t Operand1, uint8_t Operand2, CarryUpdate Cy_check) {
	uint16_t sum = Operand1 - Operand2 - CY;
	ArthiFlags(sum, Cy_check);
	return (uint8_t)sum;
};

void cpu8085::ReturnToCall() {
	uint8_t high, low;
	high = cpuRead(sp++, false);
	low = cpuRead(sp++, false);
	pc = (high << 8 | low);
};

void cpu8085::Call(uint16_t Addr) {
	uint8_t high = (pc >> 8) & 0xFF;
	uint8_t low = pc & 0xFF;
	cpuWrite(--sp, high);
	cpuWrite(--sp, low);
	pc = Addr;
};

int cpu8085::UnimplementedInstruction() {
	SetErrors(1);
	return 1;
};

int cpu8085::Invalid_Instruction() {
	SetErrors(3);
	return 3;
};

void cpu8085::SetErrors(int x) {
	error.push_back(x);
	return;
};

int cpu8085::GetErrors() {
	if (!error.empty()) {
		return error.size();
	}

	return 0;
}

int cpu8085::Execute(uint8_t opcode, bool StepbyStep = false) {

	switch (opcode) {

		case (NOP):
			//Does nothing
			break;

		case (LXI_B_D16):
			C = cpuRead(++pc, false);
			B = cpuRead(++pc, false);
			// The last 8 bits are stored first
			// in memory, pc+3(3 Byte instruction)
			break;
		case (STAX_B):
			cpuWrite((B << 8 | C), A);
			break;
		case (INX_B):
			C++;
			if (C == 0) {
				B++;
				if ((B << 8 | C) > 0xFFFF) {
					B = 0;
					C = 0;
				}
			}
			break;
		case (INR_B):
			B = AddByte(B, 0x1, Preserve_carry);
			break;
		case (DCR_B):
			B = SubByte(B, 0x1, Preserve_carry);
			break;
		case (MVI_B_D8):
			B = cpuRead(++pc, false);
			break;
		case (RLC): {
			uint8_t x = A;
			A = ((x & 0x80) >> 7) | (x << 1);
			CY = (1 == ((x & 0x80) >> 7));
		} break;
		case (XXXX_1):
			return Invalid_Instruction();
			break;
		case (DAD_B): {
			int HL = ((H << 8) | L);
			int BC = ((B << 8) | C);
			int sum = HL + BC;
			H = (sum & 0xFF00) >> 8;
			L = sum & 0xFF;
			CY = ((sum & 0xFFFF0000) != 0);
		} break;
		case (LDAX_B):
			A = cpuRead((B << 8 | C), false);
			break;
		case (DCX_B):
			C--;
			if (C == 0xFF)
				B--;
			break;
		case (INR_C):
			C = AddByte(C, 0x1, Preserve_carry);
			break;
		case (DCR_C):
			C = SubByte(C, 0x1, Preserve_carry);
			break;
		case (MVI_C_D8):
			C = cpuRead(++pc, false);
			break;
		case (RRC): {
			uint8_t x = A;
			A = ((x & 1) << 7) | (x >> 1);
			CY = (1 == (x & 1));
		} break;
		case (XXXX_2):
			return Invalid_Instruction();
			break;
		case (LXI_D_D16):
			E = cpuRead(++pc, false);
			D = cpuRead(++pc, false);
			break;
		case (STAX_D):
			cpuWrite((D << 8) | E, A);
			break;
		case (INX_D):
			E++;
			if (E == 0) {
				D++;
				if ((D << 8 | E) > 0xFFFF) {
					D = 0;
					E = 0;
				}
			}
			break;
		case (INR_D):
			D = AddByte(D, 0x1, Preserve_carry);
			break;
		case (DCR_D):
			D = SubByte(D, 0x1, Preserve_carry);
			break;
		case (MVI_D_D8):
			D = cpuRead(++pc, false);
			break;
		case (RAL): {
			uint8_t x = A;
			A = (CY | (x << 1));
			CY = (1 == ((x & 0x80) >> 7));
		} break;
		case (XXXX_3):
			return Invalid_Instruction();
			break;
		case (DAD_D): {
			int HL = ((H << 8) | L);
			int DE = ((D << 8) | E);
			int sum = HL + DE;
			H = (sum & 0xFF00) >> 8;
			L = sum & 0xFF;
			CY = ((sum & 0xFFFF0000) != 0);
		} break;
		case (LDAX_D):
			A = cpuRead((D << 8 | E), false);
			break;
		case (DCX_D):
			E--;
			if (E == 0xFF)
				D--;
			break;
		case (INR_E):
			E = AddByte(E, 0x1, Preserve_carry);
			break;
		case (DCR_E):
			E = SubByte(E, 0x1, Preserve_carry);
			break;
		case (MVI_E_D8):
			E = cpuRead(++pc, false);
			break;
		case (RAR): {
			uint8_t x = A;
			A = ((x >> 1) | (CY << 7));
			CY = (1 == (x & 0x1));
		} break;
		case (RIM):
			return UnimplementedInstruction();
			break;
		case (LXI_H_D16):
			L = cpuRead(++pc, false);
			H = cpuRead(++pc, false);
			break;
		case (SHLD_ADR): {
			uint8_t low = cpuRead(++pc, false);
			uint8_t high = cpuRead(++pc, false);
			uint16_t Addr = (low << 8) | high;
			cpuWrite(Addr, H);
			cpuWrite((Addr + 1), L);
		} break;
		case (INX_H):
			L++;
			if (L == 0) {
				H++;
				if ((H << 8 | L) > 0xFFFF) {
					H = 0;
					L = 0;
				}
			}
			break;
		case (INR_H):
			H = AddByte(H, 0x1, Preserve_carry);
			break;
		case (DCR_H):
			H = SubByte(H, 0x1, Preserve_carry);
			break;
		case (MVI_H_D8):
			D = cpuRead(++pc, false);
			break;
		case (DAA): {
			uint16_t sum = A;

			if ((A & 0x0F) > 9 || AC == 1)
				sum = AddByte(A, 0x60, Update_carry);
			if ((A & 0xF0) > 9 || CY == 1)
				sum = AddByte(A, 0x60, Update_carry);

			A = (uint8_t)sum;

		} break;
		case (XXXX_4):
			return Invalid_Instruction();
			break;
		case (DAD_H): {
			int HL = ((H << 8) | L);

			int sum = HL + HL;
			H = (sum & 0xFF00) >> 8;
			L = sum & 0xFF;
			CY = ((sum & 0xFFFF0000) > 0);
		} break;
		case (LHLD_ADR): {
			uint8_t low = cpuRead(++pc, false);
			uint8_t high = cpuRead(++pc, false);
			uint16_t Addr = ((high << 8) | low);
			L = cpuRead(Addr, false);
			H = cpuRead((Addr + 1), false);
		}

		break;
		case (DCX_H):
			L--;
			if (L == 0xFF)
				H--;
			break;
		case (INR_L):
			L = AddByte(L, 0x1, Preserve_carry);
			break;
		case (DCR_L):
			L = SubByte(L, 0x1, Preserve_carry);
			break;
		case (MVI_L):
			L = cpuRead(++pc, false);
			break;
		case (CMA):
			A = ~A;
			break;
		case (SIM):
			return UnimplementedInstruction();
			break;
		case (LXI_SP_D16): {
			uint8_t low = cpuRead(++pc, false);
			uint8_t high = cpuRead(++pc, false);
			sp = ((high << 8) | low);
		} break;
		case (STA_ADR): {
			uint8_t low = cpuRead(++pc, false);
			uint8_t high = cpuRead(++pc, false);
			uint16_t Addr = ((high << 8) | low);
			cpuWrite(Addr, A);
		} break;
		case (INX_SP):
			sp++;
			break;
		case (INR_M): {
			uint16_t Addr = (H << 8 | L);
			uint8_t data = cpuRead(Addr, false);
			data += 1;
			cpuWrite(Addr, data);
		} break;
		case (DCR_M): {
			uint16_t Addr = (H << 8 | L);
			uint8_t data = cpuRead(Addr, false);
			data -= 1;
			cpuWrite(Addr, data);
		} break;
		case (MVI_M_D8):
			cpuWrite(((H << 8) | L), cpuRead(++pc, false));
			break;
		case (STC):
			CY = 1;
			break;
		case (XXXX_5):
			return Invalid_Instruction();
			break;
		case (DAD_SP): {
			int HL = ((H << 8) | L);

			int sum = HL + sp;
			H = (sum & 0xFF00) >> 8;
			L = sum & 0xFF;
			CY = ((sum & 0xFFFF0000) > 0);
		} break;
		case (LDA_ADR): {
			uint8_t low = cpuRead(++pc, false);
			uint8_t high = cpuRead(++pc, false);
			uint16_t Addr = ((high << 8) | low);
			A = cpuRead(Addr, false);
		} break;
		case (DCX_SP):
			sp--;
			break;
		case (INR_A):
			A = AddByte(A, 0x1, Preserve_carry);
			break;
		case (DCR_A):
			A = SubByte(A, 0x1, Preserve_carry);
			break;
		case (MVI_A_D8):
			A = cpuRead(++pc, false);
			break;
		case (CMC):
			CY = ~CY;
			break;
		case (MOV_BB):
			B = B;
			break;
		case (MOV_BC):
			B = C;
			break;
		case (MOV_BD):
			B = D;
			break;
		case (MOV_BE):
			B = E;
			break;
		case (MOV_BH):
			B = H;
			break;
		case (MOV_BL):
			B = L;
			break;
		case (MOV_BM):
			B = cpuRead((H << 8 | L), false);
			break;
		case (MOV_BA):
			B = A;
			break;
		case (MOV_CB):
			C = B;
			break;
		case (MOV_CC):
			C = C;
			break;
		case (MOV_CD):
			C = D;
			break;
		case (MOV_CE):
			C = E;
			break;
		case (MOV_CH):
			C = H;
			break;
		case (MOV_CL):
			C = L;
			break;
		case (MOV_CM):
			C = cpuRead((H << 8 | L), false);
			break;
		case (MOV_CA):
			C = A;
			break;

		case (MOV_DB):
			D = B;
			break;
		case (MOV_DC):
			D = C;
			break;
		case (MOV_DD):
			D = D;
			break;
		case (MOV_DE):
			D = E;
			break;
		case (MOV_DH):
			D = H;
			break;
		case (MOV_DL):
			D = L;
			break;
		case (MOV_DM):
			D = cpuRead((H << 8 | L), false);
			break;
		case (MOV_DA):
			D = A;
			break;

		case (MOV_EB):
			E = B;
			break;
		case (MOV_EC):
			E = C;
			break;
		case (MOV_ED):
			E = D;
			break;
		case (MOV_EE):
			E = E;
			break;
		case (MOV_EH):
			E = H;
			break;
		case (MOV_EL):
			E = L;
			break;
		case (MOV_EM):
			E = cpuRead((H << 8 | L), false);
			break;
		case (MOV_EA):
			E = A;
			break;

		case (MOV_HB):
			H = B;
			break;
		case (MOV_HC):
			H = C;
			break;
		case (MOV_HD):
			H = D;
			break;
		case (MOV_HE):
			H = E;
			break;
		case (MOV_HH):
			H = H;
			break;
		case (MOV_HL):
			H = L;
			break;
		case (MOV_HM):
			H = cpuRead((H << 8 | L), false);
			break;
		case (MOV_HA):
			H = A;
			break;
		case (MOV_LB):
			L = B;
			break;
		case (MOV_LC):
			L = C;
			break;
		case (MOV_LD):
			L = D;
			break;
		case (MOV_LE):
			L = E;
			break;
		case (MOV_LH):
			L = H;
			break;
		case (MOV_LL):
			L = L;
			break;
		case (MOV_LM):
			L = cpuRead((H << 8 | L), false);
			break;
		case (MOV_LA):
			L = A;
			break;

		case (MOV_MB):
			cpuWrite((H << 8 | L), B);
			break;
		case (MOV_MC):
			cpuWrite((H << 8 | L), C);
			break;
		case (MOV_MD):
			cpuWrite((H << 8 | L), D);
			break;
		case (MOV_ME):
			cpuWrite((H << 8 | L), E);
			break;
		case (MOV_MH):
			cpuWrite((H << 8 | L), H);
			break;
		case (MOV_ML):
			cpuWrite((H << 8 | L), L);
			break;
		case (HLT):
			return 0;
			break;
		case (MOV_MA):
			cpuWrite((H << 8 | L), A);
			break;

		case (MOV_AB):
			A = B;
			break;
		case (MOV_AC):
			A = C;
			break;
		case (MOV_AD):
			A = D;
			break;
		case (MOV_AE):
			A = E;
			break;
		case (MOV_AH):
			A = H;
			break;
		case (MOV_AL):
			A = L;
			break;
		case (MOV_AM):
			A = cpuRead((H << 8 | L), false);
			break;
		case (MOV_AA):
			A = A;
			break;

		case (ADD_B):
			A = AddByte(A, B, Update_carry);
			break;
		case (ADD_C):
			A = AddByte(A, C, Update_carry);
			break;
		case (ADD_D):
			A = AddByte(A, D, Update_carry);
			break;
		case (ADD_E):
			A = AddByte(A, E, Update_carry);
			break;
		case (ADD_H):
			A = AddByte(A, H, Update_carry);
			break;
		case (ADD_L):
			A = AddByte(A, L, Update_carry);
			break;
		case (ADD_M): {
			uint8_t x = cpuRead((H << 8 | L), false);
			A = AddByte(A, x, Update_carry);
		} break;
		case (ADD_A):
			A = AddByte(A, A, Update_carry);
			break;

		case (ADC_B):
			A = AddByteWtCY(A, B, Update_carry);
			break;
		case (ADC_C):
			A = AddByteWtCY(A, C, Update_carry);
			break;
		case (ADC_D):
			A = AddByteWtCY(A, D, Update_carry);
			break;
		case (ADC_E):
			A = AddByteWtCY(A, E, Update_carry);
			break;
		case (ADC_H):
			A = AddByteWtCY(A, H, Update_carry);
			break;
		case (ADC_L):
			A = AddByteWtCY(A, L, Update_carry);
			break;
		case (ADC_M): {
			uint8_t x = cpuRead((H << 8 | L), false);
			A = AddByteWtCY(A, B, Update_carry);
		} break;
		case (ADC_A):
			A = AddByteWtCY(A, A, Update_carry);
			break;

		case (SUB_B):
			A = SubByte(A, B, Update_carry);
			break;
		case (SUB_C):
			A = SubByte(A, C, Update_carry);
			break;
		case (SUB_D):
			A = SubByte(A, D, Update_carry);
			break;
		case (SUB_E):
			A = SubByte(A, E, Update_carry);
			break;
		case (SUB_H):
			A = SubByte(A, H, Update_carry);
			break;
		case (SUB_L):
			A = SubByte(A, L, Update_carry);
			break;
		case (SUB_M): {
			uint8_t x = cpuRead((H << 8 | L), false);
			A = SubByte(A, x, Update_carry);
		} break;
		case (SUB_A):
			A = SubByte(A, A, Update_carry);
			break;
		case (SBB_B):
			A = SubByteWtBR(A, B, Update_carry);
			break;
		case (SBB_C):
			A = SubByteWtBR(A, C, Update_carry);
			break;
		case (SBB_D):
			A = SubByteWtBR(A, D, Update_carry);
			break;
		case (SBB_E):
			A = SubByteWtBR(A, E, Update_carry);
			break;
		case (SBB_H):
			A = SubByteWtBR(A, H, Update_carry);
			break;
		case (SBB_L):
			A = SubByteWtBR(A, L, Update_carry);
			break;
		case (SBB_M): {
			uint8_t x = cpuRead((H << 8 | L), false);
			A = SubByteWtBR(A, x, Update_carry);
		} break;
		case (SBB_A):
			A = SubByteWtBR(A, A, Update_carry);
			break;
		case (ANA_B):
			A = A & B;
			LogicFlags();
			break;
		case (ANA_C):
			A = A & C;
			LogicFlags();
			break;
		case (ANA_D):
			A = A & D;
			LogicFlags();
			break;
		case (ANA_E):
			A = A & E;
			LogicFlags();
			break;
		case (ANA_H):
			A = A & H;
			LogicFlags();
			break;
		case (ANA_L):
			A = A & L;
			LogicFlags();
			break;
		case (ANA_M): {
			uint8_t x = cpuRead((H << 8 | L), false);
			A = A & x;
			LogicFlags();
		} break;
		case (ANA_A):
			A = A & A;
			LogicFlags();
			break;

		case (XRA_B):
			A = A ^ B;
			LogicFlags();
			break;
		case (XRA_C):
			A = A ^ C;
			LogicFlags();
			break;
		case (XRA_D):
			A = A ^ D;
			LogicFlags();
			break;
		case (XRA_E):
			A = A ^ E;
			LogicFlags();
			break;
		case (XRA_H):
			A = A ^ H;
			LogicFlags();
			break;
		case (XRA_L):
			A = A ^ L;
			LogicFlags();
			break;
		case (XRA_M): {
			uint8_t x = cpuRead((H << 8 | L), false);
			A = A ^ x;
			LogicFlags();
		} break;
		case (XRA_A):
			A = A ^ A;
			LogicFlags();
			break;

		case (ORA_B):
			A = A | B;
			LogicFlags();
			break;
		case (ORA_C):
			A = A | C;
			LogicFlags();
			break;
		case (ORA_D):
			A = A | D;
			LogicFlags();
			break;
		case (ORA_E):
			A = A | E;
			LogicFlags();
			break;
		case (ORA_H):
			A = A | H;
			LogicFlags();
			break;
		case (ORA_L):
			A = A | L;
			LogicFlags();
			break;
		case (ORA_M): {
			uint8_t x = cpuRead((H << 8 | L), false);
			A = A | x;
			LogicFlags();
		} break;
		case (ORA_A):
			A = A | A;
			break;

		case (CMP_B):
			SubByte(A, B, Update_carry);
			break;
		case (CMP_C):
			SubByte(A, C, Update_carry);
			break;
		case (CMP_D):
			SubByte(A, D, Update_carry);
			break;
		case (CMP_E):
			SubByte(A, E, Update_carry);
			break;
		case (CMP_H):
			SubByte(A, H, Update_carry);
			break;
		case (CMP_L):
			SubByte(A, L, Update_carry);
			break;
		case (CMP_M): {
			uint8_t data = cpuRead((H << 8 | L), false);
			SubByte(A, data, Update_carry);
		} break;
		case (CMP_A):
			SubByte(A, A, Update_carry);
			break;

		case (RNZ):
			if (Z != 0) {
				ReturnToCall();
				pc--;
			}
			break;

		case (POP_B):
			C = cpuRead(sp++, false);
			B = cpuRead(sp++, false);
			break;
		case (JNZ_ADR):
			if (Z != 1) {
				uint8_t low, high;
				low = cpuRead(++pc, false);
				high = cpuRead(++pc, false);
				pc = (high << 8 | low);
				pc--;
			} else {
				pc++;
			}
			break;
		case (JMP_ADR): {
			uint8_t low, high;
			low = cpuRead(++pc, false);
			high = cpuRead(++pc, false);
			pc = (high << 8 | low);
			pc--;
		} break;

		case (CNZ_ADR):
			if (Z != 1) {
				uint8_t low, high;
				low = cpuRead(++pc, false);
				high = cpuRead(++pc, false);
				Call(high << 8 | low);
				pc--;
			} else {
				pc++;
			}
			break;
		case (PUSH_B):
			cpuWrite(--sp, B);
			cpuWrite(--sp, C);
			break;
		case (ADI_D8):
			A = AddByte(A, cpuRead(++pc, false), Update_carry);
			break;
		case (RST_0):
			Call(0xC700);
			break;
		case (RZ):
			if (Z == 0) {
				ReturnToCall();
				pc--;
			}

			break;

		case (RET):
			ReturnToCall();
			pc--;
			break;
		case (JZ_ADR):
			if (Z == 1) {
				uint8_t low, high;
				low = cpuRead(++pc, false);
				high = cpuRead(++pc, false);
				pc = (high << 8 | low);
				pc--;
			} else {
				pc++;
			}
			break;
		case (XXXX_6):
			return UnimplementedInstruction();
			break;
		case (CZ_ADR):
			if (Z == 1) {
				uint8_t low, high;
				low = cpuRead(++pc, false);
				high = cpuRead(++pc, false);
				Call(high << 8 | low);
				pc--;
			} else {
				pc++;
			}
			break;
		case (CALL_ADR): {
			uint8_t low, high;
			low = cpuRead(++pc, false);
			high = cpuRead(++pc, false);
			pc = (high << 8 | low);
			pc--;
		}

		break;
		case (ACI_D8): {
			uint8_t x = cpuRead(++pc, false);
			A = AddByteWtCY(A, x, Update_carry);
		} break;
		case (RST_1):
			Call(0xCF00);
			pc--;
			break;
		case (RNC):
			if (CY != 1) {
				ReturnToCall();
				pc--;
			} else {
				pc++;
			}
			break;
		case (POP_D):
			E = cpuRead(sp++, false);
			D = cpuRead(sp++, false);
			break;
		case (JNC_ADR):
			if (CY != 1) {
				uint8_t low, high;
				low = cpuRead(++pc, false);
				high = cpuRead(++pc, false);
				pc = (high << 8 | low);
				pc--;
			} else {
				pc++;
			}
			break;
		case (OUT_D8): {
			uint16_t addr;
			addr = cpuRead(++pc, false);
			cpuWrite(addr, A, true);
		}

		break;
		case (CNC_ADR):
			if (CY != 1) {
				uint8_t low, high;
				low = cpuRead(++pc, false);
				high = cpuRead(++pc, false);
				Call(high << 8 | low);
				pc--;
			} else {
				pc++;
			}
			break;
		case (PUSH_D):
			cpuWrite(--sp, D);
			cpuWrite(--sp, E);
			break;
		case (SUI_D8): {
			uint8_t data;
			data = cpuRead(++pc, false);
			A = SubByte(A, data, Update_carry);
		} break;
		case (RST_2):
			Call(0xD700);
			pc--;
			break;
		case (RC):
			if (CY == 1)
				ReturnToCall();
			break;
		case (XXXX_7):
			return Invalid_Instruction();
			break;
		case (JC_ADR):
			if (CY == 1) {
				uint8_t low, high;
				low = cpuRead(++pc, false);
				high = cpuRead(++pc, false);
				pc = (high << 8 | low);
				pc--;
			} else {
				pc++;
			}
			break;
		case (IN_D8): {
			uint16_t addr;
			addr = cpuRead(++pc, false);
			A = cpuRead(addr, 0, true);
		}

		break;
		case (CC_ADR):
			if (CY == 1) {
				uint8_t low, high;
				low = cpuRead(++pc, false);
				high = cpuRead(++pc, false);
				Call(high << 8 | low);
				pc--;
			} else {
				pc++;
			}
			break;
		case (XXXX_8):
			return UnimplementedInstruction();
			break;
		case (SBI_D8): {
			uint8_t x = cpuRead(++pc, false);
			A = SubByteWtBR(A, x, Update_carry);
		} break;
		case (RST_3):
			Call(0xDF00);
			pc--;
			break;
		case (RPO):
			if (P == 0) {
				ReturnToCall();
				pc--;
			}
			break;
		case (POP_H):
			L = cpuRead(sp++, false);
			H = cpuRead(sp++, false);
			break;
		case (JPO_ADR):
			if (P == 0) {
				uint8_t low, high;
				low = cpuRead(++pc, false);
				high = cpuRead(++pc, false);
				pc = (high << 8 | low);
				pc--;
			} else {
				pc++;
			}
			break;
		case (XTHL): {
			L = cpuRead(sp, false);
			H = cpuRead(sp + 1, false);
		} break;
		case (CPO_ADR):
			if (P == 0) {
				uint8_t low, high;
				low = cpuRead(++pc, false);
				high = cpuRead(++pc, false);
				Call(high << 8 | low);
				pc--;
			} else {
				pc++;
			}
			break;
		case (PUSH_H):
			cpuWrite(--sp, H);
			cpuWrite(--sp, L);
			break;
		case (ANI_D8): {
			uint8_t low, high;
			low = cpuRead(++pc, false);
			high = cpuRead(++pc, false);
			uint16_t data = (high << 8 | low);
			A = A & data;
			LogicFlags();
		} break;
		case (RST_4):
			Call(0xE700);
			pc--;
			break;
		case (RPE):
			if (P == 1) {
				ReturnToCall();
				pc--;
			}
			break;
		case (PCHL):
			pc = (H << 8 | L);
			pc--;
			break;
		case (JPE_ADR):
			if (P == 1) {
				uint8_t low, high;
				low = cpuRead(++pc, false);
				high = cpuRead(++pc, false);
				pc = (high << 8 | low);
				pc--;
			} else {
				pc++;
			}
			break;
		case (XCHG): {
			uint8_t xchg;
			xchg = D;
			H = D;
			D = xchg;

			xchg = E;
			E = L;
			L = xchg;
		}

		break;
		case (CPE_ADR):
			if (P == 1) {
				uint8_t low, high;
				low = cpuRead(++pc, false);
				high = cpuRead(++pc, false);
				Call(high << 8 | low);
				pc--;
			} else {
				pc++;
			}
			break;
		case (XXXX_9):
			return UnimplementedInstruction();
			break;
		case (XRI_D8): {
			uint8_t low, high;
			low = cpuRead(++pc, false);
			high = cpuRead(++pc, false);
			uint16_t data = (high << 8 | low);
			A = A ^ data;
			LogicFlags();
		}

		break;
		case (RST_5):
			Call(0xEF00);
			pc--;
			break;
		case (RP):
			if (P == 1) {
				ReturnToCall();
				pc--;
			}
			break;
		case (POP_PSW):
			status = cpuRead(sp++, false);
			A = cpuRead(sp++, false);
			break;
		case (JP_ADR):
			if (P == 1) {
				uint8_t low, high;
				low = cpuRead(++pc, false);
				high = cpuRead(++pc, false);
				pc = (high << 8 | low);
				pc--;
			} else {
				pc++;
			}
			break;
		case (DI):
			IntMask = 0;
			break;
		case (CP_ADR):
			if (P == 1) {
				uint8_t low, high;
				low = cpuRead(++pc, false);
				high = cpuRead(++pc, false);
				Call(high << 8 | low);
				pc--;
			} else {
				pc++;
			}
			break;
		case (PUSH_PSW): {
			cpuWrite(--sp, A);
			cpuWrite(--sp, status);
		} break;
		case (ORI_D8): {
			uint8_t low, high;
			low = cpuRead(++pc, false);
			high = cpuRead(++pc, false);
			uint16_t data = (high << 8 | low);
			A = A | data;
			LogicFlags();
		}

		break;
		case (RST_6):
			Call(0xF700);
			pc--;
			break;
		case (RM):
			if (S == 1) {
				Call((H << 8) | L);
				pc--;
			}
			break;
		case (SPHL):
			sp = (H << 8 | L);
			break;
		case (JM_ADR):
			if (S == 1) {
				uint8_t low, high;
				low = cpuRead(++pc, false);
				high = cpuRead(++pc, false);
				pc = (high << 8 | low);
				pc--;
			} else {
				pc++;
			}
			break;
		case (EI):
			IntMask = 1;
			break;
		case (CM_ADR):
			if (S == 1) {
				uint8_t low, high;
				low = cpuRead(++pc, false);
				high = cpuRead(++pc, false);
				Call(high << 8 | low);
				pc--;
			} else {
				pc++;
			}
			break;
		case (XXXX_10):
			return Invalid_Instruction();
			break;
		case (CPI_D8): {
			uint8_t x = cpuRead(++pc, false);
			SubByte(A, x, Update_carry);
		} break;
		case (RST_7):
			Call(0xFF00);
			pc--;
			break;
		default:
			break;
	}

	SetFlags();
	return 0;
}
