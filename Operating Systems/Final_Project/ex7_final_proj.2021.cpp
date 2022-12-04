// Program Name: Disk File System Simulation [Final Project]
// Program Description: This program is a Simulation of a Disk File System
// which is managed by indexed allocation method, it simulates all the steps
// that the Disk File System applies when an IO operation is required

// Student Name: Muath Abu Jamal


#include <iostream>
#include <vector>
#include <map>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>

using namespace std;

// declaring preprocessor directives
#define DISK_SIZE 256
#define DISK_SIM_FILE "DISK_SIM_FILE.txt"

#define IN_USE_BLOCK 1
#define NOT_IN_USE_BLOCK 0
#define FAILED (-1)
#define SUCCEEDED 1

// declaring the needed functions (explanation for each function bellow(where each function written))
char decToBinary(int n, char &c);

int binaryToDec(char &c);

// ==================================  class FsFile  ==========================================
// This class holds/saves the metadata for each file
// like file size, how many blocks in use by this file (without the index block), index block address, block size
// moreover it includes constructor, setters and getters
class FsFile {

    int file_size;
    int block_in_use;
    int index_block;
    int block_size;


public:

    FsFile(int _block_size) {

        file_size = 0;
        block_in_use = 0;
        block_size = _block_size;
        index_block = -1;
    }

    // ----- setters and getters -----

    int getfile_size() {

        return file_size;
    }

    void setFileSize(int fileSize) {
        file_size = fileSize;
    }

    int getBlockInUse() const {
        return block_in_use;
    }

    void setBlockInUse(int blockInUse) {
        block_in_use = blockInUse;
    }

    int getIndexBlock() const {
        return index_block;
    }

    void setIndexBlock(int indexBlock) {
        index_block = indexBlock;
    }
};

// ==================================  class FileDescriptor  ==========================================
// This class is created for each file it's job is to connect between the FsFile(file metadata) and the file name
// also it includes boolean value (inUse) for each file that saves if the file is open or close
// beside the constructor, setters and getters
class FileDescriptor {

    string file_name;
    FsFile *fs_file;
    bool inUse;

public:

    FileDescriptor(string FileName, FsFile *fsi) {

        file_name = FileName;
        fs_file = fsi;
        inUse = true;
    }

    // ----- setters and getters -----

    string getFileName() {

        return file_name;
    }

    FsFile *getFsFile() {

        return fs_file;
    }

    bool getInUse() const {

        return inUse;
    }

    void setInUse(bool _inUse) {

        inUse = _inUse;
    }

};

// ==================================  class fsDisk  ==========================================
// This class is the simulation of the Disk Management System
// it simulates the disk and all the needed operations for maneging and editing the files
// by indexed allocation method
// it includes constructor, public functions (as asked in the exercise) and private functions
// to for the purpose of modularity
class fsDisk {


    // declaring the needed private data structures, variables and functions
private:

    FILE *sim_disk_fd;
    bool is_formated;

    int BitVectorSize;
    int *BitVector;

    int block_size;
    int maxSize;
    int freeBlocks;

    // declaring a vector for the Main Directory which holds all the pointers for the file descriptors
    // of the files that are exists on the disk
    vector<FileDescriptor *> MainDir;

    // declaring a map for holding the file descriptor number and the file descriptor pointer for each open file
    // I used map because it can hold two values (key [FDNum], value [FDPointer]) and it has sum functions that
    // supplies for me what I need, and it makes the job more comfortable
    map<int, FileDescriptor *> OpenFileDescriptors;


    // This private function receives one argument: 1- file name
    // the function used for finding the index of a file in the Main Directory by it's name
    // it uses iterator to navigate through the vector and the function distance to calculate
    // how far is the item index form the beginning of the vector
    int findFileInMainDir(string fileName) {
        for (auto item = MainDir.begin(); item != MainDir.end(); ++item) {
            if ((*item)->getFileName() == fileName) {

                return (int) distance(MainDir.begin(), item);
            }
        }

        return FAILED;
    }

