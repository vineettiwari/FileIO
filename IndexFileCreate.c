/*
 *	Name:		IndexFileCreate.c
 *	Author:		Vineet Tiwari
 *	Created on:	March 25, 2016
 *	Virsion:	1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF 64
#define ACC_SIZE sizeof(struct account)
#define IDX_SIZE sizeof(struct index)

union indexKey
{
	char lastName[BUFF];
	double accountBalance;
};

struct index
{
	union indexKey key;
	long filePos;
};

struct account
{
	int accountNumber;
	char firstName[BUFF];
	char lastName[BUFF];
	double accountBalance;
	double lastPaymentAmount;
};

long fileSize(FILE *filePtr)
{
	long current = ftell(filePtr);

	rewind(filePtr);
	long start = ftell(filePtr);

	fseek(filePtr, 0L, SEEK_END);
	long end = ftell(filePtr);

	fseek(filePtr, current, SEEK_SET);

	return (end - start);
}

int compareLastName(const void *left, const void *right)
{
	struct index *index1 = (struct index *)left;
	struct index *index2 = (struct index *)right;

	return strcmp(index1->key.lastName, index2->key.lastName);
}

int main()
{
	struct index *indices = NULL;
	struct account account = {0, "", "", 0.0, 0.0};
	int recordCount = 0;
	long filePos = 0;

	FILE *dataPtr = fopen("account.dat", "rb");
	if (!dataPtr) {
		printf("ERROR: Failed to account.dat");
		return EXIT_FAILURE;
	}

	FILE *indexPtr = fopen("account.idx", "wb");
	if (!indexPtr) {
		printf("ERROR: Failed to account.idx");
		return EXIT_FAILURE;
	}

	recordCount = fileSize(dataPtr) / ACC_SIZE;

	indices = (struct index *)calloc(recordCount, IDX_SIZE);
	if (!indices) {
		printf("ERROR: Failed to allocate memory for indices");
		return EXIT_FAILURE;
	}

	rewind(dataPtr);
	for (int i = 0; i < recordCount; i++) {
		filePos = ftell(dataPtr);

		fread(&account, ACC_SIZE, 1, dataPtr);

		strcpy(indices[i].key.lastName, account.lastName);
		indices[i].filePos = filePos;
	}

	qsort(indices, recordCount, IDX_SIZE, compareLastName);

	for (int i = 0; i < recordCount; i++) {
		printf("%10s %10lu\n",
			indices[i].key.lastName, indices[i].filePos);
	}

	fwrite(&indices, IDX_SIZE, recordCount, indexPtr);

	printf("\nFile \"account.idx\" created successfully.\n");

	fclose(dataPtr);
	fclose(indexPtr);
	free(indices);

	return EXIT_SUCCESS;
}
