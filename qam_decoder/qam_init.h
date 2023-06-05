#ifndef QAM_INIT_H
#define QAM_INIT_H
#include "stdint.h"

typedef struct qam{
    uint16_t    order;
    uint16_t    qam_sym_per_frame;
    uint16_t    inf_byte_amount;
    uint8_t     pream_qam_sym;
    uint16_t    fft_order;
} qam;

void qam64_init(qam *qam_str);
void qam256_init(qam *qam_str);
void qam256_double_frame_init(qam *qam_str);
#endif // QAM_INIT_H
