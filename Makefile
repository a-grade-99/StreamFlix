CC = gcc
CFLAGS = -Wall -Wextra -g
SOURCE_DIR = source
HEADER_DIR = headers
TEST_DIR = testes

INCLUDES = -I$(HEADER_DIR)

# Fontes do programa principal
APP_SOURCES = $(filter-out $(TEST_DIR)/%.c,$(wildcard $(SOURCE_DIR)/*.c))
APP_OBJECTS = $(APP_SOURCES:.c=.o)
APP_TARGET = streamflix

# Fontes comuns (sem main.c, para usar nos testes)
COMMON_SOURCES = $(filter-out $(SOURCE_DIR)/main.c,$(APP_SOURCES))
COMMON_OBJECTS = $(COMMON_SOURCES:.c=.o)

# Testes
TEST_INTEGRATION_SRC = $(TEST_DIR)/testesDeIntegracao.c
TEST_UNIT_SRC = $(TEST_DIR)/testesUnitarios.c
TEST_INTEGRATION_OBJ = $(TEST_INTEGRATION_SRC:.c=.o)
TEST_UNIT_OBJ = $(TEST_UNIT_SRC:.c=.o)
TEST_TARGET = testes
TEST_UNIT_TARGET = testes_unitarios

# Default target
all: $(APP_TARGET)

# Programa principal
$(APP_TARGET): $(APP_OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

# Testes de integração + unitários (main dos testes)
$(TEST_TARGET): $(COMMON_OBJECTS) $(TEST_INTEGRATION_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

# Apenas testes unitários
$(TEST_UNIT_TARGET): $(COMMON_OBJECTS) $(TEST_UNIT_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

# Regra geral de compilação
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Limpeza
clean:
	del /Q $(SOURCE_DIR)\*.o $(TEST_DIR)\*.o *.exe 2> NUL
	rmdir /S /Q test_files 2> NUL
