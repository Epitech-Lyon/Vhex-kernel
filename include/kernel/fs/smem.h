#ifndef __CASIO_SMEM_H__
# define __CASIO_SMEM_H__

#include <stdint.h>
#include <stddef.h>
#include <kernel/types.h>
#include <kernel/fs/file.h>


#define CASIO_SMEM_NAME_LENGHT			12
#define CASIO_SMEM_ROOT_ID			0xffff
#define CASIO_SMEM_BLOCK_ENTRY_MAGIC		0x4200
#define CASIO_SMEM_HEADER_INFO_EXIST		0x51
#define CASIO_SMEM_HEADER_INFO_DELETE		0x01
#define CASIO_SMEM_HEADER_TYPE_DIRECTORY	0x10
#define CASIO_SMEM_HEADER_TYPE_FILE		0x20
#define CASIO_SMEM_FRAGMENT_INFO_EXIST		CASIO_SMEM_HEADER_INFO_EXIST
#define CASIO_SMEM_FRAGMENT_INFO_DELETE		CASIO_SMEM_HEADER_INFO_DELETE
#define CASIO_SMEM_FRAGMENT_MAGIC		0x0120

#define CASIO_SMEM_ROOT_ID	0xffff

#define O_RDONLY	0x00
#define O_WRONLY	0x01
#define O_RDWR		0x02

#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2

// 
typedef struct casio_smem_block_s
{
	uint16_t magic_start;		// always 0x4200
	uint16_t entry_number;		// indicate the block number
	uint32_t offset;		// offset in the memory (?)

	struct {
		uint8_t used;		// Indicate if the block is used (0xff yes, 0x00 no)
		uint8_t fileTable;	// Indicate if the block is used by File Header Table.
		uint16_t id;		// Indicate the block ID in the File Header Table.
	} info;

	uint8_t magic_end[20];		// Content always 0xff 0xff 0xff 0xff 0x00 0x00 0x00 0x01 0xff 0xff...
} casio_smem_block_t;

// Define file header.
typedef struct casio_smem_header_s
{
	uint8_t info;			// 0x51 if entry exist, 0x01 if entry refer to delete file. (?)
	uint8_t type;			// 0x10 for a directory, 0x20 for preheader.
	uint16_t id;			// Indicate the block entry ID (directory)

	struct {
		uint16_t type;		// 0xffff for root, 0x0110 for directory
		uint16_t id;		// directory ID, 0xffff for root
	} parent;

	uint16_t name[12];		// File name
} casio_smem_header_t;

// Define frament data.
typedef struct casio_smem_fragment_s
{
	uint8_t info;			// 0x51 if entry exit, 0x01 if entry refer to delete file.
	uint8_t unknown0;		// always 0x30
	uint16_t id;			// Fragment ID.
	uint16_t magic;			// always 0x0120
	uint16_t file_id;		// File ID.
	uint16_t unknown1;		// 0x0002 (?)
	uint16_t frag_total;		// total number of fragments for this file.
	uint16_t frag_current_number;	// Current fragment number.
	uint16_t data_block_id;		// ID of blocks containing data for this fragment
	uint16_t data_offset;		// offset of fragment data in the given block

	// WARNING : the size is (real_size-1), so *do not forget* to add 1 to
	// have the real size of data!
	uint16_t data_size;		// size of this fragment data

	uint8_t fill[12];		// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, ...
} casio_smem_fragment_t;

// Primitives
extern int casio_smem_mount(void);
extern int casio_smem_open(FILE *file, char const *name, int mode);
extern ssize_t casio_smem_read(FILE *file, void *buf, size_t count);
extern off_t casio_smem_lseek(FILE *file, off_t offset, int whence);

#endif /*_CASIO_SMEM_H__*/
