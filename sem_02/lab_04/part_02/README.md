# Загружаемый модуль ядра и ВФС /proc

* fortune_buffer.c - стандартная реализация "Фортунки" с буффером.
* seqfile_cycle_buffer.c - тоже самое, но с использованием seq_file
* seqfile.c - просто печать (без кольцевого буфера) с использованием seqfile и seq_operations
* single_open_buffer - тоже самое, но без seq_operations (с использованием single_open)