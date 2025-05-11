#ifndef PROJECT_MODULES_COMMUNICATION_DATA_H
#define PROJECT_MODULES_COMMUNICATION_DATA_H

#include "stm32h7xx_hal.h"

#define PAYLOAD_SIZE 19
#define BUFFER_SIZE 100

typedef struct DataBuffer
{
	uint8_t data[BUFFER_SIZE][PAYLOAD_SIZE];
	uint8_t write_ptr;
	uint8_t read_ptr;

} DataBuffer;

typedef struct Command
{
	uint8_t ID;
	uint8_t payload[8];
} Command;

typedef struct CommandBuffer
{
	Command commands[BUFFER_SIZE];
	uint8_t write_ptr;
	uint8_t read_ptr;
} CommandBuffer;

void frameDecode(uint8_t* data, Command* command);
void frameEncode(Command* command, uint8_t* data);
void commandDeepCopy(Command* cmd_dest, Command* cmd_src);

#endif //PROJECT_MODULES_COMMUNICATION_DATA_H
