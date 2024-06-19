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

bool validUser(string, string, userInfo[]); //returns true if the user can be authenticated and false otherwise

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

bool validUser(string username, string password, userInfo list[]){
    int i=0;
    while(list[i].name!=""){
        if(list[i].username==username && list[i].password==password){
            return true;
        }else{
            i++;
        }
    }
    return false;
}


int main(){

    ifstream usersData("usersData.csv");   //file with users Data
    bool inSystem=false;                   //To end or continue with the session
    int numAnswer;                         //To end or continue with the session
    userInfo usersList [500];              //users List
    getUsers(usersList, usersData);
    cout<<"¡Bienvenido a la libreria!"<<endl<<endl;
    cout<<"Log in"<<endl<<"---------------"<<endl;
    int logInAttempts=1;

    while(inSystem==false && logInAttempts<=3){
   
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

    if (validUser(username,password,usersList)){
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
}
  
    while(inSystem){
        //////////////////////all allowed within system(beginnig)
        cout<<endl<<"Ingreso y opciones"<<endl;
        ///////////////////////all allowed within system (end)


        //ask again
        cout<< "Presione 0 si desea salir y cualquier otro nro para continuar con otra acción"<<endl;
        cin>>numAnswer;
        if(numAnswer==0){
            inSystem=false;
        }else{
            inSystem= true;
        }

    }//while para seguir dentro del sistena
    cout<<"---------------"<<endl<<"Gracias por preferirnos"<<endl;

}//main