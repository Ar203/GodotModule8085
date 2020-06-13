extends ColorRect

#Setting up nodes for easier access
onready var clk = get_node("clk")
onready var ad1 = get_node("ad1")
onready var ad2 = get_node("ad2")
onready var a1 = get_node("a1")
onready var a2 = get_node("a2")
onready var ale = get_node("ale")
onready var rd = get_node("rd")
onready var wr = get_node("wr")
onready var io_m = get_node("io_m")
onready var s = get_node("s")
onready var s1 = get_node("s1")
onready var div = get_node("Division")



#Basic constants
var unit3=24
var unit=8
var unit4=32

#Original posistion
var pos_clk = Vector2(0,0)
var pos_ad1 = Vector2(0,2*unit4)
var pos_ad2 = Vector2(0,2*unit4)
var pos_a1 = Vector2(0,4*unit4)
var pos_a2 = Vector2(0,4*unit4)
var pos_ale = Vector2(0,6*unit4-unit)
var pos_rd = Vector2(0,7*unit4)
var pos_wr = Vector2(0,8*unit4)
var pos_io_m = Vector2(0,9*unit4)
var pos_s = Vector2(0,10*unit4)
var pos_s1 = Vector2(0,11*unit4)




var t_cycle_count = 0


func DrawOpcode(var val:int) ->void:
	reset()
	
	var s:String = GetTimingData(val)
	print(s)
	for ch in s:
		if(ch == 'F'):
			Opcodefetch()
		if(ch == 'S'):
			Opcodefetch(6)
		if(ch == 'R'):
			MemoryRead()
		if(ch == 'W'):
			MemoryWrite()
		if(ch == 'I'):
			IORead()
		if(ch == 'O'):
			IOWrite()
		if(ch == 'B'):
			BusIdle()
	pass

func reset()->void:

	pass

func _ready() -> void:
	#Setting up the basic object properties
	drawgraph() # Draws the cage
	set_position_default()
	set_color_default()
	
	pass 
	
func set_position_default()-> void:
		clk.set_point_position(0,pos_clk) #(0,8)
		ad1.set_point_position(0,pos_ad1) #(0,64)
		ad2.set_point_position(0,pos_ad2) 
		a1.set_point_position(0,pos_a1 )#(0,128)
		a2.set_point_position(0,pos_a2)
		ale.set_point_position(0,pos_ale) #(0,160)
		rd.set_point_position(0,pos_rd) #(0,192)
		wr.set_point_position(0,pos_wr) #(0,256)
		io_m.set_point_position(0,pos_io_m)#(0,288)
		s.set_point_position(0,pos_s)#(0,320)
		s1.set_point_position(0,pos_s1)#(0,352)
		pass
		
func set_color_default() ->void:
		clk.set_default_color(Color(0.53,0.81,0.92,1))
		ad1.set_default_color(Color(0,0,1,1))
		ad2.set_default_color(Color(0,0,1,1))
		a1.set_default_color(Color(0,0.39,0,1))
		a2.set_default_color(Color(0,0.39,0,1))
		ale.set_default_color(Color(1,0.08,0.58,1)) 
		rd.set_default_color(Color(1,0,0,1))
		rd.set_default_color(Color( 1, 0.75, 0.8, 1 ))
		io_m.set_default_color(Color (0.63, 0.13, 0.94, 1))
		s.set_default_color(Color( 0, 1, 0, 1 ))
		s1.set_default_color(Color( 1, 0.08, 0.58, 1 ))
		pass
	
func drawgraph() ->void: # To draw white lines between tstates
	div.set_point_position(0,Vector2(0,0)) #(0,8)
	div.set_default_color(Color(1,0.75,0.80,1))
	div.add_point(Vector2(0,0))
	var s = 64
	for i in range(20):
		div.add_point(Vector2(s*(i+1),0))
		div.add_point(Vector2(s*(i+1),392))
		div.add_point(Vector2(s*(i+1),0))
	div.add_point(Vector2(s*20,392))
	div.add_point(Vector2(0,392))
	div.add_point(Vector2(0,0))
	pass

func get_high(var n:Node,var pos:Vector2 ,var t:int = 0) ->void: #To go high
	var s = n.get_point_position(n.get_point_count()-1)
	if(int(pos.y) <= int(s.y)):
		if(!(pos.y -(s.y - unit3)>unit3)):
			if(pos.y ==pos_a1.y || pos.y == pos_ad1.y || pos.y == pos_clk.y):
				n.add_point(Vector2(s.x+unit,s.y-unit3))
			else:
				n.add_point(Vector2(s.x+unit,pos.y-unit3))
	var t2 = 0
	while(t>t2):
		s = n.get_point_position(n.get_point_count()-1)
		n.add_point(Vector2(s.x+unit3,s.y))
		t2 = t2+1
		
	print(s)
	pass

