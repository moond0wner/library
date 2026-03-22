cmake_minimum_required(VERSION 3.14) # Рекомендуется 3.14+ для FetchContent_MakeAvailable
project(Library)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Подключаем модуль для загрузки контента из сети
include(FetchContent)

# 2. Объявляем библиотеку nlohmann/json
FetchContent_Declare(
  json
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
# 3. Скачиваем и делаем библиотеку доступной для проекта
FetchContent_MakeAvailable(json)

# 4. Создаем исполняемый файл
add_executable(library
    src/main.cpp
    src/Book.cpp
    src/Reader.cpp
    src/Library.cpp
    src/Utils.cpp
    src/Interface.cpp
    src/Validation.cpp )

# 5. Связываем твой проект с библиотекой nlohmann_json
# Библиотека nlohmann_json является header-only, поэтому линковка произойдет мгновенно
target_link_libraries(library PRIVATE nlohmann_json::nlohmann_json)