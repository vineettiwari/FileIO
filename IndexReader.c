/*
 *	Name:		IndexReader.c
 *	Author:		Vineet Tiwari
 *	Created on:	March 27, 2016
 *	Virsion:	1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define APP_NAME "AccountRec"
#define BUFF 25

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
	union index_key key;
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

int FileSize(FILE *);

int main()
{
	FILE *data_ptr = NULL;
	FILE *index_ptr = NULL;

	int bytes_read = 0;
	int record_count = 0;

	struct index_header index_header;
	memset(&index_header, 0, sizeof(struct index_header));

	struct index_record index_record;
	memset(&index_record, 0, sizeof(struct index_record));

	struct account account = {0, "", "", 0.0, 0.0};

	data_ptr = fopen("account.dat", "rb");
	index_ptr = fopen("account.idx", "rb");
	if (!data_ptr || !index_ptr) {
		fprintf(stderr, "Error: Failed to open one of the files.");
		return EXIT_FAILURE;
	}

	record_count = FileSize(data_ptr) / sizeof(struct account);

	bytes_read = fread(&index_header,
		sizeof(struct index_header), 1, index_ptr);

	if (strcmp(index_header.app_name, APP_NAME) != 0) {
		printf("ERROR - Data and Index Files "
        		"Created by Different Application Versions\n");
        	fclose(data_ptr);
        	fclose(index_ptr);
        	return 1;
	}

	if (index_header.record_count != record_count) {
		printf("ERROR - Data and Index "
			"Files Have a Record Count Mismatch\n");
		fclose(data_ptr);
		fclose(index_ptr);
		return 1;
	}

	//printf("Data File Read in Indexed Order (Last Name)\n\n");
	//printf("%-20s%-20s%6s\n", "First Name", "Last Name", "Age");
	//printf("----------------------------------------------\n");

	do {
		bytes_read = fread(&index_record,
			sizeof(struct index_record), 1, index_ptr);

		if (fseek(data_ptr,
			index_record.file_pos, SEEK_SET) != 0) {
			printf("Seek Error\n");
			fclose(data_ptr);
			fclose(index_ptr);
			return 1;
		}

		fread(&account, sizeof(struct account), 1, data_ptr);
		printf("%6d %10s %10s %10.2lf %10.2lf\n",
				account.account_number,
				account.first_name,
				account.last_name,
				account.account_balance,
				account.last_pay_amount);
	} while ((!feof(index_ptr)) && (bytes_read == 1));

	fclose(data_ptr);
	fclose(index_ptr);

	return EXIT_SUCCESS;
}

int FileSize(FILE *file_ptr)
{
	int start = 0;
	int original = 0;
	int end = 0;

	original = ftell(file_ptr);

	rewind(file_ptr);
	start = ftell(file_ptr);

	fseek(file_ptr, 0L, SEEK_END);
	end = ftell(file_ptr);

	return (end - start);
}
