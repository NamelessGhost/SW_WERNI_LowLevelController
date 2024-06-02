/*
 * ProtocolDefWERNI.h
 *
 *  Created on: Dec 10, 2023
 *      Author: krumm
 */

#ifndef PROTOCOLDEFWERNI_H_
#define PROTOCOLDEFWERNI_H_

#include <stdint.h>

#pragma pack(1) // Set alignment to 1 byte

enum COMMAND
{
  CMD_RESERVED,
  CMD_ACKNOWLEDGE,
  CMD_NOT_ACKNOWLEDGE,
  CMD_CRC_ERROR,
  CMD_ROTATE_GRID,
  CMD_PLACE_CUBES,
  CMD_MOVE_LIFT,
  CMD_GET_STATE,
  CMD_SEND_STATE,
  CMD_PAUSE_BUILD,
  CMD_RESUME_BUILD,
  CMD_PRIME_MAGAZINE,
  CMD_SEND_IO_STATE,
  CMD_EXECUTION_FINISHED,
  CMD_RESET_ENERGY_MEASUREMENT,
  CMD_RESET_WERNI,    //This command triggers a hardware reset!
  CMD_ENABLE_BUZZER,
};

enum BTN_STATES
{
  BTN_RELEASED,
  BTN_PRESSED,
  BTN_SHORT_CLICKED,
  BTN_LONG_CLICKED,
};

enum LIFT_STATES {
  UNHOMED,
  LIFT_UP,
  LIFT_DOWN
};

enum WERNI_STATES
{
  PREPARING,
  READY,
  BUILDING,
  BUILD_PAUSED,
  BUILD_ABORTED
};


typedef struct
{
  int16_t degrees;
}cmd_rotate_grid_t;

typedef struct
{
  uint8_t cubes_red;
  uint8_t cubes_yellow;
  uint8_t cubes_blue;
}cmd_place_cubes_t;

typedef enum
{
  MOVE_UP,
  MOVE_DOWN
}cmd_move_lift_t;


typedef struct
{
  float energyConsumption;
  LIFT_STATES liftState     :8;
  WERNI_STATES werniState   :8;
}cmd_send_state_t;

typedef struct
{
  uint8_t btnEStopState;
  uint8_t btnStartState;
}cmd_send_io_state_t;

typedef struct
{
  COMMAND cmd;
  uint8_t success;
}cmd_exec_finished_t;

typedef struct
{
  uint8_t enable;
}cmd_enable_buzzer_t;

typedef union
{
  cmd_rotate_grid_t     cmdRotateGrid;
  cmd_place_cubes_t     cmdPlaceCubes;
  cmd_move_lift_t       cmdMoveLift;
  cmd_send_state_t      cmdSendState;
  cmd_send_io_state_t   cmdSendIoState;
  cmd_exec_finished_t   cmdExecFinished;
  cmd_enable_buzzer_t   cmdEnableBuzzer;
  uint8_t               dataField[16];
}data_union_t;

typedef struct{
  uint8_t       cmd;
  uint8_t       id;
  data_union_t  dataUnion;
  uint8_t       checksum;
}message_t;

#pragma pack() // Reset alignment to default

#endif /* PROTOCOLDEFWERNI_H_ */
