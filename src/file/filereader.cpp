// C++ Includes
#include <memory>
#include <cstdint>
#include <string>

// C Includes
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

// Local Includes
#include "exception/file_exception.h"
#include "filereader.h"

#define MAX_BUFFER_SIZE 4096

/**
 * Enums
 */
enum class SeekStart : uint8_t {
    kSEEK_SET = SEEK_SET,
    kSEEK_CUR = SEEK_CUR,
    kSEEK_END = SEEK_END,
    kSEEK_DATA = SEEK_DATA,
    kSEEK_HOLE = SEEK_HOLE
};

/**
 * FileReader 
 */
FileReader::FileReader() 
: eof_(false), fd_(-1), buffer_(nullptr), buffer_start_(0), buffer_end_(0), max_buffer_size_(MAX_BUFFER_SIZE)
{}

FileReader::FileReader(const std::string& file_path)
: eof_(false), buffer_start_(0), buffer_end_(0), max_buffer_size_(MAX_BUFFER_SIZE)
{
    Open(file_path.c_str());
    buffer_ = std::make_unique<uint8_t[]>(max_buffer_size_);
}

FileReader::FileReader(const char* file_path)
: eof_(false), buffer_start_(0), buffer_end_(0), max_buffer_size_(MAX_BUFFER_SIZE)
{
    Open(file_path);
    buffer_ = std::make_unique<uint8_t[]>(max_buffer_size_);
}

FileReader::~FileReader() {
    Close();
}

void FileReader::Open(const std::string& file_path) {
    if ((fd_ = open(file_path.c_str(), O_RDONLY)) == -1) {
        throw FileException((std::string("Failed to open file: ") + file_path).c_str());
    }
}

void FileReader::Open(const char* file_path) {
    if ((fd_ = open(file_path, O_RDONLY)) == -1) {
        throw FileException((std::string("Failed to open file: ") + file_path).c_str());
    }
}

void FileReader::Close() {
    if (fd_ != -1 && close(fd_) != -1) {
        throw FileException("Failed to close file");
    }
}

bool FileReader::End() noexcept {
    return eof_;
}

uint8_t FileReader::PeekByte() {
    if (buffer_end_ == buffer_start_ && FillBuffer() == 0)
        return '\0';
    return buffer_[buffer_start_];
}

uint8_t FileReader::ReadByte() {
    uint8_t ret_char = '\0';
    if (buffer_end_ == buffer_start_ && FillBuffer() == 0)
        return ret_char;
    ret_char = buffer_[buffer_start_++];
    return ret_char;
}

uint32_t FileReader::ReadBuffer(uint8_t* buffer, uint32_t buffer_size) {
    uint32_t bytes_read = 0;
    while(!eof_ && bytes_read < buffer_size) {
        uint32_t transfer = buffer_end_ - buffer_start_;
        if (buffer_start_ == buffer_end_)
            transfer = FillBuffer();
        transfer = (transfer < (buffer_size-bytes_read)) ? transfer : (buffer_size-bytes_read);
        memcpy(buffer+bytes_read, buffer_.get()+buffer_start_, transfer);
        bytes_read += transfer;
    }
    return bytes_read;
}

uint32_t FileReader::ReadLine(uint8_t* buffer, uint32_t buffer_size, uint8_t end) {
    uint32_t bytes_read = 0;
    while(!eof_ && bytes_read < buffer_size) {
        uint32_t transfer = buffer_end_ - buffer_start_;
        if (buffer_start_ == buffer_end_)
            transfer = FillBuffer();
        transfer = (transfer < (buffer_size - bytes_read)) ? transfer : (buffer_size-bytes_read);
        while(buffer_start_ < buffer_end_ && bytes_read < buffer_size) {
            buffer[bytes_read++] = buffer_[buffer_start_];
            if (buffer_[buffer_start_++] == end) {
                return bytes_read;
            }
        }
    }
    return bytes_read;
}

uint32_t FileReader::Seek(uint32_t offset, SeekStart whence) {
    uint32_t ret_val = 0;
    if ((ret_val == lseek(fd_, offset, (uint32_t)whence)) == -1)
        throw FileException("Failed to seek");
    FillBuffer();
    return ret_val;
}

uint32_t FileReader::FillBuffer() {
    buffer_start_ = 0;
    if ((buffer_end_ = read(fd_, buffer_.get(), max_buffer_size_)) == -1) {
        throw FileException("Failed to read file");
    } else if (buffer_end_ == 0) {
        eof_ = true;
    }
    return buffer_end_;
}
