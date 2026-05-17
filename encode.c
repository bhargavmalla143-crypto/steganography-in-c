/*
   File: encode.c
   Description:
   This file performs encoding of secret data into a BMP image.
   It hides the secret file inside the image using LSB technique.
*/

#include <stdio.h>  // For file handling
#include <string.h> // For string functions
#include "encode.h" // Encoding structure and prototypes
#include "common.h" // Contains MAGIC_STRING

/* Get BMP image size */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;

    fseek(fptr_image, 18, SEEK_SET);            // Move to width location
    fread(&width, sizeof(int), 1, fptr_image);  // Read width
    fread(&height, sizeof(int), 1, fptr_image); // Read height

    return width * height * 3; // Return total image capacity
}

/* Get secret file size */
uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END); // Move to end of file
    uint size = ftell(fptr);  // Get file size
    rewind(fptr);             // Reset file pointer
    return size;
}

/* Validate encode arguments */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if (strstr(argv[2], ".bmp") != NULL) // Check source image is .bmp
        encInfo->src_image_fname = argv[2];
    else
        return e_failure;

    if (strchr(argv[3], '.') != NULL) // Check secret file has extension
        encInfo->secret_fname = argv[3];
    else
        return e_failure;

    strcpy(encInfo->extn_secret_file, strchr(argv[3], '.')); // Store extension

    if (argv[4] != NULL) // Check output image name
        encInfo->stego_image_fname = argv[4];
    else
        encInfo->stego_image_fname = "stego.bmp";

    return e_success;
}

/* Open required files */
Status open_files(EncodeInfo *encInfo)
{
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");     // Open source image
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");            // Open secret file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb"); // Create output image

    if (!encInfo->fptr_src_image || !encInfo->fptr_secret || !encInfo->fptr_stego_image)
        return e_failure;

    return e_success;
}

/* Check image capacity using board formula */
Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image); // Get image size
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);           // Get secret file size

    uint required_size = 16 + 32 + 32 + 32 + 54 + (encInfo->size_secret_file * 8); // Required capacity

    if (encInfo->image_capacity > required_size)
        return e_success;
    else
        return e_failure;
}

/* Copy BMP header (first 54 bytes) */
Status copy_bmp_header(FILE *src, FILE *dest)
{
    char buffer[54];

    rewind(src);                 // Move to beginning
    fread(buffer, 1, 54, src);   // Read 54-byte header
    fwrite(buffer, 1, 54, dest); // Write header to output

    return e_success;
}

/* Encode magic string */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    char buffer[8];

    for (int i = 0; magic_string[i] != '\0'; i++)
    {
        fread(buffer, 1, 8, encInfo->fptr_src_image);    // Read 8 bytes
        encode_byte_to_lsb(magic_string[i], buffer);     // Hide one character
        fwrite(buffer, 1, 8, encInfo->fptr_stego_image); // Write modified bytes
    }

    return e_success;
}

/* Encode extension size */
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
    char buffer[32];

    fread(buffer, 1, 32, encInfo->fptr_src_image); // Read 32 bytes
    encode_size_to_lsb(size, buffer);              // Encode extension size
    fwrite(buffer, 1, 32, encInfo->fptr_stego_image);

    return e_success;
}

/* Encode extension */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char buffer[8];

    for (int i = 0; file_extn[i] != '\0'; i++)
    {
        fread(buffer, 1, 8, encInfo->fptr_src_image); // Read 8 bytes
        encode_byte_to_lsb(file_extn[i], buffer);     // Hide extension character
        fwrite(buffer, 1, 8, encInfo->fptr_stego_image);
    }

    return e_success;
}

/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char buffer[32];

    fread(buffer, 1, 32, encInfo->fptr_src_image); // Read 32 bytes
    encode_size_to_lsb(file_size, buffer);         // Encode file size
    fwrite(buffer, 1, 32, encInfo->fptr_stego_image);

    return e_success;
}

/* Encode secret file data */
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char ch;
    char buffer[8];

    while ((ch = fgetc(encInfo->fptr_secret)) != EOF) // Read secret file character
    {
        fread(buffer, 1, 8, encInfo->fptr_src_image); // Read 8 bytes
        encode_byte_to_lsb(ch, buffer);               // Hide character
        fwrite(buffer, 1, 8, encInfo->fptr_stego_image);
    }

    return e_success;
}

/* Copy remaining image data */
Status copy_remaining_img_data(FILE *src, FILE *dest)
{
    char ch;

    while (fread(&ch, 1, 1, src)) // Read remaining bytes
        fwrite(&ch, 1, 1, dest);  // Write remaining bytes

    return e_success;
}

/* Encode one character into LSB */
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for (int i = 7; i >= 0; i--)
    {
        image_buffer[7 - i] &= 0xFE;              // Clear LSB
        image_buffer[7 - i] |= ((data >> i) & 1); // Insert data bit
    }

    return e_success;
}

/* Encode integer into LSB */
Status encode_size_to_lsb(int size, char *image_buffer)
{
    for (int i = 31; i >= 0; i--)
    {
        image_buffer[31 - i] &= 0xFE;              // Clear LSB
        image_buffer[31 - i] |= ((size >> i) & 1); // Insert size bit
    }

    return e_success;
}

/* Main encoding function */
Status do_encoding(EncodeInfo *encInfo)
{
    if (open_files(encInfo) == e_failure)
        return e_failure;

    if (check_capacity(encInfo) == e_failure)
        return e_failure;

    copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image);         // Copy header
    encode_magic_string(MAGIC_STRING, encInfo);                                  // Encode magic string
    encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo);    // Encode extn size
    encode_secret_file_extn(encInfo->extn_secret_file, encInfo);                 // Encode extension
    encode_secret_file_size(encInfo->size_secret_file, encInfo);                 // Encode file size
    encode_secret_file_data(encInfo);                                            // Encode secret data
    copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image); // Copy remaining data

    return e_success;
}
