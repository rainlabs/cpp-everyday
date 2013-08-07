cpp-everyday
============

C++ classes for use everyday

## common classes

Include all: `#include "common.hpp"`

* Configuration `#include "common/include/configuration.hpp"`
* HashConfiguration `#include "common/include/hash_configuration.hpp"`
* Logger `#include "common/include/logger.hpp"`
* DaemonLogger `#include "common/include/daemon_logger.hpp"`

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
