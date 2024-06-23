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

//variables globales
userInfo emptyUser={"","", "", "", "", "", ""};

//declaraciones
void getUsers( userInfo[], ifstream& );   // gets a list with the users info as userInfo datatype

bool validUser(string, string, userInfo[], int&); //returns true if the user can be authenticated and false otherwise

bool repeatedUsername(string , userInfo []);    //returns true if the string passed is a repeated username

bool repeatedUsername(string , userInfo [], int&);    //returns true if the string passed is a repeated username

void movePByOne(userInfo [], int);                    /*modifies the array so that the elements after the index showed
                                                      appear one index ahead and the element at the such index disappears*/      

int main(){

    ifstream usersData("../assets/usersData.csv");   //file with users Data
    bool inSystem=false;                   //To end or continue with the session
    int numAnswer;                         //To end or continue with the session
    userInfo usersList [500];              //users List
    string status;                          //active or suspended
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
        status=usersList[userIndex].userStatus;
        if(status=="active"){     // si esta activo permite la entrada
            cout<<"Log in exitoso"<<endl<<"---------------"<<endl;
            inSystem=true;
        }else{                    //la unica otra opcion es que este suspendido
            cout<<"Su cuenta se encuentra suspendida"<<endl;
            inSystem=false;
            logInAttempts=4;
        }

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
            cout<<endl<<"Elija una entre las siguientes opciones: "<<endl;
            cout<<"a. Crear cuenta"<<endl;
            cout<<"b. Eliminar cuenta"<<endl;
            cout<<"c. Suspender usuario"<<endl;
            cout<<"d. Añadir libro"<<endl;
            cout<<"e. Eliminar libro"<<endl;
            cout<<"f. Modificar libro"<<endl;
            cout<<"g. Comprar libro"<<endl;
            cout<<"h. Alquilar libro"<<endl;
            cout<<"i. Retornar un libro"<<endl;
            cin>>opt;
            cout<<"---------------"<<endl;
            switch (opt)
            {
            case 'a':    //crear cuenta
                {string newName;
                string newLastName;
                string newUsername;
                string newPassword;
                string newUserStatus;
                string newUserType;
                string newBook;
                cout<<"Ingrese los datos del nuevo usuario"<<endl;
                cout<<"Nombre: ";
                cin>>newName;
                cout<<"Apellido: ";
                cin>>newLastName;
                do{
                cout<<"Nombre de usuario: ";
                cin>>newUsername;
                if(repeatedUsername(newUsername,usersList)){
                    cout<<"Ingrese un nombre de usuario no repetido"<<endl;
                }
                }while(repeatedUsername(newUsername,usersList));

                cout<<"Contraseña: ";
                cin>>newPassword;
                cout<<"Estado de usuario: ";
                cin>>newUserStatus;
                cout<<"Tipo de usuario: ";
                cin>>newUserType;
                newBook="none";
                cout<<endl<<"Usuario creado con éxito!"<<endl<<"---------------"<<endl;

                usersData.close();
                ofstream usersData1;
                usersData1.open("../assets/usersData.csv",ios:: app);
                usersData1<<"\n"<<newName<<','<<newLastName<<','<<newUsername<<','<<newPassword<<','<<newUserStatus<<','<<newUserType<<","<<newBook;

                usersData1.close();
                // leave it as it was
                usersData.open("../assets/usersData.csv");
                getUsers(usersList, usersData);

                /* code */
                break;
                }
            case 'b':   //eliminar cuenta
            {   
                string newUsername;
                int indexToDelete;
                int indexToWrite=0; 
                do{               //obtiene el indice
                cout<<"Ingrese el nombre de usuario que desea eliminar: ";
                cin>>newUsername;
                if(!repeatedUsername( newUsername, usersList, indexToDelete)){
                    cout<<"Ingrese un nombre de usuario válido"<<endl;    
                }                
                }while(!repeatedUsername( newUsername, usersList));

                movePByOne(usersList, indexToDelete);   // acomoda el arreglo
                usersData.close();                      //cierre el flujo de entrada

                ofstream usersData1("../assets/usersData.csv");   // abra el de salida

                usersData1<<"nombre,lastName,username,password,userStatus,userType,book";
                while(usersList[indexToWrite].name!=""){
                    usersData1<<endl<<usersList[indexToWrite].name<<','<<usersList[indexToWrite].lastName<<','<<usersList[indexToWrite].username<<','<<usersList[indexToWrite].password<<','<<usersList[indexToWrite].userStatus<<','<<usersList[indexToWrite].userType<<','<<usersList[indexToWrite].book;
                    indexToWrite++;
                }
                usersData1.close();                  //close ofstream
                usersData.open("../assets/usersData.csv");     //open ifstream
            }            
                /* code */
                break;
            case 'c':           //suspender a usuario
            {
                string newUsername;
                int indexToSuspend;
                int indexToWrite=0;

                do{               //obtiene el indice
                cout<<"Ingrese el nombre de usuario que desea suspender: ";
                cin>>newUsername;
                if(!repeatedUsername( newUsername, usersList, indexToSuspend)){
                    cout<<"Ingrese un nombre de usuario válido"<<endl;    
                }                
                }while(!repeatedUsername( newUsername, usersList));

                cout<<indexToSuspend;

                if(usersList[indexToSuspend].userStatus=="suspended"){
                    cout<<"Ya el usuario se encuentra suspendido"<<endl;
                }else{
                    usersList[indexToSuspend].userStatus="suspended";

                    usersData.close();                      //cierre el flujo de entrada

                    ofstream usersData1("../assets/usersData.csv");   // abra el de salida

                    usersData1<<"nombre,lastName,username,password,userStatus,userType,book";
                    while(usersList[indexToWrite].name!=""){
                        usersData1<<endl<<usersList[indexToWrite].name<<','<<usersList[indexToWrite].lastName<<','<<usersList[indexToWrite].username<<','<<usersList[indexToWrite].password<<','<<usersList[indexToWrite].userStatus<<','<<usersList[indexToWrite].userType<<','<<usersList[indexToWrite].book;
                        indexToWrite++;
                    }
                    usersData1.close();                  //close ofstream
                    usersData.open("../assets/usersData.csv");     //open ifstream
                }
            }
                /* code */
                break;
            case 'd':
                /* code */
                break;
            case 'e':
                /* code */
                break;
            case 'f':
                /* code */
                break;
            case 'g':
                /* code */
                break;
            case 'h':
                /* code */
                break;
            case 'i':
                /* code */
                break;
            default:
                cout<<opt<<". no es una opción válida"<<endl;
                break;
            }
            //end of case for admin
        }else if(actualUser.userType=="employee"){    //case for employee
            cout<<"Elija una entre las siguientes opciones: "<<endl;
            cout<<"a. Añadir libro"<<endl;
            cout<<"b. Eliminar libro"<<endl;
            cout<<"c. Modificar libro"<<endl;
            cout<<"d. Comprar libro"<<endl;
            cout<<"e. Alquilar libro"<<endl;
            cout<<"f. Retornar un libro"<<endl;
            cin>>opt;
            switch (opt)
            {
            case 'a':
                /* code */
                break;
            case 'b':
                /* code */
                break;
            case 'c':
                /* code */
                break;
            case 'd':
                /* code */
                break;
            case 'e':
                /* code */
                break;
            case 'f':
                /* code */
                break;
            default:
                cout<<opt<<". no es una opción válida"<<endl;
                break;
            }
            //end of case for employee
        }else{   //case for client
            cout<<"Elija una entre las siguientes opciones: "<<endl;
            cout<<"a. Comprar libro"<<endl;
            cout<<"b. Alquilar libro"<<endl;
            cout<<"c. Retornar un libro"<<endl;
            cin>>opt;
            switch (opt)
            {
            case 'a':
                /* code */
                break;
            case 'b':
                /* code */
                break;
            case 'c':
                /* code */
                break;
            default:
                cout<<opt<<". no es una opción válida"<<endl;
                break;
            }
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
    cout<<"---------------"<<endl<<"Gracias por preferirnos!"<<endl;
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
    while(list[counter].name!=""){
        if(list[counter].username==username){
            return true;
        }else{
        counter++;
        }
    }
    return false;   
}

