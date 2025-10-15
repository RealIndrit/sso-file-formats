#ifndef CCX_H
#define CCX_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef _WIN32
#define CCX_EXPORT __declspec(dllexport)
#else
#define CCX_EXPORT
#endif


/**
 * @enum CCXenum
 * @brief Enumeration for CCX error codes.
 *
 * This enumeration defines various error codes used in CCX file operations.
 *
 * @var CCX_NONE No error.
 * @var CCX_INVALID_CONTEXT Invalid context error.
 * @var CCX_ALLOCATION_FAILURE Allocation failure error.
 * @var CCX_FREE_ALLOCATION_FAILURE Free allocation failure error.
 */
typedef enum {
    CCX_NONE               = 0x0000, ///< No error.
    CCX_INVALID_CONTEXT    = 0x0001, ///< Invalid context error.
    CCX_ALLOCATION_FAILURE = 0x0002, ///< Allocation failure error.
    CCX_FREE_ALLOCATION_FAILURE = 0x0003 ///< Free allocation failure error.
} CCXenum;

/**
 * @typedef CCX_SPACER
 * @brief Spacer for padding/unknown data.
 *
 * This type defines a 3-byte spacer used to align data or represent unknown bytes in the structure.
 */
typedef uint8_t CCX_SPACER[3];

#pragma pack(push, 1)
/**
 * @struct CCXFileHeader
 * @brief Structure representing the header of a CCX file.
 *
 * This structure is used to store the header information for CCX files, which includes unknown data fields and the total length of the data.
 *
 * @var Magic bytes?.
 * @var spacer1 Unknown data (4 bytes).
 * @var dataLength Total length of data (4 bytes).
 */
typedef struct {
    uint8_t magicBytes[4]; ///< Magic bytes? (4 bytes).
    uint8_t unkown1[4];    ///< Unknown data (4 bytes).
    uint32_t dataLength;   ///< Total length of data (4 bytes).
} CCXFileHeader;

/**
 * @struct CCXFileData
 * @brief Structure representing CCX File Data.
 *
 * This structure is used to store information related to CCX files, including metadata, CRC checksums, and file paths.
 *
 * @var fileNameLength Length of the file name (1 byte).
 * @var spacer1 Unknown data (3 bytes) used as padding or for unknown purposes.
 * @var file File name (variable length).
 * @var unknown1 Unknown data (8 bytes).
 * @var CRC1 CRC1 checksum (4 bytes).
 * @var CRC2 CRC2 checksum (4 bytes).
 * @var boolUnkown Unknown boolean data (1 byte).
 * @var spacer2 Unknown data (3 bytes) used as padding or for unknown purposes.
 * @var unknown2 Unknown data (12 bytes).
 * @var sourceFileNumber Source file number (1 byte).
 * @var unknown3 Unknown data (7 bytes).
 * @var filePathLength Length of the file path (1 byte).
 * @var spacer3 Unknown data (3 bytes) used as padding or for unknown purposes.
 * @var filePath File path (variable length).
 */
typedef struct {
    uint8_t fileNameLength;    ///< Length of the file name (1 byte).
    CCX_SPACER spacer1;        ///< Unknown data (3 bytes) used as padding or for unknown purposes.
    char *file;                ///< File name (variable length).
    uint8_t unknown1[8];       ///< Unknown data (8 bytes).
    uint8_t ORIGINAL_CRC[4];   ///< CRC1 checksum (4 bytes).
    uint8_t EXPORTED_CRC[4];   ///< CRC2 checksum (4 bytes).
    uint8_t boolUnkown;        ///< Unknown boolean data (1 byte).
    CCX_SPACER spacer2;        ///< Unknown data (3 bytes) used as padding or for unknown purposes.
    uint8_t unknown2[12];      ///< Unknown data (12 bytes).
    uint8_t sourceFileNumber;  ///< Source file number (1 byte).
    uint8_t unknown3[7];       ///< Unknown data (7 bytes).
    uint8_t filePathLength;    ///< Length of the file path (1 byte).
    CCX_SPACER spacer3;        ///< Unknown data (3 bytes) used as padding or for unknown purposes.
    char *filePath;            ///< File path (variable length).
} CCXFileData;

#pragma pack(pop)

/**
 * @struct CCXFile
 * @brief Structure representing the overall CCX file with header and data.
 *
 * This structure is used to represent the overall CCX file, including the header information and an array of data entries.
 *
 * @var header Header of the CCX file.
 * @var data Array of data entries.
 * @var dataCount Number of data entries.
 */
typedef struct {
    CCXFileHeader header;  ///< Header of the CCX file.
    CCXFileData *data;     ///< Array of data entries.
    uint32_t dataCount;    ///< Number of data entries.
} CCXFile;

/**
 * @struct CCXContext
 * @brief Context structure for managing CCX file operations.
 *
 * This structure is used to manage CCX file operations, including storing the file path and a pointer to the CCX file structure.
 *
 * @var ccxFile Pointer to the CCX file structure.
 * @var filePath Path to the CCX file.
 */
typedef struct {
    CCXFile *ccxFile;   ///< Pointer to the CCX file structure.
    char *filePath;     ///< Path to the CCX file.
} CCXContext;

/**
 * @brief Function to initialize a CCX context with the given file path.
 *
 * This function initializes a CCX context by allocating memory and setting the file path.
 *
 * @param filePath Path to the CCX file.
 * @return A pointer to the initialized CCX context.
 */
CCX_EXPORT CCXContext* CCXInitContext(const char *filePath);

/**
 * @brief Function to read and parse the CCX file into the context.
 *
 * This function reads the CCX file from the given file path and parses its content into the provided context.
 *
 * @param context Pointer to the CCX context.
 */
CCX_EXPORT CCXenum CCXReadFile(CCXContext *context);

/**
 * @brief Searches for files by name within a CCX context.
 *
 * This function traverses the CCX context to find files whose names contain the specified substring.
 * It populates the provided `result` array with the matching CCXFileData entries and returns the
 * number of matches found.
 *
 * @param context Pointer to the CCX context that contains the file data.
 * @param filePath Substring to search for within the file names.
 * @param result Array to store the matching CCXFileData entries.
 * @return The number of matching CCXFileData entries found.
 */
CCX_EXPORT uint32_t CCXFindFilesByName(CCXContext *context, const char *filePath, CCXFileData **result);

/**
 * @brief Function to return an array of all file data entries within the CCX file.
 *
 * This function returns an array of all file data entries present in the CCX file.
 *
 * @param context Pointer to the CCX context.
 * @param an array of pointers to all CCXFileData entries.
 * @return The number of entries of CCXFileData.
 */
CCX_EXPORT uint32_t CCXGetFiles(CCXContext *context, CCXFileData **result);

/**
 * @brief Function to free the allocated memory in the CCX context.
 *
 * This function frees all allocated memory associated with the CCX context, including the CCX file structure and its data entries.
 *
 * @param context Pointer to the CCX context to be freed.
 */
CCX_EXPORT CCXenum CCXFreeContext(CCXContext *context);

#endif // CCX_H
