/*
 * Wiring:
 *
 *
 *      -G-
 *  E |     | F
 *      -D-
 *  A |     | C
 *  	-B-  .DP
 *
 *  A - 0   E  - 5
 *  B - 1   F  - 7
 *  C - 2	G  - 6
 *  D - 4   DP - 3
 */
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>


#define ARM_IO_BASE 		0x20000000
#define ARM_GPIO_BASE 		(ARM_IO_BASE + 0x200000)
//#define OFFSET_GPLEV0 	(ARM_GPIO_BASE + 0x34)
//#define OFFSET_GPFSEL1	(ARM_GPIO_BASE + 0x04)
#define OFFSET_GPLEV0		13
#define OFFSET_GPSET0		7
#define OFFSET_GPCLR0		10
#define OFFSET_GPFSEL0      0
#define OFFSET_GPFSEL1      1
#define OFFSET_GPFSEL2      2

#define OFFSET_PIN_A        17
#define OFFSET_PIN_B        18
#define OFFSET_PIN_C        27
#define OFFSET_PIN_D        23
#define OFFSET_PIN_E        24
#define OFFSET_PIN_F        4
#define OFFSET_PIN_G        25
#define OFFSET_PIN_DP       22

#define BIT_A               (1<<OFFSET_PIN_A)
#define BIT_B               (1<<OFFSET_PIN_B)
#define BIT_C               (1<<OFFSET_PIN_C)
#define BIT_D               (1<<OFFSET_PIN_D)
#define BIT_E               (1<<OFFSET_PIN_E)
#define BIT_F               (1<<OFFSET_PIN_F)
#define BIT_G               (1<<OFFSET_PIN_G)
#define BIT_DP              (1<<OFFSET_PIN_DP)

#define BITS_ZERO           (BIT_A|BIT_B|BIT_C|BIT_F|BIT_G|BIT_E)
#define BITS_ONE            (BIT_C|BIT_F)
#define BITS_TWO            (BIT_A|BIT_B|BIT_D|BIT_F|BIT_G)
#define BITS_THREE          (BITS_ONE|BIT_B|BIT_D|BIT_G)
#define BITS_FOUR           (BITS_ONE|BIT_D|BIT_E)
#define BITS_FIVE           (BIT_B|BIT_C|BIT_D|BIT_E|BIT_G)
#define BITS_SIX            (BIT_A|BIT_B|BIT_C|BIT_D|BIT_E)
#define BITS_SEVEN          (BITS_ONE|BIT_G)
#define BITS_EIGHT          (BITS_ZERO|BIT_D)
#define BITS_NINE           (BITS_SEVEN|BIT_D|BIT_E)

void gpio_write(volatile uint32_t* map, int offset, int n) {
	*(map+offset) = n;
}

void gpio_clear(volatile uint32_t* map) {
    *(map+OFFSET_GPCLR0) |= (BIT_A|BIT_B|BIT_C|BIT_D|BIT_E|BIT_F|BIT_G|BIT_DP);
}

uint32_t gpio_read(volatile uint32_t* map, int offset, int pin) {
	return  *(map+offset) & (1<<pin);
}

void gpio_set_output_mode(volatile uint32_t* map) {
// Get the relevant bit for the function select register
// based on pin BCM number
#define SBIT(n) (1<<n*3)
    *(map+OFFSET_GPFSEL0) |= SBIT(4);
    *(map+OFFSET_GPFSEL1) |= (SBIT(7)|SBIT(8));
    *(map+OFFSET_GPFSEL2) |= (SBIT(2)|SBIT(3)|SBIT(4)|SBIT(5)|SBIT(7));
#undef SBIT
}

int main() {
	volatile uint32_t* map = MAP_FAILED;
	int fd;
	uint32_t addr_p =  ARM_GPIO_BASE;

	fd = open("/dev/gpiomem", O_RDWR|O_SYNC);
	if(fd < 0) {
		printf("Failed to open /dev/mem\n");
		return -1;
	}

	map = mmap(
		0,
		0xB4,
		PROT_READ|PROT_WRITE,
		MAP_SHARED,
		fd,
		addr_p
	);

	close(fd);

	if(map == MAP_FAILED) {
		perror("mmap");
		return -1;
	}

    gpio_set_output_mode(map);

    for(int i=0; i<10; i++) {
        gpio_clear(map);
        switch(i) {
            case 0:
                gpio_write(map, OFFSET_GPSET0, BITS_ZERO);
                break;
            case 1:
                gpio_write(map, OFFSET_GPSET0, BITS_ONE);
                break;
            case 2:
                gpio_write(map, OFFSET_GPSET0, BITS_TWO);
                break;
            case 3:
                gpio_write(map, OFFSET_GPSET0, BITS_THREE);
                break;
            case 4:
                gpio_write(map, OFFSET_GPSET0, BITS_FOUR);
                break;
            case 5:
                gpio_write(map, OFFSET_GPSET0, BITS_FIVE);
                break;
            case 6:
                gpio_write(map, OFFSET_GPSET0, BITS_SIX);
                break;
            case 7:
                gpio_write(map, OFFSET_GPSET0, BITS_SEVEN);
                break;
            case 8:
                gpio_write(map, OFFSET_GPSET0, BITS_EIGHT);
                break;
            case 9:
                gpio_write(map, OFFSET_GPSET0, BITS_NINE|BIT_DP);
                break;
        }
        sleep(1);
    }
	return 0;
}
