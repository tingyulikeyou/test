
#ifndef __JBS_BMS_H__
#define __JBS_BMS_H__


enum
{
	JBS_CMD_INFOR=0X03,
	JBS_CMD_VOLT,
	JBS_CMD_VER,
	JBS_CMD_LED,
	JBS_CMD_FACTOR,
	JBS_CMD_FET,
	JBS_CMD_EXIT
};

void JbsBmsInit(void);
void JbsBms_GetInfo(uint8_t cmd);
void JbsBmsTask(void);
void JbsMosCtrl(uint8_t on);
void Jbd485Send(uint8_t *buffer,uint16_t size);

#endif

