#include "main.h"
#include "user_config.h"
#include "ST7735.h"
#include "fonts.h"
#include "window_table.h"

#include "riscv_math.h"
#include "riscv_const_structs.h"

static q15_t testInput[TEST_FFT_SIZE * 2];
static q15_t testOutput[TEST_FFT_SIZE];
//static uint32_t testIndex = 0;
//static q15_t maxValue;
extern uint16_t adc_raw[];

static uint8_t lcd_data[ST7735_WIDTH * ST7735_HEIGHT];
static uint8_t oscilscope_en = 0;
static uint8_t fft_en = 1;

void calc_fft()
{
	for(uint32_t i = 0; i < TEST_FFT_SIZE; i++)
	{
		if(adc_raw[i] >= 2048)
			testInput[i * 2 + 0] = (q15_t)((adc_raw[i] - 2048) << 6);
		else
			testInput[i * 2 + 0] = (q15_t)((((adc_raw[i] - 2048) & 0xFFF) << 6) | 0x8000);
		testInput[i * 2 + 1] = 0x00;
	}

	riscv_mult_q15((q15_t *)&testInput, (q15_t *)&window_table, (q15_t *)&testInput, TEST_FFT_SIZE);
	riscv_cfft_q15(&riscv_cfft_sR_q15_len128, (q15_t *)&testInput, 0, 1);
	riscv_cmplx_mag_q15((q15_t *)&testInput, (q15_t *)&testOutput, TEST_FFT_SIZE);
	//riscv_max_q15(testOutput, TEST_FFT_SIZE, &maxValue, &testIndex);
}

void update_lcd_oscilscope()
{
	for(uint32_t h = 0; h < ST7735_HEIGHT; h++)
	{
		for(uint32_t i = 0; i < TEST_LENGTH_SAMPLES; i++)
		{
			if(i >= ST7735_WIDTH)
				break;
			lcd_data[i + ST7735_WIDTH * h] = (((adc_raw[i] * ST7735_HEIGHT) / 4096 ) == h) ? 0xff : 0x00;
		}
	}
}

void update_lcd_fft()
{
	calc_fft();
	for(uint32_t h = 0; h < ST7735_HEIGHT; h++)
	{
		for(uint32_t i = 0; i < (TEST_LENGTH_SAMPLES / 2 - 1); i++)
			lcd_data[i + ST7735_WIDTH * h] = ((testOutput[i] / 100 ) >= h) ? 0xff : 0x00;
	}
}

void update_lcd_clear()
{
	for(uint32_t h = 0; h < ST7735_HEIGHT; h++)
	{
		for(uint32_t i = 0; i < ST7735_WIDTH; i++)
			lcd_data[i + ST7735_WIDTH * h] = 0x00;
	}
}

int main(void)
{
	SystemInit();
	//eclic_init(ECLIC_NUM_INTERRUPTS);
	//eclic_mode_enable();
	//disable_mcycle_minstret();

	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOC);
	rcu_periph_clock_enable(RCU_AF);
	rcu_periph_clock_enable(RCU_SPI1);

	rcu_periph_clock_enable(RCU_ADC0);
	rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV8);
	rcu_periph_clock_enable(RCU_DMA0);
	rcu_periph_clock_enable(RCU_TIMER1);

	// SPI1 GPIO config: SCK/PB13, MISO/PB14, MOSI/PB15
	gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13 | GPIO_PIN_15);
	gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_14);
	spi_config();

	// SPI1: CS/PC7, Reset/PA8, DC/PA9
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8 | GPIO_PIN_9);
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);

	// SPI Flash CS/PB12
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
	gpio_bit_set(GPIOB, GPIO_PIN_12);

	// ADC0 IN8/PB0
	gpio_init(GPIOB, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_0);

	// Buttons SW1/PA10 SW2/PB5
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
	gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_5);

	dma_config();
	adc_config();
	timer_config();

	ST7735_Init();

	timer_enable(TIMER1);

	for(;;)
	{
		while( !dma_flag_get(DMA0,DMA_CH0, DMA_FLAG_FTF)) {};
		dma_flag_clear(DMA0,DMA_CH0, DMA_FLAG_FTF);
		//timer_disable(TIMER1);
		if(oscilscope_en == 1)
			update_lcd_oscilscope();
		if(fft_en == 1)
			update_lcd_fft();
		//timer_enable(TIMER1);

		if(gpio_input_bit_get(GPIOA, GPIO_PIN_10) == RESET)
		{
			if(fft_en)
			{
				fft_en = 0;
				oscilscope_en = 1;
			}
		}
		else if(gpio_input_bit_get(GPIOB, GPIO_PIN_5) == RESET)
		{
			if(oscilscope_en)
			{
				oscilscope_en = 0;
				fft_en = 1;
				update_lcd_clear();
			}
		}

		ST7735_PutImageR2G3B3(0, 0, ST7735_WIDTH, ST7735_HEIGHT, (uint8_t *)&lcd_data);
	}
}