    // ------------------------------------------------------------------------

    // This private function receives one argument: 1- file name
    // the function used for finding the iterator of the open file item <key, value> in the open file descriptor map by the given name (file name)
    // it uses iterator to navigate through the map
    // in the case of failure it returns OpenFileDescriptor.end()
    auto findFileInOpenFD(string fileName) {
        for (auto item = OpenFileDescriptors.begin(); item != OpenFileDescriptors.end(); ++item) {
            if ((item->second)->getFileName() == fileName) {

                return item;
            }
        }

        return OpenFileDescriptors.end();
    }

    // ------------------------------------------------------------------------

    // This private function receives one argument: 1- fileIndex (In Main Directory)
    // the function used for freeing/deleting all the manually allocated memories of the file
    // and at the end it removes that file from the Main Directory vector
    // it uses iterator to navigate through the vector
    void eraseFile(int fileIndex) {
        delete MainDir[fileIndex]->getFsFile();
        delete MainDir[fileIndex];

        int k = 0;
        vector<FileDescriptor *>::iterator it;

        for (it = MainDir.begin(); k < fileIndex; k++, ++it);

        MainDir.erase(it);

    }

    // ------------------------------------------------------------------------

    // This private function receives one argument: 1- block index
    // The function resets the block in the DISK_SIM_FILE to '\0's
    // which means that the block now is free and others can use it
    // the calling function responsible for making the BitVector[blockIndex] = NOT_IN_USE_BLOCK
    int resetBlock(int blockIndex) {

        int j;
        size_t bytesNum = 1;
        size_t itemsNum = 1;

        if (fseek(sim_disk_fd, blockIndex * block_size, SEEK_SET) != 0) {
            return FAILED;
        }

        for (j = 0; j < block_size; j++) {

            if (fwrite("\0", bytesNum, itemsNum, sim_disk_fd) < itemsNum) {
                return FAILED;
            }

        }

        fflush(sim_disk_fd);

        return SUCCEEDED;
    }

    // ------------------------------------------------------------------------

    // This private function receives no arguments
    // the function returns the first empty block Index
    // by looping through the BitVector
    int getEmptyBlock() {

        if (freeBlocks <= 0) {
            return FAILED;
        }

        int i;

        for (i = 0; i < BitVectorSize; i++) {
            if (BitVector[i] == NOT_IN_USE_BLOCK) {
                return i;
            }
        }

        return FAILED;
    }

    // ------------------------------------------------------------------------

    // This private function receives no arguments
    // the function returns the minimum possible file descriptor number
    // according to the OpenFileDescriptor Map keys
    int getFileDescriptorNumber() {

        int FDNum = 0;

        for (auto &OpenFileDescriptor : OpenFileDescriptors) {
            if ((OpenFileDescriptor.first) > FDNum) {
                return FDNum;
            }

            FDNum++;
        }

        return FDNum;
    }

public:

    // This constructor opens the DISK_SIM_FILE,
    // and it makes all the needed initializations
    fsDisk() {

        sim_disk_fd = fopen(DISK_SIM_FILE, "r+");

        assert(sim_disk_fd);

        for (int i = 0; i < DISK_SIZE; i++) {

            int ret_val = fseek(sim_disk_fd, i, SEEK_SET);

            ret_val = fwrite("\0", 1, 1, sim_disk_fd);

            assert(ret_val == 1);

        }

        fflush(sim_disk_fd);

        block_size = 0;

        is_formated = false;

    }

    // ------------------------------------------------------------------------

    // This destructor frees/deletes all the manually allocated memories
    ~fsDisk() {

        if (is_formated) {
            int i;

            int MainDirSize = (int) MainDir.size();

            for (i = 0; i < MainDirSize; i++) {
                eraseFile(0);
            }

            OpenFileDescriptors.clear();

            delete[] BitVector;
        }

        if (sim_disk_fd != nullptr) {
            fclose(sim_disk_fd);
        }

    }

    // ------------------------------------------------------------------------