func get_low(var n:Node,var pos:Vector2, var t:int = 0) ->void:# To get low
	
	var s = n.get_point_position(n.get_point_count()-1)
	if(int(pos.y) >= int(s.y)):
		if(!((s.y +unit3) - pos.y) > unit3):
			if(pos.y ==pos_a1.y || pos.y == pos_ad1.y || pos.y == pos_clk.y):
				n.add_point(Vector2(s.x+unit,s.y+unit3))
			else:
				n.add_point(Vector2(s.x+unit,pos.y))
	var t2 = 0
	while(t>t2):
		s = n.get_point_position(n.get_point_count()-1)
		n.add_point(Vector2(s.x+unit3,s.y))
		t2 = t2+1
	pass

func get_stable(var n:Node, var t:int = 0) ->void:# To stay high or low
	var s = n.get_point_position(n.get_point_count()-1)
	n.add_point(Vector2(s.x+unit3,s.y))
	var t2 = 0
	while(t>t2):
		s = n.get_point_position(n.get_point_count()-1)
		n.add_point(Vector2(s.x+unit3,s.y))
		t2 = t2+1
	pass
	
func add_unit_till(var n:Node ,var t:int = 0) ->void:
	var s = n.get_point_position(n.get_point_count()-1)
	while(int(s.x)%64 != 0 ):
		s = n.get_point_position(n.get_point_count()-1)
		n.add_point(Vector2(s.x+1,s.y))
	pass

func DrawClock(var n:int)->void:
	for i in range(n):
		get_low(clk,pos_clk)
		get_stable(clk)
		get_high(clk,pos_clk)
		get_stable(clk)
	pass
	
func Opcodefetch(var t:int = 4)->void:
	DrawClock(t)
	
	#Low Address bits
	get_low(ad1,pos_ad1,3)
	get_high(ad2,pos_ad2,3)
	get_low(ad2,pos_ad2)
	get_high(ad1,pos_ad1)
	get_high(ad1,pos_ad1,3)
	get_low(ad2,pos_ad2,3)
	get_low(ad1,pos_ad1)
	get_high(ad2,pos_ad2)
	get_stable(ad1,2)
	get_stable(ad2,2)
	
	
	#High address bits
	get_low(a1,pos_a1)
	get_high(a2,pos_a2)
	get_stable(a1,5)
	get_stable(a2,5)
	get_low(a2,pos_a2)
	get_high(a1,pos_a1)
	get_high(a1,pos_a1)
	get_low(a2,pos_a2)
	
	get_stable(a1,2)
	get_stable(a2,2)
	get_high(a2,pos_a2)
	get_low(a1,pos_a1)
	
	#Ale
	DrawAle(t)
	
	#write
	get_high(wr,pos_wr,10)
	
	#Read
	get_high(rd,pos_rd)
	get_stable(rd,2)
	get_low(rd,pos_rd,3)
	get_high(rd,pos_rd,3)
	
	#io_m
	get_low(io_m,pos_io_m,10)
	
	#s0/1
	get_high(s,pos_s,10)
	get_high(s1,pos_s1,10)
	
	if(t == 6):
		get_stable(ale,4)
		get_stable(a1,4)
		get_stable(a2,4)
		get_stable(ad1,4)
		get_stable(ad2,4)
		get_stable(io_m,4)
		get_stable(rd,4)
		get_stable(wr,4)
		get_stable(s,4)
		get_stable(s1,4)
		
	
	
	
	#drawing till end
	add_unit_till(a1,t)
	add_unit_till(a2,t)
	add_unit_till(ad1,t)
	add_unit_till(ad2,t)
	add_unit_till(io_m,t)
	add_unit_till(rd,t)
	add_unit_till(wr,t)
	add_unit_till(s,t)
	add_unit_till(s1,t)

	pass
		
