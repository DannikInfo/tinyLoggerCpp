#TinyLogger
Библиотека регистратор логов для с++  

Сборка:
```shell
mkdir build
cd build
cmake ..
sudo make install
```

Использование:
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
Допускается применение std::string или char *

Логи сохраняюстя в ```/var/log/program``` если у запускающего пользователя нет прав, то рядом с программой в папке ```log/program```
