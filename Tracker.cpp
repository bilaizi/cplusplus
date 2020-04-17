#include <iostream>
#include <memory>
#include <cstdio>

//Closer (Custom Deleter)
using Closer = void(*)(std::FILE*); //using Closer = std::function<void(std::FILE*)>;

struct Tracker {
    static void close(std::FILE* fp) {
        if(fp) {
            //Track open files. Decrement counter
            NumOpenFiles--;
            std::fclose(fp);
        }
    }
    static auto open(const char* fileName, const char* mode) {
        //Create unique_ptr of FILE*. Tries to open the file.  
        auto fh = std::unique_ptr<std::FILE, Closer>(std::fopen(fileName, mode),  &Tracker::close);
        if(fh) { //If not NULL, the file is open, Track open file. Increment counter
            NumOpenFiles++;
        }  
        return fh;
    }
    static int NumOpenFiles;
};
int Tracker::NumOpenFiles = 0; //Initialize static counter 

int main() {
    std::cout << Tracker::NumOpenFiles << "\n"; //0
    { //Block
        auto fh = Tracker::open("test.txt", "w");
        if(fh) {
            std::fputs("Hello World!", fh.get());
        }   
        std::cout << Tracker::NumOpenFiles << "\n"; //1
    }//End Block. File Closed.
    std::cout << Tracker::NumOpenFiles << "\n"; //0
    return 0;
}
