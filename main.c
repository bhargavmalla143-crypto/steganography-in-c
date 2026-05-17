/*
   File: main.c
   Description:
   This is the main driver file of the steganography project.
   It checks whether user wants encoding or decoding
   and calls the respective functions.
*/

#include <stdio.h>  // For printf
#include <string.h> // For strcmp
#include "encode.h" // Encoding functions
#include "decode.h" // Decoding functions
#include "types.h"  // User defined data types

/* Function prototype */
OperationType check_operation_type(char *symbol); // To check -e or -d

int main(int argc, char *argv[])
{
    if (argc < 2) // Check if sufficient arguments are provided
    {
        printf("ERROR: Insufficient arguments\n");
        return 1;
    }

    /* Identify operation type */
    OperationType op_type = check_operation_type(argv[1]); // Get operation type

    /* ================= ENCODING PART ================= */
    if (op_type == e_encode)
    {
        EncodeInfo encInfo; // Structure to store encode details

        if (read_and_validate_encode_args(argv, &encInfo) == e_failure) // Validate arguments
        {
            printf("ERROR: Invalid arguments for encoding\n");
            return 1;
        }

        if (do_encoding(&encInfo) == e_failure) // Perform encoding
        {
            printf("ERROR: Encoding failed\n");
            return 1;
        }

        printf("Encoding completed successfully\n"); // Success message
    }

    /* ================= DECODING PART ================= */
    else if (op_type == e_decode)
    {
        DecodeInfo decInfo; // Structure to store decode details

        if (read_and_validate_decode_args(argv, &decInfo) == e_failure) // Validate arguments
        {
            printf("ERROR: Invalid arguments for decoding\n");
            return 1;
        }

        if (do_decoding(&decInfo) == e_failure) // Perform decoding
        {
            printf("ERROR: Decoding failed\n");
            return 1;
        }

        printf("Decoding completed successfully\n"); // Success message
    }

    /* ================= INVALID OPTION ================= */
    else
    {
        printf("Invalid option\n"); // If wrong option is given
        printf("Usage:\n");
        printf("./stego -e <source.bmp> <secret.txt> <output.bmp>\n"); // Encode format
        printf("./stego -d <stego.bmp>\n");                            // Decode format
    }

    return 0; // End of program
}

/* Function to identify encode or decode */
OperationType check_operation_type(char *symbol)
{
    if (strcmp(symbol, "-e") == 0)
        return e_encode; // Return encode if -e
    else if (strcmp(symbol, "-d") == 0)
        return e_decode; // Return decode if -d
    else
        return e_unsupported; // Return unsupported otherwise
}
