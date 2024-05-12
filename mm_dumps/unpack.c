#include <stdio.h>

static unsigned char num_blocks; 
static unsigned char control_byte, backup_control_byte;
static unsigned char buff[8];

void write_character(unsigned char c, FILE *fp) 
{
        fputc(c, fp);
        // printf("%02x ", c);
}

int main(void) {
        FILE *input_file = fopen("./packed.dmp", "rb");
        FILE *output_file = fopen("./output.hex", "wb");
        unsigned char c, x, carry;

        num_blocks = fgetc(input_file);

        do {
                // printf("\n");
                control_byte = fgetc(input_file);
                x = 8;

                if (~control_byte & 0x80)
                {
                        // read_sequenced
                        
                        do {
                                c = fgetc(input_file);
                                write_character(c, output_file);
                                
                                --x;
                                if (x) {
                                        do {
                                                control_byte <<= 1;
                                                if (~control_byte & 0x80) {
                                                        break;
                                                }
                                                write_character(c, output_file);
                                                --x;
                                        } while (x);
                                }
                        } while (x);

                        x = 8;
                        control_byte = fgetc(input_file);
                        do {
                                if (control_byte & 0x80)
                                {
                                        c = 0;
                                } else {
                                        c = fgetc(input_file);
                                        
                                }

                                write_character(c, output_file);
                                // c = fgetc(input_file);
                                
                                --x;
                                if (x) {
                                        do {
                                                control_byte <<= 1;
                                                if (~control_byte & 0x80) {
                                                        break;
                                                }
                                                write_character(c, output_file);
                                                --x;
                                        } while (x);
                                }
                        } while (x);
                } else {
                        do {
                                // printf("%02x ", control_byte);
                                // 0b09
                                c = fgetc(input_file);
                                write_character(c, output_file);
                                // printf("%02x ", c);
                                
                                buff[x] = (unsigned char) c;
                                --x;
                                if (x) {
                                        do {
                                                control_byte <<= 1;

                                                if (~control_byte & 0x80) {
                                                        c = fgetc(input_file); // break; 
                                                        // тут мне надо выйти на уровень вверх на получение нового
                                                }

                                                write_character(c, output_file);
                                                // printf("%02x ", c);
                                                buff[x] = (unsigned char) c;
                                                --x;
                                        } while (x);

                                       

                                        x = 8;
                                        control_byte = fgetc(input_file);
                                        // printf("|%02x| ", control_byte);

                                        do {
                                                carry = control_byte & 0x80;
                                                control_byte <<= 1;
                                                
                                                if (carry) {
                                                        c = buff[x];
                                                } else {
                                                        c = fgetc(input_file);
                                                }
                                                write_character(c, output_file);
                                                // printf("%02x ", c);
                                                
                                                --x;
                                        } while (x);
                                }
                        } while (x);
                        // printf("\n");
                        // read_buffered
                }

                // printf("%02x\n", num_blocks);
                --num_blocks;
        } while (num_blocks);
        

        fclose(input_file);

        return 0;
}


/**

Проблема
FF FF FF FF FF FF FF FF FC FF FF FF FF FF FF 56

FF FF FF FF FF FF FF BF FF FF FF FF FF FF BE 56

блочная проблема 


00 30 38 78 78 70 C0 00 FF CF C7 87 87 8F 3F FF
FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF
FF FF FF FF FF FF FF BF FF FF FF FF FF FF BE 56


00 30 38 78 78 70 C0 00 FF CF C7 87 87 8F 3F 
[FF] FF FF FF FF FF FF FF [FF] FF FF FF FF FF FF FF 
[FF] FF FF FF FF FF FF FF [BF] FF FF FF FF FF FF BE 56


нужно начать с af 


86 -> 85
*/