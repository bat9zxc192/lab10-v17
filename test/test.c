#include "histogram.h"
#include <stdio.h>
#include <assert.h>

static void test_create_destroy() {
    histogram_t *h = histogram_create(5, 0.0, 10.0);
    assert(h != NULL);
    assert(histogram_get_bins(h) == 5);
    assert(histogram_get_total_count(h) == 0);
    histogram_destroy(h);
    histogram_destroy(NULL);  /* безопасно */
}

static void test_add_and_count() {
    histogram_t *h = histogram_create(3, 0.0, 3.0);
    assert(h != NULL);

    /* добавление в [0,1), [1,2), [2,3] */
    assert(histogram_add(h, 0.0) == HISTOGRAM_SUCCESS);
    assert(histogram_add(h, 0.5) == HISTOGRAM_SUCCESS);
    assert(histogram_add(h, 1.2) == HISTOGRAM_SUCCESS);
    assert(histogram_add(h, 2.9) == HISTOGRAM_SUCCESS);
    assert(histogram_add(h, 3.0) == HISTOGRAM_SUCCESS);

    size_t cnt;
    assert(histogram_get_count(h, 0, &cnt) == HISTOGRAM_SUCCESS && cnt == 2);
    assert(histogram_get_count(h, 1, &cnt) == HISTOGRAM_SUCCESS && cnt == 1);
    assert(histogram_get_count(h, 2, &cnt) == HISTOGRAM_SUCCESS && cnt == 2);

    /* проверка частот */
    double freq;
    assert(histogram_get_normalized_frequency(h, 0, &freq) == HISTOGRAM_SUCCESS);
    assert(freq == 2.0 / 5.0);
    assert(histogram_get_normalized_frequency(h, 2, &freq) == HISTOGRAM_SUCCESS);
    assert(freq == 2.0 / 5.0);

    histogram_destroy(h);
}

static void test_out_of_range() {
    histogram_t *h = histogram_create(2, 0.0, 1.0);
    assert(h != NULL);
    assert(histogram_add(h, -0.1) == HISTOGRAM_ERROR_OUT_OF_RANGE);
    assert(histogram_add(h, 1.1)  == HISTOGRAM_ERROR_OUT_OF_RANGE);
    assert(histogram_get_total_count(h) == 0);
    histogram_destroy(h);
}

static void test_max_bin() {
    histogram_t *h = histogram_create(4, 0.0, 4.0);
    histogram_add(h, 0.5); /* бин 0 */
    histogram_add(h, 2.0); /* бин 2 */
    histogram_add(h, 2.3); /* бин 2 */
    histogram_add(h, 3.0); /* бин 3 */

    size_t idx, cnt;
    assert(histogram_find_max_bin(h, &idx, &cnt) == HISTOGRAM_SUCCESS);
    assert(idx == 2 && cnt == 2);

    histogram_destroy(h);
}

static void test_empty_errors() {
    histogram_t *h = histogram_create(2, 0.0, 1.0);
    size_t cnt, idx;
    /* пустая гистограмма: find_max должен дать ошибку EMPTY */
    assert(histogram_find_max_bin(h, &idx, &cnt) == HISTOGRAM_ERROR_EMPTY);
    /* частота для любого бина должна быть 0.0 */
    double freq;
    assert(histogram_get_normalized_frequency(h, 0, &freq) == HISTOGRAM_SUCCESS && freq == 0.0);
    histogram_destroy(h);
}

static void test_null_args() {
    size_t cnt;
    double freq;
    size_t idx;
    assert(histogram_add(NULL, 1.0) == HISTOGRAM_ERROR_NULL);
    assert(histogram_get_count(NULL, 0, &cnt) == HISTOGRAM_ERROR_NULL);
    assert(histogram_get_count(NULL, 0, NULL) == HISTOGRAM_ERROR_NULL);
    assert(histogram_get_normalized_frequency(NULL, 0, &freq) == HISTOGRAM_ERROR_NULL);
    assert(histogram_find_max_bin(NULL, &idx, &cnt) == HISTOGRAM_ERROR_NULL);
    assert(histogram_get_bins(NULL) == 0);
    assert(histogram_get_total_count(NULL) == 0);
}

static void test_invalid_create() {
    assert(histogram_create(0, 0.0, 1.0) == NULL);
    assert(histogram_create(5, 2.0, 1.0) == NULL);
}

int main() {
    test_create_destroy();
    test_add_and_count();
    test_out_of_range();
    test_max_bin();
    test_empty_errors();
    test_null_args();
    test_invalid_create();
    printf("All C tests passed.\n");
    return 0;
}
