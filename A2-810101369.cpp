#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <list>
#include <map>
using namespace std;
//--------------------
const char DELIMITER_SPACE=' ';
const char DELIMITER_COMMA=',';
const string CMD_REQUEST="request_spot";
const string CMD_ASSIGN="assign_spot";
const string CMD_CHECKOUT="checkout";
const string CMD_PASS_TIME="pass_time";
//--------------------
vector<string>  split(string Line,char DELIMITER)
{
    istringstream ss(Line);
    string token;
    vector<string> inputSection;
    while (getline(ss,token,DELIMITER)){
        inputSection.push_back(token);
    }
    return inputSection;
}
//--------------------
struct car
{
    string name;
    int size;
};
//--------------------
struct placePark
{
    int size;
    string type;
    int resevationDay;
    int status=0;
};
//--------------------
struct price
{
    int size;
    int staticPrice;
    int price;
};
//--------------------
struct infoPrice
{
    int staticPrice;
    int pricePerDay;
};
//--------------------
struct dataOfLastCar
{
    int size;
    string carName;
};
//--------------------
//--------------------
typedef int id;
typedef placePark idInfo;
typedef int sizeCar;
typedef vector <car> dataOfCar;
typedef map <id,idInfo> dataOfParking;
typedef map <sizeCar,infoPrice> dataOfPrice;
//--------------------
//--------------------
void handleCarData(vector<car> *carData,ifstream *fileCars){
    string line,carName,carSize;
    vector<string> lineSplit;
    if((*fileCars).is_open()){
        while (getline((*fileCars),line)){
            lineSplit=split(line,DELIMITER_COMMA);
            carName=lineSplit[0];
            carSize=lineSplit[1];
            if(carName=="name" && carSize=="size")
                continue;
            (*carData).push_back({carName,stoi(carSize)});
        }(*fileCars).close();
    }
}
void handleParkingData(map <id,idInfo> *parkingData,ifstream *fileParking){
    string  line;
    vector<string> lineSplit;
    if((*fileParking).is_open()){
        while (getline((*fileParking),line)){
            lineSplit=split(line,DELIMITER_COMMA);
            if(lineSplit[0]=="id" && lineSplit[1]=="size" && lineSplit[2]=="type")
                continue;
            (*parkingData)[stoi(lineSplit[0])]={stoi(lineSplit[1]) , (lineSplit[2]),0};
        } (*fileParking).close();    
    }
}
//--------------------
void handlePriceData(map <sizeCar,infoPrice> *priceData,ifstream *filePrice){
    string  line;
    vector<string> lineSplit;
    if((*filePrice).is_open()){
        while (getline((*filePrice),line)){
            lineSplit=split(line,DELIMITER_COMMA);
            if(lineSplit[0]=="size" && lineSplit[1]=="static_price" && lineSplit[2]=="price_per_day")
                continue;
            (*priceData)[stoi(lineSplit[0])].staticPrice=stoi(lineSplit[1]);
            (*priceData)[stoi(lineSplit[0])].pricePerDay=stoi(lineSplit[2]);
        } (*filePrice).close();    
    }
}
//--------------------
void handleInputData(char* argv[] ,dataOfCar *carData ,dataOfParking *parkingData,dataOfPrice *priceData ){
    string nameFileCar=argv[1],nameFileParking=argv[2],nameFilePrice=argv[3];
    ifstream fileCars(nameFileCar+".csv");
    ifstream fileParking(nameFileParking+".csv");
    ifstream filePrice(nameFilePrice+".csv");
    handleCarData(carData,&fileCars);
    handleParkingData(parkingData,&fileParking);
    handlePriceData(priceData,&filePrice);
}
//--------------------
void setSize(dataOfCar carData,dataOfLastCar *lastCar){
    for(auto it=carData.begin();it!=carData.end();it++){
        if((*it).name==(*lastCar).carName){
            (*lastCar).size=(*it).size;
            break;
        }
    }
}
//--------------------
void calculatePriceByType(int &normalPricePerDay,int &normalStaticPrice,string type){
    if (type=="covered"){
        normalPricePerDay=normalPricePerDay+30;
        normalStaticPrice=normalStaticPrice+50;
        return;
    }
    if (type=="CCTV"){
        normalPricePerDay=normalPricePerDay+60;
        normalStaticPrice=normalStaticPrice+80;
        return;
    }
    return;
}
//--------------------
void showProperPlace(map <id,idInfo> parkingData,dataOfLastCar *lastCar,map <sizeCar,infoPrice> priceData){
    for (auto it=parkingData.begin();it!=parkingData.end();it++){
        if (it->second.size==(*lastCar).size && it->second.status==0){
            int pricePerDay=priceData[it->second.size].pricePerDay;
            int staticPrice=priceData[it->second.size].staticPrice;
            calculatePriceByType(pricePerDay,staticPrice,it->second.type);
            cout<<it->first<<": "<<it->second.type<<" "<<staticPrice
            <<" "<<pricePerDay<<endl;
        }
    }
}
//--------------------
void requestSpot(const vector <car> &carData,dataOfLastCar *lastCar,const map <id,idInfo> &parkingData,const map <sizeCar,infoPrice> & priceData){
    setSize(carData,lastCar);
    showProperPlace(parkingData,lastCar,priceData);
}
//--------------------
void calculateCost(int firstDay,int lastDay,int id ,int size,map <sizeCar,infoPrice> priceData,string type){
    int cost;
    int pricePerDay=priceData[size].pricePerDay;
    int staticPrice=priceData[size].staticPrice;
    calculatePriceByType(pricePerDay,staticPrice,type);
    cost=(lastDay-firstDay)*(pricePerDay)+staticPrice;
    cout<<"Total cost: "<<cost<<endl;
}
//--------------------
void controlStructure(dataOfCar carData , dataOfParking parkingData , dataOfPrice priceData){
    string inputLine;
    vector <string> splitLine;
    int Localtime=0;
    dataOfLastCar lastCar;
    while (getline(cin,inputLine)){
        splitLine=split(inputLine,DELIMITER_SPACE);
        if(splitLine[0] == CMD_PASS_TIME)
            Localtime=Localtime+stoi(splitLine[1]);

        if(splitLine[0] == CMD_REQUEST){
            lastCar.carName=splitLine[1];
            requestSpot(carData,&lastCar,parkingData,priceData);
        }
        if(splitLine[0] == CMD_ASSIGN){
            parkingData[stoi(splitLine[1])].resevationDay=Localtime;
            parkingData[stoi(splitLine[1])].status=1;
            cout<<"Spot "<<splitLine[1]<<" is occupied now."<<endl;
        }
        if(splitLine[0] == CMD_CHECKOUT){
            cout<<"Spot "<<splitLine[1]<<" is free now."<<endl;
            int firstDay=parkingData[stoi(splitLine[1])].resevationDay;
            int size=parkingData[stoi(splitLine[1])].size;
            calculateCost(firstDay,Localtime,stoi(splitLine[1]),size,priceData,parkingData[stoi(splitLine[1])].type);
            parkingData[stoi(splitLine[1])].resevationDay=0;  
            parkingData[stoi(splitLine[1])].status=0;  
        }
    }
}
//-----------------





int main(int argc , char* argv[])
{
    vector <car> carData;
    map <id,idInfo> parkingData;
    map <sizeCar,infoPrice> priceData;
    handleInputData(argv,&carData,&parkingData,&priceData);
    controlStructure( carData ,  parkingData ,  priceData);
    return 0;
}













































