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

struct booksInfo{
    string bookCode="";
    string bookTitle;
    string author;
    string genre;
    string releaseYear;
    string bookPrice;
    string rentalPrice; 
    string bookStatus;
    string by="nadie";
};

//variables globales
userInfo emptyUser={"","", "", "", "", "", ""};
booksInfo emptyBook={"","","","","","","","",""};

//declaraciones
void getUsers( userInfo[], ifstream& );   // gets a list with the users info as userInfo datatype

bool validUser(string, string, userInfo[], int&); //returns true if the user can be authenticated and false otherwise

bool repeatedUsername(string , userInfo []);    //returns true if the string passed is a repeated username

bool repeatedUsername(string , userInfo [], int&);    //returns true if the string passed is a repeated username

void movePByOne(userInfo [], int);                    /*modifies the array so that the elements after the index showed
                                                      appear one index ahead and the element at the such index disappears*/
void getBooks(booksInfo [], ifstream&);

void printBooks(booksInfo[]);

void printUsers(userInfo[]);

bool repeatedBookCode(string, booksInfo[], int&);

bool repeatedBookCode(string, booksInfo[]);

bool repeatedBook(booksInfo, booksInfo[]);

void movePByOne(booksInfo[], int);

int main(){

    ifstream usersData("../assets/usersData.csv");   //file with users Data
    bool inSystem=false;                   //To end or continue with the session
    int numAnswer;                         //To end or continue with the session
    userInfo usersList [500];              //users List
    string status;                          //active or suspended
    getUsers(usersList, usersData);
    int userIndex=0;
    char opt;                              //to choose an action
    std::cout<<"¡Bienvenido a la libreria!"<<endl<<endl;
    std::cout<<"Log in"<<endl<<"---------------"<<endl;
    int logInAttempts=1;

    while(inSystem==false && logInAttempts<=3){  //log in start
   
    string username;
    string password="";
    char ch;
    char ch1;
    int i=0;
  
    std::cout<<"Usuario: ";
    cin>>username;
    std::cout<<"Contraseña: ";
    ch= getch();
    while(ch!=13){
        std::cout<<'*';
        ch1=ch;
        password+=ch1;
        ch=getch();
    }  
    
    std::cout<<endl<<"Autenticando..."<<endl;

    if (validUser(username,password,usersList, userIndex)){
        status=usersList[userIndex].userStatus;
        if(status=="active"){     // si esta activo permite la entrada
            std::cout<<"Log in exitoso"<<endl<<"---------------"<<endl;
            inSystem=true;
        }else{                    //la unica otra opcion es que este suspendido
            std::cout<<"Su cuenta se encuentra suspendida"<<endl;
            inSystem=false;
            logInAttempts=4;
        }

    }else{
        std::cout<<"Ha ingresado un nombre de usuario o contraseña incorrecto."<<endl<<"---------------"<<endl;
        logInAttempts++;
        inSystem=false;
        password="";
        if(logInAttempts==4){
            std::cout<<"fin del nro de intentos"<<endl<<"---------------"<<endl;
        }
    }
}// log in end
    ifstream booksData("../assets/booksData.csv");  
    booksInfo booksList[150];  
    getBooks(booksList, booksData); 

    userInfo actualUser;  //recover actual user info
    actualUser=usersList[userIndex];

    if(inSystem==true){   //welcome message if login was successful
        std::cout<<endl<<"Bienvenido "<<actualUser.username<<"! ("<<actualUser.userType<<')'<<endl;
    }

    while(inSystem){
        //////////////////////all allowed within system(beginnig)
        if(actualUser.userType=="admin"){   //case for admin
            std::cout<<endl<<"Elija una entre las siguientes opciones: "<<endl;
            std::cout<<"a. Crear cuenta"<<endl;
            std::cout<<"b. Eliminar cuenta"<<endl;
            std::cout<<"c. Suspender usuario"<<endl;
            std::cout<<"d. Añadir libro"<<endl;
            std::cout<<"e. Eliminar libro"<<endl;
            std::cout<<"f. Modificar libro"<<endl;
            std::cout<<"g. Comprar libro"<<endl;
            std::cout<<"h. Alquilar libro"<<endl;
            std::cout<<"i. Retornar un libro"<<endl;
            cin>>opt;
            std::cout<<"---------------"<<endl;
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
                std::cout<<"Ingrese los datos del nuevo usuario"<<endl<<"---------------"<<endl;
                std::cout<<"Nombre: ";
                cin>>newName;
                std::cout<<"Apellido: ";
                cin>>newLastName;
                do{
                std::cout<<"Nombre de usuario: ";
                cin>>newUsername;
                if(repeatedUsername(newUsername,usersList)){
                    std::cout<<"Ingrese un nombre de usuario no repetido"<<endl;
                }
                }while(repeatedUsername(newUsername,usersList));

                std::cout<<"Contraseña: ";
                cin>>newPassword;
                std::cout<<"Estado de usuario: ";
                cin>>newUserStatus;
                std::cout<<"Tipo de usuario: ";
                cin>>newUserType;
                newBook="none";
                std::cout<<endl<<"Usuario creado con éxito!"<<endl<<"---------------"<<endl;

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
                std::cout<<"Ingrese el nombre de usuario que desea eliminar: ";
                cin>>newUsername;
                if(!repeatedUsername( newUsername, usersList, indexToDelete)){
                    std::cout<<"Ingrese un nombre de usuario válido"<<endl;    
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
                std::cout<<endl<<"Usuario "<<newUsername<<" eliminado con éxito!"<<endl<<"---------------"<<endl;

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
                std::cout<<"Ingrese el nombre de usuario que desea suspender: ";
                cin>>newUsername;
                if(!repeatedUsername( newUsername, usersList, indexToSuspend)){
                    std::cout<<"Ingrese un nombre de usuario válido"<<endl;    
                }                
                }while(!repeatedUsername( newUsername, usersList));

                if(usersList[indexToSuspend].userStatus=="suspended"){
                    std::cout<<"Ya el usuario se encuentra suspendido"<<endl;
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
                    std::cout<<"El usuario ha sido suspendido"<<endl;

                }
            }
                /* code */
                break;
            case 'd':      //añadir libro
            {   
                    string newBookCode;
                    string newBookTitle;
                    string newAuthor;
                    string newGenre;
                    string newReleaseYear;
                    string newBookPrice;
                    string newRentalPrice;
                    string newBookStatus= "disponible";
                    string newBy="nadie";

                std::cout<<"Ingrese los datos del libro"<<endl<<"---------------"<<endl;
                do{
                   std::cout<<"Código del libro: ";
                   cin>>newBookCode;
                   if(repeatedBookCode(newBookCode,booksList)){
                    std::cout<<"Codigo no disponible!"<<endl;
                   }

                }while(repeatedBookCode(newBookCode,booksList));

                cin.ignore();
                std::cout<<"Título del libro: ";
                getline(cin,newBookTitle);
                std::cout<<"Autor: ";
                getline(cin,newAuthor);
                std::cout<<"Genero: ";
                cin>>newGenre;
                std::cout<<"Año: ";
                cin>>newReleaseYear;
                std::cout<<"Precio de compra: ";
                cin>>newBookPrice;
                std::cout<<"Precio de alquiler: ";
                cin>>newRentalPrice;

                booksData.close();

                ofstream booksData1;
                booksData1.open("../assets/booksData.csv", ios::app);
                booksData1<<endl<<newBookCode<<','<<newBookTitle<<','<<newAuthor<<','<<newGenre<<','<<newReleaseYear<<','<<newBookPrice<<','<<newRentalPrice<<','<<newBookStatus<<','<<newBy;

                booksData1.close();

                booksData.open("../assets/booksData.csv");
                getBooks(booksList, booksData);

                std::cout<<endl<<"Libro agregado satisfactoriamente!"<<endl<<"---------------"<<endl;
            }
                /* code */
                break;
            case 'e':                  //eliminar libro
            {
                int indice;
                string newCode;
                do{
                cout<<"Ingrese el codigo del libro que desea eliminar: ";
                cin>>newCode;
                if(!repeatedBookCode(newCode, booksList, indice)){
                    cout<<"Ingrese un codigo existente!"<<endl;
                }
                }while(!repeatedBookCode(newCode, booksList));

                movePByOne(booksList,indice);

                booksData.close();

                ofstream booksData1;
                booksData1.open("../assets/booksData.csv");

                booksData1<<"bookCode,bookTitle,author,genre,releaseYear,bookPrice,rentalPrice,bookStatus,by";
                
                int indexToWrite=0;
                while(booksList[indexToWrite].bookCode!=""){
                        booksData1<<endl<<booksList[indexToWrite].bookCode<<','<<booksList[indexToWrite].bookTitle<<','<<booksList[indexToWrite].author<<','<<booksList[indexToWrite].genre<<','<<booksList[indexToWrite].releaseYear<<','<<booksList[indexToWrite].bookPrice<<','<<booksList[indexToWrite].rentalPrice<<','<<booksList[indexToWrite].bookStatus<<','<<booksList[indexToWrite].by;
                        indexToWrite++;
                }
                
                cout<<"Libro eliminado"<<endl;
                booksData1.close();

                booksData.open("../assets/booksData.csv");

            }
                /* code */
                break;
            case 'f':                 //modificar libro
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
                std::cout<<opt<<". no es una opción válida"<<endl;
                break;
            }
            //end of case for admin
        }else if(actualUser.userType=="employee"){    //case for employee
            std::cout<<"Elija una entre las siguientes opciones: "<<endl;
            std::cout<<"a. Añadir libro"<<endl;
            std::cout<<"b. Eliminar libro"<<endl;
            std::cout<<"c. Modificar libro"<<endl;
            std::cout<<"d. Comprar libro"<<endl;
            std::cout<<"e. Alquilar libro"<<endl;
            std::cout<<"f. Retornar un libro"<<endl;
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
                std::cout<<opt<<". no es una opción válida"<<endl;
                break;
            }
            //end of case for employee
        }else{   //case for client
            std::cout<<"Elija una entre las siguientes opciones: "<<endl;
            std::cout<<"a. Comprar libro"<<endl;
            std::cout<<"b. Alquilar libro"<<endl;
            std::cout<<"c. Retornar un libro"<<endl;
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
                std::cout<<opt<<". no es una opción válida"<<endl;
                break;
            }
            //end of case for client
        }
         ///////////////////////all allowed within system (end)

        //ask again
        std::cout<<"---------------"<<endl<<"Presione 0 si desea salir y cualquier otro nro para continuar con otra acción"<<endl;
        cin>>numAnswer;
        if(numAnswer==0){
            inSystem=false;
        }else{
            inSystem= true;
        }

    }//while para seguir dentro del sistena
    std::cout<<"---------------"<<endl<<"Gracias por preferirnos!"<<endl;

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

