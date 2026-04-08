#include <iostream>
#include <cstdio>
#include <string>
#include <cctype>

using namespace std;

class cipher{
protected:
    string inputFileName;
    string outputFileName;

    string readfile(){
        FILE* filePtr = fopen(inputFileName.c_str(),"rb");
        if(filePtr == NULL){
            printf("Error: Could not open input file.\n");
            return "";
        }

        string content = "";
        int ch;
        while((ch = fgetc(filePtr)) != EOF){
            string oneChar = "";
            oneChar = oneChar + (char)ch;
            content = content + oneChar;
        }

        fclose(filePtr);
        return content;
    }

    void writefile(string content){
        FILE* filePtr = fopen(outputFileName.c_str(),"wb");
        if(filePtr == NULL){
            printf("Error: Could not open output file.\n");
            return;
        }

        for(int i = 0; i < (int)content.length(); i++){
            fputc((unsigned char)content[i],filePtr);
        }

        fclose(filePtr);
    }

public:
    cipher(string in,string out) {
        inputFileName = in;
        outputFileName = out;
    }
    virtual ~cipher() {}
    virtual void run() = 0;
};

class caesarencryptcipher : public cipher{
private:
    int shift;
public:
    caesarencryptcipher(string in,string out,int s) : cipher(in,out) {
        shift = s;
    }

    void run(){
        string data = readfile();
        if(data.empty()) return;

        string result = "";
        int actualShift = shift;

        for(int i = 0; i < (int)data.length(); i++){
            char c = data[i];
            if(isalpha((unsigned char)c)){
                int moved = c + actualShift;

                if(isupper((unsigned char)c)){
                    while(moved > 'Z'){
                        moved = moved - 26;
                    }
                    while(moved < 'A'){
                        moved = moved + 26;
                    }
                }else{
                    while(moved > 'z'){
                        moved = moved - 26;
                    }
                    while(moved < 'a'){
                        moved = moved + 26;
                    }
                }

                char changed = (char)moved;
                result = result + changed;
            }else{
                result = result + c;
            }
        }
        writefile(result);
    }
};

class caesardecryptcipher : public cipher{
private:
    int shift;
public:
    caesardecryptcipher(string in,string out,int s) : cipher(in,out) {
        shift = s;
    }

    void run(){
        string data = readfile();
        if(data.empty()) return;

        string result = "";
        int actualShift = 26 - (shift % 26);

        for(int i = 0; i < (int)data.length(); i++){
            char c = data[i];
            if(isalpha((unsigned char)c)){
                int moved = c + actualShift;

                if(isupper((unsigned char)c)){
                    while(moved > 'Z'){
                        moved = moved - 26;
                    }
                    while(moved < 'A'){
                        moved = moved + 26;
                    }
                }else{
                    while(moved > 'z'){
                        moved = moved - 26;
                    }
                    while(moved < 'a'){
                        moved = moved + 26;
                    }
                }

                char changed = (char)moved;
                result = result + changed;
            }else{
                result = result + c;
            }
        }
        writefile(result);
    }
};

class vigenereencryptcipher : public cipher{
private:
    string key;
    string formatkey(string k){
        string temp = "";
        for(int i = 0; i < (int)k.length(); i++){
            char c = k[i];
            if(isalpha((unsigned char)c)){
                char up = (char)toupper((unsigned char)c);
                temp = temp + up;
            }
        }
        return temp;
    }
public:
    vigenereencryptcipher(string in,string out,string k) : cipher(in,out){
        key = formatkey(k);
    }

    void run(){
        string text = readfile();
        if(text.empty()) return;
        if(key.empty()){
            printf("Keyword has no alphabetic characters.\n");
            return;
        }

        string result = "";
        int keyIdx = 0;
        int keyLen = (int)key.length();

        for(int i = 0; i < (int)text.length(); i++){
            char c = text[i];
            if(isalpha((unsigned char)c)){
                char base;
                if(isupper((unsigned char)c)){
                    base = 'A';
                } else{
                    base = 'a';
                }
                int shift = key[keyIdx % keyLen] - 'A';

                int current = c - base;
                int moved = current + shift;

                while(moved >= 26){
                    moved = moved - 26;
                }
                while(moved < 0){
                    moved = moved + 26;
                }

                char outChar = (char)(base + moved);
                result = result + outChar;
                keyIdx++;
            }else{
                result = result + c;
            }
        }
        writefile(result);
    }
};