func MemoryRead(var t:int = 3) ->void:
	
	#clock
	DrawClock(t)
	#Low Address bits
	get_low(ad1,pos_ad1, 2)
	get_high(ad2,pos_ad2, 2)
	
	get_low(ad2,pos_ad2)
	get_high(ad1,pos_ad1)
	get_stable(ad1)
	get_stable(ad2)
	get_high(ad2,pos_ad2,3)
	get_low(ad1,pos_ad1,3)
	get_low(ad2,pos_ad2)
	get_high(ad1,pos_ad1)
	
	
	#High address bits
	get_low(a1,pos_a1,7)
	get_high(a2,pos_a2,7)

	
	#Ale
	DrawAle(t)
	
	#write
	get_high(wr,pos_wr,7)
	
	#Read
	get_high(rd,pos_rd)
	get_stable(rd,2)
	get_low(rd,pos_rd,3)
	get_high(rd,pos_rd)
	
	#io_m
	get_low(io_m,pos_io_m,7)
	
	#s0/1
	get_high(s,pos_s,7)
	get_high(s1,pos_s1,7)
	
	#drawing till end
	add_unit_till(a1,t)
	add_unit_till(a2,t)
	add_unit_till(ad1,t)
	add_unit_till(ad2,t)
	add_unit_till(io_m,t)
	add_unit_till(rd,t)
	add_unit_till(wr,t)
	add_unit_till(s,t)
	add_unit_till(s1,t)


	pass
	
	
func MemoryWrite(var t:int = 3) ->void:
	
	
	#clock
	DrawClock(t)
	#Low Address bits
	get_low(ad1,pos_ad1, 2)
	get_high(ad2,pos_ad2, 2)
	
	get_low(ad2,pos_ad2)
	get_high(ad1,pos_ad1)
	get_high(ad2,pos_ad2,4)
	get_low(ad1,pos_ad1,4)
	get_low(ad2,pos_ad2)
	get_high(ad1,pos_ad1)
	
	
	#High address bits
	get_low(a1,pos_a1,7)
	get_high(a2,pos_a2,7)

	
	#Ale
	DrawAle(t)
	
	#write
	get_high(wr,pos_wr)
	get_stable(wr,2)
	get_low(wr,pos_wr,3)
	get_high(wr,pos_wr)
	
	#Read
	get_high(rd,pos_rd,7)
	
	#io_m
	get_low(io_m,pos_io_m,7)
	
	#s0/1
	get_high(s,pos_s,7)
	get_low(s1,pos_s1,7)
	
	#drawing till end
	add_unit_till(a1,t)
	add_unit_till(a2,t)
	add_unit_till(ad1,t)
	add_unit_till(ad2,t)
	add_unit_till(io_m,t)
	add_unit_till(rd,t)
	add_unit_till(wr,t)
	add_unit_till(s,t)
	add_unit_till(s1,t)


	pass
	
func IORead(var t:int = 3) ->void:
	
	#clock
	DrawClock(t)
	#Low Address bits
	get_low(ad1,pos_ad1, 2)
	get_high(ad2,pos_ad2, 2)
	
	get_low(ad2,pos_ad2)
	get_high(ad1,pos_ad1)
	get_stable(ad1)
	get_stable(ad2)
	get_high(ad2,pos_ad2,3)
	get_low(ad1,pos_ad1,3)
	get_low(ad2,pos_ad2)
	get_high(ad1,pos_ad1)
	
	
	#High address bits
	get_low(a1,pos_a1,7)
	get_high(a2,pos_a2,7)

	
	#Ale
	DrawAle(t)
	
	#write
	get_high(wr,pos_wr,7)
	
	#Read
	get_high(rd,pos_rd)
	get_stable(rd,2)
	get_low(rd,pos_rd,3)
	get_high(rd,pos_rd)
	
	#io_m
	get_high(io_m,pos_io_m,7)
	
	#s0/1
	get_low(s,pos_s,7)
	get_high(s1,pos_s1,7)
	
	#drawing till end
	add_unit_till(a1,t)
	add_unit_till(a2,t)
	add_unit_till(ad1,t)
	add_unit_till(ad2,t)
	add_unit_till(io_m,t)
	add_unit_till(rd,t)
	add_unit_till(wr,t)
	add_unit_till(s,t)
	add_unit_till(s1,t)


	pass
	
	
	
func IOWrite(var t:int = 3) ->void:
	
	#clock
	DrawClock(t)
	#Low Address bits
	get_low(ad1,pos_ad1, 2)
	get_high(ad2,pos_ad2, 2)
	
	get_low(ad2,pos_ad2)
	get_high(ad1,pos_ad1)
	get_stable(ad1)
	get_stable(ad2)
	get_high(ad2,pos_ad2,3)
	get_low(ad1,pos_ad1,3)
	get_low(ad2,pos_ad2)
	get_high(ad1,pos_ad1)
	
	
	#High address bits
	get_low(a1,pos_a1,7)
	get_high(a2,pos_a2,7)

	
	#Ale
	DrawAle(t)
	
	#write
	get_high(wr,pos_wr)
	get_stable(wr,2)
	get_low(wr,pos_wr,3)
	get_high(wr,pos_wr)
	
	
	#Read
	get_high(rd,pos_rd,7)
	
	#io_m
	
	get_high(io_m,pos_io_m,7)
	
	#s0/1
	get_low(s,pos_s,7)
	get_high(s1,pos_s1,7)
	
	#drawing till end
	add_unit_till(a1,t)
	add_unit_till(a2,t)
	add_unit_till(ad1,t)
	add_unit_till(ad2,t)
	add_unit_till(io_m,t)
	add_unit_till(rd,t)
	add_unit_till(wr,t)
	add_unit_till(s,t)
	add_unit_till(s1,t)


	pass

