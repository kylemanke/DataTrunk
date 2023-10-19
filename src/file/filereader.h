#ifndef DT_SRC_FILE_FILEREADER_H
#define DT_SRC_FILE_FILEREADER_H

// C++ Includes
#include <memory>
#include <cstdint>
#include <string>

enum class SeekStart : uint8_t;

/**
 * @brief Responsible for buffered file reading
 */
class FileReader {
public:
    /**
     * Tors 
     */
    FileReader();
    FileReader(const std::string& file_path);
    FileReader(const char* file_path);

    ~FileReader();

    /**
     * NON-COPYABLE
     */
    FileReader(const FileReader&) = delete;
    FileReader(FileReader&&) = delete;
    FileReader& operator=(const FileReader&) = delete;

    /**
     * File Functions
     */
    void Open(const std::string& file_path);
    void Open(const char* file_path);

    void Close();

    bool End() noexcept;

    uint8_t PeekByte();
    uint8_t ReadByte();

    uint32_t ReadBuffer(uint8_t* buffer, uint32_t buffer_size);
    uint32_t ReadLine(uint8_t* buffer, uint32_t buffer_size, uint8_t end);

    uint32_t Seek(uint32_t offset, SeekStart whence);

private:
    /**
     * Internal Functions 
     */
    uint32_t FillBuffer();

    /**
     * File Items
     */
    bool eof_;
    int32_t fd_;

    /**
     * Buffer Items 
     */
    std::unique_ptr<uint8_t[]> buffer_;
    uint32_t buffer_start_;
    int32_t buffer_end_;
    const uint32_t max_buffer_size_;
};

#endif