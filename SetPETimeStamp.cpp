#include <Windows.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[])
{
  if (argc == 1) {
    printf("usage: %s file.exe\n", argv[0]);
    return 0;
  }

  char path[255] = { 0 };
  memcpy_s(&path, 255, argv[1], 255);
  HANDLE file_handle = nullptr;
  DWORD file_size = 0, bytes_read = 0;
  LPVOID file_data = nullptr;
  PIMAGE_DOS_HEADER dos_headers = {};
  PIMAGE_NT_HEADERS nt_headers = {};
  BOOL result;
  DWORD file_pointer = 0;

  file_handle = CreateFileA(
    path,
    GENERIC_READ | GENERIC_WRITE,
    FILE_SHARE_READ,
    nullptr,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    nullptr
  );

  if (!file_handle || file_handle == INVALID_HANDLE_VALUE) {
    printf("[error]: failed opening %s\n", argv[1]);
    return -1;
  }

  file_size = GetFileSize(file_handle, nullptr);
  file_data = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, file_size);

  if (!file_data) {
    printf("[error]: failed allocating %d bytes of memory\n", file_size);
    goto cleanup;
  }

  result = ReadFile(file_handle, file_data, file_size, &bytes_read, nullptr);

  if (!result) {
    printf("[error]: failed reading file\n");
    goto cleanup;
  }

  dos_headers = (PIMAGE_DOS_HEADER)file_data;
  nt_headers = (PIMAGE_NT_HEADERS)((DWORD)file_data + dos_headers->e_lfanew);
  printf("file timestamp: %x\n", nt_headers->FileHeader.TimeDateStamp);

  time_t t;
  time(&t);
  struct tm time_info;
  gmtime_s(&time_info, &t);
  t = mktime(&time_info);
  printf("current timestamp: %x\n", t);

  nt_headers->FileHeader.TimeDateStamp = t;
  file_pointer = SetFilePointer(file_handle, NULL, NULL, FILE_BEGIN);

  if (file_pointer == INVALID_SET_FILE_POINTER) {
    printf("[error]: failed setting file pointer\n");
    goto cleanup;
  }

  result = WriteFile(file_handle, file_data, file_size, &bytes_read, 0);
  if (result) {
    printf("done!\n");
  }
  else {
    printf("[error]: failed writing to file\n");
  }

  cleanup:
  HeapFree(GetProcessHeap(), 0, file_data);
  CloseHandle(file_handle);
}