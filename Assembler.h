#include<iostream>
#include<string>
#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include<sstream>
#include<regex>
#include<map>
#include<iterator>
#include<fstream>
#include<array>

class Assembler
{
private:
	std::string m_filename; //The file name may get passed by the GUI 
    std::map<std::string, uint8_t> OpcodeList;
    

protected:
    std::vector<uint8_t> OpcodeOut;
    std::vector<std::string> ErrorList;
    unsigned int linenum = 0;
	
	

public:
    enum ErrorTable
    {
        Undefined_Symbol,
        Not_Able_to_parse,
        Data_Overflow,
        Undefined_statement,
		Unusable_Opcode,
	};


public:
//Enum object
    ErrorTable ErrorValue;

	Assembler();

//Constructor and Destructor:
    Assembler(std::string fn)
        : m_filename(fn) {
        OpcodeList = {
        {"NOP",00},
        {"LXI B,XXXX",01},
        {"STAX B", 02},
        {"INX B", 03},
        {"INR B",  04},
        {"DCR B",  05},
        {"MVI B,XX", 06},
        {"RLC",  07},
        {"",  8},
        {"DAD B", 9},
        {"LDAX B",  10},
        {"DCX B",  11},
        {"INR C",  12},
        {"DCR C",  13},
        {"MVI C,XX", 14 },
        {"RRC",  15},
        {"",  16},
        {"LXI D,XXXX", 17 },
        {"STAX D",  18},
        {"INX D",  19},
        {"INR D",  20},
        {"DCR D",  21},
        {"MVI D,XX", 22 },
        {"RAL",  23},
        {"", 24 },
        {"DAD D", 25 },
        {"LDAX D",  26},
        {"DCX D",  27},
        {"INR E",  28},
        {"DCR E",  29},
        {"MVI E,XX", 30 },
        {"RAR",  31},
        {"RIM",  32},
        {"LXI H,XXXX", 33 },
        {"SHLD XXXX",  34},
        {"INX H",  35},
        {"INR H",  36},
        {"DCR H",  37},
        {"MVI H,XX", 38 },
        {"DAA",  39},
        {"",  40},
        {"DAD H", 41 },
        {"LHLD XXXX", 42 },
        {"DCX H",  43},
        {"INR L",  44},
        {"DCR L",  45},
        {"MVI L,XX", 46 },
        {"CMA",  47},
        {"SIM",  48},
        {"LXI SP,XXXX", 49 },
        {"STA XXXX",  50},
        {"INX SP",  51},
        {"INR M",  52},
        {"DCR M",  53},
        {"MVI M,XX", 54 },
        {"STC",  55},
        {"",  56},
        {"DAD SP", 57 },
        {"LDA XXXX", 58 },
        {"DCX SP",  59},
        {"INR A",  60},
        {"DCR A",  61},
        {"MVI A,XX", 62 },
        {"CMC",  63},
        {"MOV B,B", 64 },
        {"MOV B,C",  65},
        {"MOV B,D",  66},
        {"MOV B,E",  67},
        {"MOV B,H",  68},
        {"MOV B,L",  69},
        {"MOV B,M",  70},
        {"MOV B,A",  71},
        {"MOV C,B",  72},
        {"MOV C,C",  73},
        {"MOV C,D",  74},
        {"MOV C,E",  75},
        {"MOV C,H",  76},
        {"MOV C,L",  77},
        {"MOV C,M",  78},
        {"MOV C,A",  79},
        {"MOV D,B",  80},
        {"MOV D,C",  81},
        {"MOV D,D",  82},
        {"MOV D,E",  83},
        {"MOV D,H",  84},
        {"MOV D,L",  85},
        {"MOV D,M",  86},
        {"MOV D,A",  87},
        {"MOV E,B",  88},
        {"MOV E,C",  89},
        {"MOV E,D",  90},
        {"MOV E,E",  91},
        {"MOV E,H",  92},
        {"MOV E,L",  93},
        {"MOV E,M",  94},
        {"MOV E,A",  95},
        {"MOV H,B",  96},
        {"MOV H,C",  97},
        {"MOV H,D",  98},
        {"MOV H,E",  99},
        {"MOV H,H",  100},
        {"MOV H,L",  101},
        {"MOV H,M",  102},
        {"MOV H,A",  103},
        {"MOV L,B",  104},
        {"MOV L,C",  105},
        {"MOV L,D",  106},
        {"MOV L,E",  107},
        {"MOV L,H",  108},
        {"MOV L,L",  109},
        {"MOV L,M",  110},
        {"MOV L,A",  111},
        {"MOV M,B",  112},
        {"MOV M,C",  113},
        {"MOV M,D",  114},
        {"MOV M,E",  115},
        {"MOV M,H",  116},
        {"MOV M,L",  117},
        {"HLT",  118},
        {"MOV M,A",  119},
        {"MOV A,B",  120},
        {"MOV A,C",  121},
        {"MOV A,D",  122},
        {"MOV A,E",  123},
        {"MOV A,H",  124},
        {"MOV A,L",  125},
        {"MOV A,M",  126},
        {"MOV A,A",  127},
        {"ADD B",  128},
        {"ADD C",  129},
        {"ADD D",  130},
        {"ADD E",  131},
        {"ADD H",  132},
        {"ADD L",  133},
        {"ADD M",  134},
        {"ADD A",  135},
        {"ADC B",  136},
        {"ADC C",  137},
        {"ADC D",  138},
        {"ADC E",  139},
        {"ADC H",  140},
        {"ADC L",  141},
        {"ADC M",  142},
        {"ADC A",  143},
        {"SUB B",  144},
        {"SUB C",  145},
        {"SUB D",  146},
        {"SUB E",  147},
        {"SUB H",  148},
        {"SUB L",  149},
        {"SUB M",  150},
        {"SUB A",  151},
        {"SBB B",  152},
        {"SBB C",  153},
        {"SBB D",  154},
        {"SBB E",  155},
        {"SBB H",  156},
        {"SBB L",  157},
        {"SBB M",  158},
        {"SBB A",  159},
        {"ANA B",  160},
        {"ANA C",  161},
        {"ANA D",  162},
        {"ANA E",  163},
        {"ANA H",  164},
        {"ANA L",  165},
        {"ANA M",  166},
        {"ANA A",  167},
        {"XRA B",  168},
        {"XRA C",  169},
        {"XRA D",  170},
        {"XRA E",  171},
        {"XRA H",  172},
        {"XRA L",  173},
        {"XRA M",  174},
        {"XRA A",  175},
        {"ORA B",  176},
        {"ORA C",  177},
        {"ORA D",  178},
        {"ORA E",  179},
        {"ORA H",  180},
        {"ORA L",  181},
        {"ORA M",  182},
        {"ORA A",  183},
        {"CMP B",  184},
        {"CMP C",  185},
        {"CMP D",  186},
        {"CMP E",  187},
        {"CMP H",  188},
        {"CMP L",  189},
        {"CMP M",  190},
        {"CMP A",  191},
        {"RNZ",  192},
        {"POP B",  193},
        {"JNZ XXXX",  194},
        {"JMP XXXX",  195},
        {"CNZ XXXX",  196},
        {"PUSH B",  197},
        {"ADI XX",  198},
        {"RST 0",  199},
        {"RZ",  200},
        {"RET",  201},
        {"JZ XXXX",  202},
        {"",  203},
        {"CZ XXXX",  204},
        {"CALL XXXX",  205},
        {"ACI XX",  206},
        {"RST 1",  207},
        {"RNC",  208},
        {"POP D",  209},
        {"JNC XXXX",  210},
        {"OUT XX",  211},
        {"CNC XXXX",  212},
        {"PUSH D",  213},
        {"SUI XX",  214},
        {"RST 2",  215},
        {"RC",  216},
        {"",  217},
        {"JC XXXX",  218},
        {"IN XX",  219},
        {"CC XXXX",  220},
        {"",  221},
        {"SBI XX",  222},
        {"RST 3",  223},
        {"RPO",  224},
        {"POP H",  225},
        {"JPO XXXX",  226},
        {"XTHL",  227},
        {"CPO XXXX",  228},
        {"PUSH H",  229},
        {"ANI XX",  230},
        {"RST 4",  231},
        {"RPE",  232},
        {"PCHL",  233},
        {"JPE XXXX",  234},
        {"XCHG",  235},
        {"CPE XXXX",  236},
        {"",  237},
        {"XRI XX",  238},
        {"RST 5",  239},
        {"RP",  240},
        {"POP PSW",  241},
        {"JP XXXX",  242},
        {"DI",  243},
        {"CP XXXX",  244},
        {"PUSH PSW",  245},
        {"ORI XX", 246},
        {"RST 6",  247},
        {"RM",  248},
        {"SPHL",  249},
        {"JM XXXX", 250},
        {"EI", 251},
        {"CM XXXX", 252},
        {"", 253},
        {"CPI XX", 254},
        {"RST 7", 255},
        };
    };

 public:
     std::array<std::string, 23> byte_Opcodes{ { "LXI ", "SHLD ", "LHLD ", "STA ", "LDA ", "JNZ ", "JMP ", "CNZ ", "JZ ", "CZ ", "CALL ", "JNC ", "CNC ", "JC ", "CC ", "JPO ", "CPO ", "JPE ", "CPE ", "JP ", "JM ", "CP ", "CM " } };
     
    



	void Getopcode(std::string&);
	void FormatData(std::string, unsigned int&);
	//(line to parse, line number)

	void GetData();
	std::string TrimLR(std::string&);
	bool IsValid(const std::string&);

    void StoreData();
    void Error(ErrorTable,unsigned int);
    void PrintError();
    std::string GetTimingData(uint8_t);

	int ErrorCheck = 0;

	int sizeofOpcode = 0;

	int ErrorReturn();
	std::string GetOpcodeList();
	

};

#endif // !ASSEMBLER_H
