
#include"Devices.h"

#ifndef BUS_H
#define BUS_H


class Bus
{
public:
	Ram ram;
	IODevice io;

public:
	Bus();
	~Bus();

	uint8_t read(uint16_t, bool);
	uint8_t read(uint16_t, bool, bool);
	void write(uint16_t, uint8_t);
	void write(uint16_t, uint8_t, bool);

};

#endif // !BUS_H
