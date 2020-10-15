/*==================================================================================================
PROGRAMMERS:			  Daniel Moore
 COURSE:				  CSC 525/625
 ASSIGNMENT:			  Project 1
 LAST MODIFIED DATE:	  10/14/2020
 DESCRIPTION:			  A header file that processes BMP image files
 NOTE:					  
 FILES:					  bmp.h
 IDE/COMPILER:			  MicroSoft Visual Studio 2019
 INSTRUCTION FOR COMPILATION AND EXECUTION:
		1.		Double click on labProj.sln	to OPEN the project
		2.		Press Ctrl+F7 to COMPILE
		3.		Press Ctrl+F5 to EXECUTE
==================================================================================================*/

#pragma once
#include <iostream>
#include <fstream>
#include <vector>

// Loads a BMP image file and returns a vector storing color values for a pixel map.
// w: Pointer to int that is overwritten with the width of the pixel map.
// h: Pointer to int that is overwritten with the height of the pixel map.
// Note: The BMP file MUST be in 24-bit color with no compression.
//      The returned pixel map is in reverse order, BGR, instead of RGB.
//      Returns an empty vector (size 0) if the BMP image could not be loaded.
std::vector<unsigned char> bmp(std::string filepath, int* w, int* h) {
	std::vector<unsigned char> bmp_vec(0); //Initialize empty vector
	std::ifstream bmp_file(filepath, std::ios::binary);

	if (!bmp_file.is_open()) {
		return bmp_vec; // Failed to open the file, return empty vector
	}

	//###############
	// BMP File Parsing
	// https://en.wikipedia.org/wiki/BMP_file_format#File_structure
	//###############

	int bmp_start = 0;
	bmp_file.seekg(10); // BMP Header at 0x0A -- Start offset of pixel data
	bmp_file.read((char*)&bmp_start, sizeof(int));

	int width = 0, height = 0;
	bmp_file.seekg(18); // DIB Header at 0x12 -- BMP image width
	bmp_file.read((char*)&width, sizeof(short));
	
	bmp_file.seekg(22); // DIB Header at 0x14 -- BMP image height
	bmp_file.read((char*)&height, sizeof(short));
	

	bmp_vec.resize(width * height * 3);	// Allocate space for image binary, assuming 24 bits (3 bytes) per pixel
	bmp_file.seekg(bmp_start); // Seek to the start of the pixel data
	unsigned char r = 0, g = 0, b = 0;
	
	// Reads full chunk of pixel data into vector
	bmp_file.read((char*)bmp_vec.data(), sizeof(unsigned char) * 3 * width * height);

	//for (int i = 0; i < width * height * 3; i += 3) {
	//	bmp_file.read((char*)&b, sizeof(unsigned char));
	//	bmp_file.read((char*)&g, sizeof(unsigned char));
	//	bmp_file.read((char*)&r, sizeof(unsigned char));
	//	bmp_vec[i] = b;
	//	bmp_vec[i + 1] = g;
	//	bmp_vec[i + 2] = r;
	//}

	bmp_file.close(); // Close bmp file

	*w = width;
	*h = height;

	return bmp_vec;
}