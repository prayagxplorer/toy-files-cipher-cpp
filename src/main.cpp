#include <iostream>
#include <cstdio>  // Standard C File I/O
#include <string>
#include <cctype>

using namespace std;

// --- BASE CLASS ---
class Cipher {
protected:
    string inputFileName;
    string outputFileName;

    // Reading the file using C-style FILE pointers
    string readFile() {
        FILE* filePtr = fopen(inputFileName.c_str(), "rb");
        if (filePtr == NULL) {
            printf("Error: Could not open input file.\n");
            return "";
        }

        string content = "";
        int ch;
        // Read character by character until End Of File (EOF)
        while ((ch = fgetc(filePtr)) != EOF) {
            string oneChar = "";
            oneChar += (char)ch;
            content = content + oneChar;
        }

        fclose(filePtr);
        return content;
    }

    // Writing the file using C-style FILE pointers
    void writeFile(string content) {
        FILE* filePtr = fopen(outputFileName.c_str(), "wb");
        if (filePtr == NULL) {
            printf("Error: Could not open output file.\n");
            return;
        }

        for (int i = 0; i < (int)content.length(); i++) {
            fputc((unsigned char)content[i], filePtr);
        }

        fclose(filePtr);
    }

public:
    Cipher(string in, string out) : inputFileName(in), outputFileName(out) {}
    virtual ~Cipher() {}
    virtual void run() = 0; 
};

// --- CAESAR ENCRYPT CHILD CLASS ---
class CaesarEncryptCipher : public Cipher {
private:
    int shift;
public:
    CaesarEncryptCipher(string in, string out, int s) : Cipher(in, out), shift(s) {}

    void run() {
        string data = readFile();
        if (data.empty()) return;

        string result = "";
        int actualShift = shift;

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
                int moved = current + shift;

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

// --- VIGENERE DECRYPT CHILD CLASS ---
class VigenereDecryptCipher : public Cipher {
private:
    string key;
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
            unsigned char textChar = (unsigned char)text[i];
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
    
    while (true) {
        printf("\n1. Caesar Encrypt\n2. Caesar Decrypt\n3. Vigenere Encrypt\n4. Vigenere Decrypt\n5. XOR Encrypt\n6. XOR Decrypt\n7. Exit\nChoice: ");
        cin >> choice;
        if (choice == 7) break;

        cout << "Enter Input Filename: "; cin >> in;
        cout << "Enter Output Filename: "; cin >> out;

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
        delete myCipher; 
    }
    return 0;
}
