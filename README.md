Изучение создания и использования потоков и механизмов синхронизации.
----------------------------
<ul>
<li> Разработать очередь заданий, в которую несколько потоков могут вставлять элементы атомарно.
<li> Разработать обработчик этой очереди, который извлекает из нее задания и раздает заданному количеству потоков.
<li> Разработать программу, которая использует очередь заданий и обработчик очереди для сортировки строк в текстовом файле:
Входной поток читает файл в память, нарезает его на части и создает несколько заданий на сортировку (по числу сортирующих потоков), 
которые помещает в очередь заданий. Сортирующие потоки извлекают задания, сортируют свои части файла, отдают все результаты 
выходному потоку. Выходной поток дожидается всех сортированных частей и объединяет их методом сортирующего слияния.
<ul/>
