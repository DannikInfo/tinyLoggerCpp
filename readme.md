#TinyLogger
Библиотека регистратор логов для с++  

Использование простое:
```c++
#include "logger.h"

logger::init("program", 50, 10); //название программы, максимальный размер одного файла в МБ, максимальное кол-во файлов одновременно
...
logger::info("text message");
logger::warn("text message");
logger::error("text message");
logger::success("text message");
...
```
Допускается использование std::string или char *

Логи сохраняюстя в ```/var/log/program``` если у запускающего пользователя нет прав, то рядом с программой в папке ```log/program```
