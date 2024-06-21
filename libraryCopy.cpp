#include<iostream>   
#include<fstream>
#include<sstream>
#include<conio.h>
using namespace std;

struct userInfo{
    string name;
    string lastName;
    string username;
    string password;
    string userStatus; //active o suspended
    string userType; //admin, client, employee
    string book;     //none or bookTitle  
};

//declaraciones
void getUsers( userInfo[], ifstream& );   // gets a list with the users info as userInfo datatype

bool validUser(string, string, userInfo[], int&); //returns true if the user can be authenticated and false otherwise

bool repeatedUsername(string, userInfo[]);      //returns true if the string passed is a repeated username

int main(){

    ifstream usersData("usersData.csv");   //file with users Data
    bool inSystem=false;                   //To end or continue with the session
    int numAnswer;                         //To end or continue with the session
    userInfo usersList [500];              //users List
    getUsers(usersList, usersData);
    int userIndex=0;
    char opt;                              //to choose an action
    cout<<"¡Bienvenido a la libreria!"<<endl<<endl;
    cout<<"Log in"<<endl<<"---------------"<<endl;
    int logInAttempts=1;
    while(inSystem==false && logInAttempts<=3){  //log in start
   
    string username;
    string password="";
    char ch;
    char ch1;
    int i=0;
  
    cout<<"Usuario: ";
    cin>>username;
    cout<<"Contraseña: ";
    ch= getch();
    while(ch!=13){
        cout<<'*';
        ch1=ch;
        password+=ch1;
        ch=getch();
    }  
    
    cout<<endl<<"Autenticando..."<<endl;

    if (validUser(username,password,usersList, userIndex)){
        cout<<"Log in exitoso"<<endl<<"---------------"<<endl;
        inSystem=true;
    }else{
        cout<<"Ha ingresado un nombre de usuario o contraseña incorrecto."<<endl<<"---------------"<<endl;
        logInAttempts++;
        inSystem=false;
        password="";
        if(logInAttempts==4){
            cout<<"fin del nro de intentos"<<endl<<"---------------"<<endl;
        }
    }
}// log in end

    for(int i=0; i<251;i++){
        cout<<usersList[i].username<<endl;
    }

    string you;
    cin>>you;
    if(repeatedUsername(you, usersList)){
        cout<<"repeated"<<endl;
    }else{
        cout<<"not repeated";
    }
    

}//main

//implementaciones
void getUsers( userInfo usersArray[], ifstream& file){
    string lineInfo;
    stringstream s;
    int counter=0;
    userInfo user;

    while(!file.eof()){
        getline(file,lineInfo);

        if(lineInfo.substr(0,6)=="nombre"){
            continue;
        }

        s<<lineInfo;
        getline(s, user.name, ',');
        getline(s, user.lastName, ',');
        getline(s, user.username, ',');
        getline(s, user.password, ',');
        getline(s, user.userStatus, ',');
        getline(s, user.userType, ',');
        getline(s, user.book, ',');

        usersArray[counter]=user;
        counter++;
        s.clear();
    }
}

bool validUser(string username, string password, userInfo list[], int& a){
    int i=0;
    while(list[i].name!=""){
        if(list[i].username==username && list[i].password==password){
            a=i;
            return true;
        }else{
            i++;
        }
    }
    return false;
}

bool repeatedUsername(string username, userInfo list[]){
    int counter=0;
    bool answer;
    while(list[counter].name!=""){
        cout<<list[counter].username<<','<<username<<endl;
        if(list[counter].username==username){
            return true;
        }else{
        counter++;
        }
    }
    return false;
    
}