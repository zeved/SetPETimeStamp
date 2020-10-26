# SetPETimeStamp

## Set PE file timestamp field to current time & date

Because of reproducible builds (building from the same source code should create an identical binary etc.; more: https://devblogs.microsoft.com/oldnewthing/20180103-00/?p=97705), binaries created with some compilers don't have the timestamp field (offset) set to the compilation time.
This tool simply reads the PE file header and sets the ```nt_headers->FileHeader.TimeDateStamp``` field to the current timestamp then overwrites the original file. Useful if used in a post-build step.

## TODO:
 
 1. Backup the original file
 2. Allow custom timestamp to be set
 3. Replace other timestamps in the PE header
