#include"Bus.h"


Bus::Bus()
{

}

Bus::~Bus()
{

}

uint8_t Bus::read(uint16_t addr, bool bReadOnly = false)
{

	if (addr >= 0x0000 && addr <= 0xFFFF)
		return ram.M64k[addr];

	return 0x00;
}

uint8_t Bus::read(uint16_t addr, bool bReadOnly = false, bool bIO_Operation = false)
{
	if (addr <= 0x40 && addr >= 0x00)
	{
		if (bIO_Operation)
			return io.in[addr];
	}

	return 0x00;
}



void Bus::write(uint16_t addr, uint8_t data)
{

	if (addr >= 0x0000 && addr <= 0xFFFF)
		ram.M64k[addr] = data;
	return;
}

void Bus::write(uint16_t addr, uint8_t data, bool bIO_Operation = false)
{
	if (addr <= 0x80 && addr >= 0x50)
	{
		if (bIO_Operation)
			io.out[addr] = data;
	}
	return;
}
