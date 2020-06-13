extends Control

#Main module
var Cpu = module8085.new()


#Global Variables
var table = {0:"0",1:"1",2:"2",3:"3",4:"4",5:"5",6:"6",7:"7",8:"8",9:"9",10:"A",11:"B",12:"C",13:"D",14:"E",
	15:"F"} #For int to hex conversion
	
	
#Opcode List
var OpcodeDictionary = { "NOP": 00,
 "LXIB,XXXX":01 ,
 "STAXB": 02 ,
 "INXB": 03 ,
 "INRB":  04 ,
 "DCRB":  05 ,
 "MVIB,XX": 06 ,
 "RLC":  07 ,
 "xxxxxxxxxx":  8 ,
 "DADB": 9 ,
 "LDAXB":  10 ,
 "DCXB":  11 ,
 "INRC":  12 ,
 "DCRC":  13 ,
 "MVIC,XX": 14 ,
 "RRC":  15 ,
 "xxxxxxxx":16,
 "LXID,XXXX": 17 ,
 "STAXD":  18 ,
 "INXD":  19 ,
 "INRD":  20 ,
 "DCRD":  21 ,
 "MVID,XX": 22 ,
 "RAL":  23 ,
 "xxxxxxxx1": 24 ,
 "DADD": 25 ,
 "LDAXD":  26 ,
 "DCXD":  27 ,
 "INRE":  28 ,
 "DCRE":  29 ,
 "MVIE,XX": 30 ,
 "RAR":  31 ,
 "RIM":  32 ,
 "LXIH,XXXX": 33 ,
 "SHLDXXXX":  34 ,
 "INXH":  35 ,
 "INRH":  36 ,
 "DCRH":  37 ,
 "MVIH,XX": 38 ,
 "DAA":  39 ,
 "xxxxxxxx2":  40 ,
 "DADH": 41 ,
 "LHLDXXXX": 42 ,
 "DCXH":  43 ,
 "INRL":  44 ,
 "DCRL":  45 ,
 "MVIL,XX": 46 ,
 "CMA":  47 ,
 "SIM":  48 ,
 "LXISP,XXXX": 49 ,
 "STAXXXX":  50 ,
 "INXSP":  51 ,
 "INRM":  52 ,
 "DCRM":  53 ,
 "MVIM,XX": 54 ,
 "STC":  55 ,
 "xxxxxxxx3":  56 ,
 "DADSP": 57 ,
 "LDAXXXX": 58 ,
 "DCXSP":  59 ,
 "INRA":  60 ,
 "DCRA":  61 ,
 "MVIA,XX": 62 ,
 "CMC":  63 ,
 "MOVB,B": 64 ,
 "MOVB,C":  65 ,
 "MOVB,D":  66 ,
 "MOVB,E":  67 ,
 "MOVB,H":  68 ,
 "MOVB,L":  69 ,
 "MOVB,M":  70 ,
 "MOVB,A":  71 ,
 "MOVC,B":  72 ,
 "MOVC,C":  73 ,
 "MOVC,D":  74 ,
 "MOVC,E":  75 ,
 "MOVC,H":  76 ,
 "MOVC,L":  77 ,
 "MOVC,M":  78 ,
 "MOVC,A":  79 ,
 "MOVD,B":  80 ,
 "MOVD,C":  81 ,
 "MOVD,D":  82 ,
 "MOVD,E":  83 ,
 "MOVD,H":  84 ,
 "MOVD,L":  85 ,
 "MOVD,M":  86 ,
 "MOVD,A":  87 ,
 "MOVE,B":  88 ,
 "MOVE,C":  89 ,
 "MOVE,D":  90 ,
 "MOVE,E":  91 ,
 "MOVE,H":  92 ,
 "MOVE,L":  93 ,
 "MOVE,M":  94 ,
 "MOVE,A":  95 ,
 "MOVH,B":  96 ,
 "MOVH,C":  97 ,
 "MOVH,D":  98 ,
 "MOVH,E":  99 ,
 "MOVH,H":  100 ,
 "MOVH,L":  101 ,
 "MOVH,M":  102 ,
 "MOVH,A":  103 ,
 "MOVL,B":  104 ,
 "MOVL,C":  105 ,
 "MOVL,D":  106 ,
 "MOVL,E":  107 ,
 "MOVL,H":  108 ,
 "MOVL,L":  109 ,
 "MOVL,M":  110 ,
 "MOVL,A":  111 ,
 "MOVM,B":  112 ,
 "MOVM,C":  113 ,
 "MOVM,D":  114 ,
 "MOVM,E":  115 ,
 "MOVM,H":  116 ,
 "MOVM,L":  117 ,
 "HLT":  118 ,
 "MOVM,A":  119 ,
 "MOVA,B":  120 ,
 "MOVA,C":  121 ,
 "MOVA,D":  122 ,
 "MOVA,E":  123 ,
 "MOVA,H":  124 ,
 "MOVA,L":  125 ,
 "MOVA,M":  126 ,
 "MOVA,A":  127 ,
 "ADDB":  128 ,
 "ADDC":  129 ,
 "ADDD":  130 ,
 "ADDE":  131 ,
 "ADDH":  132 ,
 "ADDL":  133 ,
 "ADDM":  134 ,
 "ADDA":  135 ,
 "ADCB":  136 ,
 "ADCC":  137 ,
 "ADCD":  138 ,
 "ADCE":  139 ,
 "ADCH":  140 ,
 "ADCL":  141 ,
 "ADCM":  142 ,
 "ADCA":  143 ,
 "SUBB":  144 ,
 "SUBC":  145 ,
 "SUBD":  146 ,
 "SUBE":  147 ,
 "SUBH":  148 ,
 "SUBL":  149 ,
 "SUBM":  150 ,
 "SUBA":  151 ,
 "SBBB":  152 ,
 "SBBC":  153 ,
 "SBBD":  154 ,
 "SBBE":  155 ,
 "SBBH":  156 ,
 "SBBL":  157 ,
 "SBBM":  158 ,
 "SBBA":  159 ,
 "ANAB":  160 ,
 "ANAC":  161 ,
 "ANAD":  162 ,
 "ANAE":  163 ,
 "ANAH":  164 ,
 "ANAL":  165 ,
 "ANAM":  166 ,
 "ANAA":  167 ,
 "XRAB":  168 ,
 "XRAC":  169 ,
 "XRAD":  170 ,
 "XRAE":  171 ,
 "XRAH":  172 ,
 "XRAL":  173 ,
 "XRAM":  174 ,
 "XRAA":  175 ,
 "ORAB":  176 ,
 "ORAC":  177 ,
 "ORAD":  178 ,
 "ORAE":  179 ,
 "ORAH":  180 ,
 "ORAL":  181 ,
 "ORAM":  182 ,
 "ORAA":  183 ,
 "CMPB":  184 ,
 "CMPC":  185 ,
 "CMPD":  186 ,
 "CMPE":  187 ,
 "CMPH":  188 ,
 "CMPL":  189 ,
 "CMPM":  190 ,
 "CMPA":  191 ,
 "RNZ":  192 ,
 "POPB":  193 ,
 "JNZXXXX":  194 ,
 "JMPXXXX":  195 ,
 "CNZXXXX":  196 ,
 "PUSH B":  197 ,
 "ADIXX":  198 ,
 "RST0":  199 ,
 "RZ":  200 ,
 "RET":  201 ,
 "JZXXXX":  202 ,
 "xxxxxxxx4":203 ,
 "CZXXXX":  204 ,
 "CALLXXXX":  205 ,
 "ACIXX":  206 ,
 "RST1":  207 ,
 "RNC":  208 ,
 "POPD":  209 ,
 "JNCXXXX":  210 ,
 "OUTXX":  211 ,
 "CNCXXXX":  212 ,
 "PUSH D":  213 ,
 "SUIXX":  214 ,
 "RST2":  215 ,
 "RC":  216 ,
 "xxxxxxxx5":  217 ,
 "JCXXXX":  218 ,
 "INXX":  219 ,
 "CCXXXX":  220 ,
 "xxxxxxxx6":  221 ,
 "SBIXX":  222 ,
 "RST3":  223 ,
 "RPO":  224 ,
 "POPH":  225 ,
 "JPOXXXX":  226 ,
 "XTHL":  227 ,
 "CPOXXXX":  228 ,
 "PUSHH":  229 ,
 "ANIXX":  230 ,
 "RST4":  231 ,
 "RPE":  232 ,
 "PCHL":  233 ,
 "JPEXXXX":  234 ,
 "XCHG":  235 ,
 "CPEXXXX":  236 ,
 "xxxxxxxx7":  237 ,
 "XRIXX":  238 ,
 "RST5":  239 ,
 "RP":  240 ,
 "POPPSW":  241 ,
 "JPXXXX":  242 ,
 "DI":  243 ,
 "CPXXXX":  244 ,
 "PUSHPSW":  245 ,
 "ORIXX": 246 ,
 "RST6":  247 ,
 "RM":  248 ,
 "SPHL":  249 ,
 "JMXXXX": 250 ,
 "EI": 251 ,
 "CMXXXX": 252 ,
 "xxxxxxxx8": 253 ,
 "CPIXX": 254, 
 "RST7": 255}

