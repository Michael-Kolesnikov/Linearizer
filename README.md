# Linearizer

**Linearizer** — это инструмент для преобразования кода на C в его линеаризованное представление.  

## Особенности
- Преобразование в последовательность элементарных операций.
- Поддержка базовых управляющих конструкций.

## Установка

```sh
git clone https://github.com/Michael-Kolesnikov/Linearizer.git
cd Linearizer
make run
```
Пример использования:
Исходный код — input.c 
```C
int main() {
    int a = 10;
    if(a>0){
        a += 1;
    }else{
        a -= 1;
    }
}
```
Полученный код — output.c
```C
int main() {
int a = 10;
if (a > 0) goto lbl1;
a -= 1;
goto lbl2;
lbl1:
a += 1;
lbl2:
}
```
