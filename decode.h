/*
   File: decode.h
   Description:
   This file contains structure definition
   and function prototypes required for decoding process.
*/

#ifndef DECODE_H
#define DECODE_H

#include <stdio.h> // For FILE type
#include "types.h" // User defined data types

/* Structure to store all decoding related information */
typedef struct
{
    char *stego_image_fname; // Stego image file name
    FILE *fptr_stego_image;  // Stego image file pointer

    char output_fname[50]; // Output file name (decoded file)
    FILE *fptr_output;     // Output file pointer

    int extn_size; // Size of file extension
    char extn[10]; // File extension

    int secret_file_size; // Secret file size

} DecodeInfo;

/* Function Prototypes for Decoding */

/* Validate decode command line arguments */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform complete decoding process */
Status do_decoding(DecodeInfo *decInfo);

/* Open stego image file */
Status open_decode_files(DecodeInfo *decInfo);

/* Decode magic string */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Decode extension */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode one character from LSB */
Status decode_byte_from_lsb(char *data, char *image_buffer);

/* Decode integer from LSB */
Status decode_size_from_lsb(int *size, char *image_buffer);

#endif
