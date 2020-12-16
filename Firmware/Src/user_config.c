#include "user_config.h"

uint16_t adc_raw[TEST_LENGTH_SAMPLES];

void spi_config(void)
{
	spi_parameter_struct spi_init_struct;

	spi_i2s_deinit(SPI1);
	spi_struct_para_init(&spi_init_struct);

	spi_init_struct.trans_mode				= SPI_TRANSMODE_FULLDUPLEX;
	spi_init_struct.device_mode				= SPI_MASTER;
	spi_init_struct.frame_size				= SPI_FRAMESIZE_8BIT;
	spi_init_struct.clock_polarity_phase	= SPI_CK_PL_LOW_PH_1EDGE;
	spi_init_struct.nss						= SPI_NSS_SOFT;
	spi_init_struct.prescale				= SPI_PSC_2;
	spi_init_struct.endian					= SPI_ENDIAN_MSB;
	spi_init(SPI1, &spi_init_struct);
	spi_enable(SPI1);
}

void dma_config(void)
{
	dma_parameter_struct dma_data_parameter;
	dma_deinit(DMA0, DMA_CH0);

	dma_data_parameter.periph_addr  = (uint32_t)(&ADC_RDATA(ADC0));
	dma_data_parameter.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
	dma_data_parameter.memory_addr  = (uint32_t)(&adc_raw);
	dma_data_parameter.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
	dma_data_parameter.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
	dma_data_parameter.memory_width = DMA_MEMORY_WIDTH_16BIT;
	dma_data_parameter.direction    = DMA_PERIPHERAL_TO_MEMORY;
	dma_data_parameter.number       = TEST_LENGTH_SAMPLES;
	dma_data_parameter.priority     = DMA_PRIORITY_HIGH;
	dma_init(DMA0, DMA_CH0, &dma_data_parameter);
	dma_circulation_enable(DMA0, DMA_CH0);

	dma_channel_enable(DMA0, DMA_CH0);
}

void adc_config(void)
{
	adc_deinit(ADC0);

	adc_mode_config(ADC_MODE_FREE);
	adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE);
	adc_special_function_config(ADC0, ADC_SCAN_MODE, DISABLE);
	adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
	adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 1);
	adc_regular_channel_config(ADC0, 0, 8, ADC_SAMPLETIME_55POINT5);
	adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_EXTTRIG_REGULAR_T1_CH1);
	adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);
	adc_enable(ADC0);
	delay_1ms(1);

	adc_calibration_enable(ADC0);
	adc_dma_mode_enable(ADC0);
}

void timer_config(void)
{
	timer_oc_parameter_struct timer_ocintpara;
	timer_parameter_struct timer_initpara;
	timer_deinit(TIMER1);

	timer_initpara.prescaler         = 25; //5
	timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection  = TIMER_COUNTER_UP;
	timer_initpara.period            = 299;//499;
	timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0;
	timer_init(TIMER1, &timer_initpara);

	timer_channel_output_struct_para_init(&timer_ocintpara);
	timer_ocintpara.ocpolarity  = TIMER_OC_POLARITY_LOW;
	timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
	timer_channel_output_config(TIMER1, TIMER_CH_1, &timer_ocintpara);

	timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_1, 100);
	timer_channel_output_mode_config(TIMER1, TIMER_CH_1, TIMER_OC_MODE_PWM1);
	timer_channel_output_shadow_config(TIMER1, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);

	timer_auto_reload_shadow_enable(TIMER1);
}