/*
 * mem_packet_sh.h
 *
 *  Created on: 12 окт. 2018 г.
 *      Author: renat.khakimullin
 */

#ifndef MEM_PACKET_SH_H_
#define MEM_PACKET_SH_H_

#include "../V1/CommandBodyHeader.h"

#define MAX_MEM_PACKET_LEN	8192

#pragma pack(push,1)
typedef struct
{
    uint32_t packet_num;              //линейно растет
    EnumDataFlags chunk_flag;         //для каждого сенсора свой
    uint32_t sample_start_packet_num; //номер пакета начала сэмпла
    uint32_t sample_id;
    uint8_t  sensor_id;
    uint8_t	 subsensor_id;
    IndigoSampleTime time;
    uint8_t  cyclo_num;
    uint8_t  scen_link;
    uint16_t Dlen;
} MemPacketHeader;
#pragma pack(pop)

//typedef IndigoMemorySample MemPacketHeader;

/*
  * \brief Структура пакета
  */
#pragma pack(push,1)
 typedef struct
 {
	 MemPacketHeader header;
	 uint8_t data[MAX_MEM_PACKET_LEN];
 } ToolMemPacket_t;
#pragma pack(pop)



#endif /* MEM_PACKET_SH_H_ */
