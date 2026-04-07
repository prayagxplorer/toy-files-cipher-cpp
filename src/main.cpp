#include <iostream>
#include <cstdio>  // Standard C File I/O
#include <string>
#include <cctype>

using namespace std;

// This program is structured around one abstract base class (Cipher)
// and several child classes. Each child class knows how to transform
// file contents using one specific algorithm.

// --- BASE CLASS ---
class Cipher {
protected:
    // The base class stores the filenames so every child class can reuse
    // the same file-reading and file-writing behavior.
    string inputFileName;
    string outputFileName;

    // Read the whole input file into a std::string using C-style file I/O.
    // "rb" means "read binary", which avoids newline conversion and makes
    // this safe for both text files and raw XOR output.
    string readFile() {
        FILE* filePtr = fopen(inputFileName.c_str(), "rb");
        if (filePtr == NULL) {
            printf("Error: Could not open input file.\n");
            return "";
        }

        string content = "";
        int ch;
        // Read one byte at a time until EOF is reached.
        while ((ch = fgetc(filePtr)) != EOF) {
            string oneChar = "";
            // fgetc returns an int, so cast back to char before appending.
            oneChar  = oneChar + (char)ch;
            content = content + oneChar;
        }

        fclose(filePtr);
        return content;
    }

    // Write the transformed content back to disk.
    // "wb" means "write binary", so bytes are written exactly as stored.
    void writeFile(string content) {
        FILE* filePtr = fopen(outputFileName.c_str(), "wb");
        if (filePtr == NULL) {
            printf("Error: Could not open output file.\n");
            return;
        }

        // Write one byte at a time to mirror the simple readFile approach.
        for (int i = 0; i < (int)content.length(); i++) {
            fputc((unsigned char)content[i], filePtr);
        }

        fclose(filePtr);
    }

public:
    // Constructor shared by all child classes.
    Cipher(string in, string out) : inputFileName(in), outputFileName(out) {}

    // Virtual destructor ensures the correct child destructor would run
    // when deleting through a Cipher* pointer.
    virtual ~Cipher() {}

    // Every concrete cipher must implement its own processing logic.
    virtual void run() = 0; 
};

// --- CAESAR ENCRYPT CHILD CLASS ---
class CaesarEncryptCipher : public Cipher {
private:
    // Number of alphabet positions to shift each letter forward.
    int shift;
public:
    CaesarEncryptCipher(string in, string out, int s) : Cipher(in, out), shift(s) {}

    void run() {
        string data = readFile();
        // In this program an empty string means either:
        // 1. the file could not be opened, or
        // 2. the file was actually empty.
        if (data.empty()) return;

        string result = "";
        // Keeping this in a named variable makes the later math easier to read.
        int actualShift = shift;

        for (int i = 0; i < (int)data.length(); i++) {
            char c = data[i];
            if (isalpha((unsigned char)c)) {
                // Caesar only shifts letters. Case is preserved by choosing
                // a different base for uppercase and lowercase characters.
                bool upper = isupper((unsigned char)c) != 0;
                char base;
                if (upper) {
                    base = 'A';
                } else {
                    base = 'a';
                }

                // Convert 'A'..'Z' or 'a'..'z' into a 0..25 range.
                int normalized = c - base;
                // Apply the shift in that 0..25 alphabet space.
                int moved = normalized + actualShift;

                // Wrap around the alphabet manually instead of using %.
                // Example: shifting 'Z' by 1 should become 'A', not '['.
                while (moved >= 26) {
                    moved = moved - 26;
                }
                while (moved < 0) {
                    moved = moved + 26;
                }

                // Convert back from 0..25 to an actual ASCII letter.
                char changed = (char)(base + moved);
                result = result + changed;
            } else {
                // Non-letter characters (spaces, punctuation, digits, etc.)
                // are copied unchanged.
                result = result + c;
            }
        }
        writeFile(result);
        printf("Caesar processing complete.\n");
    }
};

// --- CAESAR DECRYPT CHILD CLASS ---
class CaesarDecryptCipher : public Cipher {
private:
    int shift;
public:
    CaesarDecryptCipher(string in, string out, int s) : Cipher(in, out), shift(s) {}

