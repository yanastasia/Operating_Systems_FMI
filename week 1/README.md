Команди за работа с файлове и още команди за файлова система

Абсолютен vs относителен път

Aбсолютен път - този път, който започва от коренната директория (/), т.е. ако искаме да изпишем абсолютния път до файл/директория трябва да започнем с / и да изпишем всички директории. през които се преминава, за да се достигне до файла/директорията.

Относителен път - Когато се използва относителен път Linux счита, че пътя започва от текущата директория (тази в която се намираме в момента).
Например ако имаме директорията main, в нея имаме директориите dir1 dir2 dir3 и сме в /main/dir1, за да преминем от dir1 в dir3 трябва да използваме ../dir3

Забележка
С . се означава текуща директория, а с .. се означава предишната (родителска) директория.

Команди за работа с файловата система

pwd - дава абсолютния път на директорията, в която се намираме в момента.
date - дава ни системната дата
cd - чрез cd сменяме директорията. Можем да направим това чрез относителен или абсолютен път до директорията. -- cd /home/students/student/aa -- cd без аргумент ни води към началната ни потребителска директория
ls - тази команда без аргументи ни дава съдържанието на текущата директория (само имената на файловете). Също така, ако искаме да видим съдържанието на друга директория без да сменяме текущата можем да направим ls dirpath. Oпции, които ще използваме:
-- -l подрежда файловете в списък, като ни дава и повече информация за файловете.

-- -a извежда освен всички файлове и скритите (скритите са тези с точка пред името си)

-- -lh извежда информацията за файловете в human-readable format

clear - трие терминала
history - дава история на командите
tmux new - влизане в remote сесия

Работа с файлове и директории

cat file1 file2 ... - показва съдържанието на файл/овете

touch f1 f2 f3 ... - създава празни файлове

tee f1 f2 ... - създава файловете f1 f2 и ни позволява да пишем в тях едновременно

cp [source] [destination] - копира файлове

cp f1 f2 f3 path_to_dir - примерно използване

Също така можем да го използваме да копираме съдържанието от един файл в друг:

cp f1 f2 - копираме съдържанието на f1 във f2

mv f1 dir_to_be_moved - премества файла f1 в указаната директория

mv f1 f2 - преименува файла f1 на f2

mkdir f1 f2 ... - създава празни директории
mkdir -p dir1/dir2/dir3 - създава целия указан път, т.е. създава dir1, в нея създава dir2 и в dir2 създава dir3

rmdir f1 f2 ... - трие само празни директории

rm f1 f2 ... - трие файлове

rm -i f1 f2 ... - пуска ни запитване дали искаме да изтрием файловете

rm -r f1 dir .. - рекурсивно триене, чрез него можем да изтрием непразна директория

