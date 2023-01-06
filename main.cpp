#include <iostream>
#include <cstring>

using namespace std;

const string missingParameters = "Missing parameter(s). Enter -h for more information.";

void info(const char *path) {
    if(!path) throw invalid_argument(missingParameters);
    cout << "Info, path:" << path << endl;
}

void encrypt(const char *path, const char *message) {
    if(!path || !message) throw invalid_argument(missingParameters);
    cout << "Encrypt, path: " << path << " message: " << message << endl;
}

void decrypt(const char *path) {
    if(!path) throw invalid_argument(missingParameters);
    cout << "Decrypt, path:" << path << endl;
}

void check(const char *path, const char *message) {
    if(!path || !message) throw invalid_argument(missingParameters);
    cout << "Check, path: " << path << " message: " << message << endl;
}

void help() {
    cout << "Help" << endl;
}

int main(int argc, char *argv[]) {
    for (int i = 0; i < argc; i++) {
        if (i == 0) {
            if (argc == 1) help();
            continue;
        }
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            help();
            continue;
        }
        if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--info") == 0) {
            info(argv[++i]);
            continue;
        }
        if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--encrypt") == 0) {
            const char* path = argv[++i];
            const char* message = argv[++i];
            encrypt(path, message);
            continue;
        }
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--decrypt") == 0) {
            decrypt(argv[++i]);
            continue;
        }
        if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--check") == 0) {
            const char* path = argv[++i];
            const char* message = argv[++i];
            check(path, message);
            continue;
        }
        string invalidOption = "Invalid option \"" + string(argv[i]) + "\". Enter -h for more information";
        throw runtime_error(invalidOption);
    }
    return 0;
}
