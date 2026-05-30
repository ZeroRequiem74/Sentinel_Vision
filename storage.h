#ifndef STORAGE_H
#define STORAGE_H

void Storage_Init(void);

void Storage_CreateFolder(
    const char *path);

void Storage_DeleteFolder(
    const char *path);

void Storage_ClearFolder(
    const char *path);

void Storage_CopyFolder(
    const char *src,
    const char *dst);

#endif