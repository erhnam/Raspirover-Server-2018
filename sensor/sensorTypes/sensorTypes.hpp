#include <cstring>
#include <vector>

using namespace std;

struct sensor{
	unsigned short int id;
	string name;
	string portType;
};

vector <sensor> sensorTypes { 
	{1,"DHT22","GPIO"},
	{2,"HCSR04","GPIO"},
	{3,"LDR","SPI"},
	{4,"FIRE","SPI"},
	{5,"MQ2","SPI"},
	{6,"CAMERA","CSI"},
	{7,"NEO6MV2","UART"}
}; 
