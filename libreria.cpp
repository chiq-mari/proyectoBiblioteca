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


int main(){

    ifstream usersData("usersData.csv");   //file with users Data
    bool inSystem=false;                   //To end or continue with the session
    int numAnswer;                         //To end or continue with the session
    userInfo usersList [500];              //users List
    getUsers(usersList, usersData);
    int userIndex=0;
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

    userInfo actualUser;  //recover actual user info
    actualUser=usersList[userIndex];

    if(inSystem==true){   //welcome message if login was successful
        cout<<endl<<"Bienvenido "<<actualUser.username<<"! ("<<actualUser.userType<<')'<<endl;
    }

    while(inSystem){
        //////////////////////all allowed within system(beginnig)
        if(actualUser.userType=="admin"){   //case for admin
            cout<<"Elija una entre las siguientes opciones: "<<endl;
            cout<<"a. Crear cuenta"<<endl;
            cout<<"b. Eliminar cuenta"<<endl;
            cout<<"c. Modificar cuenta"<<endl;
            cout<<"d. Añadir libro"<<endl;
            cout<<"e. Eliminar libro"<<endl;
            cout<<"f. Modificar libro"<<endl;
            cout<<"g. Comprar libro"<<endl;
            cout<<"h. Alquilar libro"<<endl;
            cout<<"i. Retornar un libro"<<endl;
            //end of case for admin
        }else if(actualUser.userType=="employee"){    //case for employee
            cout<<"Elija una entre las siguientes opciones: "<<endl;
            cout<<"a. Añadir libro"<<endl;
            cout<<"b. Eliminar libro"<<endl;
            cout<<"c. Modificar libro"<<endl;
            cout<<"d. Comprar libro"<<endl;
            cout<<"e. Alquilar libro"<<endl;
            cout<<"f. Retornar un libro"<<endl;
            //end of case for employee
        }else{   //case for client
            cout<<"Elija una entre las siguientes opciones: "<<endl;
            cout<<"a. Comprar libro"<<endl;
            cout<<"b. Alquilar libro"<<endl;
            cout<<"c. Retornar un libro"<<endl;
            //end of case for client
        }
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