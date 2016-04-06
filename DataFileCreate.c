/*
 *	DataFileCreate.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DELIMITER ", "
#define BUFF 64

struct account
{
	int accountNumber;
	char firstName[BUFF];
	char lastName[BUFF];
	double accountBalance;
	double lastPaymentAmount;
};

void parseLine(struct account *account, char *line)
{
  	char *ch[10];
	int i = 0;

	ch[i] = strtok(line, DELIMITER);

	while(ch[i] != NULL) {
    		ch[++i] = strtok(NULL, DELIMITER);
    	}

    	sscanf(ch[0], "%d", &(account->accountNumber));
    	sscanf(ch[1], "%s", account->firstName);
	sscanf(ch[2], "%s", account->lastName);
	sscanf(ch[3], "%lf", &(account->accountBalance));
	sscanf(ch[4], "%lf", &(account->lastPaymentAmount));
}

void readFile(FILE *dataPtr, struct account *account)
{
	int readCount = 0;

	printf("\n");
	do {
		readCount = fread(account,
			sizeof(struct account), 1, dataPtr);

		printf("%6d %10s %10s %10.2lf %10.2lf\n", account->accountNumber,
    			account->firstName, account->lastName,
    			account->accountBalance, account->lastPaymentAmount);
	} while (readCount == 1);
	printf("\n");
}

int main()
{
	struct account account = {0, "", "", 0.0, 0.0};
	char line[BUFF];

	FILE *textPtr = fopen("account.csv", "r");
	FILE *dataPtr = fopen("account.dat", "wb");

	if (!textPtr || !dataPtr) {
		fprintf(stderr, "\nERROR: Failed to open one of the files.\n");
	};

	while (fgets(line, sizeof(line), textPtr)) {
		parseLine(&account, line);
    		fwrite(&account, sizeof(struct account), 1, dataPtr);	
	};

	fclose(textPtr);
	fclose(dataPtr);

	dataPtr = fopen("account.dat", "rb");
	if (!dataPtr) {
		fprintf(stderr, "\nERROR: Failed to open \"account.dat\".\n");
	};

	readFile(dataPtr, &account);

	fclose(dataPtr);

	return EXIT_SUCCESS;
}
