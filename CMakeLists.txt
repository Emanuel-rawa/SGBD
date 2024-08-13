cmake_minimum_required(VERSION 3.28)

project(SGBD)

set(CMAKE_C_STANDARD 11)

# Incluindo os diretórios de cabeçalho
include_directories(include)

# Especificando os arquivos fonte para mylib
file(GLOB_RECURSE MYLIB_SOURCES "src/*.c" "include/*.h")
add_library(mylib ${MYLIB_SOURCES})

# Criando o executável principal
add_executable(main src/main.c)
target_link_libraries(main mylib)
