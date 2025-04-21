# example-robin-id
Из всего этого проекта, я писал только то, что связано с update

Это тестовый пример сервиса, написанного с использованием [userver](https://github.com/userver-framework/userver), [jwt-cpp](https://github.com/Thalhammer/jwt-cpp) и [libbcrypt](https://github.com/trusch/libbcrypt).

## Установка

Клонируем репозиторий
```bash
git clone https://github.com/Ptaha-fly/example-robin-id.git
cd example-robin-id
```

Подкачиваем необходимые библиотеки
```bash
git submodule update --init
```

Устанавливаем зависимости для userver ([подробнее](https://userver.tech/de/dab/md_en_2userver_2build_2build.html))
```bash
sudo apt update
sudo apt install --allow-downgrades -y $(cat third_party/userver/scripts/docs/en/deps/ubuntu-22.04.md | tr '\n' ' ')
```
## Запуск

Запуск сервиса локально или в docker.

### Локально
```bash
make service-start-release
```
### Docker
```bash
make docker-service-start-release
```
## Тесты

Тестировать сервис можно как локально, так и в docker.

### Локально
```bash
make test-release
```
### Docker
```bash
make docker-test-release
```
## Настройка VS Code

Для удобной работы в VS Code нужно установить расширение [clangd](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd).

Далее создать файл `compile_commands.json`. По этомй файлу clangd поймет, где искать заголовочные файлы. Делается это так
```bash
mkdir build
cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
cd ..
ln -s ./build/compile_commands.json compile_commands.json
```

Остается лишь открыть VS Code в папке пректа
```bash
code .
```

## Makefile

Makefile contains typicaly useful targets for development:

* `make build-debug` - debug build of the service with all the assertions and sanitizers enabled
* `make build-release` - release build of the service with LTO
* `make test-debug` - does a `make build-debug` and runs all the tests on the result
* `make test-release` - does a `make build-release` and runs all the tests on the result
* `make service-start-debug` - builds the service in debug mode and starts it
* `make service-start-release` - builds the service in release mode and starts it
* `make` or `make all` - builds and runs all the tests in release and debug modes
* `make format` - autoformat all the C++ and Python sources
* `make clean-` - cleans the object files
* `make dist-clean` - clean all, including the CMake cached configurations
* `make install` - does a `make build-release` and runs install in directory set in environment `PREFIX`
* `make install-debug` - does a `make build-debug` and runs install in directory set in environment `PREFIX`
* `make docker-COMMAND` - run `make COMMAND` in docker environment
* `make docker-build-debug` - debug build of the service with all the assertions and sanitizers enabled in docker environment
* `make docker-test-debug` - does a `make build-debug` and runs all the tests on the result in docker environment
* `make docker-start-service-release` - does a `make install-release` and runs service in docker environment
* `make docker-start-service-debug` - does a `make install-debug` and runs service in docker environment
* `make docker-clean-data` - stop docker containers and clean database data

Edit `Makefile.local` to change the default configuration and build options.


## License

The original template is distributed under the [Apache-2.0 License](https://github.com/userver-framework/userver/blob/develop/LICENSE)
and [CLA](https://github.com/userver-framework/userver/blob/develop/CONTRIBUTING.md). Services based on the template may change
the license and CLA.
