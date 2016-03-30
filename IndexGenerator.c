/*
 *	Name:		IndexGenerator.c
 *	Author:		Vineet Tiwari
 *	Created on:	March 25, 2016
 *	Virsion:	1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF 25
#define APP_NAME "AccountRec"

enum key_name
{
	ACCOUNT_NUM,
	FIRST_NAME,
	LAST_NAME,
	PAY_AMOUNT
};

union index_key
{
	int	account_number;
	char	first_name[BUFF];
	char	last_name[BUFF];
	double	account_balance;
	double	last_pay_amount;
};

struct index_header
{
	char	app_name[BUFF];
	int	key_name;
	int	record_count;
};

struct index_record
{
	union index_key	key;
	long		file_pos;
};

struct account
{
	int	account_number;
	char	first_name[BUFF];
	char	last_name[BUFF];
	double	account_balance;
	double	last_pay_amount;
};

int fileSize(FILE *);

int compare(const void *, const void *);

int main()
{
	FILE *input_ptr = NULL;
	FILE *output_ptr = NULL;

	int record_count = 0;
	size_t bytes_read = 0;
	size_t bytes_written = 0;
	long file_pos = 0;

	enum key_name key_name = LAST_NAME;
	struct index_header index_header;
	memset(&index_header, 0, sizeof(struct index_header));

	struct index_record *index_records = NULL;
	struct account temp_account = {0, "", "", 0.0, 0.0};

	input_ptr = fopen("account.dat", "rb");
	output_ptr = fopen("account.idx", "wb");
	if (!input_ptr || !output_ptr) {
		fprintf(stderr,
			"Error: Failed to open either of the files.\n");
		return EXIT_FAILURE;
	}

	record_count = fileSize(input_ptr) / sizeof(struct account);

	index_records = (struct index_record *)calloc(record_count,
		 sizeof(struct index_record));

	strcpy(index_header.app_name, APP_NAME);
	index_header.key_name = key_name;
	index_header.record_count = record_count;

	rewind(input_ptr);
	record_count = 0;
	file_pos = ftell(input_ptr);

	bytes_read = fread(&temp_account,
		sizeof(struct account), 1, input_ptr);

	while (!feof(input_ptr) && (bytes_read == 1)) {
		strcpy(index_records[record_count].key.last_name,
			temp_account.last_name);
		index_records[record_count].file_pos = file_pos;

		record_count++;
		file_pos = ftell(input_ptr);

		bytes_read = fread(&temp_account,
			sizeof(struct account), 1, input_ptr);
	}

	qsort(index_records, record_count, sizeof(struct index_record), compare);

	bytes_written = fwrite(&index_header,
		sizeof(struct index_header), 1, output_ptr);

	bytes_written = fwrite(index_records,
		sizeof(struct index_record), record_count, output_ptr);

	free(index_records);

	fclose(input_ptr);
	fclose(output_ptr);

	return EXIT_SUCCESS;
}

int fileSize(FILE *file_ptr)
{
	int start = 0;
	int original = 0;
	int end = 0;

	original = ftell(file_ptr);
	rewind(file_ptr);
	start = ftell(file_ptr);
	fseek(file_ptr, 0L, SEEK_END);
	end = ftell(file_ptr);
	fseek(file_ptr, original, SEEK_SET);

	return (end - start);
}

int compare(const void *left_value, const void *right_value)
{
	struct index_record *l_value = (struct index_record *)left_value;
	struct index_record *r_value = (struct index_record *)right_value;

	return strcmp((l_value->key.last_name),
		(r_value->key.last_name));
}
