#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Cipher {
protected:
    string inputFileName;
    string outputFileName;

    // Helper to read the entire file into a string
    string readFile() {
        ifstream inFile(inputFileName);
        if (!inFile) {
            cout << "Error opening input file!" << endl;
            return "";
        }
        string content((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
        inFile.close();
        return content;
    }

    // Helper to write a string to the output file
    void writeFile(string content) {
        ofstream outFile(outputFileName);
        if (!outFile) {
            cout << "Error opening output file!" << endl;
            return;
        }
        outFile << content;
        outFile.close();
        cout << "Task completed. Check: " << outputFileName << endl;
    }

public:
    Cipher(string in, string out) : inputFileName(in), outputFileName(out) {}
    virtual ~Cipher() {}

    // POLYMORPHISM: The "Workhorse" function
    // This will be defined differently for every cipher
    virtual void run(bool encryptMode) = 0; 
};

class CaesarCipher : public Cipher {
private:
    int shift; // Encapsulated: specific to Caesar

public:
    CaesarCipher(string in, string out, int s) : Cipher(in, out), shift(s) {}

    void run(bool encryptMode) override {
        string data = readFile();
        string result = "";

        // Adjust shift for decryption
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
    }
};

int main() {
    int choice;
    string in, out;
    
    while (true) {
        cout << "\n1. Caesar Encrypt\n2. Caesar Decrypt\n7. Exit\nChoice: ";
        cin >> choice;
        if (choice == 7) break;

        cout << "Enter Input Filename: "; cin >> in;
        cout << "Enter Output Filename: "; cin >> out;

        Cipher* myCipher = nullptr; // Pointer for Polymorphism

        switch (choice) {
            case 1: {
                int s; cout << "Enter shift: "; cin >> s;
                myCipher = new CaesarCipher(in, out, s);
                myCipher->run(true); // Encrypt
                break;
            }
            case 2: {
                int s; cout << "Enter shift: "; cin >> s;
                myCipher = new CaesarCipher(in, out, s);
                myCipher->run(false); // Decrypt
                break;
            }
            // Add other ciphers here (Vigenere, Monoalphabetic)
        }

        delete myCipher; // Clean up memory
    }
    return 0;
}