#include <string.h>
#include "CCX.h"

CCXContext* CCXInitContext(const char *filePath) {
    CCXContext *context = (CCXContext *)malloc(sizeof(CCXContext));
    context->ccxFile = (CCXFile *)malloc(sizeof(CCXFile));
    context->ccxFile->data = NULL;
    context->filePath = strdup(filePath);
    return context;
}

CCXenum CCXReadFile(CCXContext *context) {
    if (context == NULL || context->filePath == NULL) {
        return CCX_INVALID_CONTEXT;
    }

    FILE *file = fopen(context->filePath, "rb");
    if (file == NULL) {
        return CCX_INVALID_CONTEXT;
    }

    // Read the header
    if (fread(&context->ccxFile->header, sizeof(CCXFileHeader), 1, file) != 1) {
        fclose(file);
        return CCX_INVALID_CONTEXT;
    }

    // Allocate memory for the data array
    context->ccxFile->dataCount = context->ccxFile->header.dataLength;
    context->ccxFile->data = (CCXFileData *)malloc(context->ccxFile->dataCount * sizeof(CCXFileData));
    if (context->ccxFile->data == NULL) {
        fclose(file);
        return CCX_ALLOCATION_FAILURE;
    }

    // Read the file data
    for (uint32_t i = 0; i < context->ccxFile->dataCount; i++) {
        CCXFileData *dataEntry = &context->ccxFile->data[i];

        // Read fixed-size fields
        fread(&dataEntry->fileNameLength, sizeof(dataEntry->fileNameLength), 1, file);
        fread(dataEntry->spacer1, sizeof(dataEntry->spacer1), 1, file);

        // Read variable-length file name
        dataEntry->file = (char *)malloc(dataEntry->fileNameLength + 1);
        fread(dataEntry->file, dataEntry->fileNameLength, 1, file);
        dataEntry->file[dataEntry->fileNameLength] = '\0';

        fread(dataEntry->unknown1, sizeof(dataEntry->unknown1), 1, file);
        fread(dataEntry->ORIGINAL_CRC, sizeof(dataEntry->ORIGINAL_CRC), 1, file);
        fread(dataEntry->EXPORTED_CRC, sizeof(dataEntry->EXPORTED_CRC), 1, file);
        fread(&dataEntry->boolUnkown, sizeof(dataEntry->boolUnkown), 1, file);
        fread(dataEntry->spacer2, sizeof(dataEntry->spacer2), 1, file);
        fread(dataEntry->unknown2, sizeof(dataEntry->unknown2), 1, file);
        fread(&dataEntry->sourceFileNumber, sizeof(dataEntry->sourceFileNumber), 1, file);
        fread(dataEntry->unknown3, sizeof(dataEntry->unknown3), 1, file);
        fread(&dataEntry->filePathLength, sizeof(dataEntry->filePathLength), 1, file);
        fread(dataEntry->spacer3, sizeof(dataEntry->spacer3), 1, file);

        // Read variable-length file path
        dataEntry->filePath = (char *)malloc(dataEntry->filePathLength + 1);
        fread(dataEntry->filePath, dataEntry->filePathLength, 1, file);
        dataEntry->filePath[dataEntry->filePathLength] = '\0';
    }

    fclose(file);
    return CCX_NONE;
}

uint32_t CCXGetFiles(CCXContext *context, CCXFileData **results) {
    *results = context->ccxFile->data;
    return context->ccxFile->dataCount;
}

uint32_t CCXFindFilesByName(CCXContext *context, const char *filePath, CCXFileData **results) {
    CCXFileData *tempResults = NULL;
    uint32_t count = 0;

    for (uint64_t i = 0; i < context->ccxFile->dataCount; ++i) {
        CCXFileData *data = &context->ccxFile->data[i];
        if (strstr(data->filePath, filePath) != NULL) {
            CCXFileData *newResults = realloc(tempResults, (count + 1) * sizeof(CCXFileData));
            if (newResults == NULL) {
                free(tempResults);
                *results = NULL;
                return 0;
            }
            tempResults = newResults;
            tempResults[count] = *data;
            count++;
        }
    }
    *results = tempResults;
    return count;
}


CCXenum CCXFreeContext(CCXContext *context) {
    for (size_t i = 0; i < context->ccxFile->dataCount; ++i) {
        free(context->ccxFile->data[i].file);
        free(context->ccxFile->data[i].filePath);
    }
    free(context->ccxFile->data);
    free(context->ccxFile);
    free(context->filePath);
    free(context);
    return CCX_NONE;
}