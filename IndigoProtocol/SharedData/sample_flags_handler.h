/*
 * sensor_flags_handler.h
 *
 *  Created on: 15 џэт. 2019 у.
 *      Author: renat.khakimullin
 */

#ifndef SAMPLE_FLAGS_HANDLER_H_
#define SAMPLE_FLAGS_HANDLER_H_

#include "../V1/CommandBodyHeader.h"

EnumDataFlags get_chunk_flag();
bool get_sensor_last_chunk();
void set_sensor_last_chunk_flag(bool flag);
bool get_have_data_flag();
void set_have_data_flag(bool flag);
bool get_first_chunk_flag();
void set_first_chunk_flag(bool flag);
bool check_flag_first_sample(uint8_t flag);
bool check_flag_last_sample(uint8_t flag);

#endif /* SAMPLE_FLAGS_HANDLER_H_ */
