#include <vector>
#include <fstream>
#include <iostream>
#include <cstring>
#include "encrypt.h"
#include "util.h"

#define BIT_ONE 0x01
#define BIT_TWO 0x02
#define BIT_THREE 0x04
#define BIT_FOUR 0x08
#define BIT_FIVE 0x10
#define BIT_SIX 0x20
#define BIT_SEVEN 0x40
#define BIT_EIGHT 0x80
#define BITS_TWO_EIGHT 0xFE

using namespace std;

const vector<char8_t> bits = {BIT_EIGHT, BIT_SEVEN, BIT_SIX, BIT_FIVE, BIT_FOUR, BIT_THREE, BIT_TWO, BIT_ONE};

/**
 * Encrypt message into .bmp file
 * @param myMessage enctypted message
 * @param newFile output stream
 * @param file input stream
 * @param offset start position of pixel data in the file
 * @param bitPerPixel bits used to store single pixel data
 */
void encrypt::encryptBmp(const string &myMessage, ofstream &newFile, ifstream &file, size_t offset, size_t bitPerPixel) {
    char read;
    while (file.tellg() < offset) {
        newFile.put((char) file.get());
    }

    for (char messageChar: myMessage) {
        for (uint8_t b: bits) {
            read = (char) (file.get() & BITS_TWO_EIGHT);
            if ((messageChar & b) != 0) read++;
            newFile.put(read);
            for (int j = 1; j < bitPerPixel / 8; j++) {
                newFile.put((char) file.get());
            }
        }
    }
}

/**
 * Encrypt message into raw .ppm file
 * @param myMessage enctypted message
 * @param newFile output stream
 * @param file input stream
 * @param offset start position of pixel data in the file
 */
void encrypt::encryptRawPpm(const string &myMessage, ofstream &newFile, ifstream &file, size_t offset) {
    char read;
    for(int i = 0; i < offset; i++){
        newFile.put((char) file.get());
    }
    do {
        read = (char) file.get();
        if (read < '0') newFile.put(read);
    } while (read < '0');
    for (char messageChar: myMessage) {
        for (uint8_t b: bits) {
            read = (char) (read & BITS_TWO_EIGHT);
            if ((messageChar & b) != 0) read++;
            newFile.put(read);
            newFile.put((char) file.get());
            newFile.put((char) file.get());
            read = (char) file.get();
        }
    }
    if (file.good()) {
        newFile.put(read);
    }
}

/**
 * Encrypt message into plain .ppm file
 * @param myMessage encrtypted message
 * @param newFile output stream
 * @param file input stream
 * @param offset start position of pixel data in the file
 */
void encrypt::encryptAsciiPpm(const string &myMessage, ofstream &newFile, ifstream &file, size_t offset) {
    char read, temp;
    for(int i = 0; i < offset; i++){
        newFile.put((char) file.get());
    }
    for (char messageChar: myMessage) {
        for (uint8_t b: bits) {
            util::reWriteAsciiInt(file, newFile);
            util::reWriteAsciiInt(file, newFile);
            do {
                read = (char) file.get();
                if (read < '0') { newFile.put(read); }
            } while (read < '0');
            while (read >= '0') {
                temp = read;
                read = (char) file.get();
                if (read < '0') {
                    temp = (char) (temp & BITS_TWO_EIGHT);
                    if ((messageChar & b) != 0) temp++;
                }
                newFile.put(temp);
            }
            newFile.put(read);
        }
    }
}

void encrypt::printBits(char c) {
    cout << ((c & BIT_EIGHT) >> 7) << ((c & BIT_SEVEN) >> 6) << ((c & BIT_SIX) >> 5)
         << ((c & BIT_FIVE) >> 4) << ((c & BIT_FOUR) >> 3) << ((c & BIT_THREE) >> 2)
         << ((c & BIT_TWO) >> 1) << ((c & BIT_ONE)) << endl;
}

/**
 * Extract message from .bmp file
 * @param file input stream
 * @param width image width in px
 * @param height image height in px
 * @param offset start position of pixel data in the file
 * @param bitPerPixel bits used to store single pixel data
 * @return extracted message
 */
char *encrypt::decryptBmp(ifstream &file, size_t width, size_t height, long offset, int bitPerPixel) {
    char *message = new char[width * height / 8];
    long currOffset = offset - 3;
    for (int i = 0; i < width * height / 8; i++) {
        message[i] = 0;
        for (char8_t b: bits) {
            file.seekg(currOffset += bitPerPixel / 8, ios::beg);
            message[i] <<= 1;
            message[i] += file.get() & BIT_ONE;
        }
        if (message[i] == 'D') {
            if (string(message).substr(strlen(message) - 4, 4) == "/END") {
                return message;
            }
        }
    }
    delete[] message;
    return nullptr;
}

/**
 * Extract message from raw .ppm file
 * @param file input stream
 * @param width image width in px
 * @param height image height in px
 * @param offset start position of pixel data in the file
 * @return extracted message
 */
char *encrypt::decryptRawPpm(ifstream &file, size_t width, size_t height, long offset) {
    char *message = new char[width * height / 8];
    char read;
    file.seekg(offset, ios::beg);
    do {
        read = (char) file.get();
    } while (read < '0');
    for (int i = 0; i < width * height / 8; i++) {
        message[i] = 0;
        for (char8_t b: bits) {
            message[i] <<= 1;
            message[i] += (read & BIT_ONE);
            file.get();
            file.get();
            read = (char) file.get();
        }
        if (message[i] == 'D') {
            if (string(message).substr(strlen(message) - 4, 4) == "/END") {
                return message;
            }
        }
    }
    delete[] message;
    return nullptr;
}

/**
 * Extract message from plain .ppm file
 * @param file input stream
 * @param width image width in px
 * @param height image height in px
 * @param offset start position of pixel data in the file
 * @return extracted message
 */
char *encrypt::decryptAsciiPpm(ifstream &file, size_t width, size_t height, long offset) {
    char *message = new char[width * height / 8];
    char temp, read;
    file.seekg(offset, ios::beg);
    for (int i = 0; i < width * height / 8; i++) {
        message[i] = 0;
        for (char8_t b: bits) {
            util::readAsciiInt(file);
            util::readAsciiInt(file);
            do {
                read = (char) file.get();
            } while (read < '0');
            while (read >= '0') {
                temp = read;
                read = (char) file.get();
                if (read < '0') {
                    message[i] <<= 1;
                    message[i] += (temp & BIT_ONE);
                }
            }
        }
        if (message[i] == 'D') {
            if (string(message).substr(strlen(message) - 4, 4) == "/END") {
                return message;
            }
        }
    }
    delete[] message;
    return nullptr;
}