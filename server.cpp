#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include<bits/stdc++.h>
#include <pthread.h>
#include <stdio.h>
#include <iomanip>

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

std::string shannon(double val1, double op)
{   
	
    double codes;
	
	//int len=5,count=0;
	// for (int i = 0;i < 2;i++)//seperate code just for implementing shannon
	// {
	// 	cin >> a[i];
	// 	cin >> x[i];		
	// };
   codes=0.5*val1 + op;//calculates freq
   //cumu = cumu + val;//testing
    std::string result = decimalToBinary(codes, 6).substr(1,int(ceil(log2((1/ val1))+1)));//This calculates log values 
  	return result;
}

void fireman(int)
{
   while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    signal(SIGCHLD, fireman); 
    if (argc < 2)
    {
        std::cerr << "ERROR, no port provided" << std::endl;
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "ERROR opening socket" << std::endl;
        exit(1);
    }
    // else
    // {
    //     std::cout << "socket opened" << std::endl;
    // }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) < 0)
    {
        std::cerr << "ERROR on binding" << std::endl;
        exit(1);
    }
    // else
    // {
    //   std::cout << "binded" << std::endl;
    // }
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);





  
    double val1;;
    double op;
    while (true)
    {
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
        if (fork() == 0)
        {
            val1 = 0;
            op = 0;
            if (newsockfd < 0)
            {
                std::cerr << "ERROR on accept" << std::endl;
                exit(1);
            }
            // else
            // {
            //   std::cout << "server accepted" << std::endl;
            // }

            
            n = read(newsockfd, &val1, sizeof(val1));
            if (n < 0)
            {
                std::cerr << "ERROR reading from socket" << std::endl;
                exit(1);
            }
            // else
            // {
            //   std::cout << "val1 recieved from client is " << val1 << std::endl;
            //   std::cout << "server read from client" << std::endl;
            // }

            n = read(newsockfd, &op, sizeof(op));
            if (n < 0)
            {
                std::cerr << "ERROR reading from socket" << std::endl;
                exit(1);
            }
            // else
            // {
            //   std::cout << "op recieved from client is " << op << std::endl;
            //   std::cout << "server read from client" << std::endl;
            // }


            std::string finval = shannon(val1,op);
            char sersend[finval.length()+1];
            strcpy(sersend, finval.c_str());
            int sersendsize=strlen(sersend);
          
            n = write(newsockfd, &sersendsize, sizeof(int));
            if (n < 0)
            {
                std::cerr << "ERROR writing to socket" << std::endl;
                exit(1);
            }
            // else
            // {
            //   std::cout << "server write to client" << std::endl;
            // }

            // std::cout << "the sersend from server is " << finval << std::endl;
            n = write(newsockfd, &sersend, sersendsize);
            if (n < 0)
            {
                std::cerr << "ERROR writing to socket" << std::endl;
                exit(1);
            }
            // else
            // {
            //   std::cout << "server write to client" << std::endl;
            // }
             close(newsockfd);
            _exit(0);
        }
    }
    close(sockfd);
    return 0;
}
