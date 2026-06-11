#include "histogram.h"
#include <stdlib.h>

struct histogram_t {
    size_t bins;         /**< количество бинов */
    double min;          /**< левая граница */
    double max;          /**< правая граница */
    double bin_width;    /**< ширина одного бина */
    size_t *counts;      /**< массив счётчиков (размер bins) */
    size_t total_count;  /**< общее число добавленных значений */
};

histogram_t *histogram_create(size_t bins, double min, double max) {
    histogram_t *h = NULL;
    histogram_error_t err = HISTOGRAM_SUCCESS;

    if (bins == 0 || min >= max) {
        err = HISTOGRAM_ERROR_INVALID_PARAM;
    }

    if (err == HISTOGRAM_SUCCESS) {
        h = malloc(sizeof(histogram_t));
        if (!h) err = HISTOGRAM_ERROR_MEMORY;
    }

    if (err == HISTOGRAM_SUCCESS) {
        h->counts = calloc(bins, sizeof(size_t));
        if (!h->counts) {
            err = HISTOGRAM_ERROR_MEMORY;
            free(h);
            h = NULL;
        }
    }

    if (err == HISTOGRAM_SUCCESS) {
        h->bins = bins;
        h->min = min;
        h->max = max;
        h->bin_width = (max - min) / (double)bins;
        h->total_count = 0;
    }

    return h;
}

void histogram_destroy(histogram_t *h) {
    if (h) {
        free(h->counts);
        free(h);
    }
}

histogram_error_t histogram_add(histogram_t *h, double value) {
    histogram_error_t err = HISTOGRAM_SUCCESS;
    size_t idx = 0;

    if (!h) {
        err = HISTOGRAM_ERROR_NULL;
    }

    if (err == HISTOGRAM_SUCCESS &&
        (value < h->min || value > h->max)) {
        err = HISTOGRAM_ERROR_OUT_OF_RANGE;
    }

    if (err == HISTOGRAM_SUCCESS) {
        if (value == h->max) {
            idx = h->bins - 1;  /* крайнее правое значение в последний бин */
        } else {
            idx = (size_t)((value - h->min) / h->bin_width);
        }
        h->counts[idx]++;
        h->total_count++;
    }

    return err;
}

histogram_error_t histogram_get_count(const histogram_t *h,
                                      size_t bin_index,
                                      size_t *count) {
    histogram_error_t err = HISTOGRAM_SUCCESS;

    if (!h || !count) {
        err = HISTOGRAM_ERROR_NULL;
    }

    if (err == HISTOGRAM_SUCCESS && bin_index >= h->bins) {
        err = HISTOGRAM_ERROR_INDEX;
    }

    if (err == HISTOGRAM_SUCCESS) {
        *count = h->counts[bin_index];
    }

    return err;
}

histogram_error_t histogram_get_normalized_frequency(
    const histogram_t *h, size_t bin_index, double *freq) {
    histogram_error_t err = HISTOGRAM_SUCCESS;

    if (!h || !freq) {
        err = HISTOGRAM_ERROR_NULL;
    }

    if (err == HISTOGRAM_SUCCESS && bin_index >= h->bins) {
        err = HISTOGRAM_ERROR_INDEX;
    }

    if (err == HISTOGRAM_SUCCESS) {
        if (h->total_count == 0) {
            *freq = 0.0;
        } else {
            *freq = (double)h->counts[bin_index] / (double)h->total_count;
        }
    }

    return err;
}

histogram_error_t histogram_find_max_bin(const histogram_t *h,
                                         size_t *bin_index,
                                         size_t *count) {
    histogram_error_t err = HISTOGRAM_SUCCESS;
    size_t max_idx = 0;
    size_t max_val = 0;

    if (!h || !bin_index || !count) {
        err = HISTOGRAM_ERROR_NULL;
    }

    if (err == HISTOGRAM_SUCCESS && h->total_count == 0) {
        err = HISTOGRAM_ERROR_EMPTY;
    }

    if (err == HISTOGRAM_SUCCESS) {
        max_val = h->counts[0];
        max_idx = 0;
        for (size_t i = 1; i < h->bins; i++) {
            if (h->counts[i] > max_val) {
                max_val = h->counts[i];
                max_idx = i;
            }
        }
        *bin_index = max_idx;
        *count = max_val;
    }

    return err;
}

size_t histogram_get_bins(const histogram_t *h) {
    size_t bins = 0;
    if (h) {
        bins = h->bins;
    }
    return bins;
}

size_t histogram_get_total_count(const histogram_t *h) {
    size_t total = 0;
    if (h) {
        total = h->total_count;
    }
    return total;
}
