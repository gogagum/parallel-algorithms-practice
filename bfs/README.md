# bfs

## Краткое описание

`sequential_distance_counting_bfs` - обычный bfs.

`parallel_distance_counting_bfs` - bfs с `parallel_for` на обработке слоя.
`parallel_distance_counting_blocked_bfs` - к `parallel_distance_counting_bfs` был добавлен `for` внутри `parallel_for`,
что помогло достичь ускорения.

## Результаты замеров

```
Run on (12 X 2955.97 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x6)
  L1 Instruction 32 KiB (x6)
  L2 Unified 512 KiB (x6)
  L3 Unified 4096 KiB (x2)
Load Average: 5.85, 5.27, 4.84
-----------------------------------------------------------------------------------------------
Benchmark                                                     Time             CPU   Iterations
-----------------------------------------------------------------------------------------------
BM_benchmark_sequential_bfs<true>/iterations:5       1.5994e+10 ns   1.5993e+10 ns            5
BM_benchmark_sequential_bfs<false>/iterations:5      1.2785e+10 ns   1.2785e+10 ns            5
BM_benchmark_parallel_bfs/iterations:5               1.6509e+10 ns   1.6506e+10 ns            5
BM_benchmark_parallel_bfs_blocked<2048>/iterations:5 6778366208 ns   6776476562 ns            5
BM_benchmark_parallel_bfs_blocked<1024>/iterations:5 6969341411 ns   6969123197 ns            5
BM_benchmark_parallel_bfs_blocked<256>/iterations:5  7486959530 ns   7486724216 ns            5
```

## Анализ результата замеров

Если не отключать `atomic` для расстояния при обычном `bfs`, ускорение составит
примерно `2.36`, если рассматривать честный последовательный `bfs` - `1.88`.

## Другие комментарии

Класс вершины графа может отличаться в зависимости от используемого типа для
хранения расстояния. Для реализации параллельного обхода желателен атомарный
тип, поскольку он гарантирует единственное попадание вершины в слой обработки.
Его отсутствие слабо влияет на скорость работы алгоритма. 
