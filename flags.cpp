#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/stat.h>
#include "flags.h"
#include "util.h"
#include "encrypt.h"

using namespace std;

const string missingParameters = "Missing parameter(s). Enter -h for more information.";
const string fileNotFound = "File not found.";
const string cannotEncrypt = "Encryption aborted. Run -c for more details.";

#ifdef WIN32
#define stat _stat
#endif

void flags::info(const char *&path, bool verbose) {
    if (!path) throw invalid_argument(missingParameters);

    struct stat fileStats{};
    if (stat(path, &fileStats) != 0) throw invalid_argument(fileNotFound);

    if (verbose) {
        cout << "\nFile data" << endl;
        cout << "Extension:" << '\t' << util::tryGetExtension(path) << endl;
        cout << "Size:\t" << '\t' << fileStats.st_size << " bytes" << endl;
        cout << "Size on disk: " << '\t' << fileStats.st_blocks * 512 << " bytes" << endl;
        cout << "Last modified: " << '\t' << fileStats.st_mtime << " seconds since Unix epoch" << endl;
    }
}

bool flags::check(const char *&path, const char *&message, bool verbose) {
    if (!path || !message) throw invalid_argument(missingParameters);
    info(path, false);
    string extension = util::tryGetExtension(path);
    if (verbose) {
        cout << "Checking path: " << path << " message: " << message << endl;
    }

    size_t myMsgLength = strlen(message) + 4;
    long offset;
    size_t width;
    size_t height;

    ifstream file(path, ios::binary);
    if (extension == ".bmp") {
        int bitPerPixel;
        util::readBmpHeader(file, offset, width, height, bitPerPixel);
    }
    if (extension == ".ppm") {
        bool isAscii = false;
        util::readPpmHeader(file, offset, width, height, isAscii);
    }
    bool canEncrypt = width * height / 8 > myMsgLength;
    if (verbose) {
        if (canEncrypt) {
            cout << "Message can be encrypted in the file" << endl;
        } else {
            cout << "Message cannot be encrypted in the file" << endl;
        }
    }
    return (canEncrypt);
}

void flags::encrypt(const char *&path, const char *&message) {
    if (!path || !message) throw invalid_argument(missingParameters);
    info(path, false);
    string extension = util::tryGetExtension(path);
    if (!check(path, message, false)) throw invalid_argument(cannotEncrypt);
    cout << "Encrypt, path: " << path << " message: " << message << endl;

    string newFilePath = string(path).substr(0, strlen(path) - 4)
            .append(" encrypted").append(extension);
    ofstream newFile(newFilePath, ios::binary);
    ifstream file(path, ios::binary);
    string myMessage = string(message).append("/END");
    long offset;
    size_t width;
    size_t height;
    if (extension == ".bmp") {
        int bitPerPixel;
        util::readBmpHeader(file, offset, width, height, bitPerPixel);
        encrypt::encryptBmp(myMessage, newFile, file, offset, bitPerPixel);
    }
    if (extension == ".ppm") {
        bool isAscii = false;
        util::readPpmHeader(file, offset, width, height, isAscii);
        if (isAscii)
            encrypt::encryptAsciiPpm(myMessage, newFile, file, offset);
        else encrypt::encryptRawPpm(myMessage, newFile, file, offset);
    }
    while (file.good()) {
        newFile.put((char) file.get());
    }
    newFile.close();
    file.close();
}

void flags::decrypt(const char *&path) {
    if (!path) throw invalid_argument(missingParameters);
    info(path, false);
    const string extension = util::tryGetExtension(path);
    cout << "Decrypt, path:" << path << endl;
    ifstream file(path, ios::binary);
    long offset;
    size_t width;
    size_t height;
    char *message = nullptr;
    if (extension == ".bmp") {
        int bitPerPixel;
        util::readBmpHeader(file, offset, width, height, bitPerPixel);
        message = encrypt::decryptBmp(file, width, height, offset, bitPerPixel);
    }

    if (extension == ".ppm") {
        bool isAscii;
        util::readPpmHeader(file, offset, width, height, isAscii);
        if (isAscii) {
            message = encrypt::decryptAsciiPpm(file, width, height, offset);
        } else { message = encrypt::decryptRawPpm(file, width, height, offset); }
    }

    if (message != nullptr) {
        cout << "Decrypted message: " << string(message).erase(strlen(message) - 4) << endl;
        delete[] message;
    } else cout << "No message found!" << endl;
    file.close();
}

void flags::help() {
    cout << "Help: " << endl;
    cout << "Simple image steganography tool" << endl;
    cout << "Supported extensions: .bmp, .ppm \n16-bit .ppm extensions are not supported" << endl;
    cout << "Command list:" << endl;
    cout << "-i (or --info)\t\t[path]\t\t\tImage info: extension, size, modification date" << endl;
    cout << "-c (or --check)\t\t[path][message]\tVerify if the *message* can be encrypted in a file located in *path*"
         << endl;
    cout
            << "-e (or --encrypt)\t[path][message]\tEncrypt the *message* in a file located in *path*. New file will be created."
            << endl;
    cout << "-d (or --decrypt)\t[path]\t\t\tDecrypt message from the file located in *path*." << endl;
    cout << "-h (or --help)\t\t\t\t\t\tHelp" << endl;
}
