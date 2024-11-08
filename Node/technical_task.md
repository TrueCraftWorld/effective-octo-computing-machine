## Техническое задание
### Разработка системы распределённых вычислений. 
Рассматриваемая ниже система может состоять из N узлов (далее именуемых Node). Для управления системой используется внешний и постановщик задачи (далее именуемый TaskManager). Node системы получивший соединение от TaskManager становится управляющим (ControlNode) и дополнительно выполняет распределение задачи остальным node и сбор результатов.
### Топология системы
**Полносвязная топология (полный граф)** — топология компьютерной сети, в которой каждая рабочая станция подключена ко всем остальным. Этот вариант является логически простым. Для каждой пары должно быть создано соединение.

#### Преимущества
- Имеется прямой канал до каждого узла в сети;
- Отказоустойчивость.

#### Недостатки
- Сложное расширение сети (при добавлении одного узла необходимо соединить его со всеми остальными);
- Огромное количество соединений при большом количестве узлов.

![Alt text here](images/basic_structure.drawio.png)


### Описание процедуры работы системы
**Вычислительный кластер:**
- Кластер состоит из N Node. (N >= 1)
- Каждый запущенный экземпляр приложения - отдельный узел.
- При запуске приложение формирует и хранит случайное число (приоритет).
- Приложение собирает и сохраняет информацию о вычислительной мощности узла.*  
- Каждый запщуенный экземпляр приложения по протоколу UDP отправляет широковещательный пакет (ID_CONNECT) с идентификатором для установления соединения каждый с каждым по протоколу TCP.
- UDP пакет содержит IP адрес отправителя, порт, номер приоритета и вычислительную мощность.
- При получении UDP пакета приложение формирует список доступных узлов для установления соединения.
- Соединение устанавливает то приложение у которого ниже приоритет, а если приоритеты совпали проверяется у кого меньше последний три цифры в IP адресе. Для локального варианта соединение устанавливает приложение с меньшим номером порта.
- После того как все связи будут установлены в вычислительном кластере необходимо замерить пропускную способность. Инициатором замера скорости выступает приложение которое установило связь. Для замера скорости воспользуемся сторонней утилитой iperf.
- После замера скорости, скорость обмена сохраняется в приложении выполнявшее замер, а также данное приложение отправляет данную информацию узлу который участвовал в обмене.
- В результате каждое приложение содержит таблицу: IP узлов кластера, порт, приоритет, вычислительная мощность узла и пропускная способность канала.
- Получив размер входных данных от TaskManager, узел с учётом метрики (учитывается вычислительная мощность и пропускная способность сети) составляется план по разбиению данных по узлам.** Если данных меньше чем узлов, вычислительная нагрузка ложится на узлы с более производительной метрикой. 
- Получив пакеты от TaskManager, узел распределяет их по вычислительному кластеру в соответствии с подготовленным планом.
- После завершения вычислений данные отправляются обратно узлу от которого они были получены, а он их отправляет в TaskManager.
- Допускается ситуация, при которой Node выполнит всю задачу сам

*Так как вычислительную мощность узла стандартными средствами можно узнать только под управлением nix подобными системами, а мы не хотим ограничиваться только определенными ОС и не устанавливать для этого дополнительное ПО. Необходимо реализовать алгоритм с заранее известной сложностью. Воспользуемся алгоритмом быстрого преобразования Фурье (БПФ). Данный алгоритм имеет известную сложность O(NlogN). За N примем 2048 элементов. Замерив время выполнения данного алгоритма получим производительность узла.  
**Оценка зависимости объёма эффективно обрабатываемых данных от скорости соединения и вычислительной мощности узла должна быть определена на этапе разработки. 

**TaskManager**
- Может поключиться к любому извустному узлу кластера до начала обработки задачи.
- TaskManager имеет доступ к массиву данных и математической функции (задаётся в обратной польской нотации, используемые операции +, -, /, \*, sin, cos, tan, ctan).
- При запуске TaskManager в качестве параметра необходимо передать IP адрес узла к которому хотим подключиться и порт подключения (опция -n, node), имя файла (путь до файла) где лежат данные (опция -i, input), имя файла (путь до файла) где будет сохранен результат вычисления (опция -o, output). Если имя выходного файла не задано результат вычисления будет выводится в стандартный поток вывод (stdout).
- На стороне TaskManager выполняется разбор функции (обратная польская нотация) и формируется математическое выражение.
- После подключения TaskManager передаёт узлу размер обрабатываемых данных, математическую функцию и пакеты содержащие данные с учётом их расположения в исходном файле.
- Полученные результаты вычислений собираются в выходной массив данных, сохраняются в файле на стороне TaskManager или выводятся в стандартный поток вывода.
- При количестве данных меньшем чем количесво доуступных узлов Управляющий узел оценивает необходимое количество узлов для эффективного выполнения задачи на основе метрик сети и производительности и рассылает задачу только в выбранные узлы.


### Формат входных и выходных данных
Входные данные для вычислительного кластера представляют собой текстовый файл. Данный файл содержит числовые значения записанные в один столбец. Функция применяемая к данным хранится в текстовом файле и представляет собой математическую формулу записанную в обратной польской нотации.
Выходные данные записываются в отдельный файл в той же последовательности как они были записаны во входном файле.


### Базовые сценарии тестирования
Покрытие системы unit-тестами, валидация желаемой работы каждого класса
Подготовка moc-объектов для проверки работоспособности отдельных подсистем - сеть, рапределение задач, вычисление, общение с пользователем

Проверка готовой системы на следующих сценариях работы:
Проверяем следующие критерии системы:  
1. Масштабируемость. Проверяем построением вычислительного кластера с разным количеством узлов. Создаем кластеры с одним узлом, с количесвом узлов равным количеству ядер тестовой машины, кластер на нескольких машинах.  
1.1. Проверяем отработку выключения узлов до начала выполнения задачи
2. Выполнение математических функций. Проверку математических функции разбиваем на два этапа. На первом этапе выполняем проверка каждой математической операции отдельно (+, -, /, *, sin, cos, tan, ctan) на элементарной последовательности чтобы можно было глазами проверить правильность выполнения.  
На втором этапе задаем математические выражения в разных сочетаниях.  
3. Распределение вычислительной нагрузки. Проверка распределения вычислительной нагрузки проверяется изменением метрики (уменьшаем/увеличиваем вычислительную мощность узлов, уменьшаем/увеличиваем пропускную способность соединений).  