var Page_No : int = 0
var Current_Page_Address = "0x00" 
var Opcode_String = ""
var Byte_No = 0x0000
var Input_opcode = "00"
var Slidervalue:int  = 0

#Interrupts:


#Opcode List:
var Addr_Opcode = {}
var Arr_Opcodes = []
#Flag status

var Flags_states = []



#Intiatialzing variables for the nodes
onready var MemView_X = get_node("MemViewLabels_X")
onready var MemView_Y = get_node("MemViewLabels_Y")
onready var MemViewWindow = get_node("MemViewWindow")
onready var Flags = get_node("Flags")
onready var HS_Slider = get_node("HSlider")
onready var Input_Box = get_node("Popup/LineEdit")
onready var Pop_up = get_node("Popup")
onready var Textedit = get_node("TextEdit")
onready var Output = get_node("status")
onready var Registers = get_node("Reigisters")

onready var A = get_node("Reigisters/A")
onready var B = get_node("Reigisters/B")
onready var C = get_node("Reigisters/C")
onready var D = get_node("Reigisters/D")
onready var E = get_node("Reigisters/E")
onready var H = get_node("Reigisters/H")
onready var L = get_node("Reigisters/L")
onready var Sp = get_node("Reigisters/sp")
onready var Pc = get_node("Reigisters/pc")
onready var Psw = get_node("Reigisters/psw")
onready var Plot = get_node("TimingGraph")

