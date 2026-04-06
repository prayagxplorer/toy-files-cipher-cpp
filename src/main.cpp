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
    virtual void run(bool encryptMode) = 0; 
};

// --- CAESAR CHILD CLASS ---
class CaesarCipher : public Cipher {
private:
    int shift;
public:
    CaesarCipher(string in, string out, int s) : Cipher(in, out), shift(s) {}

    void run(bool encryptMode) {
        string data = readFile();
        if (data.empty()) return;

        string result = "";
        int actualShift = encryptMode ? shift : (26 - (shift % 26));

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

// --- VIGENERE CHILD CLASS ---
class VigenereCipher : public Cipher {
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
    VigenereCipher(string in, string out, string k) : Cipher(in, out) {
        key = formatKey(k);
    }

    void run(bool encryptMode) {
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
                int moved;
                if (encryptMode) {
                    moved = current + shift;
                } else {
                    moved = current - shift;
                }

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

// --- XOR CHILD CLASS ---
class XorCipher : public Cipher {
private:
    string key;
public:
    XorCipher(string in, string out, string k) : Cipher(in, out), key(k) {}

    void run(bool encryptMode) {
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
        if (encryptMode) {
            printf("XOR encryption complete.\n");
        } else {
            printf("XOR decryption complete.\n");
        }
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
                myCipher = new CaesarCipher(in, out, s);
                myCipher->run(true);
                break;
            }
            case 2: {
                int s; cout << "Enter shift: "; cin >> s;
                myCipher = new CaesarCipher(in, out, s);
                myCipher->run(false);
                break;
            }
            case 3: {
                cout << "Enter Keyword: "; cin >> k;
                myCipher = new VigenereCipher(in, out, k);
                myCipher->run(true);
                break;
            }
            case 4: {
                cout << "Enter Keyword: "; cin >> k;
                myCipher = new VigenereCipher(in, out, k);
                myCipher->run(false);
                break;
            }
            case 5: {
                cout << "Enter Key: "; cin >> k;
                myCipher = new XorCipher(in, out, k);
                myCipher->run(true);
                break;
            }
            case 6: {
                cout << "Enter Key: "; cin >> k;
                myCipher = new XorCipher(in, out, k);
                myCipher->run(false);
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
