#ifndef  __BSP_SPI_HW_H__
#define  __BSP_SPI_HW_H__

void bsp_InitSPIBus(void);
void bsp_SPI_Init(uint16_t _cr1);
__STATIC_INLINE void bsp_spiWrite0(uint8_t _ucByte);
__STATIC_INLINE uint8_t bsp_spiRead0(void);
#endif
