#pragma once
#include "nand_ecc.h"

#define MAX_PAGE_SIZE 8192
#define MAX_SPARE_SIZE 448

/******************Structures******************/
#pragma pack(push,1)
typedef struct
{
	uint8_t		page_ecc[MAX_PAGE_SIZE / 512][3];//todo
	uint32_t	crc32;
	uint8_t		crc32_ecc[3];
	uint8_t		reserved[MAX_SPARE_SIZE - (MAX_PAGE_SIZE / 512 * 3 * sizeof(uint8_t)) - (sizeof(uint32_t)) - (3 * sizeof(uint8_t))];//todo
} CSpare;
#pragma pack(pop)

/*
 * \brief Шапка страницы nand
 */
#pragma pack(push,1)
typedef struct
{
	uint8_t status;
	uint8_t param;
	uint16_t data_shift;		// адрес начала нового пакета в данной странице
} NandPageHeader_t;
#pragma pack(pop)

/*
 * \brief Структура страницы nand
 */
#pragma pack(push,1)
typedef struct
{
	NandPageHeader_t page_header;
	uint8_t data[MAX_PAGE_SIZE - sizeof(NandPageHeader_t)];//todo
	//uint8_t* data;
} PagePack_t;
#pragma pack(pop)

/*
 *\ brief Структура временного буфера записи в NAND
 */
#pragma pack(push,1)
typedef struct
{
	PagePack_t page_buf[2];
	uint16_t nand_buf_idx;
	bool buf_sel;
} NandTmpBuf_t;
#pragma pack(pop)

/**************Functions**************/
bool check_and_correct_page(uint8_t* data, uint32_t page_size, const uint8_t* nand_spare);
bool nand_read_page_corrected(uint8_t *data, uint32_t page_number);
eccdiff_t crc32_check_ecc(uint32_t page_size, CSpare spare);
uint32_t CRC32Calc(uint32_t* data, uint32_t len);