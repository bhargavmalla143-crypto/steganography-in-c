# Steganography in C

A console-based steganography application developed using C programming language.

## Features

- Encode secret text into BMP image
- Decode hidden text from image
- Magic string verification
- File extension encoding
- File size encoding
- Command-line execution
- LSB (Least Significant Bit) encoding technique

## Technologies Used

- C Language
- File Handling
- Bitwise Operations
- Structures
- BMP Image Processing
- Modular Programming (Header Files)

## How to Run

### Compile

```bash
gcc main.c encode.c decode.c -o stego
```

### Encoding

```bash
./stego -e beautiful.bmp secret.txt output.bmp
```

### Decoding

```bash
./stego -d output.bmp
```

## Project Structure

```bash
.
├── main.c
├── encode.c
├── decode.c
├── encode.h
├── decode.h
├── common.h
├── types.h
├── beautiful.bmp
├── output.bmp
├── secret.txt
└── decoded.txt
```

## Concepts Used

- LSB Steganography
- Bit Manipulation
- BMP File Handling
- Dynamic File Operations
- Command Line Arguments

## Future Improvements

- PNG support
- Password protection
- Encryption support
- GUI version

## Author

Surya Bhargav