    // This function receives one argument: 1- block size
    // the function formats the disk and makes all the needed calculations and initializations
    void fsFormat(int blockSize) {

        block_size = blockSize;

        maxSize = blockSize * blockSize;

        BitVectorSize = DISK_SIZE / block_size;

        // if the disk is formated that means that there is a previous BitVector
        // that may be with a different size, so I delete the previous one
        if (is_formated) {

            delete[] BitVector;
        }

        BitVector = new int[BitVectorSize];

        freeBlocks = BitVectorSize;

        int i;

        for (i = 0; i < BitVectorSize; i++) {

            BitVector[i] = NOT_IN_USE_BLOCK;
            resetBlock(i);
        }

        int MainDirSize = (int) MainDir.size();

        for (i = 0; i < MainDirSize; i++) {
            eraseFile(0);
        }

        OpenFileDescriptors.clear();

        is_formated = true;

    }

    // ------------------------------------------------------------------------


    // This function receives one argument: 1- file name
    // the function creates a file if the disk is formated and the file not exists
    // it makes a FsFile(metadata holder) and FileDescriptor for the file, and
    // it saves that file descriptor pointer in the Main Directory vector and OpenFileDescriptor map
    // the number of the open FD is supported by the function getFileDescriptorNumber() which returns the minimum
    // possible number in the map <key>, at the end this function returns the FDNum
    int CreateFile(string fileName) {

        if (!is_formated) {
            return FAILED;
        }

        if (findFileInMainDir(fileName) != FAILED) {
            return FAILED;
        }

        auto *newFsFile = new FsFile(block_size);

        auto *newFileDescriptor = new FileDescriptor(fileName, newFsFile);

        MainDir.push_back(newFileDescriptor);

        int FDNum = getFileDescriptorNumber();

        OpenFileDescriptors.insert(pair<int, FileDescriptor *>(FDNum, newFileDescriptor));

        newFileDescriptor->setInUse(true);

        return FDNum;
    }

    // -------------------------------------------------------------------------

    // This function receives one argument: 1- file name
    // the function opens the file by changing the bool value of InUse of the file to true
    // and adding the file to the OpenFileDescriptor
    // before that it checks if the file exists or if the file is already open
    int OpenFile(string fileName) {

        int fileMainDirIndex = findFileInMainDir(fileName);

        if (fileMainDirIndex == FAILED) {
            return FAILED;
        }

        auto fileOpenFDIterator = findFileInOpenFD(fileName);

        if (fileOpenFDIterator != OpenFileDescriptors.end()) {
            return FAILED;
        }

        FileDescriptor *currentFD = MainDir[fileMainDirIndex];

        int FDNum = getFileDescriptorNumber();

        OpenFileDescriptors.insert(pair<int, FileDescriptor *>(FDNum, currentFD));

        currentFD->setInUse(true);

        return FDNum;

    }

    // ------------------------------------------------------------------------

    // This function receives one argument: 1- File Descriptor Number
    // The function closes the file with the file Descriptor number after checking if it is open
    // closing means to change the bool value InUse to false and removing the item of the file from the OpenFileDescriptor map
    // at the end it returns the nme of the file that have been close in case of failure it returns "-1"
    string CloseFile(int fd) {

        auto iterator = OpenFileDescriptors.find(fd);

        if (iterator == OpenFileDescriptors.end()) {
            return "-1";
        }

        iterator->second->setInUse(false);

        OpenFileDescriptors.erase(iterator);

        return "1";

    }

    // ------------------------------------------------------------------------

