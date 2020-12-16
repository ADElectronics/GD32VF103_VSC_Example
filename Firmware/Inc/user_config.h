#ifndef _USER_CONFIG_H_
#define _USER_CONFIG_H_

#include "gd32vf103.h"
#include "systick.h"
#include <stdio.h>

void spi_config(void);
void dma_config(void);
void adc_config(void);
void timer_config(void);

#define TEST_LENGTH_SAMPLES 160 //256
#define TEST_FFT_SIZE 		128

#endif // _USER_CONFIG_H_