var assembled:bool = false

func _ready() -> void:
	#Setting address labes for the MemView_X
	u_set_label_x()
	#Setting the address labes for the MemView_Y
	_on_HSlider_value_changed(Slidervalue)
	#Setting intial null values in the MemView
	u_set_button_texts()
	
	#Ready state
	u_out("Ready! :-)")
	#Settin up connections
	u_connect_MemView()
	u_connect_Flags()
	u_Set_Registers()
	u_get_status()
	
	pass



	
	
#For realtime calculation and adjustments
func _process(delta: float) -> void:
	pass
	
	
	
	


func _on_Assemble_pressed() -> void:
	print("assemble")
	u_Assemble()
	assembled = true
	Steps = 0
	pass 

var Steps = 0

func _on_Run_pressed() -> void:
	if(assembled):
		for i in Arr_Opcodes:
			Cpu.RunOpcode(i)
			u_Set_Registers()
			u_get_status()
			Steps = Steps + 1
		
	pass 
	


func _on_Step_pressed() -> void:
	if(Steps == 0 && assembled):
		Cpu.RunOpcode(Arr_Opcodes[Steps])
		u_Set_Registers()
		u_get_status()
		Steps = Steps + 1
	if(Steps == Arr_Opcodes.size()):
		u_out("Execution Complete")
		
	pass 


func _on_Stop_pressed() -> void:
	print("Stop")
	pass
	


