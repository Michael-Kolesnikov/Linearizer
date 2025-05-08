# Linearizer

**Linearizer** — это инструмент для преобразования кода на языке C в его линеаризованную форму. Он упрощает вложенные выражения и управляющие конструкции, представляя их в виде линейной последовательности элементарных операций и меток перехода.

## Установка
1. Клонировать репозиторий
```sh
git clone https://github.com/Michael-Kolesnikov/Linearizer.git
cd Linearizer
```
2. Установить зависимости (на Ubuntu/Debian)
```sh
sudo apt update
sudo apt install -y build-essential flex bison check libsubunit-dev
```
3. Сборка проекта
```sh
make all
```
4. Запуск проекта
```sh
make run
```
## Использование проекта
1. Напишите код исходной функции в файле `input.c`. Файл должен содержать только одну функцию.
2. Запустите обработку с помощью `make run`
3. Результат работы программы находится в `output.c`
## Примеры
### Условный оператор
<table>
<tr>
<th>Входная функция</th>
<th>Линеаризованная функция</th>
</tr>
<tr>
<td>
  
```C
void check_number(int number) {
    if (number > 0) {
        printf("Число %d положительное.\n", number);
        
        if (number % 2 == 0) {
            printf("Число %d четное.\n", number);
        } else {
            printf("Число %d нечетное.\n", number);
        }
    } else if (number < 0) {
        printf("Число %d отрицательное.\n", number);
    } else {
        printf("Число равно нулю.\n");
    }
}
```
  
</td>
<td>

```C
void check_number(int number){
int __internal_temp1 = (number > 0);
if (__internal_temp1) goto lbl1;
int __internal_temp2 = (number < 0);
if (__internal_temp2) goto lbl3;
printf("Число равно нулю.\n");
goto lbl4;
lbl3:
printf("Число %d отрицательное.\n", number);
lbl4:goto lbl2;
lbl1:
printf("Число %d положительное.\n", number);
int __internal_temp3 = number % 2;
int __internal_temp4 = (__internal_temp3 == 0);
if (__internal_temp4) goto lbl5;
printf("Число %d нечетное.\n", number);
goto lbl6;
lbl5:
printf("Число %d четное.\n", number);
lbl6:
lbl2:
}

```

</td>
</tr>
</table>

#### Цикл While
<table>
<tr>
<th>Входная функция</th>
<th>Линеаризованная функция</th>
</tr>
<tr>
<td>
  
```C
void fibonacci_up_to_n(int n) {
    int a = 0, b = 1;
    
    while (a <= n) {
        printf("%d ", a);
        int temp = a;
        a = b;
        b = temp + b;
    }
    printf("\n");
}
```

</td>
<td>
  
```C
void fibonacci_up_to_n(int n){
int a = 0, b = 1;
lbl1:
int __internal_temp1 = a <= n;
int __internal_temp2 = !__internal_temp1;
if(__internal_temp2) goto lbl2;
printf("%d ", a);
int temp = a;
a = b;
b = temp + b;
goto lbl1;
lbl2:
printf("\n");
}
```

</td>
</tr>
</table>

#### Цикл DoWhile
<table>
<tr>
<th>Входная функция</th>
<th>Линеаризованная функция</th>
</tr>
<tr>
<td>
  
```C
void fibonacci_up_to_n(int n) {
    int a = 0, b = 1;

    if (a > n) {
        printf("\n");
        return;
    }

    do {
        printf("%d ", a);
        int temp = a;
        a = b;
        b = temp + b;
    } while (a <= n);

    printf("\n");
}

```
</td>
<td>

```C
void fibonacci_up_to_n(int n){
int a = 0, b = 1;
int __internal_temp1 = a > n;
if (__internal_temp1) goto lbl1;
goto lbl2;
lbl1:
printf("\n");
return ;
lbl2:
lbl3:
printf("%d ", a);
int temp = a;
a = b;
b = temp + b;
int __internal_temp2 = (a <= n);
if (__internal_temp2) goto lbl3;
printf("\n");
}
```

</td>
</tr>
</table>


#### Цикл For
<table>
<tr>
<th>Входная функция</th>
<th>Линеаризованная функция</th>
</tr>
<tr>
<td>
  
```C
long long factorial(int n) {
    long long result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}
```
</td>
<td>

```C
long long factorial(int n){
long long result = 1;
int i = 1;
lbl1:
int __internal_temp1 = (i <= n);
int __internal_temp2 = !__internal_temp1;
if (__internal_temp2) goto lbl2;
result *= i;
i++;
goto lbl1;
lbl2:
return result;
}
```

</td>
</tr>
</table>
