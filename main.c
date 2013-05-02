#define _XOPEN_SOURCE


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void usage_print(char* message);


void arguments_parse(int argc, char* argv[], char** key, char** salt,
		    int* count) {
	// Validate argument count.
	if (argc < 4) {
		usage_print("Too few arguments");
	}

	// Parse the encryption key.
	(*key) = argv[1];

	// Parse the encryption salt.
	(*salt) = argv[2];

	// Parse the iteration count.
	(*count) = atoi(argv[3]);
	if ((*count) < 1) {
		usage_print("Iteration count must be > 0");
	}
}


/**
 * Print the usage message and exit the program.
 */
void usage_print(char* message) {
	// Print the specified message.
	if (message) {
		fprintf(stderr, "ERROR: %s.\n\n", message);
	}

	// Print the usage message.
	fprintf(stderr, "USAGE: crappycrypt <key> <salt> <iterations>\n");

	// Exit the program.
	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
	char* key; // = "blah";
	char* salt_base; // = "$6$saltsaltsaltsalt$y";
	char salt_full[128];
	int count;
	int offset;
	int i;

	// Parse the arguments.
	arguments_parse(argc, argv, &key, &salt_base, &count);

	// Create the salt.
	memset(salt_full, 0, sizeof(salt_full));
	strcpy(salt_full, "$6$");
	strcat(salt_full, salt_base);
	strcat(salt_full, "$");

	// Calculate the offset of the encrypted key.
	for (i = offset = 0; i < 3; offset++) {
		i += (salt_full[offset] == '$');
	}

	// Hash the key.
	for (i = 0; i < count; i++) {
		key = crypt(key, salt_full);
		key += offset;
	}
	printf("%s\n", key);
}