    // This function receives three arguments : 1- file descriptor number, 2- a buffer that holds the string to be written, 3- the length of that string (in the buffer)
    // the function makes the needed checks like if the file is open, if there is enough space in the file and the disk .....
    // then the function seeks in the disk file and starts writing and allocating the needed blocks by the help of the private and non-private functions
    // at the end it returns the number of chars that have been written in the file
    int WriteToFile(int fd, char *buf, int len) {

        if (!is_formated) {
            return FAILED;
        }

        auto iterator = OpenFileDescriptors.find(fd);

        if (iterator == OpenFileDescriptors.end()) {
            return FAILED;
        }

        if (len <= 0) {
            return FAILED;
        }

        FileDescriptor *currentFD = (iterator->second);

        if (currentFD->getFsFile()->getIndexBlock() == -1) {

            int emptyBlockIndex = getEmptyBlock();

            if (emptyBlockIndex == FAILED) {
                return FAILED;
            }

            currentFD->getFsFile()->setIndexBlock(emptyBlockIndex);

            BitVector[emptyBlockIndex] = IN_USE_BLOCK;

            freeBlocks--;
        }

        int emptyFileCharsNum = maxSize - currentFD->getFsFile()->getfile_size();

        if (emptyFileCharsNum < len) {
            len = emptyFileCharsNum;
        }

        int neededBlocks = ceil(((float) (len - (emptyFileCharsNum % block_size))) / ((float) block_size));

        if ((freeBlocks - neededBlocks) < 0) {
            return FAILED;
        }

        int i;
        size_t bytesNum = 1;
        size_t itemsNum = 1;

        int writingIndex;
        int indexBlock = currentFD->getFsFile()->getIndexBlock();
        int indexBlockPointer;
        int innerOffset;
        int emptyBlockIndex;
        char tempIndexChar;

        // Checking if there is space in the blocks of the file (last block of the file in our case)
        if (currentFD->getFsFile()->getfile_size() % block_size != 0) {

            indexBlockPointer = (indexBlock * block_size) + currentFD->getFsFile()->getBlockInUse() - 1;

            if (fseek(sim_disk_fd, indexBlockPointer, SEEK_SET) != 0) {
                return FAILED;
            }

            if (fread(&tempIndexChar, bytesNum, itemsNum, sim_disk_fd) == 0) {
                if (ferror(sim_disk_fd) != 0) {
                    return FAILED;
                }
            }

            innerOffset = currentFD->getFsFile()->getfile_size() % block_size;

            writingIndex = (binaryToDec(tempIndexChar) * block_size) + innerOffset;

            if (fseek(sim_disk_fd, writingIndex, SEEK_SET) != 0) {
                return FAILED;
            }
        }

        for (i = 0; i < len; i++) {
            // checking if there is a need for new block if true then allocate new one and save it's index in the indexes block
            if (currentFD->getFsFile()->getfile_size() % block_size == 0) {

                emptyBlockIndex = getEmptyBlock();

                if (emptyBlockIndex == FAILED) {
                    return FAILED;
                }

                decToBinary(emptyBlockIndex, tempIndexChar);

                indexBlockPointer = (indexBlock * block_size) + currentFD->getFsFile()->getBlockInUse();

                if (fseek(sim_disk_fd, indexBlockPointer, SEEK_SET) != 0) {
                    return FAILED;
                }

                if (fwrite(&tempIndexChar, bytesNum, itemsNum, sim_disk_fd) < itemsNum) {
                    return FAILED;
                }

                BitVector[emptyBlockIndex] = IN_USE_BLOCK;

                currentFD->getFsFile()->setBlockInUse(currentFD->getFsFile()->getBlockInUse() + 1);

                writingIndex = emptyBlockIndex * block_size;

                if (fseek(sim_disk_fd, writingIndex, SEEK_SET) != 0) {
                    return FAILED;
                }
            }

            if (fwrite(buf + i, bytesNum, itemsNum, sim_disk_fd) < itemsNum) {
                return FAILED;
            }

            currentFD->getFsFile()->setFileSize(currentFD->getFsFile()->getfile_size() + 1);

        }

        freeBlocks = freeBlocks - neededBlocks;

        return len;

    }

    // ------------------------------------------------------------------------

