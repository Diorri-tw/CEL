#include <stdio.h>
#include <stdint.h>
#include <windows.h>

#define MEMORY_SIZE 4096
#define START_ADRESS 0x200
#define COMMANDS_TO_SHOW 30

void print_mnemonic(uint16_t op) {
    uint8_t x = (op >> 8) & 0x0F;
    uint8_t y = (op >> 4) & 0x0F;
    uint8_t n = op & 0x0F;
    uint8_t nn = op & 0xFF;
    uint16_t nnn = op & 0x0FFF;

    switch (op >> 12) {
        case 0x0:
            if (op == 0x00E0) printf("CLS");
            else if (op == 0x00EE) printf("RET");
            else printf("SYS 0x%03X", nnn);
            break;
        case 0x1: printf("JP 0x%03X", nnn); break;
        case 0x2: printf("CALL 0x%03X", nnn); break;
        case 0x3: printf("SE V%X, 0x%02X", x, nn); break;
        case 0x4: printf("SNE V%X, 0x%02X", x, nn); break;
        case 0x5: printf("SE V%X, V%X", x, y); break;
        case 0x6: printf("LD V%X, 0x%02X", x, nn); break;
        case 0x7: printf("ADD V%X, 0x%02X", x, nn); break;
        case 0x8:
            switch (n) {
                case 0x0: printf("LD V%X, V%X", x, y); break;
                case 0x1: printf("OR V%X, V%X", x, y); break;
                case 0x2: printf("AND V%X, V%X", x, y); break;
                case 0x3: printf("XOR V%X, V%X", x, y); break;
                case 0x4: printf("ADD V%X, V%X", x, y); break;
                case 0x5: printf("SUB V%X, V%X", x, y); break;
                case 0x6: printf("SHR V%X", x); break;
                case 0x7: printf("SUBN V%X, V%X", x, y); break;
                case 0xE: printf("SHL V%X", x); break;
                default:  printf("??? 8%X%X%X", x, y, n); break;
            }
            break;
        case 0x9: printf("SNE V%X, V%X", x, y); break;
        case 0xA: printf("LD I, 0x%03X", nnn); break;
        case 0xB: printf("JP V0, 0x%03X", nnn); break;
        case 0xC: printf("RND V%X, 0x%02X", x, nn); break;
        case 0xD: printf("DRW V%X, V%X, %X", x, y, n); break;
        case 0xE:
            if (nn == 0x9E) printf("SKP V%X", x);
            else if (nn == 0xA1) printf("SKNP V%X", x);
            else printf("??? %04X", op);
            break;
        case 0xF:
            switch (nn) {
                case 0x07: printf("LD V%X, DT", x); break;
                case 0x0A: printf("LD V%X, K", x); break;
                case 0x15: printf("LD DT, V%X", x); break;
                case 0x18: printf("LD ST, V%X", x); break;
                case 0x1E: printf("ADD I, V%X", x); break;
                case 0x29: printf("LD F, V%X", x); break;
                case 0x33: printf("LD B, V%X", x); break;
                case 0x55: printf("LD [I], V%X", x); break;
                case 0x65: printf("LD V%X, [I]", x); break;
                default:   printf("??? %04X", op); break;
            }
            break;
        default:  printf("??? %04X", op); break;
    }
}

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    static uint8_t memory[MEMORY_SIZE];

    const char *rom_path = "roms/Pong.ch8";
    FILE *rom = fopen(rom_path, "rb");
    if (rom == NULL) {
        printf("Ошибка: Не удалось открыть файл %s\n", rom_path);
        return 1;
    }

    fseek(rom, 0, SEEK_END);
    long rom_size = ftell(rom);
    rewind(rom);

    if (rom_size > (MEMORY_SIZE - START_ADRESS)) {
        printf("Ошибка: ROM слишком большой для памяти Chip-8!\n");
        fclose(rom);
        return 1;
    }

    fread(&memory[START_ADRESS], sizeof(uint8_t), rom_size, rom);
    fclose(rom);

    printf("Дизассемблер: первые команды из %s\n\n", rom_path);

    long commands = rom_size / 2;
    if (commands > COMMANDS_TO_SHOW) commands = COMMANDS_TO_SHOW;

    for (long i = 0; i < commands; i++) {
        uint16_t address = START_ADRESS + i * 2;
        uint16_t op = (memory[address] << 8) | memory[address + 1];

        printf("0x%03X: %04X   ", address, op);
        print_mnemonic(op);
        printf("\n");
    }

    return 0;
}