func _on_HSlider_value_changed(value: float) -> void:
	var i:int = 0
	Slidervalue = value
	for child in MemView_Y.get_children():
		child.set_text(table[Page_No] + table[Slidervalue] + table[i])
		i+=1
		
	u_set_Current_Address()
	u_set_button_texts()
	
	pass 

	
func _on_NextPage_pressed() -> void:
	if(Page_No < 15):
		Page_No+=1
		_on_HSlider_value_changed(Slidervalue)
		u_set_Current_Address()
		u_set_button_texts()
	pass 


func _on_PrevPage_pressed() -> void:
	if(Page_No > 0):
		Page_No-=1
		_on_HSlider_value_changed(Slidervalue)
		u_set_Current_Address()	
		u_set_button_texts()
	pass 


#Functionality of the address Input-------------------------------------------------------------------------
func _on_AddressInput_text_entered(new_text: String) -> void:
	if(!new_text.is_valid_hex_number()):
		_on_AddressInput_text_change_rejected()
		return
	#To go to the new address:
	Current_Page_Address = new_text
	new_text.split("")
	var pn:String = "0x" + new_text[0]
	var sl:String = "0x" + new_text[1]
	Page_No = pn.hex_to_int()
	HS_Slider.set_value(sl.hex_to_int())
	pass 
#Error Message
func _on_AddressInput_text_change_rejected() -> void:
	u_out("Invalied Address")
	pass 

func _on_AddressInput_text_changed(new_text: String) -> void:
	pass 

func _on_LineEdit_text_entered(new_text: String) -> void:
	if(!new_text.is_valid_hex_number()):
		_on_LineEdit_text_change_rejected()
		return
	Addr_Opcode[Byte_No] = new_text
	Pop_up.hide()
	u_set_button_texts()
	Input_Box.clear()
	pass 

func _on_Popup_popup_hide() -> void:
	pass 

#Status reset
func _on_status_clear_pressed() -> void:
	Output.text = "Ready"
	pass 

func _on_LineEdit_text_change_rejected() -> void:
	u_out("Invalid Opcode")
	u_set_button_texts()
	pass 


func _on_pc_text_entered(new_text: String) -> void:
	print(new_text.is_valid_hex_number())
	if(new_text.is_valid_hex_number()):
		Cpu.Set_PC(("0x"+new_text).hex_to_int())
		print(Cpu.Get_PC())
		u_Set_Registers()
		return
	else:
		u_out("Invalied Input")
		u_Set_Registers()
	pass 


func _on_L_text_entered(new_text: String) -> void:
	if(new_text.is_valid_hex_number()):
		Cpu.Set_L(("0x"+new_text).hex_to_int())
		u_Set_Registers()
		return
	else:
		u_out("Invalied Input")
		u_Set_Registers()
	pass 


func _on_E_text_entered(new_text: String) -> void:
	if(new_text.is_valid_hex_number()):
		Cpu.Set_E(("0x"+new_text).hex_to_int())
		u_Set_Registers()
		return
	else:
		u_out("Invalied Input")
		u_Set_Registers()
	pass 


func _on_C_text_entered(new_text: String) -> void:
	if(new_text.is_valid_hex_number()):
		Cpu.Set_C(("0x"+new_text).hex_to_int())
		u_Set_Registers()
		return
	else:
		u_out("Invalied Input")
		u_Set_Registers()
	pass 
	

func _on_psw_text_entered(new_text: String) -> void:
	if(new_text.is_valid_hex_number()):
		print(new_text)
	else:
		u_out("Invalied Input")
		u_Set_Registers()
	pass 



func _on_A_text_entered(new_text: String) -> void:
	if(new_text.is_valid_hex_number()):
		Cpu.Set_A(("0x"+new_text).hex_to_int())
		u_Set_Registers()
		return
	else:
		u_out("Invalied Input")
		u_Set_Registers()
	pass 
	

func _on_B_text_entered(new_text: String) -> void:
	if(new_text.is_valid_hex_number()):
		Cpu.Set_B(("0x"+new_text).hex_to_int())
		u_Set_Registers()
		return
	else:
		u_out("Invalied Input")
		u_Set_Registers()
	pass 
	


