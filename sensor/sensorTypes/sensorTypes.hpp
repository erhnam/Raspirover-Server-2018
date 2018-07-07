#include <cstring>
#include <vector>

using namespace std;

struct sensor{
	unsigned short int id;
	string name;
	string portType;
};

vector <sensor> sensorTypes { 
	{1,"DHT22TEMP","GPIO"},
	{2,"DHT22HUM","GPIO"},
	{3,"HCSR04","GPIO"},
	{4,"LDR","SPI"},
	{5,"FIRE","SPI"},
	{6,"MQ2","SPI"},
	{7,"CAMERA","CSI"},
	{8,"NEO6MV2","UART"}
}; 