bool repeatedUsername(string username, userInfo list[], int& a){
    int counter=0;
    while(list[counter].name!=""){
        if(list[counter].username==username){
            a= counter;
            return true;
        }else{
        counter++;
        a=-1;
        }
    }
    return false;   
}

void movePByOne(userInfo usersArray[], int a){   //knwoing it has 500 elements at most
    for(int i=a; i<499; i++){
        usersArray[i]=usersArray[i+1];
    }
    usersArray[499]=emptyUser;
    
}

void getBooks(booksInfo booksArray[], ifstream& file){   
    string lineInfo;
    stringstream v;
    int counter=0;

    booksInfo newBook;

    while(!file.eof()){
        
        getline(file, lineInfo);
        if(lineInfo.substr(0,8)=="bookCode"){
            continue;
        }
        v<<lineInfo;

        getline(v, newBook.bookCode, ',');
        getline(v, newBook.bookTitle, ',');
        getline(v, newBook.author, ',');
        getline(v, newBook.genre, ',');
        getline(v, newBook.releaseYear, ',');
        getline(v, newBook.bookPrice, ',');
        getline(v, newBook.rentalPrice, ',');
        getline(v, newBook.bookStatus, ',');
        getline(v, newBook.by, ',');

        booksArray[counter]=newBook;
        counter++;
        lineInfo="";
        v.clear();
    }
}