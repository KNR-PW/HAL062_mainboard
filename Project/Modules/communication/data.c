#include "communication/data.h"

static void encodeSingleByte(uint8_t value, uint8_t* hex);

/**
 * *******************************************************************************
 * @details		:	all data are sent as HEX number coded in ASCII
 * 					(f.e number 140 is 8C so we send 0x38 (8 in ASCII)
 * 					and 0x43 (C in ASCII)
 * @see			:	UART frame documentation
 * *******************************************************************************
*/
void frameDecode(uint8_t* data, Command* command)
{

	/*Zamiana hex w ACSII na liczbe*/
	if(data[0] == '#')
	{
		if (data[1] >= 65)
			command->ID = (data[1] - 55) * 0x10;
		else
			command->ID = (data[1] - 48) * 0x10;

		if (data[2] >= 65)
			command->ID += (data[2] - 55);
		else
			command->ID += (data[2] - 48);


		uint8_t i_data = 3;
		uint8_t i_cmd = 0;
		while (i_data < PAYLOAD_SIZE && data[i_data]!='x' && data[i_data]!='X')
		//x or X - end of transmission
		{
			command->payload[i_cmd] = 0;
			if (data[i_data] >= 65)
				command->payload[i_cmd] = (data[i_data] - 55) * 0x10;
			else
				command->payload[i_cmd] = (data[i_data] - 48) * 0x10;

			++i_data;

			if (data[i_data] >= 65)
				command->payload[i_cmd] += (data[i_data] - 55);
			else
				command->payload[i_cmd] += (data[i_data] - 48);

			++i_data;
			++i_cmd;
		}
		for(uint8_t j=i_cmd;j<8;j++){
			command->payload[j]='X';
		}

	}

}

void frameEncode(Command* command, uint8_t* data)
{

	data[0] = '#';
	encodeSingleByte(command->ID, data+1);

	uint8_t i_cmd = 0;
	uint8_t i_data = 3;
	while (i_cmd<8 && command->payload[i_cmd] != 'X' && command->payload[i_cmd] != 'x')
	{
		encodeSingleByte(command->payload[i_cmd], data+i_data);
		++i_cmd;
		++i_data;
		++i_data;

	}

	for(uint8_t j=i_data;j<19;++j)
	{
		data[j] = 'X';
	}


}

static void encodeSingleByte(uint8_t value, uint8_t* hex)
{
    const uint8_t hex_digits[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

    hex[0] = hex_digits[(value >> 4) & 0x0F];  // High nibble
    hex[1] = hex_digits[value & 0x0F];         // Low nibble
}


void commandDeepCopy(Command* cmd_dest, Command* cmd_src)
{
	cmd_dest->ID = cmd_src->ID;
	for(uint8_t i = 0; i<8; ++i)
	{
		cmd_dest->payload[i] = cmd_src->payload[i];
	}
}
