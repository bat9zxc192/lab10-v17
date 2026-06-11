/**
 * @file histogram.h
 * @brief Динамическая гистограмма вещественных чисел.
 *
 * Гистограмма разбивает отрезок [min, max] на bins равных интервалов
 * и подсчитывает количество попавших в каждый интервал значений.
 *
 * Коды ошибок:
 * - HISTOGRAM_SUCCESS (0)            — успех
 * - HISTOGRAM_ERROR_NULL (-1)        — передан NULL-указатель
 * - HISTOGRAM_ERROR_INVALID_PARAM (-2) — неверные параметры (bins==0 или min>=max)
 * - HISTOGRAM_ERROR_OUT_OF_RANGE (-3) — значение вне [min, max] при добавлении
 * - HISTOGRAM_ERROR_INDEX (-4)       — индекс бина вне [0, bins-1]
 * - HISTOGRAM_ERROR_MEMORY (-5)      — ошибка выделения памяти
 * - HISTOGRAM_ERROR_EMPTY (-6)       — гистограмма пуста (нет добавленных значений)
 */

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Коды ошибок */
typedef enum {
    HISTOGRAM_SUCCESS = 0,
    HISTOGRAM_ERROR_NULL = -1,
    HISTOGRAM_ERROR_INVALID_PARAM = -2,
    HISTOGRAM_ERROR_OUT_OF_RANGE = -3,
    HISTOGRAM_ERROR_INDEX = -4,
    HISTOGRAM_ERROR_MEMORY = -5,
    HISTOGRAM_ERROR_EMPTY = -6
} histogram_error_t;

/** Непрозрачный тип гистограммы */
typedef struct histogram_t histogram_t;

/**
 * @brief Создаёт новую гистограмму.
 * @param bins количество бинов (должно быть > 0)
 * @param min  левая граница
 * @param max  правая граница (должна быть > min)
 * @return указатель на histogram_t или NULL при ошибке
 */
histogram_t *histogram_create(size_t bins, double min, double max);

/**
 * @brief Уничтожает гистограмму и освобождает память.
 * @param h указатель на гистограмму (допустим NULL)
 */
void histogram_destroy(histogram_t *h);

/**
 * @brief Добавляет значение в гистограмму.
 * Значение должно принадлежать отрезку [min, max].
 * @param h     указатель на гистограмму (не NULL)
 * @param value добавляемое значение
 * @return код ошибки
 */
histogram_error_t histogram_add(histogram_t *h, double value);

/**
 * @brief Возвращает количество попаданий в бин с заданным индексом.
 * @param h          указатель на гистограмму (не NULL)
 * @param bin_index  индекс бина (0..bins-1)
 * @param count      [out] указатель для записи счётчика (не NULL)
 * @return код ошибки
 */
histogram_error_t histogram_get_count(const histogram_t *h,
                                      size_t bin_index,
                                      size_t *count);

/**
 * @brief Возвращает нормированную частоту для бина (count / total).
 * Если total == 0, частота полагается равной 0.0.
 * @param h          указатель на гистограмму (не NULL)
 * @param bin_index  индекс бина
 * @param freq       [out] указатель для записи частоты (не NULL)
 * @return код ошибки
 */
histogram_error_t histogram_get_normalized_frequency(
    const histogram_t *h, size_t bin_index, double *freq);

/**
 * @brief Находит бин с максимальным количеством попаданий.
 * При наличии нескольких таких бинов возвращается первый.
 * @param h          указатель на гистограмму (не NULL)
 * @param bin_index  [out] указатель для индекса найденного бина (не NULL)
 * @param count      [out] указатель для записи количества (не NULL)
 * @return код ошибки (HISTOGRAM_ERROR_EMPTY, если гистограмма пуста)
 */
histogram_error_t histogram_find_max_bin(const histogram_t *h,
                                         size_t *bin_index,
                                         size_t *count);

/**
 * @brief Возвращает количество бинов.
 * @param h указатель на гистограмму
 * @return число бинов или 0 при h == NULL
 */
size_t histogram_get_bins(const histogram_t *h);

/**
 * @brief Возвращает общее количество добавленных значений.
 * @param h указатель на гистограмму
 * @return total count или 0 при h == NULL
 */
size_t histogram_get_total_count(const histogram_t *h);

#ifdef __cplusplus
}
#endif

#endif // HISTOGRAM_H
