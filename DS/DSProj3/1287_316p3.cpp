// Garrett McDonnell
// ID: 3021287

#include <iostream>
#include <cstdlib>
#include <ctime>

using std::cout;
using std::endl;

int primeFinder(const int hash);

int main()
{
	const int hashSize=10001;
	const int highestPrime=primeFinder(hashSize); // A slower, but bruteforce way of finding the highest prime number within the range from 1 to hashSize, for use later in double hashing
	int hash[hashSize];
	srand(time(0));
	int sequence=1000;
	int collisions=0;
	cout << "---Linear Hashing---" << endl;
	while (sequence<hashSize)
	{
		for (int i=0; i<hashSize; ++i)
		{
			hash[i]=-1;
		}
		int randInt;
		collisions=0;
		for (int i=0; i<sequence; ++i)
		{
			randInt=rand();
			int hashFunc=randInt%hashSize;
			int j=0;
			while (j<hashSize)
			{
				if (j==hashFunc)
				{
					if (hash[j]==-1)
					{
						hash[j]=randInt;
						j=hashSize;
					}
					else
					{
						while (hash[j]!=-1)
						{
							++collisions;
							++j;
							if (j>=hashSize)
								j=0;
						}
						hash[j]=randInt;
						j=hashSize;
					} 
				}
				++j;
			}
		}
		cout << "Size: " << sequence << ", Collisions: " << collisions << endl;
		sequence=sequence+1000;
	}
	
	
	sequence=1000;
	collisions=0;
	cout << endl << "---Quadratic Hashing---" << endl;
	while (sequence<hashSize)
	{
		for (int i=0; i<hashSize; ++i)
		{
			hash[i]=-1;
		}
		int randInt;
		collisions=0;
		int failure=0;
		for (int i=0; i<sequence; ++i)
		{
			randInt=rand();
			int hashFunc=randInt%hashSize;
			int j=0;
			while (j<hashSize)
			{
				if (j==hashFunc)
				{
					if (hash[j]==-1)
					{
						hash[j]=randInt;
						j=hashSize;
					}
					else
					{
						int inc=1;
						while (hash[j]!=-1)
						{
							++collisions;
							j=(hashFunc+inc*inc)%hashSize;
							++inc;
							if (inc>hashSize)
								{
									++failure;
									break;
								}
						}
						if (j<hashSize)
							hash[j]=randInt;
						j=hashSize;
					} 
				}
				++j;
			}
		}
		cout << "Size: " << sequence << ", Collisions: " << collisions << ", # of Failures: " << failure << endl;
		sequence=sequence+1000;
	}
	
	
	sequence=1000;
	collisions=0;
	cout << endl << "---Double Hashing---" << endl;
	while (sequence<hashSize)
	{
		for (int i=0; i<hashSize; ++i)
		{
			hash[i]=-1;
		}
		int randInt;
		collisions=0;
		int failure=0;
		for (int i=0; i<sequence; ++i)
		{
			randInt=rand();
			int hashFunc=randInt%hashSize;
			int j=0;
			while (j<hashSize)
			{
				if (j==hashFunc)
				{
					if (hash[j]==-1)
					{
						hash[j]=randInt;
						j=hashSize;
					}
					else
					{
						int inc=1;
						while (hash[j]!=-1)
						{
							++collisions;
							j=hashFunc+inc*(highestPrime-(randInt%highestPrime));
							++inc;
							if (j>hashSize-1)
								j=j-hashSize;  // To induce wrap-around of table if needed
							if (j>=hashSize || inc>hashSize)
								{ // After adjusting for wrap around, if j is still bigger than the hash table size, that means failure has undoubtably occured
									++failure;
									break;
								}
						}
						if (j<hashSize)
							hash[j]=randInt;
						j=hashSize;
					} 
				}
				++j;
			}
		}
		cout << "Size: " << sequence << ", Collisions: " << collisions << ", # of Failures: " << failure << endl;
		sequence=sequence+1000;
	}
	return 0;
}

int primeFinder(const int hash)
{
	int highestPrime;
	int x=2;
	while (x<hash)
	{
		int y=2,count=0;
		while (y<x)
		{
			if (x%y==0 && y!=x)
				++count;
			++y;
		}
		if (count==0)
			highestPrime=x;
		++x;
	}
	return highestPrime;
}
