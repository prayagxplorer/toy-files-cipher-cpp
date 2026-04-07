#ifndef CIPHER_H
#define CIPHER_H

// Minimal abstract interface for a cipher.
// In this repository the full implementation lives in src/main.cpp,
// but this header shows the inheritance idea: every cipher type shares
// a common base and can be destroyed safely through a base pointer.
class Cipher {
public:
    virtual ~Cipher() = default;
};

#endif
