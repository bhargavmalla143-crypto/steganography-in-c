/*
   File: encode.h
   Description:
   This file contains structure definition
   and function prototypes required for encoding process.
*/

#ifndef ENCODE_H
#define ENCODE_H

#include <stdio.h> // For FILE type
#include "types.h" // User defined data types

/* Structure to store all encoding related information */
typedef struct
{
    /* Source Image Information */
    char *src_image_fname; // Source image file name
    FILE *fptr_src_image;  // Source image file pointer
    uint image_capacity;   // Total image capacity

    /* Secret File Information */
    char *secret_fname;       // Secret file name
    FILE *fptr_secret;        // Secret file pointer
    char extn_secret_file[5]; // Secret file extension
    long size_secret_file;    // Secret file size

    /* Stego Image Information */
    char *stego_image_fname; // Output image file name
    FILE *fptr_stego_image;  // Output image file pointer

} EncodeInfo;

/* Function Prototypes for Encoding */

/* Validate encoding command line arguments */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

/* Perform complete encoding process */
Status do_encoding(EncodeInfo *encInfo);

/* Open source, secret and output files */
Status open_files(EncodeInfo *encInfo);

/* Check image capacity before encoding */
Status check_capacity(EncodeInfo *encInfo);

/* Get BMP image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get secret file size */
uint get_file_size(FILE *fptr);

/* Copy 54-byte BMP header */
Status copy_bmp_header(FILE *src, FILE *dest);

/* Encode magic string */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo);

/* Encode extension size */
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo);

/* Encode extension */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo);

/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);

/* Encode secret file data */
Status encode_secret_file_data(EncodeInfo *encInfo);

/* Encode one character into LSB */
Status encode_byte_to_lsb(char data, char *image_buffer);

/* Encode integer into LSB */
Status encode_size_to_lsb(int size, char *image_buffer);

/* Copy remaining image data */
Status copy_remaining_img_data(FILE *src, FILE *dest);

#endif
