#include "qam_init.h"


void qam64_init(qam *qam_str){
    qam_str->order              = 64;
    qam_str->qam_sym_per_frame  = 340;
    qam_str->inf_byte_amount    = 222;
    qam_str->pream_qam_sym      = 7;
    qam_str->fft_order          = 4096;
}

void qam256_init(qam *qam_str){
    qam_str->order              = 256;
    qam_str->qam_sym_per_frame  = 255;
    qam_str->inf_byte_amount    = 212;
    qam_str->pream_qam_sym      = 15;
    qam_str->fft_order          = 2048;
}

void qam256_double_frame_init(qam *qam_str){
    qam_str->order              = 256;
    qam_str->qam_sym_per_frame  = 512;
    qam_str->inf_byte_amount    = 469;
    qam_str->pream_qam_sym      = 15;
    qam_str->fft_order          = 4096;
}
