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
    
    /**
     * @brief Construct an empty FileReader
     */
    FileReader();

    /**
     * @brief Construct a FileReader from the given file_path
     * @param file_path const std::string&
     */
    FileReader(const std::string& file_path);

    /**
     * @brief Construct a FileReader from the given file_path
     * @param file_path const char*
     */
    FileReader(const char* file_path);

    /**
     * @brief Close the file and clean up memory
     */
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
    
    /**
     * @brief Open the file found at file_path
     * @param file_path 
     */
    void Open(const std::string& file_path);
    
    /**
     * @brief Open the file found at file_path
     * @param file_path 
     */
    void Open(const char* file_path);

    /**
     * @brief Close the file if open
     */
    void Close();

    /**
     * @brief Check if the end of file has been reached
     * @return 
     */
    bool End() noexcept;

    /**
     * @brief Return the next byte but do not increment the file pointer
     * @return The next byte
     */
    uint8_t PeekByte();

    /**
     * @brief Read the next byte and increment the file pointer
     * @return The byte read
     */
    uint8_t ReadByte();

    /**
     * @brief Read up to buffer_size into the buffer
     * @param buffer Destination buffer of at least size buffer_size
     * @param buffer_size The maximum number of bytes to read
     * @return The number of bytes read
     */
    uint32_t ReadBuffer(uint8_t* buffer, uint32_t buffer_size);

    /**
     * @brief Read up to, and including, the char at end or buffer_size bytes, whichever comes first
     * @param buffer The destination buffer of at least size buffer_size
     * @param buffer_size The maximum number of bytes to read
     * @param end The terminating char to be read
     * @return Number of bytes read into buffer
     */
    uint32_t ReadLine(uint8_t* buffer, uint32_t buffer_size, uint8_t end);

    /**
     * @brief Move the file pointer location in the file
     * @param offset The offset from which to seek to
     * @param whence Where the offset should be calculated from
     * @return The updated file offset
     */
    uint32_t Seek(uint32_t offset, SeekStart whence);

private:
    /**
     * Internal Functions 
     */
    
    /**
     * @brief Fill the buffer with new bytes from the file
     * @return The number of bytes read into the buffer
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