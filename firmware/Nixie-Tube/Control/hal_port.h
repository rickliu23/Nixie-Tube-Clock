#ifndef _HAL_PORT_H
#define _HAL_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

void hal_HV_Enable(void);
void hal_HV_Disable(void);

void hal_DOT_CTRL_Enable(void);
void hal_DOT_CTRL_Disable(void);
void hal_DOT_CTRL_Trigger(void);

void hal_Time_Dsiable(void);
void hal_Time_Output(char hours, char minutes);

#ifdef __cplusplus
}
#endif

#endif /* _HAL_PORT_H */
