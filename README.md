cpp-everyday
============

C++ classes for use everyday

*README in progress...*

## common classes

Include all: `#include "common.hpp"`

* Configuration `#include "common/include/configuration.hpp"`
* HashConfiguration `#include "common/include/hash_configuration.hpp"`
* Logger `#include "common/include/logger.hpp"`
* DaemonLogger `#include "common/include/daemon_logger.hpp"`
* Singleton `#include "common/include/singleton.hpp"`

**Don't forget add *.cpp files to your Makefile**

- **common/src/logger.cpp**
- **common/src/daemon_logger.cpp**

### Configuration

Common class for configuration files. Load string lines from file and get formatted values.

### HashConfiguration

Class for load hash values from configuration file (extend *Configuration*).

### Logger

Common class for logging messages with 4 log levels: INFO, WARNING, ERROR, DEBUG. Write process run in other thread.

### DaemonLogger

Class for logging long time (extend *Logger*). Created path for every day, when program running and place in this path target log file.

### Singleton

Simple template of Singleton. 

## Simple example

```
#include <iostream>
#include "common.hpp"

int main(int argc, char** argv)
{
    rainlabs::DaemonLogger log("data");
    //log.enableDebug(true);
    rainlabs::HashConfiguration<int, atoi> simple_config("data/config.txt", &log);
    simple_config.load();
    log.info("test message");
    log.warning("warn text");
    printf("bla: %i\n", simple_config["bla"]);
    getchar();
    return 0;
}
```

## Using template "Singleton"

```
#include "cpp-everyday/common.hpp"

class SomeClass : public rainlabs::Singleton<SomeClass>
{
public:

    /**
     * ...
     * some code
     */

protected:

    /**
     * ...
     * some code
     */

    friend class rainlabs::Singleton <SomeClass>;
private:
    SomeClass();
    ~SomeClass();
};


int main(int* argc, char** argv)
{
    SomeClass::getInstance()->someMethod();
    return 0;
}
```
