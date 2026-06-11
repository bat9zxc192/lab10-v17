# libhistogram — гистограмма вещественных чисел

Библиотека на C реализует построение гистограммы: разбиение отрезка [min, max]
на заданное число равных интервалов (бинов) и подсчёт числа попаданий в каждый бин.
Собирается как разделяемый объект (`libhistogram.so`) и может использоваться из C и Python.

## Требования
- GCC (C99) с поддержкой -fPIC
- make
- Python 3 (для тестов)
- clang-tidy (опционально)
- doxygen (опционально)

## Структура проекта
├── include/

│ └── histogram.h # публичный интерфейс с Doxygen

├── src/

│ └── histogram.c # реализация

├── test/

│ ├── test.c # C-тесты

│ └── test.py # Python-тесты

├── Makefile

├── Doxyfile

├── .gitignore

└── README.md

## Сборка
make
Создаст `libhistogram.so`.

## Тесты
- C-тесты: `make test`
- Python-тесты: `make python_test`

## Статический анализ и санитайзеры
- `make analyze` — clang-tidy, лог в `reports/clang-tidy.log`
- `make sanitize` — AddressSanitizer + UBSan, запуск C-тестов

## Документация
make docs
Откроется `docs/html/index.html`.

## Очистка
make clean 

## Коды ошибок
- `HISTOGRAM_SUCCESS` (0)             — успех
- `HISTOGRAM_ERROR_NULL` (-1)         — NULL-указатель
- `HISTOGRAM_ERROR_INVALID_PARAM` (-2) — bins==0 или min>=max
- `HISTOGRAM_ERROR_OUT_OF_RANGE` (-3) — значение вне [min,max]
- `HISTOGRAM_ERROR_INDEX` (-4)        — некорректный индекс бина
- `HISTOGRAM_ERROR_MEMORY` (-5)       — ошибка выделения памяти
- `HISTOGRAM_ERROR_EMPTY` (-6)        — гистограмма пуста (find_max_bin)

## Вызов из Python (пример)
```python
import ctypes
lib = ctypes.CDLL('./libhistogram.so')
# ... описание типов
h = lib.histogram_create(5, 0.0, 10.0)
lib.histogram_add(h, 3.14)
# ... получение counts, частот, поиск максимума
lib.histogram_destroy(h)
