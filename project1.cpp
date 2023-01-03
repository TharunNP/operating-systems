#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <iomanip>
#include <stdio.h>
#include<bits/stdc++.h>

using namespace std;

string decimalToBinary(double num, int k_prec)//referred from geeksforgeeks
{
	string binary = "";
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

struct operation
{
	double val1;
	char val2;
    double op;
    string result;//result is a string so we can get 0 infront of values ex:001
};

void *shannon(void *pos_void_ptr)
{   
	struct operation *pos_ptr = (struct operation *)pos_void_ptr;
    double codes;
	
	//int len=5,count=0;
	// for (int i = 0;i < 2;i++)//seperate code just for implementing shannon
	// {
	// 	cin >> a[i];
	// 	cin >> x[i];		
	// };
   codes=0.5*pos_ptr->val1 + pos_ptr->op;//calculates freq
   //cumu = cumu + val;//testing
   pos_ptr->result = decimalToBinary(codes, 6).substr(1,int(ceil(log2((1/ pos_ptr->val1))+1)));//This calculates log values 
  	return NULL;
}

int main()
{   
    string a;
    getline(cin,a);// gets the firstline of characters
 	float size1 = a.size(); //size includes white spaces as well
 	float temp= ceil(size1/2);// to round up the size for use
 	static int size=temp;// to keep values static for struct operation
 	//cout<<temp<<endl;
 	int x=0;
	struct operation operations[size];
 	pthread_t tid[size];
   for(int i=0;i<size1;i++)//loop for characters
 	{    
 	    
 	    operations[i].val2 = a[x];
 	    x=x+2;//to skip the spaces in between values
         
     }
  for(int i=0;i<size;i++)//loop gets values in and also calculates cumulative freq
	{   
        cin >> operations[i].val1;
        operations[i+1].op = operations[i].op+operations[i].val1;
        
    }
    // for(int i=0;i<NOPER;i++)//testing for cumulative freq
    // {   
    //     operations[i].op = operations[i].op+operations[i].val1;
        
    // }
        
        //cout<<operations[i].op<<endl; testing
    
	for(int i=0;i<size;i++)
	{   
		if(pthread_create(&tid[i], NULL, shannon, &operations[i]))//from practice exam
		{   
			fprintf(stderr, "Error creating thread\n");
			return 1;

		}//cout<<operations[i].op<<endl; testing		
	}
	
	// Wait for the other threads to finish.
	for (int i = 0; i < size; i++)
     {
            pthread_join(tid[i], NULL);
            
            
     }
     cout<<"SHANNON-FANO-ELIAS Codes:"<<endl<<endl;//output
    for(int i=0;i<size;i++)
	{
        cout <<"Symbol "<<operations[i].val2<<" , Code: "<< operations[i].result<<endl;
    }
    
  
    
    return 0;
}
