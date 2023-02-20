# task 0 Create a tree 

mkdir baba
mkdir dyado
mkdir dyado/foo
touch dyado/bar
touch baba/banitsa

#OR

mkdir baba dyado dyado/foo
touch baba/banitsa dyado/bar

#02-a-0100.txt Направете копие на файла /etc/passwd във вашата home директория под името my_passwd.

cp /etc/passwd my_passwd

#02-a-0500.txt Направете директория practice-test в home директорията ви. Вътре направете директория test1. 
#Можете ли да направите тези две неща наведнъж? Разгледайте нужната man страница. След това създайте празен файл вътре, който да се казва test.txt, 
#преместете го в practice-test чрез релативни пътища.

mkdir practice-test practice-test/test1
touch practice-test/test1/test.txt
mv practice-test/test1/test.txt practice-test

#OR

mkdir -p ~/practice-test/test1
touch ~/practice-test/test1/test.txt
mv test.txt ~/practice-test

#02-a-0600.txt Създайте директорията practice/01 във вашата home директория. Създайте 3 файла в нея - f1, f2, f3.
#Копирайте файловете f1, f2, f3 от директорията practice/01/ в директория dir1, намираща се във вашата home директория. Ако нямате такава, създайте я.

mkdir -p practice/01
cd practice/01
touch f1 f2 f3
mkdir ~/dir1
cp f1 f2 f3 ~/dir1

#02-a-0601.txt Нека файлът f2 бъде преместен в директория dir2, намираща се във вашата home директория и бъде преименуван на numbers.

mkdir ~/dir2
mv ~/dir1/f2 ~/dir2/numbers

#02-a-1200.txt ​Отпечатайте имената на всички директории в директорията /home.

ls -d */