func _on_D_text_entered(new_text: String) -> void:
	if(new_text.is_valid_hex_number()):
		Cpu.Set_D(("0x"+new_text).hex_to_int())
		u_Set_Registers()
		return
	else:
		u_out("Invalied Input")
		u_Set_Registers()
	pass 



func _on_sp_text_entered(new_text: String) -> void:
	if(new_text.is_valid_hex_number()):
		Cpu.Set_SP(("0x"+new_text).hex_to_int())
		u_Set_Registers()
		return
	else:
		u_out("Invalied Input")
		u_Set_Registers()
	pass 



func _on_Cleartxt_pressed() -> void:
	Textedit.text = ""
	pass






#File Saving and opening
var saved:bool = false
var pathmain:String = ""
func _on_Open_pressed() -> void:
	$OpenFile.popup()
	pass 


func _on_Save_pressed() -> void:
	if(!saved):
		$SaveFile.popup()
		return
	_on_SaveFile_file_selected(pathmain)
	
	pass 
	



func _on_OpenFile_file_selected(path: String) -> void:
	var file = File.new()
	file.open(path,1)
	Textedit.text = file.get_as_text()
	file.close()
	pass 



func _on_SaveFile_file_selected(path: String) -> void:
	if(!saved):
		pathmain = path
		var file = File.new()
		file.open(path,2)
		file.store_string(Textedit.text)
		file.close()
		saved = true;
		return
	else:
		pathmain = path
		var file = File.new()
		file.open(path,2)
		file.store_string(Textedit.text)
		file.close()
	pass 





func _on_InPortSlider_value_changed(value: float) -> void:
	var s = u_int_to_hex(int(value)*10,false)
	s = s.replace("0x","")
	$Inaddr.text = s
	
	$in.text = String(Cpu.GetAtPort(int(value)*10))
	
	pass 


func _on_OutPortSlider_value_changed(value: float) -> void:
	var s = u_int_to_hex(int(value)*10)
	s = s.replace("0x","")
	$Outaddr.text = s
	
	$outout.text = String(Cpu.GetAtPort(s.hex_to_int()))
	pass 


func _on_in_text_entered(new_text: String) -> void:
	if(new_text.is_valid_hex_number()):
		Cpu.SetAtPort(int($InPortSlider.get_value()*10),new_text.hex_to_int())
		return
	u_out("Invalied Input")
	pass 
	
	
	
	
	
	

#Helper functions ("u_") :

func u_connect_MemView() ->void:
	for child in MemViewWindow.get_children():
		child.connect("pressed",self, "u_on_button_pressed",[child.get_position_in_parent(),child])
	pass

func u_connect_Flags() ->void:
	for child in Flags.get_children():
		child.connect("pressed",self,"u_on_bit_toggled",[])
	pass
	
func u_on_bit_toggled() ->void:
	var i:int = 0
	Flags_states.clear()
	for child in Flags.get_children():
		Flags_states.push_back(child.pressed)
		i = i+1
	Cpu.Set_S(Flags_states[0])
	Cpu.Set_Z(Flags_states[1])
	Cpu.Set_AC(Flags_states[3])
	Cpu.Set_P(Flags_states[5])
	Cpu.Set_CY(Flags_states[7])
	
	u_Set_Registers()
	pass
	


func u_set_label_x() -> void:
	var i:int = 0
	for child in MemView_X.get_children():
		child.set_text(table[i])
		i+=1
	pass 
	pass


func u_int_to_hex(integer, var s:bool = true) -> String:
	var remainder: int = 0
	var int_hex: int = 0
	var count: int=0
	
	var arr:Array
	var hex:String
	
	if(integer == 0):
		if(s):
			return "0x0"
		elif(!s):
			return "0"
	while(integer != 0):
		remainder = integer % 16
		arr.push_front(table[remainder])
		integer = integer/16
		count=+1
	if(s):
		arr.push_front("0x")	
	hex = PoolStringArray(arr).join("")
	return hex
	pass
	

func u_set_Current_Address(): 
	Current_Page_Address = "0x" + table[Page_No] + table[int(HS_Slider.get_value())]
	pass
	
