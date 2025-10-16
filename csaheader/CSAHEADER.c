#include <string.h>
#include "CSAHEADER.h"

CSA_H_Context* CSA_H_InitContext(const char *filePath) {
    CSA_H_Context *context = (CSA_H_Context *)malloc(sizeof(CSA_H_Context));
    context->csaHFile = (CSA_H_File *)malloc(sizeof(CSA_H_File));
    context->csaHFile->data = NULL;
    context->csaHFile->index = NULL;
    context->filePath = strdup(filePath);
    return context;
}

CSA_H_enum CCX_ReadFile(CSA_H_Context *context) {
    if (context == NULL || context->filePath == NULL) {
        return CSA_H_INVALID_CONTEXT;
    }

    FILE *file = fopen(context->filePath, "rb");
    if (file == NULL) {
        return CSA_H_INVALID_CONTEXT;
    }

    // Read the header
    if (fread(&context->csaHFile->header, sizeof(CSA_H_FileHeader), 1, file) != 1) {
        fclose(file);
        return CSA_H_INVALID_CONTEXT;
    }

    // Allocate memory for the data array
    context->csaHFile->dataCount = context->csaHFile->header.dataLength;
    context->csaHFile->data = (CSA_H_FileData *)malloc(context->csaHFile->dataCount * sizeof(CSA_H_FileData));
    if (context->csaHFile->data == NULL) {
        fclose(file);
        return CSA_H_ALLOCATION_FAILURE;
    }

    fclose(file);
    return CSA_H_NONE;
}

uint32_t CSA_H_GetFiles(CSA_H_Context *context, CSA_H_FileData **results) {
    *results = context->csaHFile->data;
    return context->csaHFile->dataCount;
}

CSA_H_enum CCX_FreeContext(CSA_H_Context *context) {
    for (size_t i = 0; i < context->csaHFile->dataCount; ++i) {
//        free(context->csaHFile->data[i].file);
//        free(context->csaHFile->data[i].filePath);
    }
    free(context->csaHFile->data);
    free(context->csaHFile->index);
    free(context->csaHFile);
    free(context->filePath);
    free(context);
    return CSA_H_NONE;
}