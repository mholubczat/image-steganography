#include <string>
#include <cstring>
#include <stdexcept>
#include <fstream>
#include "util.h"

using namespace std;

const string fileNotSupported = "File not supported.";
const string fileCorrupted = "Incorrect data. The file is corrupted.";

string util::tryGetExtension(const char *&path) {
    string extension;
    extension = string(path).substr(strlen(path) - 4, 4);
    if (strlen(path) < 4 || !((extension == ".bmp") || (extension == ".ppm"))) {
        throw invalid_argument(fileNotSupported);
    }
    return extension;
}

void util::readBmpHeader(ifstream &file, long &offset, size_t &width, size_t &height, int &bitPerPixel) {
    offset = 0;
    width = 0;
    height = 0;
    bitPerPixel = 0;

    file.seekg(10, ios::beg);
    for (int i = 0; i < 4; i++) {
        offset += (file.get() << 8 * i);
    }
    file.seekg(28, ios::beg);
    for (int i = 0; i < 2; i++) {
        bitPerPixel += (file.get() << 8 * i);
    }
    file.seekg(18, ios::beg);
    for (int i = 0; i < 4; i++) {
        width += (file.get() << 8 * i);
    }
    file.seekg(22, ios::beg);
    for (int i = 0; i < 4; i++) {
        height += (file.get() << 8 * i);
    }
    file.seekg(0, ios::beg);
}

void util::readPpmHeader(ifstream &file, long &offset, size_t &width, size_t &height, bool &isAscii) {
    char read;
    file.seekg(0, ios::beg);
    read = (char) file.get();
    if (read != 'P') throw runtime_error(fileCorrupted);
    read = (char) file.get();
    switch (read) {
        case '3':
            isAscii = true;
            break;
        case '6':
            isAscii = false;
            break;
        default:
            throw runtime_error(fileCorrupted);
    }
    width = readAsciiInt(file);
    height = readAsciiInt(file);
    size_t maxVal = readAsciiInt(file);
    if (maxVal > 255) throw invalid_argument(fileNotSupported);
    offset = file.tellg();
    file.seekg(0, ios::beg);
}

size_t util::readAsciiInt(ifstream &file) {
    int res = 0;
    char read;
    do {
        read = (char) file.get();
        if(read == '#'){
            file.getline(nullptr, 70);
        }
    } while (read < '0');

    do {
        res *= 10;
        res += (read - 48);
        read = (char) file.get();
        if (read > '9') { throw runtime_error(fileCorrupted); }
    } while (read >= '0');
    return res;
}

void util::reWriteAsciiInt(ifstream &file, ofstream &newFile) {
    char read;
    do {
        read = (char) file.get();
        newFile.put(read);
    } while (read < '0');
    newFile.put(read);
    do {
        read = (char) file.get();
        newFile.put(read);
        if (read > '9') { throw runtime_error(fileCorrupted); }
    } while (read >= '0');
}

