# Формат данных для модулей
Файл с формулой:
Содержит в себе числа и символы, идущие в столбик, например

5.4321
S
X
+

Обозначения:
Число - некоторая константа, с которой будут проводится операции
"+" - знак сложения
"-" - знак вычитания
"*" - знак умножения
"/" - знак деления
"S" - взятие синуса
"С" - взятие косинуса
"T" - взятие тангенса
"Y" - взятие котангенса
"X" - некоторое значение из массива данных

Файл с данным
Представляет из себя файл с набором чисел, идущих в столбик, например

4.32
1E8
0.1234502

Discovery-сервис
Хранит в себе список данных о других узлах
IP - quint32
Port - quint16
Priority - quint32
Mips (Производительность) - double
Throughput (Пропускная способность) - double

Вычислительное ядро
Ждет на входt последовательность байт QByteArray, в которой закодирована последовательность действий

REG 4.2 REG 2.3 MUL

где
```c++
constexpr char START = 0x00;
constexpr char REG = 0x01;
constexpr char ARR = 0x02;

constexpr char ADD = 0x03;
constexpr char SUB = 0x04;
constexpr char MUL = 0x05;

constexpr char SIN = 0x06;
constexpr char COS = 0x07;
constexpr char TAN = 0x08;
constexpr char CTAN = 0x09;

constexpr char DIV = 0x0A;
constexpr char END = 0x0B;
```


# Ссылка на оригинал
https://app.diagrams.net/?src=about#G1C3g-jYotdKjMz7idopCemMiMy58YbjSy

# Этапы работы TaskManager
![t drawio](https://github.com/TrueCraftWorld/effective-octo-computing-machine/assets/78808066/4d8359f6-2fd3-4e8d-9386-221ff1f25f99)

# Этапы работы узла при подключении к уже работающему кластеру

## Этап 1: Формирование данных об узле и их отправка
![f1 drawio](https://github.com/TrueCraftWorld/effective-octo-computing-machine/assets/78808066/99569d09-c7d4-43d2-b7f0-357d095fb8bb)
Если это первый узел в кластере, то ответа по UDP не приходит. Работа узла переходит на "статическое состояние узла"

## Этап 2: Прием данных о других узлах и их обработка
![f2 drawio](https://github.com/TrueCraftWorld/effective-octo-computing-machine/assets/78808066/0a2af1c0-ac39-43cb-becc-d80e4bfebe00)


## Этап 3: Бенчмарк сети и формирование списка для MapReduce
![f3 drawio](https://github.com/TrueCraftWorld/effective-octo-computing-machine/assets/78808066/71272382-df19-4170-b19a-af6cb5c5464c)


# Статическое состояние узла
![s drawio](https://github.com/TrueCraftWorld/effective-octo-computing-machine/assets/78808066/a3ab629a-9da0-48e8-8f79-a4f256583404)


# Этапы подключения нового узла
![c drawio](https://github.com/TrueCraftWorld/effective-octo-computing-machine/assets/78808066/4c6fd134-c4d0-4863-9233-432c07417d3f)
После данного этапа происходит последовательность, аналогичная 3-ему этапу "работы узла при подключении к уже работающему кластеру". Затем происходит переход к статическому состоянию

# Этапы вычисления основного узла
## Этап 1: Бенчмарк сети
![m1 drawio](https://github.com/TrueCraftWorld/effective-octo-computing-machine/assets/78808066/47ffb344-ade4-453d-84ce-d411ed1576ff)

Далее возможно некоторое разветвление в зависимости от того, параллельно ли мы отправляем данные с обработкой или отправляем только после того, как все сделали

## Ветвь 1, Этап 2
![qweqwe drawio](https://github.com/TrueCraftWorld/effective-octo-computing-machine/assets/78808066/42f056be-0cf8-4896-874a-b5b9b5ae54a8)

## Ветвь 1, Этап 3
![m31 drawio (1)](https://github.com/TrueCraftWorld/effective-octo-computing-machine/assets/78808066/72c4a718-034c-4a99-8e71-327fb42e7228)

## Ветвь 2, Этап 2
![m22 drawio](https://github.com/TrueCraftWorld/effective-octo-computing-machine/assets/78808066/2e77f192-c7a5-434c-b241-68b039b078ef)

# Этапы вычисления поддерживающего узла
## Этап 1: Идентификация себя, как поддерживающего
![ss1 drawio](https://github.com/TrueCraftWorld/effective-octo-computing-machine/assets/78808066/e28212cd-72ca-4532-8edd-e077c807bdb7)

## Этап 2: Проведение вычислений
![ss2 drawio](https://github.com/TrueCraftWorld/effective-octo-computing-machine/assets/78808066/5634df14-f8eb-4f88-a373-fcaedca504f0)