    void run() {
        string data = readFile();
        if (data.empty()) return;

        string result = "";
        // Decryption is the opposite of encryption.
        // Shifting backward by N is the same as shifting forward by 26 - N
        // in a 26-letter alphabet.
        int actualShift = 26 - (shift % 26);

        for (int i = 0; i < (int)data.length(); i++) {
            char c = data[i];
            if (isalpha((unsigned char)c)) {
                bool upper = isupper((unsigned char)c) != 0;
                char base;
                if (upper) {
                    base = 'A';
                } else {
                    base = 'a';
                }

                int normalized = c - base;
                int moved = normalized + actualShift;

                // Wrap back into the valid alphabet range.
                while (moved >= 26) {
                    moved = moved - 26;
                }
                while (moved < 0) {
                    moved = moved + 26;
                }

                char changed = (char)(base + moved);
                result = result + changed;
            } else {
                result = result + c;
            }
        }
        writeFile(result);
        printf("Caesar processing complete.\n");
    }
};

// --- VIGENERE ENCRYPT CHILD CLASS ---
class VigenereEncryptCipher : public Cipher {
private:
    string key;

    // Vigenere works best when the keyword is reduced to letters only
    // and normalized to one case. This avoids repeated case handling later.
    string formatKey(string k) {
        string temp = "";
        for (int i = 0; i < (int)k.length(); i++) {
            char c = k[i];
            if (isalpha((unsigned char)c)) {
                char up = (char)toupper((unsigned char)c);
                temp = temp + up;
            }
        }
        return temp;
    }
public:
    VigenereEncryptCipher(string in, string out, string k) : Cipher(in, out) {
        key = formatKey(k);
    }

    void run() {
        string text = readFile();
        if (text.empty()) return;
        if (key.empty()) {
            // A keyword like "1234" becomes empty after formatKey().
            printf("Error: Keyword has no alphabetic characters.\n");
            return;
        }

        string result = "";
        // keyIdx tracks how many letters from the plaintext/ciphertext have
        // been processed. It advances only for alphabetic characters.
        int keyIdx = 0;
        int keyLen = (int)key.length();

        for (int i = 0; i < (int)text.length(); i++) {
            char c = text[i];
            if (isalpha((unsigned char)c)) {
                char base;
                if (isupper((unsigned char)c)) {
                    base = 'A';
                } else {
                    base = 'a';
                }
                // 'A' means shift by 0, 'B' by 1, ..., 'Z' by 25.
                int shift = key[keyIdx % keyLen] - 'A';

                int current = c - base;
                int moved = current + shift;

                // Manual wrap-around keeps the result inside the alphabet.
                while (moved >= 26) {
                    moved = moved - 26;
                }
                while (moved < 0) {
                    moved = moved + 26;
                }

                char outChar = (char)(base + moved);
                result = result + outChar;
                // Advance to the next keyword letter only when we consume
                // an actual alphabetic character from the input text.
                keyIdx++;
            } else {
                // Punctuation/spacing is preserved and does not consume key.
                result = result + c;
            }
        }
        writeFile(result);
        printf("Vigenere processing complete.\n");
    }
};

// --- VIGENERE DECRYPT CHILD CLASS ---
class VigenereDecryptCipher : public Cipher {
private:
    string key;

    // Same cleanup step as encryption: keep letters only, force uppercase.
    string formatKey(string k) {
        string temp = "";
        for (int i = 0; i < (int)k.length(); i++) {
            char c = k[i];
            if (isalpha((unsigned char)c)) {
                char up = (char)toupper((unsigned char)c);
                temp = temp + up;
            }
        }
        return temp;
    }
public:
    VigenereDecryptCipher(string in, string out, string k) : Cipher(in, out) {
        key = formatKey(k);
    }

    void run() {
        string text = readFile();
        if (text.empty()) return;
        if (key.empty()) {
            printf("Error: Keyword has no alphabetic characters.\n");
            return;
        }

        string result = "";
        int keyIdx = 0;
        int keyLen = (int)key.length();

        for (int i = 0; i < (int)text.length(); i++) {
            char c = text[i];
            if (isalpha((unsigned char)c)) {
                char base;
                if (isupper((unsigned char)c)) {
                    base = 'A';
                } else {
                    base = 'a';
                }
                int shift = key[keyIdx % keyLen] - 'A';

                int current = c - base;
                // Decryption reverses the keyword shift instead of adding it.
                int moved = current - shift;

                while (moved >= 26) {
                    moved = moved - 26;
                }
                while (moved < 0) {
                    moved = moved + 26;
                }

                char outChar = (char)(base + moved);
                result = result + outChar;
                keyIdx++;
            } else {
                result = result + c;
            }
        }
        writeFile(result);
        printf("Vigenere processing complete.\n");
    }
};

