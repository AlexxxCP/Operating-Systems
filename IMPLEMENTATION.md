# Файлы реализаций

Структура ориентирована на отдельные каталоги лабораторных с отчётами, кодом и инструкциями. Материалы восстановлены из собственных шести PDF.

| Работа | Дополнительные файлы |
| --- | --- |
| [Lab 1](lab1/) | [commands.sh](lab1/commands.sh) |
| [Lab 2](lab2/) | [user/sleep.c](lab2/user/sleep.c), [Makefile.inc](lab2/Makefile.inc) |
| [Lab 3](lab3/) | [generate_access_log.sh](lab3/generate_access_log.sh), [oneliners.sh](lab3/oneliners.sh), [report.sh](lab3/report.sh), [data/access.log](lab3/data/access.log), [expected-report.txt](lab3/expected-report.txt) |
| [Lab 4](lab4/) | [user/sleep.c](lab4/user/sleep.c), [user/args.c](lab4/user/args.c), [args_linux.c](lab4/args_linux.c), [Makefile.inc](lab4/Makefile.inc), [xv6-commands.txt](lab4/xv6-commands.txt) |
| [Lab 5](lab5/) | [commands.sh](lab5/commands.sh) |
| [Lab 6](lab6/) | [user/mystat.c](lab6/user/mystat.c), [user/find.c](lab6/user/find.c), [Makefile.inc](lab6/Makefile.inc), [xv6-commands.txt](lab6/xv6-commands.txt) |

## Как восстановлены материалы

- Lab 1 и Lab 5: команды собраны в сценарии с отдельной временной рабочей папкой.
- Lab 2: `sleep.c` переписан из полного листинга с вызовом `pause()`.
- Lab 3: скрипт сводки и однострочные команды переписаны из отчёта. Генератор воспроизводит исходную формулу журнала; проверка аргумента добавлена явно.
- Lab 4: xv6-листинги переписаны из отчёта; Linux-вариант восстановлен по описанию и выводу.
- Lab 6: к опубликованным в PDF фрагментам `mystat` и `find` добавлены заголовки, `main()`, проверки аргументов, длины пути и ошибок.
- `Makefile.inc` содержит только добавление к `UPROGS`; исходное дерево xv6 требуется отдельно.

Служебные файлы macOS и промежуточные файлы сборки исключены через `.gitignore`. Форматы PDF и DOCX отмечены как бинарные в `.gitattributes`.

## Проверка

- Все шесть PDF сверены по SHA-256 с исходными файлами: содержимое не изменилось.
- Проверены ZIP-структура и XML всех шести DOCX, наличие текста и всех исходных страниц в конвертированных отчётах.
- Проверены локальные ссылки в Markdown: 91.
- Все пять Bash-сценариев прошли проверку синтаксиса. Для Lab 3 выполнены генерация 500 строк, точное сравнение сводки, проверки Q1–Q6, отсутствующего аргумента/файла, пустого журнала, журнала без ошибок и запрета перезаписи.
- Linux-вариант args собран GCC с предупреждениями как ошибками и запущен с проверкой аргументов, содержащих пробелы.
- Пять C-файлов xv6 прошли проверку типов и синтаксиса GCC с официальными заголовками [mit-pdos/xv6-riscv](https://github.com/mit-pdos/xv6-riscv/tree/f780d7850765d74cbdcd1fff4b822d8829f7c9bd).
- Полная сборка и запуск xv6 в QEMU, а также Linux-эксперименты с /proc и правами доступа на этом Windows-хосте не выполнялись.
