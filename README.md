# Operating Systems

Десять лабораторных работ по дисциплине **Operating Systems**, Technical University of Moldova.

Студент: **Luchiciov Alexei**, группа **FAF-241**. Проверила: **Reiţman Patricia**.

## Лабораторные работы

| № | Тема | Среда | Отчёт |
| --- | --- | --- | --- |
| [1](lab1/) | The OS as a Resource Manager | Linux | [PDF](lab1/Lab1_Luchiciov_Alexei_Reitman_Patricia.pdf) · [Markdown](lab1/report.md) · [DOCX](lab1/report.docx) |
| [2](lab2/) | Meet the OS You Will Build | xv6 | [PDF](lab2/Lab2_Luchiciov_Alexei_Reitman_Patricia.pdf) · [Markdown](lab2/report.md) · [DOCX](lab2/report.docx) |
| [3](lab3/) | The Command Line: Shell, Pipes and Text Filters | Linux | [PDF](lab3/Lab3_Luchiciov_Alexei_Reitman_Patricia.pdf) · [Markdown](lab3/report.md) · [DOCX](lab3/report.docx) |
| [4](lab4/) | The xv6 Shell from the Outside | xv6 | [PDF](lab4/Lab4_Luchiciov_Alexei_Reitman_Patricia.pdf) · [Markdown](lab4/report.md) · [DOCX](lab4/report.docx) |
| [5](lab5/) | The File System with Your Own Hands | Linux | [PDF](lab5/Lab5_Luchiciov_Alexei_Reitman_Patricia.pdf) · [Markdown](lab5/report.md) · [DOCX](lab5/report.docx) |
| [6](lab6/) | Inside the xv6 File System | xv6 | [PDF](lab6/Lab6_Luchiciov_Alexei_Reitman_Patricia.pdf) · [Markdown](lab6/report.md) · [DOCX](lab6/report.docx) |
| [7](lab7/) | Three Small Programs: Processes, Redirection, a Pipe | Linux | [PDF](lab7/Lab7_Luchiciov_Alexei_Reitman_Patricia.pdf) · [Markdown](lab7/report.md) · [DOCX](lab7/report.docx) |
| [8](lab8/) | The Same Calls, Inside Your Own OS | xv6 | [PDF](lab8/Lab8_Luchiciov_Alexei_Reitman_Patricia.pdf) · [Markdown](lab8/report.md) · [DOCX](lab8/report.docx) |
| [9](lab9/) | Watching System Calls with strace | Linux | [PDF](lab9/Lab9_Luchiciov_Alexei_Reitman_Patricia.pdf) · [Markdown](lab9/report.md) · [DOCX](lab9/report.docx) |
| [10](lab10/) | Add Your Own System Call: trace | xv6 | [PDF](lab10/Lab10_Luchiciov_Alexei_Reitman_Patricia.pdf) · [Markdown](lab10/report.md) · [DOCX](lab10/report.docx) |

В каждой папке `lab1`–`lab10` находятся отчёт, файлы с программами или командами и README с инструкциями запуска.

## Форматы и происхождение файлов

- **PDF** — исходные отчёты, сохранённые без изменения содержимого.
- **Markdown** — текст отчёта для чтения прямо на GitHub.
- **DOCX** — редактируемая версия, восстановленная из PDF.
- **C и Bash** — листинги и команды из отчётов; недостающие части восстановленных программ отмечены в описаниях.
- **Makefile.inc** — фрагменты для подключения пользовательских программ к существующей сборке xv6.
- **Данные Lab 3** — воспроизводимый пример журнала из 500 запросов и ожидаемая сводка.
- **Lab 7 и Lab 9** — Makefile и сценарии проверок для Ubuntu; Lab 9 также содержит команды для получения новых журналов `strace`.
- **Lab 10** — полный патч системного вызова `trace` для указанной версии xv6 и проверка диспетчера на компьютере без QEMU.

При конвертации PDF могут измениться переносы, таблицы и расположение текста. Исходное оформление и зафиксированные результаты доступны в PDF. Отдельных встроенных изображений в исходных отчётах нет.

## Запуск

Linux-эксперименты выполняются в Ubuntu; программы xv6 собираются в существующем дереве `xv6-riscv` и запускаются в QEMU. Подробные шаги находятся в README каждой лабораторной.

В отчётах используется версия xv6 с системным вызовом `pause(int)`. Это учтено в `sleep.c`; перед сборкой проверьте объявления в `user/user.h`.

Для Lab 10 патч подготовлен к фиксированной версии xv6 с `SYS_sync = 22`, `SYS_trace = 23` и `printk`. Инструкция применения и команды проверки находятся в [lab10/README.md](lab10/README.md). Зафиксированные результаты из отчётов и проверки восстановленного кода описаны отдельно.

[Карта файлов и сведения о проверке](IMPLEMENTATION.md)
