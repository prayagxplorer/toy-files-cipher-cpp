# Toy File Cipher (Educational)

## Overview
This project implements educational cryptography algorithms in C++.
It demonstrates OOPS concepts and basic file encryption/decryption.
Not intended for production security use.

## Features
- Encrypt file with a key
- Decrypt file with the same key
- CLI-based interface

## Build
g++ -std=c++17 src/*.cpp -o cipher

## Run
./cipher encrypt input.txt output.enc mykey
./cipher decrypt output.enc output.txt mykey

## Design
- Cipher logic separated from file I/O
- Clean class interfaces

## Security Disclaimer
This is an educational implementation. Do not use for real security.
