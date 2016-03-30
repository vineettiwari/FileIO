/*
 *	Name:		BinaryGenerator.c
 *	Author:		Vineet Tiwari
 *	Created on:	March 05, 2016
 *	Virsion:	1.2
 */

/*	include the required C libraries
 *	for the program to work
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*	macros
 */
#define BUFF_SIZE 20
#define LINE_SIZE 256

/*	structure for storing the account data
 */
struct account {
	int	account_number;
	char	first_name[BUFF_SIZE], last_name[BUFF_SIZE];
	double	account_balance, last_payment_amount;
};

/*	function declarations
 */
bool check_file(FILE *file_ptr, char *file_name);

char *trim_comma(char *str_value);

bool parse_line(char *line, struct account *customer_account);

void write_file(FILE *ftext_ptr, FILE *fbinary_ptr, char *input_file,
	char *output_file, struct account *customer_account);

void read_file(FILE *fbinary_ptr, char *output_file,
	struct account *customer_account);

/*	singuler point of entry to the program
 */
int main(int argc, char *argv[])
{
	FILE		*ftext_ptr = NULL;
	FILE		*fbinary_ptr = NULL;
	struct account	customer_account;

	/*	check for the right number of command line arguments
	 */
	if (argc != 3) {
		fprintf(stderr,
			"\nError: Insufficient number of argumnts\n");
		exit(EXIT_FAILURE);
	}

	/*	check if the binary file already exists
	 *	if so, get user's permition to overwrite the
	 *	file or exit the program
	 */
	if (check_file(fbinary_ptr, argv[2])) {
		printf("\nFile - %s was not overwritten\n\n", argv[2]);
		exit(3);
	}

	/*	write the data from the text file into the binary file
	 */
	write_file(ftext_ptr, fbinary_ptr, argv[1],
		argv[2], &customer_account);

	/*	read the data from the binary file and display it to 
	 *	the standard output.
	 */
	read_file(fbinary_ptr, argv[2], &customer_account);

	printf("\nFile copied successfully!\n\n");
	return EXIT_SUCCESS;
}

/*	function definitions
 */
bool check_file(FILE *file_ptr, char *file_name)
{
	char choice;
	bool should_exit = false;

	/*	read file in binary mode
	 */
	file_ptr = fopen(file_name, "rb");
	if (file_ptr) {
		fprintf(stdout, "\nWarning: %s already exixts.\n"
			"Do you wish to overwrite it? (Y/N) ", file_name);

		choice = getchar();

		/*	set the bool value to 'true'
		 *	if user does not wish to overwrite
		 */
		if((choice == 'N') || (choice == 'n'))
			should_exit = true;

		fclose(file_ptr);
	}

	return should_exit;
}

char *trim_comma(char *str_value)
{
	int length = strlen(str_value);

	/*	check if the last character in the array is a comma
	 *	if so, replace it with a null terminator
	 */
	if (str_value[length - 1] == ',')
		str_value[length - 1] = '\0';

	return str_value;
}

bool parse_line(char *line, struct account *customer_account)
{
	/*	store the number of successful scans
	 */
	size_t success = 0;

	char 	temp_acnum[BUFF_SIZE], temp_fname[BUFF_SIZE],
		temp_lname[BUFF_SIZE], temp_acbal[BUFF_SIZE],
		temp_lpay[BUFF_SIZE];

	/*	scan and store the contents of the line
	 *	into apporprite variables
	 */
	success = sscanf(line, "%s%s%s%s%s", temp_acnum,
		temp_fname, temp_lname, temp_acbal, temp_lpay);

	/*	remove the comma from these char arrays
	 *	ex: "John," to "John"
	 */
	trim_comma(temp_acnum);
	trim_comma(temp_fname);
	trim_comma(temp_lname);
	trim_comma(temp_acbal);
	trim_comma(temp_lpay);

	/*	if needed, convert the variables from char array(string)
	 *	type to the apporprite data types and
	 *	store them into the account structure
	 */
	success += sscanf(temp_fname, "%s", customer_account->first_name);
	success += sscanf(temp_lname, "%s", customer_account->last_name);
	success += sscanf(temp_acnum, "%d",
		&(customer_account->account_number));
	success += sscanf(temp_acbal, "%lf",
		&(customer_account->account_balance));
	success += sscanf(temp_lpay, "%lf",
		&(customer_account->last_payment_amount));

	return (success == 10);
}

void write_file(FILE *ftext_ptr, FILE *fbinary_ptr,
	char *input_file, char *output_file, struct account *customer_account)
{
	char line[LINE_SIZE];

	/*	open in reading-text mode 
	 */
	ftext_ptr = fopen(input_file, "r");

	/*	open in writing-binary mode
	 */
	fbinary_ptr = fopen(output_file, "wb");

	if (!ftext_ptr || !fbinary_ptr) {
		fprintf(stderr,
			"\nError: Failed to open one of the files.\n");
		return;
	}

	putchar('\n');
	while (fgets(line, sizeof(line), ftext_ptr)) {
		if (parse_line(line, customer_account)) {
			if (fwrite(customer_account,
				sizeof(struct account), 1, fbinary_ptr) != 1) {
				fprintf(stderr,
					"\nError: Unable to save to disk\n");
				return;
			}
		} else fprintf(stderr, "\nWarning: Misformed line\n");
	}

	if ((fclose(ftext_ptr) != 0) || (fclose(fbinary_ptr) != 0))
		fprintf(stderr, "\nError: Failed to "
			"close one of the files.\n");
}

void read_file(FILE *fbinary_ptr, char *output_file,
		struct account *customer_account)
{
	/*	open in reading-binary mode
	 */
	fbinary_ptr = fopen(output_file, "rb");
	if (!fbinary_ptr) {
		fprintf(stderr, "\nError: Unable to "
			"open file - account.dat\n");
		return;
	}

	while (fread(customer_account,
			sizeof(struct account), 1, fbinary_ptr) == 1) {
		printf("%6d %10s %10s %10.2lf %10.2lf\n",
				customer_account->account_number,
				customer_account->first_name,
				customer_account->last_name,
				customer_account->account_balance,
				customer_account->last_payment_amount);
	}

	if (fclose(fbinary_ptr) != 0)
		fprintf(stderr, "\nError: Unable to "
			"close file - account.dat\n");
}




