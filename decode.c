/*
   File: decode.c
   Description:
   This file contains all decoding functions.
   It extracts hidden data from a stego BMP image using LSB technique.
*/

#include <stdio.h>  // For file handling
#include <string.h> // For string operations
#include "decode.h" // Decode structure and prototypes
#include "common.h" // Contains MAGIC_STRING

/* Validate decode arguments */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if (strstr(argv[2], ".bmp") != NULL) // Check if input file is .bmp
        decInfo->stego_image_fname = argv[2];
    else
        return e_failure;

    return e_success;
}

/* Open stego image file */
Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb"); // Open image

    if (decInfo->fptr_stego_image == NULL)
        return e_failure;

    return e_success;
}

/* Decode magic string */
Status decode_magic_string(DecodeInfo *decInfo)
{
    char buffer[8];
    char decoded[3];

    fseek(decInfo->fptr_stego_image, 54, SEEK_SET); // Skip 54-byte BMP header

    for (int i = 0; i < strlen(MAGIC_STRING); i++)
    {
        fread(buffer, 1, 8, decInfo->fptr_stego_image); // Read 8 bytes
        decode_byte_from_lsb(&decoded[i], buffer);      // Extract character
    }

    decoded[strlen(MAGIC_STRING)] = '\0'; // Add null terminator

    if (strcmp(decoded, MAGIC_STRING) == 0) // Verify magic string
        return e_success;
    else
        return e_failure;
}

/* Decode extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char buffer[32];

    fread(buffer, 1, 32, decInfo->fptr_stego_image);   // Read 32 bytes
    decode_size_from_lsb(&decInfo->extn_size, buffer); // Extract size

    return e_success;
}

/* Decode extension */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char buffer[8];

    for (int i = 0; i < decInfo->extn_size; i++)
    {
        fread(buffer, 1, 8, decInfo->fptr_stego_image);  // Read 8 bytes
        decode_byte_from_lsb(&decInfo->extn[i], buffer); // Extract character
    }

    decInfo->extn[decInfo->extn_size] = '\0'; // Null terminate extension

    strcpy(decInfo->output_fname, "decoded");     // Set output base name
    strcat(decInfo->output_fname, decInfo->extn); // Add extension

    decInfo->fptr_output = fopen(decInfo->output_fname, "w"); // Create output file

    return e_success;
}

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char buffer[32];

    fread(buffer, 1, 32, decInfo->fptr_stego_image);          // Read 32 bytes
    decode_size_from_lsb(&decInfo->secret_file_size, buffer); // Extract size

    return e_success;
}

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char buffer[8];
    char ch;

    for (int i = 0; i < decInfo->secret_file_size; i++) // Loop till file size
    {
        fread(buffer, 1, 8, decInfo->fptr_stego_image); // Read 8 bytes
        decode_byte_from_lsb(&ch, buffer);              // Extract character
        fputc(ch, decInfo->fptr_output);                // Write to output file
    }

    return e_success;
}

/* Decode one character from LSB */
Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    *data = 0;

    for (int i = 0; i < 8; i++)
    {
        *data = (*data << 1) | (image_buffer[i] & 1); // Extract LSB bit
    }

    return e_success;
}

/* Decode integer from LSB */
Status decode_size_from_lsb(int *size, char *image_buffer)
{
    *size = 0;

    for (int i = 0; i < 32; i++)
    {
        *size = (*size << 1) | (image_buffer[i] & 1); // Extract 32 bits
    }

    return e_success;
}

/* Main decoding function */
Status do_decoding(DecodeInfo *decInfo)
{
    if (open_decode_files(decInfo) == e_failure)
        return e_failure;

    if (decode_magic_string(decInfo) == e_failure)
        return e_failure;

    decode_secret_file_extn_size(decInfo); // Get extension size
    decode_secret_file_extn(decInfo);      // Get extension
    decode_secret_file_size(decInfo);      // Get secret file size
    decode_secret_file_data(decInfo);      // Extract secret data

    return e_success;
}
