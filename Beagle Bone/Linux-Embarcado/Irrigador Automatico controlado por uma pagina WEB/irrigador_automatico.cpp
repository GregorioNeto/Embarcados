//Teste unindo  C e C++
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>

#define MAX_BUF 200 

#define SYSFS_ADC_DIR "/sys/bus/iio/devices/iio:device0/in_voltage4_raw" 

using namespace std;

/*************************************
	CODIGO DE PEGAR O VALOR DO ADC
	E CONFIGURA O ANALOGIC 4
*************************************/

int sensorvalue() 
{ 	
 	ifstream fs;
 
	int value;
	fs.open ("/sys/bus/iio/devices/iio:device0/in_voltage4_raw");
	fs >> value;
	fs.close();
	//cout << value << endl;
	   return value;
} 

/*************FIM ADC***************/


/******************************************************
 Função de configuração dos pinos que serão utilizados
 	pin 12 = Bomba = 32*1+ 12 = 44
 	pin 11 = Led ON = 32*1 + 13 = 45
 	pin 16 = Led OFF = 32*1 + 14 = 46
*******************************************************/
void confg_pin(){
std::fstream fs;

	   fs.open ("/sys/class/gpio/export", std::fstream::out);
	   fs << 44;
	   fs.close();
	   fs.open ("/sys/class/gpio/export", std::fstream::out);
	   fs << 45;
	   fs.close();
	   fs.open ("/sys/class/gpio/export", std::fstream::out);
	   fs << 46;
	   fs.close();
	   

	   fs.open ("/sys/class/gpio/gpio44/direction", std::fstream::out);
	   fs << "out";
	   fs.close();
	   fs.open ("/sys/class/gpio/gpio45/direction", std::fstream::out);
	   fs << "out";
	   fs.close();
	   fs.open ("/sys/class/gpio/gpio46/direction", std::fstream::out);
	   fs << "out";
	   fs.close();
	}
/**************************************************
	FUNÇÕES DE LIGAR/DESLIGAR E MODO AUTOMATICO
	DO MOTOR
**************************************************/

void funcoff(){
	std::fstream fs;
	fs.open ("/sys/class/gpio/gpio46/value", std::fstream::out);
	fs << 1;
	fs.close();
	fs.open ("/sys/class/gpio/gpio45/value", std::fstream::out);
	fs << 0;			
	fs.close();
	fs.open ("/sys/class/gpio/gpio44/value", std::fstream::out);
	fs << 0;
	//cout << "Status: - Motor [OFF] -" << endl;
	fs.close();
} 

void funcopen(float sensor){
	std::fstream fs;
	fs.open ("/sys/class/gpio/gpio46/value", std::fstream::out);
	fs << 0;
	fs.close();
	fs.open ("/sys/class/gpio/gpio45/value", std::fstream::out);
	fs << 1;
	fs.close();
	fs.open ("/sys/class/gpio/gpio44/value", std::fstream::out);
	fs << 1;
	fs.close();	
	//cout << "Status: - Motor [ON] -" << endl;	
		
		if(sensor <= 700) {
			funcoff();
		}
}

void funcauto(int x, float sensor){
	std::fstream fs;	

	funcoff();
	//cout << "Status: - Motor [AUTO] -" << endl;
	sleep(x);	
	funcopen(sensor);
	sleep(x);
}
/******************* FIM FUNÇÕES DO MOTOR ******************/

/*******************CABEÇARIOS******************************/
	void funcopen(float sensor);
	void funcoff();
	void funcauto(int x, float sensor);
/**********************************************************/

int main(){
	int value;
	int value1;
	int sensor;


	//int sensorvalue();  

	//sensor = sensorvalue();

	ifstream leitorArq;	
	ifstream leitorArq1;

	std::fstream fs;
	 confg_pin();
	 //confg_pin();

	while(1){

		leitorArq.open("/usr/Projeto/status_project.txt");
		leitorArq >> value;
		leitorArq1.open("/usr/Projeto/status_timer.txt");
		leitorArq1 >> value1;
		sensor = sensorvalue();
		
		if(value == 0){
			funcoff();
		}else if(value == 1){
			funcopen(sensor);
		}else if(value == 2){
			funcauto(value1,sensor);
		}

		leitorArq.close();
		leitorArq1.close();

		system("sleep 1");
	}

	return 0;
}	