void printBooks(booksInfo booksArray[]){
    int count=0;
    while(booksArray[count].bookCode!=""){
        std::cout<<booksArray[count].bookCode<<','<<booksArray[count].bookTitle<<','<<booksArray[count].author<<','<<booksArray[count].genre<<','<<booksArray[count].releaseYear<<','<<booksArray[count].bookPrice<<','<<booksArray[count].rentalPrice<<','<<booksArray[count].bookStatus<<','<<booksArray[count].by<<endl;
        count++;
    }

}

void printUsers(userInfo usersArray[]){
    int count=0;
    while(usersArray[count].name!=""){
        std::cout<<usersArray[count].name<<','<<usersArray[count].lastName<<','<<usersArray[count].username<<','<<usersArray[count].password<<','<<usersArray[count].userStatus<<','<<usersArray[count].userType<<','<<usersArray[count].book<<endl;
        count++;
    }

}
bool repeatedBookCode(string code, booksInfo booksArray[]){  
    int counter=0;
    while(booksArray[counter].bookCode!=""){
        if(code==booksArray[counter].bookCode){
            return true;
        }
        else{
            counter++;
        }
    }
    return false;
}

bool repeatedBookCode(string code, booksInfo booksArray[], int& b){  
    int counter=0;
    while(booksArray[counter].bookCode!=""){
        if(code==booksArray[counter].bookCode){
            b=counter;
            return true;
        }
        else{
            counter++;
            b=-1;
        }
    }
    return false;
}

bool repeatedBook(booksInfo a, booksInfo booksArray[]){ 
    int counter=0;
    //two books are the same if they have equal title, author, genre, year
    while(booksArray[counter].bookCode!=""){
        std::cout<<booksArray[counter].bookCode<<endl;
        if(a.bookTitle==booksArray[counter].bookTitle && a.author==booksArray[counter].author && a.genre==booksArray[counter].genre && a.releaseYear==booksArray[counter].releaseYear){
            return true;
        }
        else{
            counter++;
        }
    }
    return false;
    
}

void movePByOne(booksInfo booksArray[], int a){   //knwoing it has 150 elements at most
    for(int i=a; i<149; i++){
        booksArray[i]=booksArray[i+1];
    }
    booksArray[149]=emptyBook;
    
}