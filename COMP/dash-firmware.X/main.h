/* 
 * File:   main.h
 * Author: Adam
 *
 * Created on June 15, 2025, 12:08 AM
 */

#ifndef MAIN_H
#define	MAIN_H

typedef enum {
  STATE_STARTUP,
  STATE_TS_BUTTON_WAIT,
  STATE_WAIT_NEG_AIR,
  STATE_PRECHARGE_WAIT,
  STATE_R2D_WAIT,
  STATE_TS_ON,
  STATE_R2D_OFF,
  STATE_RTD_OFF_WAIT,
  STATE_TS_ACTIVATION_FAILURE,
  STATE_TS_RUN_FAULT,
  STATE_SPIN,
  STATE_APPS_SCS_FAULT,
  STATE_APPS_BPS_FAULT,
} DisplayState_t;

typedef enum {
  NORMAL,
  TEMPS,
  POWER,
} InfoState_t;

DisplayState_t display_state = STATE_STARTUP;

volatile InfoState_t current_state = NORMAL;



void display_tson_state(InfoState_t current_state, DashboardData_t* data);
void vcu_number(DashboardData_t* data);
void ButtonPressHandler(void);
void ButtonHoldTimer(void);
void RealTimeClockHandler(void);

#endif	/* MAIN_H */

