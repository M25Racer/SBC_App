//#include <QCoreApplication>
//#include "qam_decoder.h"
//#include "HS_EWL_FREQ_ACQ.h"
//#include "HS_EWL_FREQ_ACQ_emxAPI.h"
//#include "HS_EWL_FREQ_ACQ_terminate.h"
//#include "HS_EWL_FREQ_ACQ_types.h"
//#include "rt_nonfinite.h"
//#include <QElapsedTimer>
//#include <QDebug>

//static QElapsedTimer timer;

//static double Fs = 1832061.0687022900763358778625954; //280000;//ADC sample rate
////Fs = 1,832,061.0687022900763358778625954
//static double f0 = 35000;//carrier freq
//static double mode = 1;//1-both stages enabled, 0-only sevond stage
//static double preamble_len = 20;//preamble length
//static double message_len = 255;
//static double QAM_order = 256;
//static double preamble_QAM_symbol = 128;//QAM symbol used in preamble
//static double f_est_data;//estimated frequency
//static int f_est_size;

//void qam_decoder(double *signal, double len)
//{
////    double len = SIG_LEN;
//    emxArray_real_T *in_data = NULL;
//    in_data = emxCreateWrapper_real_T(signal, 1, len);

//    timer.start();
//    HS_EWL_FREQ_ACQ(in_data, len, Fs,
//                    f0, mode, preamble_len,
//                    message_len, QAM_order, preamble_QAM_symbol,
//                    (double *)&f_est_data,
//                    *(int(*)[1]) & f_est_size);
//    qDebug() << "freq = " << f_est_data << "Hz" << "elapsed time = " << timer.elapsed() << "ms";
//}