    // This function receives three arguments: 1- the file descriptor number, 2- the buffer to write on what we read, 3- the length of characters that we want to read
    // the function makes the needed checks like if the file is open, if the disk is formatted ....
    // then the function starts seeking and reading from the file according to the indexes of it's data blocks that are saved in the index block
    // at the end it returns the length of the characters that have been read
    // if the asked len is bigger than the file size it returns all the content of the file
    int ReadFromFile(int fd, char *buf, int len) {

        if (!is_formated) {
            return FAILED;
        }

        auto iterator = OpenFileDescriptors.find(fd);

        if (iterator == OpenFileDescriptors.end()) {
            strcpy(buf, ((string) ("-1")).c_str());
            return FAILED;
        }

        FileDescriptor *currentFD = iterator->second;

        if (currentFD->getFsFile()->getfile_size() < len) {
            len = currentFD->getFsFile()->getfile_size();
        }

        int i;
        size_t bytesNum = 1;
        size_t itemsNum = 1;

        int readingIndex;
        int indexBlockPointer;
        int indexBlockOffset = 0;
        char tempIndexChar;
        string tempStr;

        for (i = 0; i < len; i++) {

            if (i % block_size == 0) {
                indexBlockPointer = (currentFD->getFsFile()->getIndexBlock() * block_size) + indexBlockOffset;

                if (fseek(sim_disk_fd, indexBlockPointer, SEEK_SET) != 0) {
                    return FAILED;
                }

                if (fread(&tempIndexChar, bytesNum, itemsNum, sim_disk_fd) == 0) {
                    if (ferror(sim_disk_fd) != 0) {
                        return FAILED;
                    }
                }

                readingIndex = binaryToDec(tempIndexChar) * block_size;

                if (fseek(sim_disk_fd, readingIndex, SEEK_SET) != 0) {
                    return FAILED;
                }

                indexBlockOffset++;
            }


            if (fread((buf + i), bytesNum, itemsNum, sim_disk_fd) == 0) {
                if (ferror(sim_disk_fd) != 0) {
                    return FAILED;
                }
            }

        }

        if (i < DISK_SIZE) {
            buf[i] = '\0';
        } else {
            buf[DISK_SIZE - 1] = '\0';
        }

        return len;

    }

    // ------------------------------------------------------------------------

    // This function receives one argument: 1- file name
    // the function deletes the file after making the needed checks like if the disk is formated, if the file exists ...
    // also it cares about freeing/deleting all the manually allocating memories by the help of the private functions
    // resetBlock(), eraseFile(), ...
    int DelFile(string FileName) {

        if (!is_formated) {
            return FAILED;
        }

        auto openFDIterator = findFileInOpenFD(FileName);

        if (openFDIterator != OpenFileDescriptors.end()) {
            return FAILED;
        }

        int mainDirFileIndex = findFileInMainDir(FileName);

        if (mainDirFileIndex == FAILED) {
            return FAILED;
        }

        size_t bytesNum = 1;
        size_t itemsNum = 1;
        char tempIndexChar;

        int indexBlockPointer;

        int dataBlockIndex;

        int indexBlock = MainDir[mainDirFileIndex]->getFsFile()->getIndexBlock();
        int indexBlockOffset;

        int blocksInUse = MainDir[mainDirFileIndex]->getFsFile()->getBlockInUse();

        for (indexBlockOffset = 0; indexBlockOffset < blocksInUse; indexBlockOffset++) {

            indexBlockPointer = (indexBlock * block_size) + indexBlockOffset;

            if (fseek(sim_disk_fd, indexBlockPointer, SEEK_SET) != 0) {
                return FAILED;
            }

            if (fread(&tempIndexChar, bytesNum, itemsNum, sim_disk_fd) == 0) {
                if (ferror(sim_disk_fd) != 0) {
                    return FAILED;
                }
            }

            dataBlockIndex = binaryToDec(tempIndexChar);

            resetBlock(dataBlockIndex);

            BitVector[binaryToDec(tempIndexChar)] = NOT_IN_USE_BLOCK;

        }

        if (indexBlock != -1) {

            resetBlock(indexBlock);

            BitVector[indexBlock] = NOT_IN_USE_BLOCK;

            freeBlocks = freeBlocks + blocksInUse + 1; // ( 1 ) for the index block

        }

        eraseFile(mainDirFileIndex);

        return SUCCEEDED;

    }

