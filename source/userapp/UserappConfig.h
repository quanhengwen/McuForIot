
#ifndef __APPCREATE_H
#define __APPCREATE_H 

#include <includes.h>
#include "DriverConfig.h" 

#include "5110Task.h"
#include "DebugTask.h"
#include "MPU6050Task.h"
#include "ledstask.h"

#define	 CONFIG_DEBUG       1   // 1 ��� 0ȡ��  
#define	 CONFIG_LEDS        1  // 1 ��� 0ȡ��  
#define	 CONFIG_LCD5110     0   // 1 ��� 0ȡ��  
#define	 CONFIG_MPU6050     0   // 1 ��� 0ȡ��  

extern void AppCreate(void);

#endif

