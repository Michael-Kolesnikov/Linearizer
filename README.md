# Linearizer

**Linearizer** — это инструмент для преобразования кода на C в его линеаризованное представление.  

## Особенности
- Преобразование в последовательность элементарных операций.
- Поддержка базовых управляющих конструкций.

## Установка
### Клонирование репозитория
```sh
git clone https://github.com/Michael-Kolesnikov/Linearizer.git
cd Linearizer
```
### Установка зависимостей
На Ubuntu/Debian:  
```sh
sudo apt-get update
sudo apt-get install -y build-essential flex bison check libsubunit-dev
```

### Сборка проекта
```sh
make all
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
