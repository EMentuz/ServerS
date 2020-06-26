// ServerS.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "pch.h"
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#pragma warning(disable: 4996)
#include <opencv2/opencv.hpp>
#include <opencv/cv.hpp>

#include <opencv2/highgui.hpp>
#include <stdlib.h>

#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char* argv[]) {
	//WSAStartup
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

	if (newConnection == 0) {
		std::cout << "Error #2\n";
	}
	else {
		std::cout << "Client Connected!\n";
	}

	VideoCapture cap(0);

	// if not success, exit program
	if (cap.isOpened() == false)
	{
		std::cout << "Cannot open the video camera\n";
		return -1;
	}

	Mat frame;
	bool bSuccess = cap.read(frame); // read a new frame from video 

	//Breaking the while loop if the frames cannot be captured
	if (bSuccess == false)
	{
		std::cout << "Video camera is disconnected" << std::endl;
		std::cin.get(); //Wait for any key press
		return -1;
	}
	// do any processing
	imwrite("image.png", frame);


	FILE *in = fopen("image.png", "rb");

	fseek(in, 0, SEEK_END);
	int size_t = ftell(in);
	std::cout << "Razmer file " << size_t << std::endl;
	fseek(in, 0, SEEK_SET);


	char *bufer = new char[size_t];

	send(newConnection, (char*)&size_t, sizeof(int), NULL);//отправка размера файла



	if (fread(bufer, 1, size_t, in) != 0)
		send(newConnection, bufer, size_t, 0);

	delete[] bufer;

	system("pause");
	return 0;
}