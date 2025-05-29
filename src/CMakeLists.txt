cmake_minimum_required(VERSION 3.5)
project(3dViewer)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Поиск необходимых пакетов
find_package(Qt6 COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# библиотека gtest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Добавление исполняемого файла приложения
add_executable(3dViewer
    main.cpp
    viewer_model/strucutures.h
    viewer_model/viewer_model.h
    viewer_model/viewer_model.cpp
    viewer_controller/viewer_controller.h
    viewer_controller/viewer_controller.cpp
    viewer_view/viewer_view.h
    viewer_view/viewer_view.cpp
    viewer_facade/viewer_facade.h
    viewer_facade/viewer_facade.cpp
)

# Подключение библиотек для основного приложения
target_link_libraries(3dViewer
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)

# Создание пользовательской цели для тестов
add_custom_target(3dViewer_tests_target
    DEPENDS 3dViewer_tests
)

# Добавление исполняемого файла тестов
add_executable(3dViewer_tests
    unit-tests/tests.cpp
    viewer_model/strucutures.h
    viewer_model/viewer_model.h
    viewer_model/viewer_model.cpp
)

# Подключение библиотек для тестов
target_link_libraries(3dViewer_tests
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
    ${GTEST_LIBRARIES}
    --coverage
)

# Установка зависимости для цели tests
add_custom_target(tests
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target 3dViewer_tests
    COMMENT "Building and running tests"
)

# Добавление зависимости: цель test зависит от 3dViewer_tests_target
add_dependencies(tests 3dViewer_tests_target)

# Флаги компиляции
target_compile_options(3dViewer PRIVATE
    -Wall
    -Wextra
    -Werror
)
target_compile_options(3dViewer_tests PRIVATE
    -Wall
    -Wextra
    -Werror
    --coverage
)

# покрытие кода
add_custom_target(coverage
    COMMAND ./3dViewer_tests  # Запуск тестов
    COMMAND lcov --capture --directory . --output-file coverage.info --ignore-errors mismatch  # Сбор данных о покрытии
    COMMAND lcov --remove coverage.info '/usr/*' --output-file coverage.info  # Удаление системных файлов
    COMMAND genhtml coverage.info --output-directory coverage_report  # Генерация HTML-отчета
    COMMAND lcov --list coverage.info  # Вывод информации о покрытии в консоль
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}  # Рабочая директория
    COMMENT "Generating coverage report and HTML output"  # Комментарий
)

# clang-format
add_custom_target(clangn
    COMMAND ${CMAKE_COMMAND} -E copy ../../materials/linters/.clang-format .clang-format
    COMMAND clang-format -n ../main.cpp ../viewer_view/*.h ../viewer_view/*.cpp ../viewer_model/*.h ../viewer_model/*.cpp ../viewer_controller/*.h ../viewer_controller/*.cpp ../viewer_facade/*.h ../viewer_facade/*.cpp ../unit-tests/tests.cpp
    COMMAND ${CMAKE_COMMAND} -E remove .clang-format
    COMMENT "Running clang-format in check mode"
)
add_custom_target(clangi
    COMMAND ${CMAKE_COMMAND} -E copy ../../materials/linters/.clang-format .clang-format 
    COMMAND clang-format -i ../main.cpp ../viewer_view/*.h ../viewer_view/*.cpp ../viewer_model/*.h ../viewer_model/*.cpp ../viewer_controller/*.h ../viewer_controller/*.cpp ../viewer_facade/*.h ../viewer_facade/*.cpp ../unit-tests/tests.cpp
    COMMAND ${CMAKE_COMMAND} -E remove .clang-format
    COMMENT "Running clang-format in auto-fix mode"
)

# dvi
set(DOXYFILE_PATH "../Doxyfile")
add_custom_target(dvi
    COMMAND doxygen ${DOXYFILE_PATH}
    COMMENT "Generating documentation with Doxygen"
    VERBATIM
)

# dist
add_custom_target(dist
    COMMAND ${CMAKE_COMMAND} -E tar czf 3dviewer.tar.gz ./*
    COMMENT "Creating a distribution tarball"
)

# make install (после make)
# адрес установки: cmake -DCMAKE_INSTALL_PREFIX=/my/custom/path ..
install(TARGETS 3dViewer 3dViewer_tests
    RUNTIME DESTINATION bin #/usr/local/bin по умолч.
    LIBRARY DESTINATION lib #/usr/local/lib
    ARCHIVE DESTINATION lib #/usr/local/lib
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/brick_game/
    DESTINATION include/brick_game #/usr/local/include/brick_game
    FILES_MATCHING PATTERN "*.h"
)

# uninstall 
configure_file(
    "${CMAKE_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
    @ONLY
)
add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_uninstall.cmake
    COMMENT "Uninstalling snake_game"
)

# make clean_all для удаления всех файлов из папки build
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/*
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/Makefile
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CTestTestfile.cmake
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/DartConfiguration.tcl
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/cmake_uninstall.cmake
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/libsnake_game_static.a
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/coverage.info
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/3dViewer
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/snake.tar.gz
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/install_manifest.txt
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/3dviewer.tar.gz
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/3dViewer_tests
    COMMENT "Cleaning the build directory"
)