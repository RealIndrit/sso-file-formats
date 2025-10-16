#ifndef CSA_HEADER_H
#define CSA_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef _WIN32
#define CSA_HEADER_EXPORT __declspec(dllexport)
#else
#define CSA_HEADER_EXPORT
#endif

/**
 * @enum CSA_H_enum
 * @brief Enumeration for CSA_H error codes.
 *
 * This enumeration defines various error codes used in CSA_H file operations.
 *
 * @var CSA_H_NONE No error.
 * @var CSA_H_INVALID_CONTEXT Invalid context error.
 * @var CSA_H_ALLOCATION_FAILURE Allocation failure error.
 * @var CSA_H_FREE_ALLOCATION_FAILURE Free allocation failure error.
 */
typedef enum {
    CSA_H_NONE               = 0x0000, ///< No error.
    CSA_H_INVALID_CONTEXT    = 0x0001, ///< Invalid context error.
    CSA_H_ALLOCATION_FAILURE = 0x0002, ///< Allocation failure error.
    CSA_H_FREE_ALLOCATION_FAILURE = 0x0003 ///< Free allocation failure error.
} CSA_H_enum;


typedef uint8_t CSA_H_SPACER[3];

#pragma pack(push, 1)
typedef struct {
    uint8_t unkown1[4];
    uint8_t randombytes[116];
    uint8_t unkown2[4];
    uint8_t unkown3[4];
    uint8_t randombytes2[16];
    uint8_t unkown4[4];
    uint8_t unkown5[4];
    uint8_t randombytes3[12];
    uint32_t dataLength;
} CSA_H_FileHeader;

typedef struct {
    uint8_t fileNameLength;
    CSA_H_SPACER spacer1;
    char *file;
} CSA_H_FileNameIndex;

typedef struct {

} CSA_H_FileData;
#pragma pack(pop)


typedef struct {
    CSA_H_FileHeader header;
    CSA_H_FileData *data;
    CSA_H_FileNameIndex *index;
    uint32_t dataCount;
} CSA_H_File;

typedef struct {
    CSA_H_File *csaHFile;
    char *filePath;
} CSA_H_Context;

CSA_HEADER_EXPORT CSA_H_Context* CSA_H_InitContext(const char *filePath);
CSA_HEADER_EXPORT uint32_t CSA_H_GetFiles(CSA_H_Context *context, CSA_H_FileData **result);
CSA_HEADER_EXPORT CSA_H_enum CSA_H_ReadFile(CSA_H_Context *context);
CSA_HEADER_EXPORT CSA_H_enum CCX_FreeContext(CSA_H_Context *context);

#endif // CCA_HEADER_H
