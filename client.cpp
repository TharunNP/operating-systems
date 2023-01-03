#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <bits/stdc++.h>
#include <pthread.h>
#include <stdio.h>
#include <iomanip>

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
    int sockID;
    int portnum;
    struct hostent * server;
    struct sockaddr_in addr;
    socklen_t addr_size;

	  double val1;
	  char val2;
    double op;
    std::string result;//result is a string so we can get 0 infront of values ex:001
};

void * getCharacter (void * vec) 
{
    //std::cout<<"exe getChar function" <<std::endl;
    struct operation * dat = (struct operation *) vec;
    int sockID = socket(AF_INET,SOCK_STREAM,0);
    bcopy((char *)dat->server->h_addr, (char *)&dat->addr.sin_addr.s_addr, dat->server->h_length);
    
    if (connect(sockID, (struct sockaddr *)&dat->addr, sizeof(dat->addr)) < 0)
    {
        std::cerr << "ERROR connecting to thread" << std::endl;
        exit(1);
    }
      /*
    else
    {
      std::cout << "connected to server in thread" << std::endl;
    }
    */
  
    int n = 0;

  

    n = write(sockID, &dat->val1, sizeof(dat->val1));
    if (n < 0)
    {
        std::cerr << "ERROR writing to socket";
        exit(1);
    }

      /*
    else
    {
      std::cout << "the val1 client send to server is " << dat->val1 << std::endl;
      std::cout << "client write to server" << std::endl;
    }
    */
  
    n = write(sockID,&dat->op, sizeof(dat->op));
    if (n < 0)
    {
        std::cerr << "ERROR writing to socket";
        exit(1);
    }

      /*
    else
    {
      std::cout << "the op client send to server is " << dat->op << std::endl;
      std::cout << "client write to server" << std::endl;
    }
    */

    int tempSize;
    n = read(sockID, &tempSize, sizeof(tempSize));
    if (n < 0)
    {
        std::cerr << "ERROR reading from socket";
        exit(1);
    }

      /*
    else
    {
      std::cout << "client read from server" << std::endl;
    }
      */

  
    char *buffer = new char[tempSize + 1];
    bzero(buffer, tempSize + 1);

    n = read(sockID, buffer, tempSize);
    if (n < 0)
    {
        std::cerr << "ERROR reading from socket";
        exit(1);
    }
    // else
    // {
    //   std::cout << "client read from server" << std::endl;
    // }
    std::string finalResult = convertToString(buffer, tempSize);
    dat->result = finalResult;
    // std::cout << "the final result is " << dat->result << std::endl;

    delete [] buffer;
    close(sockID);
    return nullptr;
}

