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
  CMD_ROTATE_GRID,
  CMD_PLACE_CUBES,
  CMD_MOVE_LIFT,
  CMD_STATE,
};

typedef struct
{
  uint8_t degrees_h;
  uint8_t degrees_l;
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

typedef union
{
  cmd_rotate_grid_t cmdRotateGrid;
  cmd_place_cubes_t cmdPlaceCubes;
  cmd_move_lift_t cmdMoveLift;
  uint8_t dataField[16];
}data_union_t;

typedef struct{
  uint8_t cmd;
  uint8_t len;
  data_union_t dataUnion;
  uint8_t checksum;
}message_t;

#pragma pack() // Reset alignment to default

#endif /* PROTOCOLDEFWERNI_H_ */
