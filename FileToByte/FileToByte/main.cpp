#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <windows.h>

using namespace std; // using std namespace because dont want to type std::cout every time

// definition of length of vector and attached file name
int length;
string filePath;

string getFile(string filePath)
{
    // definition of hex data, vector and file reader
    std::string hex_data;
    std::vector<unsigned char> bytes;
    std::ifstream infile(filePath, std::ios_base::binary);

    // getting length of file
    infile.seekg(0, infile.end);
    length = infile.tellg();
    infile.seekg(0, infile.beg);
    
    // adding vector's length
    bytes.reserve(length);

    // if reading byte is successful
    if (infile.read(reinterpret_cast<char*>(bytes.data()), length)) {
        std::ostringstream oss; // definition of stringstream
        for (size_t i = 0; i < length; ++i) {
            if (i % 12 == 0) // if i mode 12 is 0
                oss << "\n\t"; // skip to line below
            oss << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned short>(bytes[i]) << ", "; // write it as a hex data
        }
        hex_data = oss.str(); // transferring data to hex_data
    }
    hex_data.pop_back(); hex_data.pop_back();
    //hex_data.substr(0, hex_data.size()-2); // to delete ", " at the end of string
    return hex_data; // return hex data
}

string addHeader(string str)
{
    string fileName = filePath.substr(filePath.find_last_of("/\\") + 1); // remove path
    string::size_type const p(fileName.find_last_of('.'));
    string rawFileName = fileName.substr(0, p); // remove .exe

    if (rawFileName.find(" ") == std::string::npos) // if file name contains " " then use rawData name instead of file name.
    {
        str = "/* FileToByte by lithellx - " + fileName + " */\n\nunsigned char " + rawFileName + "[" + std::to_string(length) + "] = {" + str + "\n}; ";
        //str = "/* FileToByte by lithellx - " + fileName + " */\n\nbyte[] " + rawFileName + " = {" + std::to_string(length) + "] = {\n" + str + "\n};";
    }
    else
    {
        str = "/* FileToByte by lithellx - " + fileName +" */\n\nunsigned char rawData[" + std::to_string(length) + "] = {" + str + "\n}; ";
        //str = "/* FileToByte by lithellx - " + fileName + " */\n\nbyte[] rawData = {" + std::to_string(length) + "] = {\n" + str + "\n};";
    }

    return str; // return str
}

int main(int argc, char* argv[])
{
    system("title FileToByte - lithellx");

    if (argc-1 == 1)
    {
        cout << "Converting...";
        filePath = argv[1];
        std::ofstream out("output.cpp"); // defining and creating output.txt and file writer
        out << addHeader(getFile(argv[1])); // writing file into output.txt
        out.close(); // ending file writer process
        cout << "\nDone!";
        Sleep(1000);
    }
    else
    {
        cout << "Wrong usage. Drag and drop file on FileToByte.exe";
        getchar(); // wait for input
    }

    return 0;
}

// https://github.com/lithellx