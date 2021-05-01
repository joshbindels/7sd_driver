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
//#define GPLEV0 			(ARM_GPIO_BASE + 0x34)
//#define GPFSEL1			(ARM_GPIO_BASE + 0x04)
#define GPLEV0				13
#define GPLEV1				14
#define GPSET0				7
#define GPSET1				8
#define GPCLR0				10
#define GPCLR1				11

struct gpio_pin_info {
	int line;
	int pin;
};

struct gpio_pin_info GPIO_0 = {0, 6};
struct gpio_pin_info GPIO_1 = {1, 6};
struct gpio_pin_info GPIO_2 = {0, 7};
struct gpio_pin_info GPIO_3 = {0, 8};
struct gpio_pin_info GPIO_4 = {1, 8};
struct gpio_pin_info GPIO_5 = {1, 9};
struct gpio_pin_info GPIO_6 = {1, 11};
struct gpio_pin_info GPIO_7 = {0, 4};
/*
 * struct gpio_pin_info GPIO_21 = {0, 15};
 * struct gpio_pin_info GPIO_22 = {0, 16};
 * struct gpio_pin_info GPIO_23 = {0, 17};
 * struct gpio_pin_info GPIO_24 = {0, 18};
 * struct gpio_pin_info GPIO_25 = {0, 19};
 * struct gpio_pin_info GPIO_26 = {1, 16};
 * struct gpio_pin_info GPIO_27 = {1, 18};
 * struct gpio_pin_info GPIO_28 = {1, 19};
 * struct gpio_pin_info GPIO_29 = {1, 20};
*/

struct sseg_num_conf {
	int num;
	struct gpio_pin_info* pins[];
};

struct sseg_num_conf sseg_zero = {0, {}};
struct sseg_num_conf sseg_one = {0, {&GPIO_7, &GPIO_2}};

void gpio_write(volatile uint32_t* map, int offset, int n) {
	*(map+offset) |= (1<<n);
}

#define GBS(n) (1<<n)

void gpio_clear(volatile uint32_t* map) {
	*(map+GPCLR0) |= ~(GBS(4)|GBS(6)|GBS(7)|GBS(8));
	*(map+GPCLR1) |= ~(GBS(6)|GBS(8)|GBS(9)|GBS(11));
}

uint32_t gpio_read(volatile uint32_t* map, int offset, int n) {
	return  *(map+offset) & (1<<n);
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
		printf("MAP_FAILED\n");
		perror("mmap");
		return -1;
	}

	printf("Showing: %d\n", sseg_one.num);
	gpio_clear(map);
	return 0;

	uint32_t v = gpio_read(map, GPLEV0, 4);
	if(v > 0) {
		printf("High\n");
		gpio_write(map, GPCLR0, 4);
	}
	else {
		printf("Low\n");
		gpio_write(map, GPSET0, 4);
	}

	printf("v: %u\n", v);
	printf("v: %x\n", *(map+13));
	return 0;
}