func u_define_button_texts():
	pass
	
func u_set_button_texts():
	var val = Current_Page_Address.hex_to_int()
	
	for child in MemViewWindow.get_children():
		if(Addr_Opcode.has(u_get_Byte_no(child))):
			child.text = Addr_Opcode[u_get_Byte_no(child)]
		else:
			child.text = "-"
	pass
	
#MemView functionality:
func u_on_button_pressed(var s, var c):
	Input_Box.clear()
	Byte_No = 256*Current_Page_Address.hex_to_int() + s
	Input_Box.set_position(MemViewWindow.get_position() + c.get_position())
	Pop_up.popup()
	pass

func u_get_Byte_no(var n:Node) ->int:
	return 256*Current_Page_Address.hex_to_int() + n.get_position_in_parent()
	pass

	
func u_out(var s:String) ->void:
	Output.text = Output.text + "\n" + s
	pass
		
func u_Assemble()->void:
	u_set_button_texts()
	Arr_Opcodes.clear()
	
	var regComments = RegEx.new()
	regComments.compile("([//].*$)")
	var regDataDec = RegEx.new()
	regDataDec.compile("d(?<Data>[0-9]+)")
	var regDataHex = RegEx.new()
	regDataHex.compile("x(?<Data>[0-9a-f]+)")
	var OperandData:int = 00;
	var FoundData:bool = false;
	
	
	var Opcode:int = 00
	var GotTheOpcode:bool = false
	
	var usable:String = ""
	if(!usable.empty()):
		usable = ""
	var LineNum = 0
	var DataIn:String = ""

	
	if(!Textedit.text.empty()):
		DataIn = ""
		DataIn = Textedit.text
		var Lines = DataIn.split("\n")
		for line in Lines:
			LineNum+=1	
			if(regComments.search(line)):
				usable = u_remove_comments(line)
			else:
				usable = line;
			#Getting the data
			var result = regDataDec.search(usable)
			if (result && !FoundData):
				var s:String = result.get_string("Data")
				OperandData = int(s)
				FoundData = true
				
			result = regDataHex.search(usable)
			if (result && !FoundData):
				var s:String = result.get_string("Data")
				OperandData = (("0x" + s).hex_to_int())
				FoundData = true
			
			usable = usable.to_upper()
			usable = usable.strip_escapes()
			usable = usable.strip_edges()
			
			var JustOpcode:String = usable.substr(0,usable.find(' '))
			
			if(FoundData):
				usable = u_check_Data(OperandData, usable, LineNum, JustOpcode)
			
			
			usable = usable.replace(" ", "")

			if(!u_is_valid(usable)):
				u_out("Invalid Character(s) at:" + String(LineNum))
				
			if(u_GetTheOpcode(usable) == 700000):
				u_out("Unknown Statement at:" + String(LineNum))
				return;
			
			Arr_Opcodes.push_back(u_GetTheOpcode(usable))
			if(FoundData):
				var low:int = (OperandData & 0xFF)
				var high:int = (OperandData & 0xFF00)>>8
				
				Arr_Opcodes.push_back(low);
				if (high > 0):
					Arr_Opcodes.push_back(high);
				OperandData = 00
				FoundData = false
		var n = 0
		for i in Arr_Opcodes:
			Cpu.Set_OpcodeAt(Cpu.Get_SP()+n, i)
			var s = u_int_to_hex(i)
			Addr_Opcode[n] = s.replace("0x","")
			n+=1
		u_set_button_texts()
			
			
	pass
	
