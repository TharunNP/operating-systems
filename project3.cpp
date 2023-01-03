#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <pthread.h>
#include <stdio.h>
#include <iomanip>
#include <vector>


//helper function from hw2
std::string decimalToBinary(double num, int k_prec)//referred from geeksforgeeks
{
	std::string binary = "";
	int Integral = num;
	double fractional = num - Integral;
	while (Integral)
	{
		int rem = Integral % 2;

		// Append 0 in binary
		binary.push_back(rem +'0');

		Integral /= 2;
	}
	reverse(binary.begin(),binary.end());
	binary.push_back('.');
	while (k_prec--)
	{
		// Find next bit in fraction
		fractional *= 2;
		int fract_bit = fractional;

		if (fract_bit == 1)
		{
			fractional -= fract_bit;
			binary.push_back(1 + '0');
		}
		else
			binary.push_back(0 + '0');
	}

	return binary;
}

//helper function from hw2
std::string convertToString(char* a, int size)
{
    int i;
    std::string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}


struct operation
{  
  double val1; //probibality
	char val2; //char
  double op; //cumulative prob
  std::string result;//result is a string so we can get 0 infront of values ex:001
};

struct operationParameters
{
  pthread_mutex_t *bsem;
  pthread_cond_t *waitForTurn;
  int *turnNum;
  int loc;

  std::vector<operation> *parameters;
};

void *shannon(void *dataptr)
{
  operationParameters *currentParameter = (operationParameters *) dataptr;
  int currentLoc = currentParameter->loc;
  currentParameter->loc++;
  std::string result = "";
  double codes;
  codes = 0.5*currentParameter->parameters->at(currentLoc).val1 + currentParameter->parameters->at(currentLoc).op;
  pthread_mutex_unlock(currentParameter->bsem);

  //critical section 1
  pthread_mutex_lock(currentParameter->bsem);
  
  currentParameter->parameters->at(currentLoc).result = decimalToBinary(codes, 6).substr(1,int(ceil(log2((1/ currentParameter->parameters->at(currentLoc).val1))+1)));

  while(*(currentParameter->turnNum) != currentLoc)
  {
    pthread_cond_wait(currentParameter->waitForTurn, currentParameter->bsem);
  }
  pthread_mutex_unlock(currentParameter->bsem);
  //output result
  std::cout <<"Symbol "<< currentParameter->parameters->at(currentLoc).val2 <<" , Code: "<< currentParameter->parameters->at(currentLoc).result <<std::endl;

  //critical section 2
  pthread_mutex_lock(currentParameter->bsem);
  
  *(currentParameter->turnNum) = *(currentParameter->turnNum) +1;
  
  pthread_cond_broadcast(currentParameter->waitForTurn);
  pthread_mutex_unlock(currentParameter->bsem);
  
  return nullptr;  
}

int main() {
  std::string inputString;
  std::getline(std::cin,inputString);
  // std::cout<<"the input string is " << inputString << std::endl;
  int size = 0;
  size = 5;
  double tempCu = 0;
  int tempN1 = inputString.length();
  char str[tempN1];
  strcpy(str, inputString.c_str());
  char val1[5];
  std::string s= str;
  int n1 =strlen(str);
  

  double freq[256] = {0}, val2[5];
   
  //Calculating frequency of each character.
  for(int i = 0; str[i] != '\0'; i++)
  {
    freq[str[i]]++;
  }
  int count=0,h=0;
  //Printing frequency of each character.
  for(int i = 0; i < 256; i++)
  {
    if(freq[i] != 0)
    {
      //std::cout<<"The frequency of "<<char(i)<<" is "<<freq[i]/n1<<std::endl;
      val1[h]=char(i);
      val2[h]=freq[i]/n1;
      h++;
      count++;   
    }
  }
  size = h;
  std::vector<operation> tempVect;
  struct operationParameters parameters;
  for(int i = 0; i < h; i++)
  {
    operation operations;
    operations.val1 = val2[i];
    operations.val2 = val1[i];
    operations.op = tempCu;
    tempCu = tempCu + val2[i];
    tempVect.push_back(operations);
    //std::cout<<val1[i]<<" proba is "<<val2[i]<<"cumulative proba is " << operations.op << std::endl;
  }
    
  int turn = 0;

  static pthread_mutex_t bsem;
  static pthread_cond_t waitForTurn;
  pthread_mutex_init(&bsem, nullptr);
  pthread_cond_init(&waitForTurn, nullptr);
  parameters.waitForTurn = &waitForTurn;
  parameters.bsem = &bsem;
  parameters.turnNum = &turn;
  parameters.loc = 0;
  parameters.parameters = &tempVect;

  std::cout << "SHANNON-FANO-ELIAS Codes:" << std::endl;
  std::cout << std::endl;
  pthread_t *thread0 = new pthread_t[h];
  for(int i = 0; i < h; i++)
  {
    pthread_mutex_lock(&bsem);
    if(pthread_create(&thread0[i], nullptr, shannon, &parameters))//from practice exam
		{
			fprintf(stderr, "Error creating thread\n");
			return 1;
		}
  }
  for (int i = 0; i < h; i++) 
  {
    pthread_join(thread0[i], nullptr);
  }
  delete[] thread0;
  return 0;
}