func BusIdle(var t:int = 6) ->void:
	
	#clock
	DrawClock(t)
	#Low Address bits
	get_stable(ad1,7)
	get_stable(ad1,7)
	
	#High address bits
	get_low(a1,pos_a1,7)
	get_high(a2,pos_a2,7)
	get_low(a2,pos_a2)
	get_high(a1,pos_a1)
	get_low(a1,pos_a1,7)
	get_high(a2,pos_a2,7)
	get_low(a2,pos_a2)
	get_high(a1,pos_a1)
	

	
	#Ale
	DrawAle(13)
	
	#write
	get_high(wr,pos_wr,14)
	
	
	
	#Read
	get_high(rd,pos_rd,14)
	
	#io_m
	get_low(io_m,pos_io_m,14)
	
	#s0/1
	get_low(s,pos_s,14)
	get_low(s1,pos_s1,14)
	
	#drawing till end
	add_unit_till(a1,t)
	add_unit_till(a2,t)
	add_unit_till(ad1,t)
	add_unit_till(ad2,t)
	add_unit_till(io_m,t)
	add_unit_till(rd,t)
	add_unit_till(wr,t)
	add_unit_till(s,t)
	add_unit_till(s1,t)

	
	
	pass
	


func DrawAle(var t:int) ->void:
	get_high(ale,pos_ale)
	get_stable(ale)
	get_low(ale,pos_ale)
	get_stable(ale,t)
	add_unit_till(ale)
	pass
	
