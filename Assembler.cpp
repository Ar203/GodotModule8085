#include "Assembler.h"

//Retrives the data from the file
void Assembler::GetData() {
	std::string str;

	std::ifstream FileRead;
	FileRead.open(m_filename);
	while (FileRead) {
		std::getline(FileRead, str);
		++linenum;
		if (str.empty() || str.find_first_not_of(' ') == std::string::npos)
			continue;
		FormatData(str, linenum);
	}
	FileRead.close();
}

void Assembler::FormatData(std::string str, unsigned int &line) {
	std::cout << std::boolalpha;

	//Regex List
	const static std::regex regExtraSpace("((\\s+)|(\t+))");
	const static std::regex regComment("([//].*$)");
	const static std::regex regLabel("(#(\\w)+[\\s+]?:)");

	//Removing all the comments
	str = std::regex_replace(str, regComment, "");

	//Removing extra spaces in between the string elements and from left and right
	str = std::regex_replace(str, regExtraSpace, " ");
	//Triming the string from left and right
	str = TrimLR(str);

	//Checking for unusable special characters
	if (!IsValid(str)) {
		Error(ErrorTable::Undefined_Symbol, linenum);
		return;
	}

	//Erasing spaces after the main mnemonic
	{
		auto pos = str.find_first_of(' ');
		if (pos != std::string::npos) {
			str.erase(std::remove((str.begin() + str.find_first_of(' ') + 1), str.end(), ' '), str.end());
		}
		str.shrink_to_fit();
	}

	//Getting the labels
	std::smatch label;
	uint16_t n = 0;
	while (std::regex_search(str, label, regLabel)) {
		if (n != 0) {
			std::cout << "Error";
			break;
		}
		n++;
		std::cout << label.str() << std::endl;
		str = std::regex_replace(str, regLabel, "");
	}

	//Capitalizing the string
	for (auto &s : str)
		s = std::toupper(s);

	Getopcode(str);
}

void Assembler::Getopcode(std::string &str) {
	//String extraction
	std::smatch sData;
	int Data = 00;
	bool DataReady = false; //For inserting the data in the output
	bool found = false; //Data existence
	bool int_type = false; // 0 = hex, 1 = int

	//Reegex for Data extraction
	const static std::regex regDataHex("(0X)([0-9A-Fa-z]+)");
	const static std::regex regDataInt("(\\d+)D?");

	//Searching for data patter
	if (std::regex_search(str, sData, regDataHex)) {
		{
			std::stringstream ss;
			ss << std::hex << sData[2].str();
			ss >> Data;
			Data = static_cast<int>(Data);
		}
		if ((Data < -128) || (Data > 0xFFFF)) {
			Error(ErrorTable::Data_Overflow, linenum);
			return;
		}
		found = true;
		int_type = false;
		DataReady = true;
	}
	if (!found) {
		if (std::regex_search(str, sData, regDataInt)) {
			Data = std::stoi(sData[1].str());
			if ((Data < -128) || (Data > 0xFFFF)) {
				Error(ErrorTable::Data_Overflow, linenum);
				return;
			}
			int_type = true;
			found = true;
			DataReady = true;
		} else {
			//std::cout << "Data is ambiguous" << std::endl;
			Data = NULL;
		}
	}

	if (found) {
		std::string temp;
		temp = str;

		for (auto i = temp.end() - 1; i > temp.begin() + 1; i--) {
			if ((*i == ' ') || (*i == ','))
				break;
			temp.pop_back();
		}

		bool addressopcode = false;
		for (const auto &c : byte_Opcodes) {
			if (c.compare(temp) == 0) {
				addressopcode = true;
				break;
			}
		}

		temp.push_back('X');
		temp.push_back('X');
		if (addressopcode) {
			temp.push_back('X');
			temp.push_back('X');
			sizeofOpcode = 3;
		}

		str = temp;
		str.shrink_to_fit();
	}

	if (!str.empty()) {
		auto search = OpcodeList.find(str);
		if (search != OpcodeList.end())
			OpcodeOut.push_back(search->second);
		if (search == OpcodeList.end()) {
			Error(ErrorTable::Undefined_statement, linenum);
			return;
		}
	}

	if (DataReady) {
		uint8_t low = (Data & 0xFF);
		uint8_t high = (Data & 0xFF00) >> 8;

		OpcodeOut.push_back(low);
		if ((int)high > 0)
			OpcodeOut.push_back(high);
	}
	if (!found) {
		sizeofOpcode = 1;
	}
}

//Trimming Functions
std::string Assembler::TrimLR(std::string &str) {
	auto first = str.find_first_not_of(' ');
	if (first == std::string::npos) {
		return str;
	}
	auto last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}

//Checking validity of string
bool Assembler::IsValid(const std::string &str) {
	for (auto &c : str) {
		if (!(std::isalnum(c) || (c == ',') || (c == ' ') || (c == '#') || (c == ':')))
			return false;
	}
	return true;
}

