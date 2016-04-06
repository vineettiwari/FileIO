/*
 *	Name:		IndexFileRead.c
 *	Author:		Vineet Tiwari
 *	Created on:	March 27, 2016
 *	Virsion:	1.0
 */
 
#include <stdio.h>
#include <stdlib.h>

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

int main()
{
	int readCount = 0;
	struct index index;
	struct account account = {0, "", "", 0.0, 0.0};
	FILE *dataPtr;
	FILE *indexPtr;

	int recordCount = 0;

	dataPtr = fopen("account.dat", "rb");
	indexPtr = fopen("account.idx", "rb");

	recordCount = fileSize(dataPtr) / ACC_SIZE;

	do {
		readCount = fread(&index, IDX_SIZE, 1, indexPtr);
		
		fseek(dataPtr, index.filePos, SEEK_SET);
		
		fread(&account, ACC_SIZE, 1, dataPtr);
		
		printf("%10s %10s\n",
			account.firstName, account.lastName);
	} while (readCount == 1);
	
	fclose(dataPtr);
	fclose(indexPtr);

	return EXIT_SUCCESS;
}
