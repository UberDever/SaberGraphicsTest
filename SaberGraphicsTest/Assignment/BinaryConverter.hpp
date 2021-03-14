#pragma once
/*
	BinaryForm - converts signed integer to its binary (actually string) representation

	Made as class for proper memory management, so string representation can be created and deleted automatically
*/
class BinaryForm
{
	char* binaryAsString;
	int length;
	bool isNeg;

	// Getting log base 2 for allocating exact amount of chars
	int log2(int number)
	{
		int counter = 1;
		while (number >>= 1) counter++;
		return counter;
	}

	void toBinary(int number)
	{
		if (number == 0)
		{
			binaryAsString[0] = '0';
			return;
		}
		for (int i = 1; i <= length && number != 0; i++)
		{
			int remainder = number % 2;
			number /= 2;
			// Writing to binaryAsString in reverse order
			binaryAsString[length - i] = '0' + remainder;
		}
	}
public:
	BinaryForm(int number) : binaryAsString(nullptr), isNeg(false), length(0)
	{
		if (number < 0)
		{
			isNeg = true;
			number = -number;
		}
		// Getting the length of number in binary, allocating string and writing converted number to it
		length = log2(number);
		binaryAsString = new char[length + 1];
		binaryAsString[length] = 0;
		toBinary(number);
	}

	~BinaryForm()
	{
		delete[] binaryAsString;
	}

	bool isNegative()
	{
		return isNeg;
	}

	int getLength()
	{
		return length;
	}

	const char* toString()
	{
		return binaryAsString;
	}

	void print()
	{
		if (isNeg)
			std::cout << "-";
		std::cout << toString() << std::endl;
	}
};