void Assembler::StoreData() {
	std::ofstream outdata;
	outdata.open(m_filename + std::string(".hex"));
	for (auto &s : OpcodeOut) {
		std::cout << (int)s << std::endl;
		outdata << std::hex << (int)s << std::endl;
	}
	outdata.close();
}

void Assembler::Error(ErrorTable E, unsigned int line) {

	if (E == 0)
		ErrorList.push_back("Undefined Symbol at: " + std::to_string(line));
	if (E == 1)
		ErrorList.push_back("Not able to parse at :" + std::to_string(line));
	if (E == 2)
		ErrorList.push_back("Data Overflow at :" + std::to_string(line));
	if (E == 3)
		ErrorList.push_back("Undfined Statement at :" + std::to_string(line));
	if (E == 4)
		ErrorList.push_back("Unusable Opcode at :" + std::to_string(line));
	ErrorCheck = E + 1;
}

int Assembler::ErrorReturn() {
	return ErrorCheck;
}

void Assembler::PrintError() {
	for (auto &i : ErrorList)
		std::cout << i << std::endl;
}

std::string Assembler::GetTimingData(uint8_t opcode) {
	switch (opcode) {
		case 0:
			return "F";
		case 1:
			return "FRR";
		case 2:
			return "FW";
		case 3:
			return "S";
		case 4:
			return "F";
		case 5:
			return "F";
		case 6:
			return "FR";
		case 7:
			return "F";
		case 8:
			return "F";
		case 9:
			return "FBB";
		case 10:
			return "FR";
		case 11:
			return "S";
		case 12:
			return "F";
		case 13:
			return "F";
		case 14:
			return "FR";
		case 15:
			return "F";
		case 16:
			return "F";
		case 17:
			return "FRR";
		case 18:
			return "FW";
		case 19:
			return "S";
		case 20:
			return "F";
		case 21:
			return "F";
		case 22:
			return "FR";
		case 23:
			return "F";
		case 24:
			return "F";
		case 25:
			return "FBB";
		case 26:
			return "FR";
		case 27:
			return "S";
		case 28:
			return "F";
		case 29:
			return "F";
		case 30:
			return "FR";
		case 31:
			return "F";
		case 32:
			return "F";
		case 33:
			return "FRR";
		case 34:
			return "FRRWW";
		case 35:
			return "S";
		case 36:
			return "F";
		case 37:
			return "F";
		case 38:
			return "FR";
		case 39:
			return "F";
		case 40:
			return "F";
		case 41:
			return "FBB";
		case 42:
			return "FRRRR";
		case 43:
			return "S";
		case 44:
			return "F";
		case 45:
			return "F";
		case 46:
			return "FR";
		case 47:
			return "F";
		case 48:
			return "F";
		case 49:
			return "FRR";
		case 50:
			return "FRRW";
		case 51:
			return "S";
		case 52:
			return "FRW";
		case 53:
			return "FRW";
		case 54:
			return "FRW";
		case 55:
			return "F";
		case 56:
			return "F";
		case 57:
			return "FBB";
		case 58:
			return "FRRR";
		case 59:
			return "S";
		case 60:
			return "F";
		case 61:
			return "F";
		case 62:
			return "FR";
		case 63:
			return "F";
		case 64:
			return "F";
		case 65:
			return "F";
		case 66:
			return "F";
		case 67:
			return "F";
		case 68:
			return "F";
		case 69:
			return "F";
		case 70:
			return "FR";
		case 71:
			return "F";
		case 72:
			return "F";
		case 73:
			return "F";
		case 74:
			return "F";
		case 75:
			return "F";
		case 76:
			return "F";
		case 77:
			return "F";
		case 78:
			return "FR";
		case 79:
			return "F";
		case 80:
			return "F";
		case 81:
			return "F";
		case 82:
			return "F";
		case 83:
			return "F";
		case 84:
			return "F";
		case 85:
			return "F";
		case 86:
			return "FR";
		case 87:
			return "F";
		case 88:
			return "F";
		case 89:
			return "F";
		case 90:
			return "F";
		case 91:
			return "F";
		case 92:
			return "F";
		case 93:
			return "F";
		case 94:
			return "FR";
		case 95:
			return "F";
		case 96:
			return "F";
		case 97:
			return "F";
		case 98:
			return "F";
		case 99:
			return "F";
		case 100:
			return "F";
		case 101:
			return "F";
		case 102:
			return "FR";
		case 103:
			return "F";
		case 104:
			return "F";
		case 105:
			return "F";
		case 106:
			return "F";
		case 107:
			return "F";
		case 108:
			return "F";
		case 109:
			return "F";
		case 110:
			return "FR";
		case 111:
			return "F";
		case 112:
			return "FW";
		case 113:
			return "FW";
		case 114:
			return "FW";
		case 115:
			return "FW";
		case 116:
			return "FW";
		case 117:
			return "FW";
		case 118:
			return "FB";
		case 119:
			return "FW";
		case 120:
			return "F";
		case 121:
			return "F";
		case 122:
			return "F";
		case 123:
			return "F";
		case 124:
			return "F";
		case 125:
			return "F";
		case 126:
			return "FR";
		case 127:
			return "F";
		case 128:
			return "F";
		case 129:
			return "F";
		case 130:
			return "F";
		case 131:
			return "F";
		case 132:
			return "F";
		case 133:
			return "F";
		case 134:
			return "FR";
		case 135:
			return "F";
		case 136:
			return "F";
		case 137:
			return "F";
		case 138:
			return "F";
		case 139:
			return "F";
		case 140:
			return "F";
		case 141:
			return "F";
		case 142:
			return "FR";
		case 143:
			return "F";
		case 144:
			return "F";
		case 145:
			return "F";
		case 146:
			return "F";
		case 147:
			return "F";
		case 148:
			return "F";
		case 149:
			return "F";
		case 150:
			return "FR";
		case 151:
			return "F";
		case 152:
			return "F";
		case 153:
			return "F";
		case 154:
			return "F";
		case 155:
			return "F";
		case 156:
			return "F";
		case 157:
			return "F";
		case 158:
			return "FR";
		case 159:
			return "F";
		case 160:
			return "F";
		case 161:
			return "F";
		case 162:
			return "F";
		case 163:
			return "F";
		case 164:
			return "F";
		case 165:
			return "F";
		case 166:
			return "FR";
		case 167:
			return "F";
		case 168:
			return "F";
		case 169:
			return "F";
		case 170:
			return "F";
		case 171:
			return "F";
		case 172:
			return "F";
		case 173:
			return "F";
		case 174:
			return "FR";
		case 175:
			return "F";
		case 176:
			return "F";
		case 177:
			return "F";
		case 178:
			return "F";
		case 179:
			return "F";
		case 180:
			return "F";
		case 181:
			return "F";
		case 182:
			return "FR";
		case 183:
			return "F";
		case 184:
			return "F";
		case 185:
			return "F";
		case 186:
			return "F";
		case 187:
			return "F";
		case 188:
			return "F";
		case 189:
			return "F";
		case 190:
			return "FR";
		case 191:
			return "F";
		case 192:
			return "SRR";
		case 193:
			return "FRR";
		case 194:
			return "FRR";
		case 195:
			return "FRR";
		case 196:
			return "SRRWW";
		case 197:
			return "SWW";
		case 198:
			return "FR";
		case 199:
			return "SWW";
		case 200:
			return "SRR";
		case 201:
			return "SRR";
		case 202:
			return "FRR";
		case 203:
			return "F";
		case 204:
			return "SRRWW";
		case 205:
			return "SRRWW";
		case 206:
			return "FR";
		case 207:
			return "SWW";
		case 208:
			return "SRR";
		case 209:
			return "FRR";
		case 210:
			return "FRR";
		case 211:
			return "FRO";
		case 212:
			return "SRRWW";
		case 213:
			return "SWW";
		case 214:
			return "FR";
		case 215:
			return "SWW";
		case 216:
			return "SRR";
		case 217:
			return "F";
		case 218:
			return "FRR";
		case 219:
			return "FRI";
		case 220:
			return "SRRWW";
		case 221:
			return "F";
		case 222:
			return "FR";
		case 223:
			return "SWW";
		case 224:
			return "SRR";
		case 225:
			return "FRR";
		case 226:
			return "FRR";
		case 227:
			return "FRRWW";
		case 228:
			return "SRRWW";
		case 229:
			return "SWW";
		case 230:
			return "FR";
		case 231:
			return "SWW";
		case 232:
			return "SRR";
		case 233:
			return "S";
		case 234:
			return "FRR";
		case 235:
			return "F";
		case 236:
			return "SRRWW";
		case 237:
			return "F";
		case 238:
			return "FR";
		case 239:
			return "SWW";
		case 240:
			return "SRR";
		case 241:
			return "FRR";
		case 242:
			return "FRR";
		case 243:
			return "F";
		case 244:
			return "SRRWW";
		case 245:
			return "SWW";
		case 246:
			return "FR";
		case 247:
			return "SWW";
		case 248:
			return "SRR";
		case 249:
			return "S";
		case 250:
			return "FRR";
		case 251:
			return "F";
		case 252:
			return "SRRWW";
		case 253:
			return "F";
		case 254:
			return "FR";
		case 255:
			return "SWW";
		default: return "0";
	}
}

//Default constructor without file name
Assembler::Assembler() {

}


std::string Assembler::GetOpcodeList() {
	std::string op = "";
	for (auto &i : OpcodeOut) {
		op = op + "," + std::to_string(i);
	};
	return op;
}
