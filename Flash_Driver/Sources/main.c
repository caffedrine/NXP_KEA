#include <stdio.h>

#include "derivative.h"
#include "CLK/CLK.h"
#include "DbgConsole/DbgConsole.h"
#include "FLASH/FLASH.h"

static int i = 0;

int main(void)
{
	/* Init clock for serial */
	Clk_Init();

	/* Init debgging console */
	DbgConsole_Init( UART2, 16000000, 9600 );

	/* Init flash */
	printf( "\nInitializaing flash memory..." );
	uint16_t flash_init_result = FLASH_Init( 16000000 );
	if ( flash_init_result != FLASH_ERR_SUCCESS )
	{
		printf( "failed\n" );
		printf( "Fail code: %.2X\n", flash_init_result );
		fflush( stdout );
		while ( 1 )
			;
	}
	printf( "done\n" );
	printf( "Flash sector size\t: %d\n", FLASH_SECTOR_SIZE );
	printf( "Flash start address\t: 0x%.8X\n", FLASH_START_ADDRESS );
	printf( "EEPROM start address\t: 0x%.8X\n", EEPROM_START_ADDRESS );

	/* Begin test */
	uint16_t start_write_address = 0x3000; /* Must be 4 bytes aligned (must be divided by 4) */
	uint8_t *payload = "Hello world!";
	uint16_t size = 11;

	/* Sector must be cleared before write */
	printf("Erasing sector statting at 0x%.8X before write...", start_write_address);
	uint16_t flash_erase_sector_result = FLASH_EraseSector(start_write_address);
	if (flash_erase_sector_result != FLASH_ERR_SUCCESS)
	{
		printf("failed\n");
		printf("Fail code: %.2X\n", flash_erase_sector_result);
		fflush(stdout);
		while (1)
			;
	}
	printf("done\n");
	printf( "Sector 0x%.4X - 0x%.4X was erased\n", start_write_address,  start_write_address + FLASH_SECTOR_SIZE);

	/* Write data to flash */
	printf("Writing '%s' (%d bytes) at address 0x%.8X...", payload, size, start_write_address);
	uint16_t flash_write_result = FLASH_Program(start_write_address, payload, size);

	if (flash_write_result != FLASH_ERR_SUCCESS)
	{
		printf("failed\n");
		printf("Fail code: %.2X\n", flash_write_result);
		fflush(stdout);

		while (1)
			;
	}
	printf("done\n");

	fflush( stdout );

	for ( ;; )
	{
		i++;
	}
	/* Never leave main */
	return 0;
}

