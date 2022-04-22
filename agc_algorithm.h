#ifndef AGC_ALGORITHM_H
#define AGC_ALGORITHM_H

// Состояния работы алгоритма АРУ. Используются при начальной инициализации, запуске работы и оценке критериев
typedef enum
{
    AGC_OK = 0x00,          // Настройка аттенюатора/усилителя закончена
    AGC_INIT = 0x01,        // Выполнена инициализация АРУ
    AGC_START = 0x02,       // Запуск АРУ произведен
    AGC_OVERVOLTAGE = 0x03, // Уровень сигнала превышает максимально возможное значение АЦП
    AGC_ADJUST_MIN = 0x04,  // Уровень сигнала сильно ослаблен (лежит ниже порогового значения)
    AGC_ADJUST_MAX = 0x05,  // Уровень сигнала превышает максимально допустимое значение АЦП (лежит выше порогового значения АЦП)
    AGC_ERROR = 0x0F        // Обнаружена ошибка АРУ (не выполнена инициализация; процедура запуска; не удается выполнить настройку)
}agc_state_t;

#endif // AGC_ALGORITHM_H