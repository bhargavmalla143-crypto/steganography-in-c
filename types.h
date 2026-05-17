/*
   File: types.h
   Description:
   This file contains user defined data types
   used throughout the steganography project.
*/

#ifndef TYPES_H
#define TYPES_H

/* Shortcut name for unsigned int */
typedef unsigned int uint; // Used for image size and file size

/* Status type used for function return values */
typedef enum
{
    e_failure, // Indicates failure
    e_success  // Indicates success
} Status;

/* Operation type to identify encode or decode */
typedef enum
{
    e_encode,     // For encoding operation
    e_decode,     // For decoding operation
    e_unsupported // For invalid option
} OperationType;

#endif