func GetTimingData(var x:int) ->String:
	match x:
		0:
			return "F"
		1:
			return "FRR"
		2:
			return "FW"
		3:
			return "S"
		4:
			return "F"
		5:
			return "F"
		6:
			return "FR"
		7:
			return "F"
		8:
			return "F"
		9:
			return "FBB"
		10:
			return "FR"
		11:
			return "S"
		12:
			return "F"
		13:
			return "F"
		14:
			return "FR"
		15:
			return "F"
		16:
			return "F"
		17:
			return "FRR"
		18:
			return "FW"
		19:
			return "S"
		20:
			return "F"
		21:
			return "F"
		22:
			return "FR"
		23:
			return "F"
		24:
			return "F"
		25:
			return "FBB"
		26:
			return "FR"
		27:
			return "S"
		28:
			return "F"
		29:
			return "F"
		30:
			return "FR"
		31:
			return "F"
		32:
			return "F"
		33:
			return "FRR"
		34:
			return "FRRWW"
		35:
			return "S"
		36:
			return "F"
		37:
			return "F"
		38:
			return "FR"
		39:
			return "F"
		40:
			return "F"
		41:
			return "FBB"
		42:
			return "FRRRR"
		43:
			return "S"
		44:
			return "F"
		45:
			return "F"
		46:
			return "FR"
		47:
			return "F"
		48:
			return "F"
		49:
			return "FRR"
		50:
			return "FRRW"
		51:
			return "S"
		52:
			return "FRW"
		53:
			return "FRW"
		54:
			return "FRW"
		55:
			return "F"
		56:
			return "F"
		57:
			return "FBB"
		58:
			return "FRRR"
		59:
			return "S"
		60:
			return "F"
		61:
			return "F"
		62:
			return "FR"
		63:
			return "F"
		64:
			return "F"
		65:
			return "F"
		66:
			return "F"
		67:
			return "F"
		68:
			return "F"
		69:
			return "F"
		70:
			return "FR"
		71:
			return "F"
		72:
			return "F"
		73:
			return "F"
		74:
			return "F"
		75:
			return "F"
		76:
			return "F"
		77:
			return "F"
		78:
			return "FR"
		79:
			return "F"
		80:
			return "F"
		81:
			return "F"
		82:
			return "F"
		83:
			return "F"
		84:
			return "F"
		85:
			return "F"
		86:
			return "FR"
		87:
			return "F"
		88:
			return "F"
		89:
			return "F"
		90:
			return "F"
		91:
			return "F"
		92:
			return "F"
		93:
			return "F"
		94:
			return "FR"
		95:
			return "F"
		96:
			return "F"
		97:
			return "F"
		98:
			return "F"
		99:
			return "F"
		100:
			return "F"
		101:
			return "F"
		102:
			return "FR"
		103:
			return "F"
		104:
			return "F"
		105:
			return "F"
		106:
			return "F"
		107:
			return "F"
		108:
			return "F"
		109:
			return "F"
		110:
			return "FR"
		111:
			return "F"
		112:
			return "FW"
		113:
			return "FW"
		114:
			return "FW"
		115:
			return "FW"
		116:
			return "FW"
		117:
			return "FW"
		118:
			return "FB"
		119:
			return "FW"
		120:
			return "F"
		121:
			return "F"
		122:
			return "F"
		123:
			return "F"
		124:
			return "F"
		125:
			return "F"
		126:
			return "FR"
		127:
			return "F"
		128:
			return "F"
		129:
			return "F"
		130:
			return "F"
		131:
			return "F"
		132:
				return "F"
		133:
			return "F"
		134:
			return "FR"
		135:
			return "F"
		136:
			return "F"
		137:
			return "F"
		138:
			return "F"
		139:
			return "F"
		140:
			return "F"
		141:
			return "F"
		142:
			return "FR"
		143:
			return "F"
		144:
			return "F"
		145:
			return "F"
		146:
			return "F"
		147:
			return "F"
		148:
			return "F"
		149:
			return "F"
		150:
			return "FR"
		151:
			return "F"
		152:
			return "F"
		153:
			return "F"
		154:
			return "F"
		155:
			return "F"
		156:
			return "F"
		157:
			return "F"
		158:
			return "FR"
		159:
			return "F"
		160:
			return "F"
		161:
			return "F"
		162:
			return "F"
		163:
			return "F"
		164:
			return "F"
		165:
			return "F"
		166:
			return "FR"
		167:
			return "F"
		168:
			return "F"
		169:
			return "F"
		170:
			return "F"
		171:
			return "F"
		172:
			return "F"
		173:
			return "F"
		174:
			return "FR"
		175:
			return "F"
		176:
			return "F"
		177:
			return "F"
		178:
			return "F"
		179:
			return "F"
		180:
			return "F"
		181:
			return "F"
		182:
			return "FR"
		183:
			return "F"
		184:
			return "F"
		185:
			return "F"
		186:
			return "F"
		187:
			return "F"
		188:
			return "F"
		189:
			return "F"
		190:
			return "FR"
		191:
			return "F"
		192:
			return "SRR"
		193:
			return "FRR"
		194:
			return "FRR"
		195:
			return "FRR"
		196:
			return "SRRWW"
		197:
			return "SWW"
		198:
			return "FR"
		199:
			return "SWW"
		200:
			return "SRR"
		201:
			return "SRR"
		202:
			return "FRR"
		203:
			return "F"
		204:
			return "SRRWW"
		205:
			return "SRRWW"
		206:
			return "FR"
		207:
			return "SWW"
		208:
			return "SRR"
		209:
			return "FRR"
		210:
			return "FRR"
		211:
			return "FRO"
		212:
			return "SRRWW"
		213:
			return "SWW"
		214:
			return "FR"
		215:
			return "SWW"
		216:
			return "SRR"
		217:
			return "F"
		218:
			return "FRR"
		219:
			return "FRI"
		220:
			return "SRRWW"
		221:
			return "F"
		222:
			return "FR"
		223:
			return "SWW"
		224:
			return "SRR"
		225:
			return "FRR"
		226:
			return "FRR"
		227:
			return "FRRWW"
		228:
			return "SRRWW"
		229:
			return "SWW"
		230:
			return "FR"
		231:
			return "SWW"
		232:
			return "SRR"
		233:
			return "S"
		234:
			return "FRR"
		235:
			return "F"
		236:
			return "SRRWW"
		237:
			return "F"
		238:
			return "FR"
		239:
			return "SWW"
		240:
			return "SRR"
		241:
			return "FRR"
		242:
			return "FRR"
		243:
			return "F"
		244:
			return "SRRWW"
		245:
			return "SWW"
		246:
			return "FR"
		247:
			return "SWW"
		248:
			return "SRR"
		249:
			return "S"
		250:
			return "FRR"
		251:
			return "F"
		252:
			return "SRRWW"
		253:
			return "F"
		254:
			return "FR"
		255:
			return"SWW"
		_:
			return "0"
	pass	
	

