#!/bin/sh
# проверка на количество аргументов
if [ $# -ne 3 ]; then
echo "format: .sh <command> <stdout_file or terminal> <stderr_file or
terminal>"
exit 1
fi
# запись аргументов в переменные
STDOUT=$2
STDERR=$3
if [ "$2" = "terminal" ] # если stdout в консоль
then
if [ "$3" = "terminal" ] # если stderr в консоль
then
./$1
echo $?
exit 1
else # если stderr в файл
# удаление существующего файла, если такой имеется
if [ -f "$STDERR" ]
then
rm $STDERR
fi
exec 2>"$3" #stderr в файл из аргумента3
./$1
echo $?
echo $? >>"$3" #код выхода в файл из аргумента3
exit 1
fi
else # если stdout в файл
# удаление существующего файла, если такой имеется
if [ -f "$STDOUT" ]
then
rm $STDOUT
fi
exec 6>&1 # создание временного потока
exec > "$2" # запись stdout в файл
if [ "$3" = "terminal" ] # запись stderr в консоль
6
then
./$1
exec 1>&6 6>&- # stdout в состояние по умолчанию
echo $?
echo $? >> "$2" #код выхода в файл из аргумента2
exit 1
else # запись stderr в файл
# удаление существующего файла, если такой имеется
if [ -f "$STDERR" ]
then
rm $STDERR
fi
exec 2>"$3" # поток stderr в файл из аргумента3
./$1
exec 1>&6 6>&- # stderr в состояние по умолчанию
# код выхода во все источники
echo $?
echo $? >> "$2"
echo $? >> "$3"
exit 1
fi
fi
exit 0