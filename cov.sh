#!/bin/bash
set -e  # Остановить при любой ошибке

# 1. Подготовка сборочной папки
if [ -d "build" ]; then
    echo "Удаляю существующую папку build/"
    rm -rf build
fi

mkdir build
cd build

# 2. Генерация Make-файлов
cmake ..

# 3. Сборка проекта
cmake --build .

# 4. Очистка предыдущих счётчиков
lcov --directory . --zerocounters

# 5. Запуск тестов
./Calculator_test

# 6. Сбор покрытия с игнорированием ошибок GTest-макросов
lcov --directory . --capture --output-file coverage.info --ignore-errors mismatch

# 7. Удаление мусора: stdlib, GTest, зависимости
lcov --remove coverage.info '/usr/*' '*/_deps/*' --output-file coverage_filtered.info

# 8. Генерация HTML-отчёта
genhtml coverage_filtered.info --output-directory coverage_report

# 9. Открытие в браузере
xdg-open coverage_report/index.html