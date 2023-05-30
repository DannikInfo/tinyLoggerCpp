# TinyLogger
[![Build Status](http://www1.biryusa.corp:3002/api/badges/kozlovskiy-di/tinyLogger/status.svg)](http://www1.biryusa.corp:3002/kozlovskiy-di/tinyLogger)  
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

std::thread::id mainThreadID = std::this_thread::get_id();
logger::setThread("MainThread", mainThreadID); //Именование потока, иначе будет undefinedThread

```
Допускается применение std::string или char *

Логи сохраняюстя в ```/var/log/program``` если у запускающего пользователя нет прав, то рядом с программой в папке ```log/program```
