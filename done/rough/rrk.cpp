//rough round key

#include <iostream>

int main()
{
	using namespace std;
	
	unsigned char array_1[4] = {0x04, 0x66, 0x81, 0xe5};
	unsigned char array_2[4] = {0xa0, 0xfa, 0xfe, 0x17};
	unsigned char array_3[4];
	
	for(int i = 0; i < 4; i++)
	{
		array_3[i] = (array_1[i] ^ array_2[i]);
	}
	
	for(int i = 0; i < 4; i++)
	{
		cout << hex << static_cast<int>(array_3[i]) << "\t";
	}
	
	return 0;
}