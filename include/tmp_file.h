#pragma once

#include <string>

class TmpFile {
  public:
    TmpFile();
    TmpFile(std::string filename_template);

    TmpFile(const TmpFile &) = delete;
    TmpFile(TmpFile &&);
    TmpFile &operator=(const TmpFile &) = delete;
    TmpFile &operator=(TmpFile &&);

    ~TmpFile();

    std::string get_name();

  private:
    std::string filename_;
};