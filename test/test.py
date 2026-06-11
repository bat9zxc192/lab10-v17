import ctypes
import os

lib_path = os.path.join(os.path.dirname(__file__), "..", "libhistogram.so")
lib = ctypes.CDLL(lib_path)

# Типы
class Histogram(ctypes.Structure):
    pass

histogram_ptr = ctypes.POINTER(Histogram)

lib.histogram_create.argtypes = [ctypes.c_size_t, ctypes.c_double, ctypes.c_double]
lib.histogram_create.restype = histogram_ptr

lib.histogram_destroy.argtypes = [histogram_ptr]
lib.histogram_destroy.restype = None

lib.histogram_add.argtypes = [histogram_ptr, ctypes.c_double]
lib.histogram_add.restype = ctypes.c_int

lib.histogram_get_count.argtypes = [histogram_ptr, ctypes.c_size_t, ctypes.POINTER(ctypes.c_size_t)]
lib.histogram_get_count.restype = ctypes.c_int

lib.histogram_get_normalized_frequency.argtypes = [histogram_ptr, ctypes.c_size_t, ctypes.POINTER(ctypes.c_double)]
lib.histogram_get_normalized_frequency.restype = ctypes.c_int

lib.histogram_find_max_bin.argtypes = [histogram_ptr, ctypes.POINTER(ctypes.c_size_t), ctypes.POINTER(ctypes.c_size_t)]
lib.histogram_find_max_bin.restype = ctypes.c_int

lib.histogram_get_bins.argtypes = [histogram_ptr]
lib.histogram_get_bins.restype = ctypes.c_size_t

lib.histogram_get_total_count.argtypes = [histogram_ptr]
lib.histogram_get_total_count.restype = ctypes.c_size_t

# Тесты
h = lib.histogram_create(3, 0.0, 3.0)
assert h is not None, "Create failed"

assert lib.histogram_get_bins(h) == 3
assert lib.histogram_get_total_count(h) == 0

# Добавление значений
assert lib.histogram_add(h, 0.5) == 0
assert lib.histogram_add(h, 1.2) == 0
assert lib.histogram_add(h, 2.9) == 0
assert lib.histogram_add(h, 3.0) == 0
assert lib.histogram_add(h, 0.0) == 0

cnt = ctypes.c_size_t()
assert lib.histogram_get_count(h, 0, ctypes.byref(cnt)) == 0
assert cnt.value == 2  # 0.5 и 0.0
assert lib.histogram_get_count(h, 1, ctypes.byref(cnt)) == 0
assert cnt.value == 1  # 1.2
assert lib.histogram_get_count(h, 2, ctypes.byref(cnt)) == 0
assert cnt.value == 2  # 2.9 и 3.0

# Нормированная частота
freq = ctypes.c_double()
assert lib.histogram_get_normalized_frequency(h, 0, ctypes.byref(freq)) == 0
assert abs(freq.value - 2.0/5.0) < 1e-9

# Проверка на выход за границы
assert lib.histogram_add(h, -0.1) == -3
assert lib.histogram_add(h, 3.1) == -3

# Максимальный бин (бин 0 и 2 имеют по 2, должен вернуть первый)
idx = ctypes.c_size_t()
max_cnt = ctypes.c_size_t()
assert lib.histogram_find_max_bin(h, ctypes.byref(idx), ctypes.byref(max_cnt)) == 0
assert idx.value == 0
assert max_cnt.value == 2

# Пустая гистограмма
h_empty = lib.histogram_create(4, 0.0, 4.0)
# find_max должен вернуть ошибку EMPTY (-6)
assert lib.histogram_find_max_bin(h_empty, ctypes.byref(idx), ctypes.byref(max_cnt)) == -6
# частота при пустом = 0
freq_empty = ctypes.c_double()
assert lib.histogram_get_normalized_frequency(h_empty, 0, ctypes.byref(freq_empty)) == 0
assert freq_empty.value == 0.0
lib.histogram_destroy(h_empty)

# NULL-тесты
assert lib.histogram_add(None, 1.0) == -1
assert lib.histogram_get_count(None, 0, ctypes.byref(cnt)) == -1
assert lib.histogram_get_normalized_frequency(h, 0, None) == -1
assert lib.histogram_find_max_bin(None, ctypes.byref(idx), ctypes.byref(max_cnt)) == -1
assert lib.histogram_get_bins(None) == 0

lib.histogram_destroy(h)
print("All Python tests passed.")
