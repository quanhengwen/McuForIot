
#ifndef __DRIVER_CONFIG_H
#define __DRIVER_CONFIG_H

/********************************************/
#define	 CONFIG_DEV_RS232     1   // 1 ��Ӵ��� 0ȡ������
#define	 CONFIG_DEV_PRINTF    0   // 1 ��� 0ȡ��

/*********************************************/
#include "RS232.h"
#include "Printf.h"
#include "nokia_5110.h"

extern void DriverInt(void);

#endif

