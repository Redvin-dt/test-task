#include "tmp_file.h"

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <utility>

namespace {
constexpr char FILENAME_TEMPLATE[20] = "/tmp/storage.XXXXXX";
} // namespace

TmpFile::TmpFile(std::string filename_template) {
    char *filename = filename_template.data();
    int fh = mkstemp(filename);
    close(fh);
    this->filename = filename;
}

TmpFile::TmpFile() : TmpFile(FILENAME_TEMPLATE) {}

TmpFile::TmpFile(TmpFile &&other)
    : filename(std::exchange(other.filename, std::string())) {}

TmpFile &TmpFile::operator=(TmpFile &&other) {
    if (this != &other) {
        filename = std::exchange(other.filename, std::string());
    }
    return *this;
}

TmpFile::~TmpFile() {
    if (!filename.empty()) {
        remove(filename.c_str());
    }
}

std::string TmpFile::get_name() { return filename; }
