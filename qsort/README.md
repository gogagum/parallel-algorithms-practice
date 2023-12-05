# Быстрая сортировка

## Краткое описание

### `sequential_quicksort`

Быстрая сортировка с выбором первого элемента в качестве опорного.

### `parallel_naive_quicksort`

Попытка написать параллельный алгоритм с максимальным использованием примитивов
стандартной библиотеки.

### `parallel_selective_quicksort`

То же самое, что и `parallel_naive_quicksort` только при сортировке маленьких
массивов начинает запускаться обычная сортировка. Соответственно, присутствует 
дополнительный параметр, регулирующий размер блока.

### `parallel_forkjoin_quicksort`

Получить хоть насколько-то значимого ускорения при помощи параллельных
`std::copy_if` и `std::copy` не получилось, поэтому в работе присутствует 
параллельная быстрая сортировка с fork-join.

## Результаты

В отдельном файле [results.txt](results.txt) результаты замеров. Вот их копия:

```
Run on (12 X 3080.64 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x6)
  L1 Instruction 32 KiB (x6)
  L2 Unified 512 KiB (x6)
  L3 Unified 4096 KiB (x2)
Load Average: 2.36, 3.72, 3.66
------------------------------------------------------------------------------------------------------------------
Benchmark                                                                        Time             CPU   Iterations
------------------------------------------------------------------------------------------------------------------
BM_benchmark_sequential_quicksort/100000000/iterations:5                2.1135e+10 ns   2.1069e+10 ns            5
BM_benchmark_parallel_naive_quicksort/100000000/iterations:5            1.2218e+11 ns   1.2183e+11 ns            5
BM_benchmark_parallel_selective_quicksort_1000/100000000/iterations:5   2.8003e+10 ns   2.7918e+10 ns            5
BM_benchmark_parallel_selective_quicksort_100000/100000000/iterations:5 2.4424e+10 ns   2.4316e+10 ns            5
BM_benchmark_parallel_forkjoin_quicksort/100000000/iterations:5         1.0664e+10 ns   1.0599e+10 ns            5
```

Время работы параллельного forkjoin алгоритма составляет 50,3% от времени работы последовательного алгоритма.

