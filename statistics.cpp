/*
 * statistics.c
 *
 *  Created on: 21 окт. 2016 г.
 *      Author: gleb.maslennikov
 */

#include "statistics.h"

static data_quality received_data_quality = { 0, 0 };

void statisics_reset()
{
    received_data_quality.good_crc = 0;
    received_data_quality.good_synhro = 0;
}

void good_synchro_received()
{
    if (received_data_quality.good_synhro < MAX_STATISTICS)
    {
        received_data_quality.good_synhro++;
    }
}

void good_crc_received()
{
    if (received_data_quality.good_crc < MAX_STATISTICS)
    {
        received_data_quality.good_crc++;
    }
}

void bad_synchro_received()
{
    if (received_data_quality.good_synhro > 0)
    {
        received_data_quality.good_synhro--;
    }

    if (received_data_quality.good_crc > 0)
    {
        received_data_quality.good_crc--;
    }
}

void bad_crc_received()
{
    if (received_data_quality.good_crc > 0)
    {
        received_data_quality.good_crc--;
    }
}

data_quality get_data_statistics()
{
    data_quality l_quality;
    l_quality.good_crc = received_data_quality.good_crc;
    l_quality.good_synhro = received_data_quality.good_synhro;

    return l_quality;
}
