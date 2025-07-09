#include "hal_port.h"
#include "main.h"
#include "spi.h"

void hal_HV_Enable(void)
{
    HAL_GPIO_WritePin(HV_ENABLE_GPIO_Port, HV_ENABLE_Pin, GPIO_PIN_SET);
}

void hal_HV_Disable(void)
{
    HAL_GPIO_WritePin(HV_ENABLE_GPIO_Port, HV_ENABLE_Pin, GPIO_PIN_RESET);
}

void hal_DOT_CTRL_Enable(void)
{
    HAL_GPIO_WritePin(DOT_CTRL_GPIO_Port, DOT_CTRL_Pin, GPIO_PIN_SET);
}

void hal_DOT_CTRL_Disable(void)
{
    HAL_GPIO_WritePin(DOT_CTRL_GPIO_Port, DOT_CTRL_Pin, GPIO_PIN_RESET);
}

void hal_DOT_CTRL_Trigger(void)
{
    HAL_GPIO_TogglePin(DOT_CTRL_GPIO_Port, DOT_CTRL_Pin);
}

void hal_STCP_Reset(void)
{
    HAL_GPIO_WritePin(STCP_GPIO_Port, STCP_Pin, GPIO_PIN_SET);
}

void hal_STCP_Set(void)
{
    HAL_GPIO_WritePin(STCP_GPIO_Port, STCP_Pin, GPIO_PIN_RESET);
}


char NumArray[11] = 
{
    0b0000, // 0
    0b1000, // 1
    0b0100, // 2
    0b1100, // 3
    0b0010, // 4
    0b1010, // 5
    0b0110, // 6
    0b1110, // 7
    0b0001, // 8
    0b1001, // 9
    0b1111  // Invalid
};

void hal_Time_Dsiable(void)
{
    unsigned char hourForCtrl = 0, minuteForCtrl = 0;

    hourForCtrl = (NumArray[10] << 4) |  NumArray[10];
    minuteForCtrl =  (NumArray[10] << 4) |  NumArray[10];

    hal_STCP_Reset();
    HAL_Delay(1);
    
    HAL_SPI_Transmit(&hspi1, &hourForCtrl, 1, 50);
    HAL_SPI_Transmit(&hspi1, &minuteForCtrl, 1, 50);
    
    HAL_Delay(1);
    hal_STCP_Set();
}

void hal_Time_Output(char hours, char minutes)
{
    char hours_tens,  hours_ones,  minutes_tens,  minutes_ones;

    if (hours < 0 || hours > 23)
    {
        hours = 0; // Reset to 0 if out of range
    }
    if (minutes < 0 || minutes > 59)
    {
        minutes = 0; // Reset to 0 if out of range
    }

    hours_tens = hours / 10;
    hours_ones = hours % 10;

    minutes_tens = minutes / 10;
    minutes_ones = minutes % 10;

    hours_tens = NumArray[hours_tens];
    hours_ones = NumArray[hours_ones];
    minutes_tens = NumArray[minutes_tens];
    minutes_ones = NumArray[minutes_ones];

    unsigned char hourForCtrl = 0, minuteForCtrl = 0;

    hourForCtrl = hours_tens << 4 | hours_ones;
    minuteForCtrl = minutes_tens << 4 | minutes_ones;

    hal_STCP_Reset();
    HAL_Delay(1);
    
    HAL_SPI_Transmit(&hspi1, &minuteForCtrl, 1, 50);
    HAL_SPI_Transmit(&hspi1, &hourForCtrl, 1, 50);
    
    HAL_Delay(1);
    hal_STCP_Set();
}



#define TIME_GAP   (100)  // ms

void hal_Time_Loop(int time_ms)
{
    unsigned char Ctrl = 0;
    for (int i = 0; i < 10; i++)
    {
        Ctrl = NumArray[i] << 4 | NumArray[i];

        hal_STCP_Reset();
        HAL_Delay(1);
        HAL_SPI_Transmit(&hspi1, &Ctrl, 1, 50);
        HAL_SPI_Transmit(&hspi1, &Ctrl, 1, 50);
        HAL_Delay(1);
        hal_STCP_Set();

        HAL_Delay(TIME_GAP);
    }
}
