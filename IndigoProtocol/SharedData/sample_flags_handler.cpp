/*
 * sample_flags_handler.cpp
 *
 *  Created on: 15 янв. 2019 г.
 *      Author: renat.khakimullin
 */

//#include "StdAfx.h"

#include <QtGlobal>
#include "sample_flags_handler.h"
 //#include "../../IndigoProtocol/V1/CommandBodyHeader.h"

static bool have_data_to_write = false;
EnumDataFlags chunk_flag = DataFlagsChunkNotLast;
static bool sensor_last_chunk = false;
static bool first_chunk_flag = true;

void set_have_data_flag(bool flag)
{
	have_data_to_write = flag;
}

void set_first_chunk_flag(bool flag)
{
	first_chunk_flag = flag;
}

bool get_first_chunk_flag()
{
	return first_chunk_flag;
}

bool get_have_data_flag()
{
	return have_data_to_write;
}

void set_sensor_last_chunk_flag(bool flag)
{
	sensor_last_chunk = flag;
}

bool get_sensor_last_chunk()
{
	return sensor_last_chunk;
}

EnumDataFlags get_chunk_flag()
{
	if((!first_chunk_flag & !have_data_to_write & ! sensor_last_chunk) || (!first_chunk_flag & have_data_to_write & !sensor_last_chunk) || (!first_chunk_flag & have_data_to_write & sensor_last_chunk))
		return DataFlagsChunkMiddle;

	if(first_chunk_flag & !sensor_last_chunk)
		return DataFlagsChunkNotLast;

	if(!first_chunk_flag & !have_data_to_write & sensor_last_chunk)
		return DataFlagsChunkNotFirst;

	if(first_chunk_flag & !have_data_to_write & sensor_last_chunk)
	{
		uint8_t res  = (DataFlagsChunkNotFirst & DataFlagsChunkNotLast);
		return (EnumDataFlags)res;
	}

	return (EnumDataFlags)(uint8_t)0;
}

/*
 * \brief Проверяем является ли чанк с флагом flag первым
 */
bool check_flag_first_sample(uint8_t flag)
{
	return(!(flag & ((~DataFlagsChunkNotLast) & 0x3)));
}

/*
 * \brief Проверяем является ли чанк с флагом flag последним
 */

bool check_flag_last_sample(uint8_t flag)
{
	return(!(flag & ((~DataFlagsChunkNotFirst) & 0x3)));
}
