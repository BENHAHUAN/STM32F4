//#include "BSP_ADC.h"
#include "includes.h"
//#include "stm32f4xx.h"
/* 电池充满电adc检测在2.9V左右，当adc检测值小于2.59时电池电量过低，报警 */
static uint16_t get_ADC(uint8_t ch);
static void temperature_ADC_Reset(void);
void temperature_ADC_init(void)
{
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
	GPIO_Config(RCC_AHB1Periph_GPIOA ,GPIOA,GPIO_Pin_0,GPIO_Mode_AN,GPIO_OType_PP,GPIO_PuPd_NOPULL);	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);

    ADC_TempSensorVrefintCmd(ENABLE);

    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
    ADC_CommonInit(&ADC_CommonInitStructure);
    temperature_ADC_Reset();

    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_15Cycles);
    ADC_Cmd(ADC1, ENABLE);
}

static void temperature_ADC_Reset(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

}

float get_temprate(void)
{
    uint16_t adcx = 0;
    float temperate;
    temperature_ADC_Reset();
    adcx = get_ADC(ADC_Channel_0);
    temperate = (float)adcx * (3.3f / 4096.0f);
    //temperate = (temperate - 0.76f) / 0.0025f + 25.0f;
    return temperate;
}

static uint16_t get_ADC(uint8_t ch)
{

    ADC_ClearFlag(ADC1,ADC_FLAG_STRT|ADC_FLAG_OVR|ADC_FLAG_EOC);
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_15Cycles);

    ADC_SoftwareStartConv(ADC1);

    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
    {
        ;
    }
    return ADC_GetConversionValue(ADC1);
}
