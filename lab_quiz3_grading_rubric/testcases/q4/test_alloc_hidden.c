#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include "alloc.h"

int main()
{
	//malloc to get page
	if(init_alloc())
		return 1;

	char* first = NULL;
	char *str = alloc(1024);
	first = str;
	printf("offset: %lu\n", str - first);
	char *str2 = alloc(1024);
	printf("offset: %lu\n", str2 - first);

	if(str == NULL || str2 == NULL)
	{
		printf("alloc failed\n");
		return 1;
	}

	strcpy(str, "Hello, world!");
	if(strcmp("Hello, world!", str))
	  printf("Hello, world! test failed\n");
	
	else
	  printf("Hello, world! test passed\n");
	
	dealloc(str);
	dealloc(str2);

	printf("Elementary tests passed\n");
	
	printf("Starting comprehensive tests (see details in code)\n");

	/*
	Comprehensive tests:
	1. Populate entire thing in blocks of for loop (a's, b's, c's, and d's) equal size.
	2. Dealloc c's, reallocate and replace with x's.
	3. 
	*/

	/*** test 1 ****/
	
	//Generating ideal strings for comparison
	char stringA[1024], stringB[1024], stringC[1024], stringD[1024], stringX[1024];
	for(int i = 0; i < 1023; i++)
	{
		stringA[i] = 'a';
		stringB[i] = 'b';
		stringC[i] = 'c';
		stringD[i] = 'd';
		stringX[i] = 'x';
	}

	stringA[1023] = stringB[1023] = stringC[1023] = stringD[1023] = stringX[1023] = '\0';

	char *strA = alloc(1024);
	printf("offset: %lu\n", strA - first);
	char *strB = alloc(1024);
	printf("offset: %lu\n", strB - first);
	char *strC = alloc(1024);
	printf("offset: %lu\n", strC - first);
	char *strD = alloc(1024);
	printf("offset: %lu\n", strD - first);

	for(int i = 0; i < 1023; i++)
	{
		strA[i] = 'a';
		strB[i] = 'b';
		strC[i] = 'c';
		strD[i] = 'd';
	}
	strA[1023] = strB[1023] = strC[1023] = strD[1023] = '\0';

	if(strcmp(stringA, strA) == 0 && strcmp(stringB, strB) == 0 && strcmp(stringC, strC) == 0 && strcmp(stringD, strD) == 0)
	  printf("Test 1 passed: allocated 4 chunks of 1KB each\n");
	else
	  printf("Test 1 failed: A: %d, B: %d, C: %d, D: %d\n", strcmp(stringA, strA), strcmp(stringB, strB), strcmp(stringC, strC), strcmp(stringD, strD));

	/**** test 2 ****/
	
	dealloc(strC);

	char *strX = alloc(1024);
	printf("offset: %lu\n", strX - first);

	for(int i = 0; i < 1023; i++)
	{
		strX[i] = 'x';
	}
	strX[1023] = '\0';

	if(strcmp(stringX, strX) == 0)
	  printf("Test 2 passed: dealloc and realloc worked\n");
	else
	  printf("Test 2 failed: X: %d\n", strcmp(stringX, strX));

	/*** test 3 ***/
	
	char stringYZ[2048];
	for(int i = 0; i < 1024; i++)
	{
		stringYZ[i] = 'y';
	}

	for(int i = 1024; i < 2047; i++)
	{
		stringYZ[i] = 'z';
	}

	stringYZ[2047] = '\0';

	dealloc(strA);
	dealloc(strB);

	char *strYZ = alloc(2048);
	printf("offset: %lu\n", strYZ - first);
		
	for(int i = 0; i < 1024; i++)
	{
		strYZ[i] = 'y';
	}
	for(int i = 1024; i < 2047; i++)
	{
		strYZ[i] = 'z';
	}
	strYZ[2047] = '\0';

	if(strcmp(stringYZ, strYZ) == 0)
		printf("Test 3a passed: Buddies United\n");
	else{
		printf("Test 3a failed: YZ: %d\n", strcmp(stringYZ, strYZ));
	
	}
	// test 3b

	dealloc(strC);
	dealloc(strD);
	char* strAB;
	strAB=alloc(2048);
	printf("offset: %lu\n", strAB - first);

	for(int i = 0; i < 1024; i++)
	{
		strAB[i] = 'a';
	}
	for(int i = 1024; i < 2047; i++)
	{
		strAB[i] = 'b';
	}
	strAB[2047] = '\0';

	char stringAB[2048];

	for(int i = 0; i < 1024; i++)
	{
		stringAB[i] = 'a';
	}
	for(int i = 1024; i < 2047; i++)
	{
		stringAB[i] = 'b';
	}
	stringAB[2047]='\0';

	if(strcmp(stringAB, strAB) == 0)
		printf("Test 3b passed: Buddies United 2nd half\n");
	else{
		printf("Test 3b failed: AB: %d\n", strcmp(stringAB, strAB));
	}

	dealloc(strAB);

	strX=alloc(1024);
	printf("offset: %lu\n", strX - first);
	strD=alloc(1024);
	printf("offset: %lu\n", strD - first);

	for(int i=0;i<1023;i++){

		strX[i]='x';
	}

	strX[1023]='\0';

	if(strcmp(stringX, strX) == 0)
		printf("Test 3c passed: Allocate with buddy\n");
	else{
		printf("Test 3c failed: X: %d\n", strcmp(stringX, strX));
	}

	//test 4: 
	
	dealloc(strD);
	dealloc(strYZ);
	dealloc(strX);
	

	char* strXX=alloc(4096);
	printf("offset: %lu\n", strXX - first);

	for(int i = 0; i < 4095; i++)
	{
		strXX[i] = 'x';
	}
	strXX[4095] = '\0';

	char stringXX[4096];

	for(int i = 0; i < 4095; i++)
	{
		stringXX[i] = 'x';
	}
	stringXX[4095] = '\0';

	if(strcmp(stringXX, strXX) == 0)
	  printf("Test 4 passed: full allocation worked\n");
	else
	  printf("Test 4 failed: X: %d\n", strcmp(stringX, strX));

	fflush(stdout);
	//test 5
	
	char* strP = alloc(1024);

	if(!strP)
		printf("Test 5 passed: Null returned if out of memory\n");
	else
		printf("Test 5 failed\n");
	
	dealloc(strXX);
	//test 6

	strA= alloc(1024);
	printf("offset: %lu\n", strA - first);
	strB= alloc(1024);
	printf("offset: %lu\n", strB - first);
	strC= alloc(1024);
	printf("offset: %lu\n", strC - first);
	strD= alloc(1024);
	printf("offset: %lu\n", strD - first);

	dealloc(strA);
	dealloc(strD);
	
	strP = alloc(2048);

	if(!strP)
		printf("Test 6 passed: Null returned if contiguous memory not available\n");
	else
		printf("Test 6 failed\n");
	
	// test 7

	strA=alloc(1024);
	printf("offset: %lu\n", strA - first);

	strD=alloc(1024);
	printf("offset: %lu\n", strD - first);

	dealloc(strC);
	dealloc(strB);

	strP = alloc(2048);

	if(!strP)
		printf("Test 7 passed: Null returned if contiguous memory available but in different blocks\n");
	else
		printf("Test 7 failed\n");
	
	///////////////////////////

	
	if(cleanup())
		return 1;
	return 0;
}