    // ------------------------------------------------------------------------

    // This function receives no arguments
    // the function prints [ indexes, names, isInUse ] for all the file that are exists in the Main Directory
    // also it prints all the Disk Content including the indexes blocks binary values
    void listAll() {

        int i = 0;

        for (auto &fileDescriptor : MainDir) {

            cout << "index: " << i << ": FileName: " << fileDescriptor->getFileName() << " , isInUse: "
                 << fileDescriptor->getInUse() << endl;

            i++;

        }

        char bufy;
        int ret_val;

        cout << "Disk content: '";

        for (i = 0; i < DISK_SIZE; i++) {

            ret_val = fseek(sim_disk_fd, i, SEEK_SET);

            if (ret_val != 0) {
                return;
            }

            ret_val = fread(&bufy, 1, 1, sim_disk_fd);

            if (ret_val == 0) {
                if (ferror(sim_disk_fd) != 0) {
                    return;
                }
            }

            cout << bufy;

        }

        cout << "'" << endl;

    }

};

// ==================================  Main  ==========================================

int main() {
    int blockSize;
    string fileName;
    char str_to_write[DISK_SIZE];
    char str_to_read[DISK_SIZE];
    int size_to_read;
    int _fd;
    int writed;

    fsDisk *fs = new fsDisk();
    int cmd_;
    while (1) {
        cin >> cmd_;
        switch (cmd_) {
            case 0:   // exit
                delete fs;
                exit(0);
                break;

            case 1:  // list-file
                fs->listAll();
                break;

            case 2:    // format
                cin >> blockSize;
                fs->fsFormat(blockSize);
                break;

            case 3:    // creat-file
                cin >> fileName;
                _fd = fs->CreateFile(fileName);
                cout << "CreateFile: " << fileName << " with File Descriptor #: " << _fd << endl;
                break;

            case 4:  // open-file
                cin >> fileName;
                _fd = fs->OpenFile(fileName);
                cout << "OpenFile: " << fileName << " with File Descriptor #: " << _fd << endl;
                break;

            case 5:  // close-file
                cin >> _fd;
                fileName = fs->CloseFile(_fd);
                cout << "CloseFile: " << fileName << " with File Descriptor #: " << _fd << endl;
                break;

            case 6:   // write-file
                cin >> _fd;
                cin >> str_to_write;
                writed = fs->WriteToFile(_fd, str_to_write, strlen(str_to_write));
                cout << "Writed: " << writed << " Char's into File Descriptor #: " << _fd << endl;
                break;

            case 7:    // read-file
                cin >> _fd;
                cin >> size_to_read;
                fs->ReadFromFile(_fd, str_to_read, size_to_read);
                cout << "ReadFromFile: " << str_to_read << endl;
                break;

            case 8:   // delete file
                cin >> fileName;
                _fd = fs->DelFile(fileName);
                cout << "DeletedFile: " << fileName << " with File Descriptor #: " << _fd << endl;
                break;
            default:
                break;
        }
    }

}

// This function receives two arguments: 1- the number which need to be converted to binary, 2- a pointer to char to store the binary result
// the function resets the char by making & with zero then it converts the number to binary using the help of bitwise operations
char decToBinary(int n, char &c) {

    c = c & 0;

    // array to store binary number
    int binaryNum[8];

    // counter for binary array
    int i = 0;
    while (n > 0) {
        // storing remainder in binary array
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }

    // printing binary array in reverse order
    for (int j = i - 1; j >= 0; j--) {
        if (binaryNum[j] == 1)
            c = c | 1u << j;
    }

    return c;
}

// This function receives one argument: 1- a pointer to the char that holds the binary value that need to convert it to decimal(int) value
// the function converts the binary to int using the help of bitwise operations
int binaryToDec(char &c) {

    int result = 0;

    int i;
    for (i = 0; i < 8; i++) {

        result += (1u & (c >> i)) * pow(2, i);
    }

    return result;

}