class vigeneredecryptcipher : public cipher{
private:
    string key;
    string formatkey(string k){
        string temp = "";
        for(int i = 0; i < (int)k.length(); i++){
            char c = k[i];
            if(isalpha((unsigned char)c)){
                char up = (char)toupper((unsigned char)c);
                temp = temp + up;
            }
        }
        return temp;
    }
public:
    vigeneredecryptcipher(string in,string out,string k) : cipher(in,out){
        key = formatkey(k);
    }

    void run(){
        string text = readfile();
        if(text.empty()) return;
        if(key.empty()){
            printf("Keyword have no alphabetic characters.\n");
            return;
        }

        string result = "";
        int keyIdx = 0;
        int keyLen = (int)key.length();

        for(int i = 0; i < (int)text.length(); i++){
            char c = text[i];
            if(isalpha((unsigned char)c)){
                char base;
                if(isupper((unsigned char)c)){
                    base = 'A';
                }else{
                    base = 'a';
                }
                int shift = key[keyIdx % keyLen] - 'A';

                int current = c - base;
                int moved = current - shift;

                while(moved >= 26){
                    moved = moved - 26;
                }
                while(moved < 0){
                    moved = moved + 26;
                }

                char outChar = (char)(base + moved);
                result = result + outChar;
                keyIdx++;
            }else{
                result = result + c;
            }
        }
        writefile(result);
        
    }
};

class xorencryptcipher : public cipher{
private:
    string key;
public:
    xorencryptcipher(string in,string out,string k) : cipher(in,out){
        key = k;
    }

    void run(){
        string text = readfile();
        if(text.empty()) return;
        if(key.empty()){
            printf("Key cannot be empty.\n");
            return;
        }

        string result = "";
        int keyLen = (int)key.length();

        for(int i = 0; i < (int)text.length(); i++){
            unsigned char textChar = (unsigned char)text[i];
            unsigned char keyChar = (unsigned char)key[i % keyLen];
            char outChar = (char)(textChar ^ keyChar);
            result = result + outChar;
        }

        writefile(result);
        
    }
};

class xordecryptcipher : public cipher{
private:
    string key;
public:
    xordecryptcipher(string in,string out,string k) : cipher(in,out){
        key = k;
    }

    void run(){
        string text = readfile();
        if(text.empty()) return;
        if(key.empty()){
            printf("Key cannot be empty.\n");
            return;
        }

        string result = "";
        int keyLen = (int)key.length();

        for(int i = 0; i < (int)text.length(); i++){
            unsigned char textChar = (unsigned char)text[i];
            unsigned char keyChar = (unsigned char)key[i % keyLen];
            char outChar = (char)(textChar ^ keyChar);
            result = result + outChar;
        }

        writefile(result);
        
    }
};

int main(){
    int choice;
    string in,out,k;

    while(true){
        printf("\n1. Caesar Encrypt\n2. Caesar Decrypt\n3. Vigenere Encrypt\n4. Vigenere Decrypt\n5. XOR Encrypt\n6. XOR Decrypt\n7. Exit\nChoice: ");
        cin >> choice;
        if(choice == 7) break;

        cout << "Enter Input Filename: "; cin >> in;
        cout << "Enter Output Filename: "; cin >> out;

        cipher* myCipher = nullptr;

        switch(choice){
            case 1:{
                int s; cout << "Enter shift: "; cin >> s;
                myCipher = new caesarencryptcipher(in,out,s);
                myCipher->run();
                break;
            }
            case 2:{
                int s; cout << "Enter shift: "; cin >> s;
                myCipher = new caesardecryptcipher(in,out,s);
                myCipher->run();
                break;
            }
            case 3:{
                cout << "Enter Keyword: "; cin >> k;
                myCipher = new vigenereencryptcipher(in,out,k);
                myCipher->run();
                break;
            }
            case 4:{
                cout << "Enter Keyword: "; cin >> k;
                myCipher = new vigeneredecryptcipher(in,out,k);
                myCipher->run();
                break;
            }
            case 5:{
                cout << "Enter Key: "; cin >> k;
                myCipher = new xorencryptcipher(in,out,k);
                myCipher->run();
                break;
            }
            case 6:{
                cout << "Enter Key: "; cin >> k;
                myCipher = new xordecryptcipher(in,out,k);
                 myCipher->run();
                break;
            }
            default:{
                printf("Invalid choice.\n");
                break;
            }
        }
        delete myCipher;
    }
    return 0;
}
