#include <bits/stdc++.h>

using namespace std;

void caesar_encry(){


}

void caesar_decry(){


}

void monoalphabetic_encry(){
    

}

void monoalphabetic_decry(){
    

}

void Vigenere_encry(){


}

void Vigenere_decry(){

    
}

int main() {
    
    int choice = 0;
    while(choice != 7){
        cout << "Choose Cipher for Encryption or Decryption" << endl;
        cout << "1.Caesar Cipher\n2.Caesar Cipher\n3.Mono-Alphabetic\n4.Mono-Alphabetic\n5.Vigenere Cipher\n6.Vigenere Cipher\n7.Exit\n";
        cin >> choice;
        switch(choice){
            case 1:{
                caesar_encry();break;
            }
            case 2:{
                caesar_decry();break;
            }
            case 3:{
                monoalphabetic_encry();break;
            }
            case 4:{
                monoalphabetic_decry();break;
            }
            case 5:{
                Vigenere_encry();break;
            }
            case 6:{
                Vigenere_decry();break;
            }
            case 7:{
                break;
            }

        }
    }
    

    return 0;
}
