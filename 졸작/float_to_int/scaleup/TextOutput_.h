#include<iostream>
#include<fstream>

using namespace std;

void TextOutput_(int* input, char* oFile_name, int length)
{
	int i;

	ofstream oFile(oFile_name);

	for (i = 0; i<length; i++)
	{
		oFile << input[i] << endl;
	}
}