// --- XOR ENCRYPT CHILD CLASS ---
class XorEncryptCipher : public Cipher {
private:
    // XOR uses a raw byte key rather than alphabet-only logic.
    string key;
public:
    XorEncryptCipher(string in, string out, string k) : Cipher(in, out), key(k) {}

    void run() {
        string text = readFile();
        if (text.empty()) return;
        if (key.empty()) {
            printf("Error: Key cannot be empty.\n");
            return;
        }

        string result = "";
        int keyLen = (int)key.length();

        for (int i = 0; i < (int)text.length(); i++) {
            // Cast to unsigned char so bitwise XOR is performed on raw bytes
            // without sign-extension surprises.
            unsigned char textChar = (unsigned char)text[i];
            // Reuse the key cyclically when the file is longer than the key.
            unsigned char keyChar = (unsigned char)key[i % keyLen];
            char outChar = (char)(textChar ^ keyChar);
            result = result + outChar;
        }

        writeFile(result);
        printf("XOR encryption complete.\n");
    }
};

// --- XOR DECRYPT CHILD CLASS ---
class XorDecryptCipher : public Cipher {
private:
    string key;
public:
    XorDecryptCipher(string in, string out, string k) : Cipher(in, out), key(k) {}

    void run() {
        string text = readFile();
        if (text.empty()) return;
        if (key.empty()) {
            printf("Error: Key cannot be empty.\n");
            return;
        }

        string result = "";
        int keyLen = (int)key.length();

        for (int i = 0; i < (int)text.length(); i++) {
            unsigned char textChar = (unsigned char)text[i];
            unsigned char keyChar = (unsigned char)key[i % keyLen];

            // XOR is symmetric: applying the same key again undoes the first
            // XOR operation. That is why encrypt/decrypt look identical here.
            char outChar = (char)(textChar ^ keyChar);
            result = result + outChar;
        }

        writeFile(result);
        printf("XOR decryption complete.\n");
    }
};

int main() {
    int choice;
    string in, out, k;
    
    // Simple menu loop: keep asking until the user chooses Exit.
    while (true) {
        printf("\n1. Caesar Encrypt\n2. Caesar Decrypt\n3. Vigenere Encrypt\n4. Vigenere Decrypt\n5. XOR Encrypt\n6. XOR Decrypt\n7. Exit\nChoice: ");
        cin >> choice;
        if (choice == 7) break;

        cout << "Enter Input Filename: "; cin >> in;
        cout << "Enter Output Filename: "; cin >> out;

        // Polymorphism: whichever child object we create can be handled
        // through a base-class pointer because all ciphers expose run().
        Cipher* myCipher = nullptr; 

        switch (choice) {
            case 1: {
                int s; cout << "Enter shift: "; cin >> s;
                myCipher = new CaesarEncryptCipher(in, out, s);
                myCipher->run();
                break;
            }
            case 2: {
                int s; cout << "Enter shift: "; cin >> s;
                myCipher = new CaesarDecryptCipher(in, out, s);
                myCipher->run();
                break;
            }
            case 3: {
                cout << "Enter Keyword: "; cin >> k;
                myCipher = new VigenereEncryptCipher(in, out, k);
                myCipher->run();
                break;
            }
            case 4: {
                cout << "Enter Keyword: "; cin >> k;
                myCipher = new VigenereDecryptCipher(in, out, k);
                myCipher->run();
                break;
            }
            case 5: {
                cout << "Enter Key: "; cin >> k;
                myCipher = new XorEncryptCipher(in, out, k);
                myCipher->run();
                break;
            }
            case 6: {
                cout << "Enter Key: "; cin >> k;
                myCipher = new XorDecryptCipher(in, out, k);
                myCipher->run();
                break;
            }
            default: {
                printf("Invalid choice.\n");
                break;
            }
        }

        // Safe even when myCipher is still nullptr after an invalid choice.
        delete myCipher; 
    }
    return 0;
}
