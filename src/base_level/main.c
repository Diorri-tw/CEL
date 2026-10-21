#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>

#define MEMORY_SIZE 4096
#define START_ADRESS 0x200
#define REGISTERED_COUNT 16
#define STACK_SIZE 16

typedef struct {
    uint8_t memory[MEMORY_SIZE];
    uint8_t V[REGISTERED_COUNT];
    uint16_t I;
    uint16_t pc;
    uint16_t stack[STACK_SIZE];
    uint8_t sp;
    uint8_t delay_timer;
    uint8_t sound_timer;
} Chip8;

int load_rom(Chip8 *chip8, const char *filename) {
    FILE *rom = fopen(filename, "rb");

    if (rom == NULL) {
        printf("Ошибка: Не удалось открыть файл %s\n", filename);
        return 0;
    }

    fseek(rom, 0, SEEK_END);
    long rom_size = ftell(rom);
    rewind(rom);

    printf("Успешно нашли ROM. Размер: %ld байт.\n", rom_size);

    if (rom_size > (MEMORY_SIZE - START_ADRESS)) {
        printf("Ошибка: ROM слишком большой для памяти Chip-8!\n");
        fclose(rom);
        return 0;
    }

    fread(&chip8->memory[START_ADRESS], sizeof(uint8_t), rom_size, rom);

    fclose(rom);
    return 1;
}

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    Chip8 cpu = {0};

    cpu.pc = START_ADRESS;

    printf("Эмулятор Chip-8 инициализирован\n");

    const char *rom_path = "roms/Pong.ch8";

    if (load_rom(&cpu, rom_path)) {
        printf("ROM успешно загружен в виртуальную память\n");

        for (int i = 0; i < 10; i += 2) {
            uint8_t byte1 = cpu.memory[START_ADRESS + i];
            uint8_t byte2 = cpu.memory[START_ADRESS + i + 1];
            printf("Адрес 0x%03X: %02X%02X\n", START_ADRESS + i, byte1, byte2);
        }
    }

    return 0;
}