int main(int argc, char *argv[])
{
    std::string inputString;
    std::getline(std::cin,inputString);
    // std::cout<<"the input string is " << inputString << std::endl;

  
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    if (argc < 3)
    {
        std::cerr << "usage " << argv[0] << "hostname port\n";
        exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "ERROR opening socket";
    }
    // else
    // {
    //   std::cout << "socket opened on client side" << std::endl;
    // }
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        std::cerr << "ERROR, no such host\n";
        exit(0);
    }
    // else
    // {
    //   std::cout << "host fetched" << std::endl;
    // }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    // std::cout << "sockfd is " << sockfd << std::endl;
    // std::cout << "portno is " << portno << std::endl;

    int size = 0;
      size = 5;
    double tempCu = 0;
    
    
  /*
    int tempN = inputString.length();
    std::cout << "the length of input is " << tempN << std::endl;
    double frequency[256] = {0};
    for(int i = 0; inputString[i] != '\0'; i++)
    {
      frequency[inputString[i]]++;
    }

    for(int i = 0; i < 256; i++)
    {
      if(frequency[i] != 0)
      {
        std::cout << "the frequency of " << char(i) << " is " << frequency[i]/tempN << std::endl;
        size++;
      }
    }
  
    
*/
  /*
    for(int i = 0; i < 256; i++)
    {
      if(frequency[i] != 0)
      {
          operations[i].val1 = frequency[i]/tempN;
          operations[i].op = tempCu;
          tempCu = tempCu + frequency[i]/tempN;
          operations[i].val2 = char(i);
          std::cout << "val 1 for " << i << " is " << frequency[i]/tempN << std::endl;
      }
    }
  */
    
    /*
    for(int i = 0; i < 256; i++)
    {
      for(int j = 0; j < size; j++)
      {
        if(frequency[i] != 0)
        {
          operations[i].val1 = frequency[i]/tempN;
          operations[i].op = tempCu;
          tempCu = tempCu + frequency[i]/tempN;
          operations[i].val2 = char(i);
          std::cout << "val 1 for " << i << " is " << frequency[i]/tempN << std::endl;
        }
      }
    }
  */


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
           // std::cout<<"The frequency of "<<char(i)<<" is "<<freq[i]/n1<<std::endl;
           val1[h]=char(i);
          val2[h]=freq[i]/n1;
          h++;
          count++;
          
        }
    }
  // std::cout << "h is " << h << " count is " << count << std::endl;
  size = h;
  struct operation operations[size];
  for(int i = 0; i < h; i++)
    {
      operations[i].val1 = val2[i];
      operations[i].val2 = val1[i];
      operations[i].op = tempCu;
      tempCu = tempCu + val2[i];
      // std::cout<<val1[i]<<"  is "<<val2[i]<<"tempCu is " << tempCu << std::endl;
    }


/*
    operations[0].val1 = 0.3;
    operations[0].val2 = 'A';
    operations[0].op = 0.3;
  
    operations[1].val1 = 0.25;
    operations[1].val2 = 'B';
    operations[1].op = 0.55;

  operations[2].val1 = 0.2;
    operations[2].val2 = 'C';
    operations[2].op = 0.75;

  operations[3].val1 = 0.15;
    operations[3].val2 = 'D';
    operations[3].op = 0.9;

  operations[0].val1 = 0.1;
    operations[0].val2 = 'E';
    operations[0].op = 1.0;
  */
    
    // std::cout << "the size is " << size << std::endl;
    tempCu = 0;
    for(int i=0;i<size;i++)//loop gets values in and also calculates cumulative freq
 	  {   
//         operations[i].val1=fr[i]/20;
//         operations[i].op = tempCP;
//         tempCP = tempCP + operations[i].val1;
        /*
        operations[i].val1 = 0.2;
        operations[i].op = tempCu;
      tempCu = tempCu + 0.2;
        operations[i].val2 = 'A';
        */
      
        operations[i].sockID=sockfd;
        operations[i].addr.sin_family=AF_INET;
        operations[i].addr.sin_port=htons(portno);
        operations[i].server=server;
        //std::cout << "sockid is in loop " << sockfd << std::endl;
        //std::cout << "sockfd assigned to " << operations[i].sockID << std::endl;
    }
  /*
  for(int i = 0; i < size; i++)
    {
      std::cout << "val 1 of " << i << " is " << operations[i].val1 << std::endl;
      std::cout << "op of " << i << " is " << operations[i].op << std::endl;
    }
    */

    pthread_t tid[size];
    for(int i=0;i<size;i++)
	{ 
		if(pthread_create(&tid[i], nullptr, getCharacter, &operations[i]))//from practice exam
		{   //std::cout<<"testingpo" <<std::endl;
			fprintf(stderr, "Error creating thread\n");
			return 1;

		}
	}
    for (int i = 0; i < size; i++)
     {
            pthread_join(tid[i], NULL);
            
            // std::cout<<"testingpo2" <<std::endl;
     }

  
  std::cout<<"SHANNON-FANO-ELIAS Codes:"<<std::endl<<std::endl;//output
    for(int i=0;i<size;i++)
	{
        std::cout <<"Symbol "<<operations[i].val2<<" , Code: "<< operations[i].result<<std::endl;
    }
 
    return 0;
}
