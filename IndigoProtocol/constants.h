/*
 * constants.h
 *
 *  Created on: 14 мая 2018 г.
 *      Author: andrey.bukanov
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

#ifdef __cplusplus
 extern "C" {
#endif

 /* MEMORY CONSTANTS */
#define  FRAME_DATA_BUFFER_SIZE  (256)
#define  PACKET_DATA_BUFFER_SIZE (4096)

/* ADRESS CONSTANTS */
#define  MASTER_TOOL_TYPE     (00)
#define  MASTER_SERIAL_NUMBER (0000)

/*COMMANDS*/
#define  SELF_CMD_VERSION (1)
//#define  CMD_SCAN (0x0a)
#define     CMD_READ_NAND_PAGE (0x10)
#define     CMD_NAND_TST_WR    (0x12)
#define     CMD_NAND_TST_R        (0x11)
/*TIMINGS*/

#ifdef __cplusplus
}
#endif

#endif // CONSTANTS_H