func u_check_Data(var data:int, var s:String, var line:int = 0, var main:String ="")->String:
	if((data < -128) || (data > 65535)):
		u_out("Data overflow at: " + String(line))
		return s
		
	var PossibleOpcodes16bit = ["LXI", "SHLD", "LHLD", "STA", "LDA", "JNZ", "JMP", "CNZ", "JZ", "CZ", "CALL", "JNC", "CNC", "JC", "CC", "JPO", "CPO", "JPE", "CPE", "JP", "JM", "CP", "CM"]
	var PossibleOpcodes8bit = ["ADI","ACI", "OUT","SUI","IN","SBI","ANI","XRI","ORI","CPI","OUT", "IN"]
	var check:bool = false
	var val16bit:bool  = false
	
	if(!PossibleOpcodes16bit.has(main) && data>255):
		val16bit = false
		u_out("Data overflow at:" + String(line))
		return s
	elif(PossibleOpcodes16bit.has(main)):
		val16bit = true;
		if(main == "LXI"):
			s.erase(s.find(',')+1, s.length())
			s = s+"XXXX"
			return s
		else:
			s.erase(s.find(' ')+1, s.length())
			s = s+"XXXX"
			return s
	else:
		if(main == "MVI"):
			s.erase(s.find(',')+1, s.length())
			s = s+"XX"
			return s
		elif(PossibleOpcodes8bit.has(main)):
			s.erase(s.find(' '), s.length())
			s = s+"XX"	
			return s
	return s
	pass
	
func u_remove_comments(var s:String)->String:
	var i = s.find("//")
	var new = s.substr(0,i)
	if(!new.empty()):
		return new
	return s
	pass
		
	
	
func u_remove_middlespace(var s:String)->String:
	var i:int = s.find(" ")
	s.erase(i,(int(s.length() - i)))
	return s
	pass
	
func u_is_valid(var s:String)->bool:
	for ch in s:
		if(!(ch.is_valid_identifier() || ch == "," || ch.is_valid_integer()) || ch == "_"):
			return false
			pass
	return true	
	pass
	
func u_GetTheOpcode(var s:String)->int:
	if(OpcodeDictionary.has(s)):
		return OpcodeDictionary[s]
	else:
		return 700000
	
	pass
	
func u_Set_Registers()->void:
	A.text = u_int_to_hex(Cpu.Get_A());
	B.text = u_int_to_hex(Cpu.Get_B());
	C.text = u_int_to_hex(Cpu.Get_C());
	D.text = u_int_to_hex(Cpu.Get_D());
	E.text = u_int_to_hex(Cpu.Get_E());
	H.text = u_int_to_hex(Cpu.Get_H());
	L.text = u_int_to_hex(Cpu.Get_L());
	
	Pc.text = u_int_to_hex(Cpu.Get_PC());
	Sp.text = u_int_to_hex(Cpu.Get_SP());
	
	U_SetPSW();
	pass	
	
func U_SetPSW()->void:
	var value:int = 0
	value = (int(Cpu.Get_S()) << 7 | value)
	value = (int(Cpu.Get_Z()) << 5 | value)
	value = (int(Cpu.Get_AC()) << 3 | value)
	value = (int(Cpu.Get_P()) << 1 |value )
	value = (int(Cpu.Get_CY()) << 0 | value)
	
	Psw.text = u_int_to_hex(value)
	pass
	
func u_get_status()->void:
	$Flags/AC.pressed = bool(Cpu.Get_AC())
	$Flags/CY.pressed = bool(Cpu.Get_CY())
	$Flags/Z.pressed = bool(Cpu.Get_Z())
	$Flags/P.pressed = bool(Cpu.Get_P())
	$Flags/S.pressed = bool(Cpu.Get_S())
	pass
	


func _on_H_text_entered(new_text: String) -> void:
	if(new_text.is_valid_hex_number()):
		Cpu.Set_H(("0x"+new_text).hex_to_int())
		u_Set_Registers()
		return
	else:
		u_out("Invalied Input")
		u_Set_Registers()
	
	pass 


func _on_TextEdit_text_changed() -> void:
	assembled = false;
	pass 


func _on_Reset2_pressed() -> void:
	Cpu = module8085.new()
	Addr_Opcode.clear()
	Arr_Opcodes.clear()
	u_set_Current_Address()
	u_set_button_texts()
	u_Set_Registers()
	u_get_status()
	Plot.reset()
	pass 


func _on_GraphOpcode_text_entered(new_text: String) -> void:
	if(new_text.is_valid_hex_number()):
		Plot.DrawOpcode(("0x"+new_text).hex_to_int())
	pass 
