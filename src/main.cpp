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
        FILE* filePtr = fopen(inputFileName.c_str(), "r");
        if (filePtr == NULL) {
            printf("Error: Could not open input file.\n");
            return "";
        }

        string content = "";
        char ch;
        // Read character by character until End Of File (EOF)
        while ((ch = fgetc(filePtr)) != EOF) {
            content += ch;
        }

        fclose(filePtr);
        return content;
    }

    // Writing the file using C-style FILE pointers
    void writeFile(string content) {
        FILE* filePtr = fopen(outputFileName.c_str(), "w");
        if (filePtr == NULL) {
            printf("Error: Could not open output file.\n");
            return;
        }

        // fputs writes the entire string to the file
        fputs(content.c_str(), filePtr);

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

    void run(bool encryptMode) override {
        string data = readFile();
        if (data.empty()) return;

        string result = "";
        int actualShift = encryptMode ? shift : (26 - (shift % 26));

        for (char &c : data) {
            if (isalpha(c)) {
                char base = isupper(c) ? 'A' : 'a';
                result += (c - base + actualShift) % 26 + base;
            } else {
                result += c;
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
        for (char c : k) if (isalpha(c)) temp += toupper(c);
        return temp;
    }
public:
    VigenereCipher(string in, string out, string k) : Cipher(in, out) {
        key = formatKey(k);
    }

    void run(bool encryptMode) override {
        string text = readFile();
        if (text.empty()) return;

        string result = "";
        int keyIdx = 0;
        int keyLen = key.length();

        for (char c : text) {
            if (isalpha(c)) {
                char base = isupper(c) ? 'A' : 'a';
                int shift = key[keyIdx % keyLen] - 'A';
                if (encryptMode)
                    result += (c - base + shift) % 26 + base;
                else
                    result += (c - base - shift + 26) % 26 + base;
                keyIdx++;
            } else {
                result += c;
            }
        }
        writeFile(result);
        printf("Vigenere processing complete.\n");
    }
};

int main() {
    int choice;
    string in, out, k;
    
    while (true) {
        printf("\n1. Caesar Encrypt\n2. Caesar Decrypt\n3. Vigenere Encrypt\n4. Vigenere Decrypt\n5. Exit\nChoice: ");
        cin >> choice;
        if (choice == 5) break;

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
        }
        delete myCipher; 
    }
    return 0;
}