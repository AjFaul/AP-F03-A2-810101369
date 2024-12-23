#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <list>
#include <map>
using namespace std;






class Restaurant
{
private:
    string name , district;
    int open_time,close_time, num_of_table;
    map<string , int> menu_item;
public:
    Restaurant(vector <string> data)
    {
        int size=data.size();
        name=data[0];
        district=data[1];
        for(int i=2; i<(size - 3) ; i=i+2)
            menu_item[data[i]]=stoi(data[i+1]);
        open_time=stoi(data[size-3]);
        close_time=stoi(data[size-2]);
        num_of_table=stoi(data[size-1]);
    }
};



class App
{
private:
    vector<Restaurant> restaurants ;
    int status_app;
public:
    App(){status_app=1;}
    void Add_restaurant(Restaurant restaurant){restaurants.push_back(restaurant);}
    
};









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

void handle_restaurants(){}

void handle_districts(){}




void handle_input_data(char* argv[])
{
    string name_file_restaurants=argv[1];
    string name_file_districts=argv[2];
    
    ifstream file_restaurants(name_file_restaurants);
    ifstream file_districts(name_file_districts);

    handle_restaurants();
    handle_districts();

}













int main(int argc , char* argv[])
{

}
