#include<iostream>   
#include<fstream>
#include<sstream>
#include<conio.h>
#include<iomanip>
using namespace std;

struct userInfo{
    string name;
    string lastName;
    string username;
    string password;
    string userStatus; //active o suspended
    string userType; //admin, client, employee
    string book;     //none or bookCode
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

void printByCategory(string, booksInfo[]);

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
    std::cin>>username;
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
            std::cin>>opt;
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
                std::cin>>newName;
                std::cout<<"Apellido: ";
                std::cin>>newLastName;
                do{
                std::cout<<"Nombre de usuario: ";
                std::cin>>newUsername;
                if(repeatedUsername(newUsername,usersList)){
                    std::cout<<"Ingrese un nombre de usuario no repetido"<<endl;
                }
                }while(repeatedUsername(newUsername,usersList));

                std::cout<<"Contraseña: ";
                std::cin>>newPassword;
                newUserStatus="active";  //todo usuario nuevo comineza siendo activo
                do{
                    std::cout<<"Tipo de usuario: ";
                    std::cin>>newUserType;
                    if(newUserType!="client" && newUserType!="employee"){
                        cout<<"El tipo debe ser client o employee"<<endl;
                    }
                }while(newUserType!="client" && newUserType!="employee");
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
                std::cin>>newUsername;
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
                std::cin>>newUsername;
                if(!repeatedUsername( newUsername, usersList, indexToSuspend)){
                    std::cout<<"Ingrese un nombre de usuario válido"<<endl;    
                }                
                }while(!repeatedUsername( newUsername, usersList));

                if(usersList[indexToSuspend].userStatus=="suspended"){
                    std::cout<<endl<<"Ya el usuario se encuentra suspendido"<<endl;
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
                    std::cout<<endl<<"El usuario ha sido suspendido"<<endl;

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
                   std::cin>>newBookCode;
                   if(repeatedBookCode(newBookCode,booksList)){
                    std::cout<<"Codigo no disponible. Ingrese un código nuevo"<<endl;
                   }

                }while(repeatedBookCode(newBookCode,booksList));

                std::cin.ignore();
                std::cout<<"Título del libro: ";
                std::getline(std::cin,newBookTitle);
                std::cout<<"Autor: ";
                std::getline(std::cin,newAuthor);
                do{
                    std::cout<<"Genero: ";
                    std::cin>>newGenre;
                    if(newGenre!="non-fiction" && newGenre!="sci-fiction" && newGenre!="mystery" && newGenre!="romance"){
                        cout<<"Ingrese un genero válido: mystery, non-fiction, sci-fiction o romance"<<endl;
                    }
                }while(newGenre!="non-fiction" && newGenre!="sci-fiction" && newGenre!="mystery" && newGenre!="romance");
                std::cout<<"Año: ";
                std::cin>>newReleaseYear;
                std::cout<<"Precio de compra: ";
                std::cin>>newBookPrice;
                std::cout<<"Precio de alquiler: ";
                std::cin>>newRentalPrice;

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
                string titulo;
                do{
                cout<<"Ingrese el codigo del libro que desea eliminar: ";
                std::cin>>newCode;
                if(!repeatedBookCode(newCode, booksList, indice)){
                    cout<<"Ingrese un codigo existente!"<<endl;
                }
                }while(!repeatedBookCode(newCode, booksList));

                titulo=booksList[indice].bookTitle;
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
                cout<<endl<<"Se ha eliminado el libro: "<<titulo<<endl;
                booksData1.close();

                booksData.open("../assets/booksData.csv");
            }
                /* code */
                break;
            case 'f':                 //modificar libro
            {
                char opt1;
                string newCode;
                int indice;
                do{
                cout<<"Ingrese el codigo del libro que desea modificar: ";
                std::cin>>newCode;
                if(!repeatedBookCode(newCode, booksList, indice)){
                    cout<<"Ingrese un codigo existente!"<<endl;
                }
                }while(!repeatedBookCode(newCode, booksList));
                cout<<"---------------"<<endl;
                cout<<"Modificara el libro "<<booksList[indice].bookTitle<<endl<<endl;
                cout<<"¿Qué desea modificar?"<<endl;
                cout<<"a.bookCode"<<endl<<"b.bookTitle"<<endl<<"c.author"<<endl<<"d.genre"<<endl<<"e.releaseYear"<<endl<<"f.bookPrice"<<endl<<"g.rentalPrice"<<endl<<"h.bookStatus"<<endl<<"i. Quien lo tiene"<<endl;
                std::cin>>opt1;
                switch (opt1)
                {
                case 'a':  //codigo
                {
                    do{
                    cout<<"Ingrese el codigo nuevo: ";
                    std::cin>>newCode;
                    if(repeatedBookCode(newCode, booksList)){
                        cout<<"Ingrese un codigo no repetido!"<<endl;
                    }
                    }while(repeatedBookCode(newCode, booksList));

                    booksList[indice].bookCode=newCode;
                    break;
                }
                case 'b':   //Titulo
                {
                    cout<<"Ingrese el nuevo título:  ";
                    std::cin.ignore();
                    std::getline(std::cin,booksList[indice].bookTitle);
                }
                    break;
                case 'c':    //autor
                {
                    cout<<"Ingrese el autor modificado:  ";
                    std::cin.ignore();
                    std::getline(std::cin,booksList[indice].author);
                }
                    break;
                case 'd':    //genero
                {
                    do{
                        cout<<"Ingrese el genero modificado:  ";
                        std::cin>>booksList[indice].genre;
                        if(booksList[indice].genre!="non-fiction" && booksList[indice].genre!="sci-fiction" && booksList[indice].genre!="mystery" && booksList[indice].genre!="romance"){
                            cout<<"Ingrese un genero válido: mystery, non-fiction, sci-fiction o romance"<<endl;
                        }
                    }while(booksList[indice].genre!="non-fiction" && booksList[indice].genre!="sci-fiction" && booksList[indice].genre!="mystery" && booksList[indice].genre!="romance");
                }
                    break;
                case 'e':     //año
                {
                    cout<<"Ingrese el año modificado:  ";
                    std::cin>>booksList[indice].releaseYear;
                }
                    break;
                case 'f':      //precio del libro
                {
                    cout<<"Ingrese el nuevo precio del libro:  ";
                    std::cin>>booksList[indice].bookPrice;
                }
                    break;
                case 'g':       //renta del libro
                {
                    cout<<"Ingrese el nuevo valor de alquiler:  ";
                    std::cin>>booksList[indice].rentalPrice;
                }
                    break;
                case 'h':        //estado del libro
                {
                    do{
                        cout<<"Ingrese el nuevo estado del libro:  ";
                        std::cin>>booksList[indice].bookStatus;
                    if (booksList[indice].bookStatus!="alquilado" && booksList[indice].bookStatus!="disponible" ){
                        cout<<"Los posibles estados son solo: disponible o alquilado"<<endl;
                    }
                    }while(booksList[indice].bookStatus!="alquilado" && booksList[indice].bookStatus!="disponible" );
                }
                    break;
                case 'i':        //quien lo tiene
                {
                    do{
                    cout<<"Ingrese el nombre de usuario correcto de quien lo alquilo o nadie:  ";
                    std::cin>>booksList[indice].by;
                    if(!repeatedUsername(booksList[indice].by, usersList) && booksList[indice].by!="nadie"){
                        cout<<"Ingrese un nombre de usuario existente o nadie"<<endl;
                    }
                    }while(!repeatedUsername(booksList[indice].by, usersList) && booksList[indice].by!="nadie");

                    if(booksList[indice].by!="nadie"){
                        int index=0;
                        repeatedUsername(booksList[indice].by, usersList, index);
                        usersList[index].book=newCode;

                        usersData.close();                      //cierre el flujo de entrada
                        ofstream usersData1("../assets/usersData.csv");   // abra el de salida
                        int indexToWrite=0;
                        usersData1<<"nombre,lastName,username,password,userStatus,userType,book";
                        while(usersList[indexToWrite].name!=""){
                            usersData1<<endl<<usersList[indexToWrite].name<<','<<usersList[indexToWrite].lastName<<','<<usersList[indexToWrite].username<<','<<usersList[indexToWrite].password<<','<<usersList[indexToWrite].userStatus<<','<<usersList[indexToWrite].userType<<','<<usersList[indexToWrite].book;
                            indexToWrite++;
                        }
                        usersData1.close();                  //close ofstream
                        usersData.open("../assets/usersData.csv");     //open ifstream
                    }
                }
                    break;
                
                default:
                    {std::cout<<opt1<<". no es una opción válida"<<endl;}
                    break;
                }
                booksData.close();
                ofstream booksData1;
                booksData1.open("../assets/booksData.csv");

                booksData1<<"bookCode,bookTitle,author,genre,releaseYear,bookPrice,rentalPrice,bookStatus,by";
                
                int indexToWrite=0;
                while(booksList[indexToWrite].bookCode!=""){
                        booksData1<<endl<<booksList[indexToWrite].bookCode<<','<<booksList[indexToWrite].bookTitle<<','<<booksList[indexToWrite].author<<','<<booksList[indexToWrite].genre<<','<<booksList[indexToWrite].releaseYear<<','<<booksList[indexToWrite].bookPrice<<','<<booksList[indexToWrite].rentalPrice<<','<<booksList[indexToWrite].bookStatus<<','<<booksList[indexToWrite].by;
                        indexToWrite++;
                }                
                cout<<"Libro modificado exitosamente!"<<endl;
                booksData1.close();

                booksData.open("../assets/booksData.csv");
            }      //end of modificar libro
                break;
            case 'g':        //comprar libro
            {
                int indice;
                string genero;
                char opt2;
                string code;
                cout<<"Seleccione el genero que desea de libro"<<endl<<"a.romance"<<endl<<"b.non-fiction"<<endl<<"c.sci-fiction"<<endl<<"d.mystery"<<endl;
                std::cin>>opt2;
                cout<<"---------------"<<endl;
                switch (opt2)
                {
                case 'a':
                    {genero="romance";}
                    break;
                case 'b':
                    {genero="non-fiction";}
                    break;
                case 'c':
                    {genero="sci-fiction";}
                    break;
                case 'd':
                    {genero="mystery";}
                    break;
                
                default:
                    cout<<"Dicho genero no existe "<<endl;
                    genero="no existe";
                    break;
                }

                if(genero=="no existe"){
                    break;
                }
                cout<<"Estos son los libros disponibles para su categoria"<<endl<<"---------------"<<endl;
                printByCategory(genero, booksList);
                cout<<"---------------"<<endl;

                do{
                cout<<"Ingrese el codigo del libro que desea comprar: ";
                std::cin>>code;
                if(!repeatedBookCode(code, booksList, indice)){
                    cout<<"Ingrese un codigo existente!"<<endl;
                }
                }while(!repeatedBookCode(code, booksList));

                //eliminar
                movePByOne(booksList, indice);
                booksData.close();

                ofstream booksData1;
                booksData1.open("../assets/booksData.csv");

                booksData1<<"bookCode,bookTitle,author,genre,releaseYear,bookPrice,rentalPrice,bookStatus,by";
                
                int indexToWrite=0;
                while(booksList[indexToWrite].bookCode!=""){
                        booksData1<<endl<<booksList[indexToWrite].bookCode<<','<<booksList[indexToWrite].bookTitle<<','<<booksList[indexToWrite].author<<','<<booksList[indexToWrite].genre<<','<<booksList[indexToWrite].releaseYear<<','<<booksList[indexToWrite].bookPrice<<','<<booksList[indexToWrite].rentalPrice<<','<<booksList[indexToWrite].bookStatus<<','<<booksList[indexToWrite].by;
                        indexToWrite++;
                }                
                booksData1.close();
                booksData.open("../assets/booksData.csv");

                //eliminar
                cout<<endl<<"Listo!"<<endl<<"Gracias por su adquisición!"<<endl;
                
            }
                /* code */
                break;
            case 'h':        //alquilar libro
            {
                int indice2=0;
                repeatedUsername(actualUser.username,usersList, indice2);
                if(usersList[indice2].book!="none"){
                    cout<<"Usted ya ha alquilado un libro"<<endl;
                    break;
                }

                int indice;
                string genero;
                char opt2;
                string code;
                cout<<"Seleccione el genero que desea de libro"<<endl<<"a.romance"<<endl<<"b.non-fiction"<<endl<<"c.sci-fiction"<<endl<<"d.mystery"<<endl;
                std::cin>>opt2;
                cout<<"---------------"<<endl;
                switch (opt2)
                {
                case 'a':
                    {genero="romance";}
                    break;
                case 'b':
                    {genero="non-fiction";}
                    break;
                case 'c':
                    {genero="sci-fiction";}
                    break;
                case 'd':
                    {genero="mystery";}
                    break;
                
                default:
                    cout<<"Dicho genero no existe "<<endl;
                    genero="no existe";
                    break;
                }

                if(genero=="no existe"){
                    break;
                }
                cout<<"Estos son los libros disponibles para su categoria"<<endl<<"---------------"<<endl;
                printByCategory(genero, booksList);
                cout<<"---------------"<<endl;

                do{
                cout<<"Ingrese el codigo del libro que desea alquilar: ";
                std::cin>>code;
                if(!repeatedBookCode(code, booksList, indice)){
                    cout<<"Ingrese un codigo existente!"<<endl;
                }
                }while(!repeatedBookCode(code, booksList));

                // modifica en lista(Book), file, usuario
                booksList[indice].bookStatus="alquilado";   //lista
                booksList[indice].by=actualUser.username;

                booksData.close();   

                //escribe en file books
                ofstream booksData1;
                booksData1.open("../assets/booksData.csv");

                booksData1<<"bookCode,bookTitle,author,genre,releaseYear,bookPrice,rentalPrice,bookStatus,by";
                
                int indexToWrite=0;
                while(booksList[indexToWrite].bookCode!=""){
                        booksData1<<endl<<booksList[indexToWrite].bookCode<<','<<booksList[indexToWrite].bookTitle<<','<<booksList[indexToWrite].author<<','<<booksList[indexToWrite].genre<<','<<booksList[indexToWrite].releaseYear<<','<<booksList[indexToWrite].bookPrice<<','<<booksList[indexToWrite].rentalPrice<<','<<booksList[indexToWrite].bookStatus<<','<<booksList[indexToWrite].by;
                        indexToWrite++;
                }                
                booksData1.close();
                booksData.open("../assets/booksData.csv");
                // termina en files book

                //escribe en usuario
                int indice1=0;
                repeatedUsername(actualUser.username,usersList, indice1);

                usersList[indice1].book=code;
                
                //escribe en file usuarios
                usersData.close();                      //cierre el flujo de entrada

                ofstream usersData1("../assets/usersData.csv");   // abra el de salida

                indexToWrite=0;
                usersData1<<"nombre,lastName,username,password,userStatus,userType,book";
                while(usersList[indexToWrite].name!=""){
                    usersData1<<endl<<usersList[indexToWrite].name<<','<<usersList[indexToWrite].lastName<<','<<usersList[indexToWrite].username<<','<<usersList[indexToWrite].password<<','<<usersList[indexToWrite].userStatus<<','<<usersList[indexToWrite].userType<<','<<usersList[indexToWrite].book;
                    indexToWrite++;
                }
                usersData1.close();                  //close ofstream
                usersData.open("../assets/usersData.csv");     //open ifstream
                //termina file usuarios
                // modificaciones
                cout<<"Alquiler procesado"<<endl;
            }
                break;
            case 'i':        //retornar libro de mi parte
                {
                    string code;
                    int indice3;    //indice para el libro
                    int indice4;    //indice para la persona
                    cout<<"Ingrese el codigo del libro que desea retornar: ";
                    std::cin>>code;
                    if(!repeatedBookCode(code, booksList, indice3)){
                        cout<<"El codigo no corresponde a ningún libro"<<endl;
                        break;                                                // el codigo esta
                    }else if(booksList[indice3].bookStatus=="disponible"){   //si se da que corresponde a un libro ya disponible
                            cout<<"El libro ya esta devuelto"<<endl;
                            break;
                    }else if(booksList[indice3].by!=actualUser.username){   // el libro en efecto esta alquilado
                        cout<<"El libro solo lo puede devolver la persona que lo alquilo"<<endl;    //corresponde a alguien que no soy yo
                        break;
                    }else{                                                   //el libro esta alquilado por mi

                    booksList[indice3].bookStatus="disponible";      //cambia estado de libro
                    booksList[indice3].by="nadie"; 
                    repeatedUsername(actualUser.username,usersList,indice4 );
                    usersList[indice4].book="none";

                    //escribelos

                    booksData.close();   
                    //escribe en file books
                    ofstream booksData1;
                    booksData1.open("../assets/booksData.csv");

                    booksData1<<"bookCode,bookTitle,author,genre,releaseYear,bookPrice,rentalPrice,bookStatus,by";
                
                    int indexToWrite=0;
                    while(booksList[indexToWrite].bookCode!=""){
                            booksData1<<endl<<booksList[indexToWrite].bookCode<<','<<booksList[indexToWrite].bookTitle<<','<<booksList[indexToWrite].author<<','<<booksList[indexToWrite].genre<<','<<booksList[indexToWrite].releaseYear<<','<<booksList[indexToWrite].bookPrice<<','<<booksList[indexToWrite].rentalPrice<<','<<booksList[indexToWrite].bookStatus<<','<<booksList[indexToWrite].by;
                            indexToWrite++;
                    }                
                    booksData1.close();
                    booksData.open("../assets/booksData.csv");

                    //en file users
                    usersData.close();                      //cierre el flujo de entrada

                    ofstream usersData1("../assets/usersData.csv");   // abra el de salida

                    indexToWrite=0;
                    usersData1<<"nombre,lastName,username,password,userStatus,userType,book";
                    while(usersList[indexToWrite].name!=""){
                        usersData1<<endl<<usersList[indexToWrite].name<<','<<usersList[indexToWrite].lastName<<','<<usersList[indexToWrite].username<<','<<usersList[indexToWrite].password<<','<<usersList[indexToWrite].userStatus<<','<<usersList[indexToWrite].userType<<','<<usersList[indexToWrite].book;
                        indexToWrite++;
                    }
                    usersData1.close();                  //close ofstream
                    usersData.open("../assets/usersData.csv");     //open ifstream
                    cout<<"Libro retornado con exito!"<<endl;
                    }
                }
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
            std::cin>>opt;
            switch (opt)
            {
            case 'a':      //añadir libro
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
                   std::cin>>newBookCode;
                   if(repeatedBookCode(newBookCode,booksList)){
                    std::cout<<"Codigo no disponible. Ingrese un código nuevo"<<endl;
                   }

                }while(repeatedBookCode(newBookCode,booksList));

                std::cin.ignore();
                std::cout<<"Título del libro: ";
                std::getline(std::cin,newBookTitle);
                std::cout<<"Autor: ";
                std::getline(std::cin,newAuthor);
                do{
                    std::cout<<"Genero: ";
                    std::cin>>newGenre;
                    if(newGenre!="non-fiction" && newGenre!="sci-fiction" && newGenre!="mystery" && newGenre!="romance"){
                        cout<<"Ingrese un genero válido: mystery, non-fiction, sci-fiction o romance"<<endl;
                    }
                }while(newGenre!="non-fiction" && newGenre!="sci-fiction" && newGenre!="mystery" && newGenre!="romance");
                std::cout<<"Año: ";
                std::cin>>newReleaseYear;
                std::cout<<"Precio de compra: ";
                std::cin>>newBookPrice;
                std::cout<<"Precio de alquiler: ";
                std::cin>>newRentalPrice;

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
            case 'b':                  //eliminar libro
            {
                int indice;
                string newCode;
                string titulo;
                do{
                cout<<"Ingrese el codigo del libro que desea eliminar: ";
                std::cin>>newCode;
                if(!repeatedBookCode(newCode, booksList, indice)){
                    cout<<"Ingrese un codigo existente!"<<endl;
                }
                }while(!repeatedBookCode(newCode, booksList));

                titulo=booksList[indice].bookTitle;
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
                cout<<endl<<"Se ha eliminado el libro: "<<titulo<<endl;
                booksData1.close();

                booksData.open("../assets/booksData.csv");
            }
                /* code */
                break;
            case 'c':                 //modificar libro
            {
                char opt1;
                string newCode;
                int indice;
                do{
                cout<<"Ingrese el codigo del libro que desea modificar: ";
                std::cin>>newCode;
                if(!repeatedBookCode(newCode, booksList, indice)){
                    cout<<"Ingrese un codigo existente!"<<endl;
                }
                }while(!repeatedBookCode(newCode, booksList));
                cout<<"---------------"<<endl;
                cout<<"Modificara el libro "<<booksList[indice].bookTitle<<endl<<endl;
                cout<<"¿Qué desea modificar?"<<endl;
                cout<<"a.bookCode"<<endl<<"b.bookTitle"<<endl<<"c.author"<<endl<<"d.genre"<<endl<<"e.releaseYear"<<endl<<"f.bookPrice"<<endl<<"g.rentalPrice"<<endl<<"h.bookStatus"<<endl<<"i. Quien lo tiene"<<endl;
                std::cin>>opt1;
                switch (opt1)
                {
                case 'a':  //codigo
                {
                    do{
                    cout<<"Ingrese el codigo nuevo: ";
                    std::cin>>newCode;
                    if(repeatedBookCode(newCode, booksList)){
                        cout<<"Ingrese un codigo no repetido!"<<endl;
                    }
                    }while(repeatedBookCode(newCode, booksList));

                    booksList[indice].bookCode=newCode;
                    break;
                }
                case 'b':   //Titulo
                {
                    cout<<"Ingrese el nuevo título:  ";
                    std::cin.ignore();
                    std::getline(std::cin,booksList[indice].bookTitle);
                }
                    break;
                case 'c':    //autor
                {
                    cout<<"Ingrese el autor modificado:  ";
                    std::cin.ignore();
                    std::getline(std::cin,booksList[indice].author);
                }
                    break;
                case 'd':    //genero
                {
                    do{
                        cout<<"Ingrese el genero modificado:  ";
                        std::cin>>booksList[indice].genre;
                        if(booksList[indice].genre!="non-fiction" && booksList[indice].genre!="sci-fiction" && booksList[indice].genre!="mystery" && booksList[indice].genre!="romance"){
                            cout<<"Ingrese un genero válido: mystery, non-fiction, sci-fiction o romance"<<endl;
                        }
                    }while(booksList[indice].genre!="non-fiction" && booksList[indice].genre!="sci-fiction" && booksList[indice].genre!="mystery" && booksList[indice].genre!="romance");
                }
                    break;
                case 'e':     //año
                {
                    cout<<"Ingrese el año modificado:  ";
                    std::cin>>booksList[indice].releaseYear;
                }
                    break;
                case 'f':      //precio del libro
                {
                    cout<<"Ingrese el nuevo precio del libro:  ";
                    std::cin>>booksList[indice].bookPrice;
                }
                    break;
                case 'g':       //renta del libro
                {
                    cout<<"Ingrese el nuevo valor de alquiler:  ";
                    std::cin>>booksList[indice].rentalPrice;
                }
                    break;
                case 'h':        //estado del libro
                {
                    do{
                        cout<<"Ingrese el nuevo estado del libro:  ";
                        std::cin>>booksList[indice].bookStatus;
                    if (booksList[indice].bookStatus!="alquilado" && booksList[indice].bookStatus!="disponible" ){
                        cout<<"Los posibles estados son solo: disponible o alquilado"<<endl;
                    }
                    }while(booksList[indice].bookStatus!="alquilado" && booksList[indice].bookStatus!="disponible" );
                }
                    break;
                case 'i':        //quien lo tiene
                {
                    do{
                    cout<<"Ingrese el nombre de usuario correcto de quien lo alquilo o nadie:  ";
                    std::cin>>booksList[indice].by;
                    if(!repeatedUsername(booksList[indice].by, usersList) && booksList[indice].by!="nadie"){
                        cout<<"Ingrese un nombre de usuario existente o nadie"<<endl;
                    }
                    }while(!repeatedUsername(booksList[indice].by, usersList) && booksList[indice].by!="nadie");

                    if(booksList[indice].by!="nadie"){
                        int index=0;
                        repeatedUsername(booksList[indice].by, usersList, index);
                        usersList[index].book=newCode;

                        usersData.close();                      //cierre el flujo de entrada
                        ofstream usersData1("../assets/usersData.csv");   // abra el de salida
                        int indexToWrite=0;
                        usersData1<<"nombre,lastName,username,password,userStatus,userType,book";
                        while(usersList[indexToWrite].name!=""){
                            usersData1<<endl<<usersList[indexToWrite].name<<','<<usersList[indexToWrite].lastName<<','<<usersList[indexToWrite].username<<','<<usersList[indexToWrite].password<<','<<usersList[indexToWrite].userStatus<<','<<usersList[indexToWrite].userType<<','<<usersList[indexToWrite].book;
                            indexToWrite++;
                        }
                        usersData1.close();                  //close ofstream
                        usersData.open("../assets/usersData.csv");     //open ifstream
                    }
                }
                    break;
                
                default:
                    {std::cout<<opt1<<". no es una opción válida"<<endl;}
                    break;
                }
                booksData.close();
                ofstream booksData1;
                booksData1.open("../assets/booksData.csv");

                booksData1<<"bookCode,bookTitle,author,genre,releaseYear,bookPrice,rentalPrice,bookStatus,by";
                
                int indexToWrite=0;
                while(booksList[indexToWrite].bookCode!=""){
                        booksData1<<endl<<booksList[indexToWrite].bookCode<<','<<booksList[indexToWrite].bookTitle<<','<<booksList[indexToWrite].author<<','<<booksList[indexToWrite].genre<<','<<booksList[indexToWrite].releaseYear<<','<<booksList[indexToWrite].bookPrice<<','<<booksList[indexToWrite].rentalPrice<<','<<booksList[indexToWrite].bookStatus<<','<<booksList[indexToWrite].by;
                        indexToWrite++;
                }                
                cout<<"Libro modificado exitosamente!"<<endl;
                booksData1.close();

                booksData.open("../assets/booksData.csv");
            }      //end of modificar libro
                break;
            case 'd':        //comprar libro
            {
                int indice;
                string genero;
                char opt2;
                string code;
                cout<<"Seleccione el genero que desea de libro"<<endl<<"a.romance"<<endl<<"b.non-fiction"<<endl<<"c.sci-fiction"<<endl<<"d.mystery"<<endl;
                std::cin>>opt2;
                cout<<"---------------"<<endl;
                switch (opt2)
                {
                case 'a':
                    {genero="romance";}
                    break;
                case 'b':
                    {genero="non-fiction";}
                    break;
                case 'c':
                    {genero="sci-fiction";}
                    break;
                case 'd':
                    {genero="mystery";}
                    break;
                
                default:
                    cout<<"Dicho genero no existe "<<endl;
                    genero="no existe";
                    break;
                }

                if(genero=="no existe"){
                    break;
                }
                cout<<"Estos son los libros disponibles para su categoria"<<endl<<"---------------"<<endl;
                printByCategory(genero, booksList);
                cout<<"---------------"<<endl;

                do{
                cout<<"Ingrese el codigo del libro que desea comprar: ";
                std::cin>>code;
                if(!repeatedBookCode(code, booksList, indice)){
                    cout<<"Ingrese un codigo existente!"<<endl;
                }
                }while(!repeatedBookCode(code, booksList));

                //eliminar
                movePByOne(booksList, indice);
                booksData.close();

                ofstream booksData1;
                booksData1.open("../assets/booksData.csv");

                booksData1<<"bookCode,bookTitle,author,genre,releaseYear,bookPrice,rentalPrice,bookStatus,by";
                
                int indexToWrite=0;
                while(booksList[indexToWrite].bookCode!=""){
                        booksData1<<endl<<booksList[indexToWrite].bookCode<<','<<booksList[indexToWrite].bookTitle<<','<<booksList[indexToWrite].author<<','<<booksList[indexToWrite].genre<<','<<booksList[indexToWrite].releaseYear<<','<<booksList[indexToWrite].bookPrice<<','<<booksList[indexToWrite].rentalPrice<<','<<booksList[indexToWrite].bookStatus<<','<<booksList[indexToWrite].by;
                        indexToWrite++;
                }                
                booksData1.close();
                booksData.open("../assets/booksData.csv");

                //eliminar
                cout<<endl<<"Listo!"<<endl<<"Gracias por su adquisición!"<<endl;
                
            }
                /* code */
                break;
            case 'e':        //alquilar libro
            {
                int indice2=0;
                repeatedUsername(actualUser.username,usersList, indice2);
                if(usersList[indice2].book!="none"){
                    cout<<"Usted ya ha alquilado un libro"<<endl;
                    break;
                }

                int indice;
                string genero;
                char opt2;
                string code;
                cout<<"Seleccione el genero que desea de libro"<<endl<<"a.romance"<<endl<<"b.non-fiction"<<endl<<"c.sci-fiction"<<endl<<"d.mystery"<<endl;
                std::cin>>opt2;
                cout<<"---------------"<<endl;
                switch (opt2)
                {
                case 'a':
                    {genero="romance";}
                    break;
                case 'b':
                    {genero="non-fiction";}
                    break;
                case 'c':
                    {genero="sci-fiction";}
                    break;
                case 'd':
                    {genero="mystery";}
                    break;
                
                default:
                    cout<<"Dicho genero no existe "<<endl;
                    genero="no existe";
                    break;
                }

                if(genero=="no existe"){
                    break;
                }
                cout<<"Estos son los libros disponibles para su categoria"<<endl<<"---------------"<<endl;
                printByCategory(genero, booksList);
                cout<<"---------------"<<endl;

                do{
                cout<<"Ingrese el codigo del libro que desea alquilar: ";
                std::cin>>code;
                if(!repeatedBookCode(code, booksList, indice)){
                    cout<<"Ingrese un codigo existente!"<<endl;
                }
                }while(!repeatedBookCode(code, booksList));

                // modifica en lista(Book), file, usuario
                booksList[indice].bookStatus="alquilado";   //lista
                booksList[indice].by=actualUser.username;

                booksData.close();   

                //escribe en file books
                ofstream booksData1;
                booksData1.open("../assets/booksData.csv");

                booksData1<<"bookCode,bookTitle,author,genre,releaseYear,bookPrice,rentalPrice,bookStatus,by";
                
                int indexToWrite=0;
                while(booksList[indexToWrite].bookCode!=""){
                        booksData1<<endl<<booksList[indexToWrite].bookCode<<','<<booksList[indexToWrite].bookTitle<<','<<booksList[indexToWrite].author<<','<<booksList[indexToWrite].genre<<','<<booksList[indexToWrite].releaseYear<<','<<booksList[indexToWrite].bookPrice<<','<<booksList[indexToWrite].rentalPrice<<','<<booksList[indexToWrite].bookStatus<<','<<booksList[indexToWrite].by;
                        indexToWrite++;
                }                
                booksData1.close();
                booksData.open("../assets/booksData.csv");
                // termina en files book

                //escribe en usuario
                int indice1=0;
                repeatedUsername(actualUser.username,usersList, indice1);

                usersList[indice1].book=code;
                
                //escribe en file usuarios
                usersData.close();                      //cierre el flujo de entrada

                ofstream usersData1("../assets/usersData.csv");   // abra el de salida

                indexToWrite=0;
                usersData1<<"nombre,lastName,username,password,userStatus,userType,book";
                while(usersList[indexToWrite].name!=""){
                    usersData1<<endl<<usersList[indexToWrite].name<<','<<usersList[indexToWrite].lastName<<','<<usersList[indexToWrite].username<<','<<usersList[indexToWrite].password<<','<<usersList[indexToWrite].userStatus<<','<<usersList[indexToWrite].userType<<','<<usersList[indexToWrite].book;
                    indexToWrite++;
                }
                usersData1.close();                  //close ofstream
                usersData.open("../assets/usersData.csv");     //open ifstream
                //termina file usuarios
                // modificaciones
                cout<<"Alquiler procesado"<<endl;
            }
                break;
            case 'f':        //retornar libro de mi parte
                {
                    string code;
                    int indice3;    //indice para el libro
                    int indice4;    //indice para la persona
                    cout<<"Ingrese el codigo del libro que desea retornar: ";
                    std::cin>>code;
                    if(!repeatedBookCode(code, booksList, indice3)){
                        cout<<"El codigo no corresponde a ningún libro"<<endl;
                        break;                                                // el codigo esta
                    }else if(booksList[indice3].bookStatus=="disponible"){   //si se da que corresponde a un libro ya disponible
                            cout<<"El libro ya esta devuelto"<<endl;
                            break;
                    }else if(booksList[indice3].by!=actualUser.username){   // el libro en efecto esta alquilado
                        cout<<"El libro solo lo puede devolver la persona que lo alquilo"<<endl;    //corresponde a alguien que no soy yo
                        break;
                    }else{                                                   //el libro esta alquilado por mi

                    booksList[indice3].bookStatus="disponible";      //cambia estado de libro
                    booksList[indice3].by="nadie"; 
                    repeatedUsername(actualUser.username,usersList,indice4 );
                    usersList[indice4].book="none";

                    //escribelos

                    booksData.close();   
                    //escribe en file books
                    ofstream booksData1;
                    booksData1.open("../assets/booksData.csv");

                    booksData1<<"bookCode,bookTitle,author,genre,releaseYear,bookPrice,rentalPrice,bookStatus,by";
                
                    int indexToWrite=0;
                    while(booksList[indexToWrite].bookCode!=""){
                            booksData1<<endl<<booksList[indexToWrite].bookCode<<','<<booksList[indexToWrite].bookTitle<<','<<booksList[indexToWrite].author<<','<<booksList[indexToWrite].genre<<','<<booksList[indexToWrite].releaseYear<<','<<booksList[indexToWrite].bookPrice<<','<<booksList[indexToWrite].rentalPrice<<','<<booksList[indexToWrite].bookStatus<<','<<booksList[indexToWrite].by;
                            indexToWrite++;
                    }                
                    booksData1.close();
                    booksData.open("../assets/booksData.csv");

                    //en file users
                    usersData.close();                      //cierre el flujo de entrada

                    ofstream usersData1("../assets/usersData.csv");   // abra el de salida

                    indexToWrite=0;
                    usersData1<<"nombre,lastName,username,password,userStatus,userType,book";
                    while(usersList[indexToWrite].name!=""){
                        usersData1<<endl<<usersList[indexToWrite].name<<','<<usersList[indexToWrite].lastName<<','<<usersList[indexToWrite].username<<','<<usersList[indexToWrite].password<<','<<usersList[indexToWrite].userStatus<<','<<usersList[indexToWrite].userType<<','<<usersList[indexToWrite].book;
                        indexToWrite++;
                    }
                    usersData1.close();                  //close ofstream
                    usersData.open("../assets/usersData.csv");     //open ifstream
                    cout<<"Libro retornado con exito!"<<endl;
                    }
                }
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
            std::cin>>opt;
            switch (opt)
            {
            case 'a':        //comprar libro
            {
                int indice;
                string genero;
                char opt2;
                string code;
                cout<<"Seleccione el genero que desea de libro"<<endl<<"a.romance"<<endl<<"b.non-fiction"<<endl<<"c.sci-fiction"<<endl<<"d.mystery"<<endl;
                std::cin>>opt2;
                cout<<"---------------"<<endl;
                switch (opt2)
                {
                case 'a':
                    {genero="romance";}
                    break;
                case 'b':
                    {genero="non-fiction";}
                    break;
                case 'c':
                    {genero="sci-fiction";}
                    break;
                case 'd':
                    {genero="mystery";}
                    break;
                
                default:
                    cout<<"Dicho genero no existe "<<endl;
                    genero="no existe";
                    break;
                }

                if(genero=="no existe"){
                    break;
                }
                cout<<"Estos son los libros disponibles para su categoria"<<endl<<"---------------"<<endl;
                printByCategory(genero, booksList);
                cout<<"---------------"<<endl;

                do{
                cout<<"Ingrese el codigo del libro que desea comprar: ";
                std::cin>>code;
                if(!repeatedBookCode(code, booksList, indice)){
                    cout<<"Ingrese un codigo existente!"<<endl;
                }
                }while(!repeatedBookCode(code, booksList));

                //eliminar
                movePByOne(booksList, indice);
                booksData.close();

                ofstream booksData1;
                booksData1.open("../assets/booksData.csv");

                booksData1<<"bookCode,bookTitle,author,genre,releaseYear,bookPrice,rentalPrice,bookStatus,by";
                
                int indexToWrite=0;
                while(booksList[indexToWrite].bookCode!=""){
                        booksData1<<endl<<booksList[indexToWrite].bookCode<<','<<booksList[indexToWrite].bookTitle<<','<<booksList[indexToWrite].author<<','<<booksList[indexToWrite].genre<<','<<booksList[indexToWrite].releaseYear<<','<<booksList[indexToWrite].bookPrice<<','<<booksList[indexToWrite].rentalPrice<<','<<booksList[indexToWrite].bookStatus<<','<<booksList[indexToWrite].by;
                        indexToWrite++;
                }                
                booksData1.close();
                booksData.open("../assets/booksData.csv");

                //eliminar
                cout<<endl<<"Listo!"<<endl<<"Gracias por su adquisición!"<<endl;
                
            }
                /* code */
                break;
            case 'b':        //alquilar libro
            {
                int indice2=0;
                repeatedUsername(actualUser.username,usersList, indice2);
                if(usersList[indice2].book!="none"){
                    cout<<"Usted ya ha alquilado un libro"<<endl;
                    break;
                }

                int indice;
                string genero;
                char opt2;
                string code;
                cout<<"Seleccione el genero que desea de libro"<<endl<<"a.romance"<<endl<<"b.non-fiction"<<endl<<"c.sci-fiction"<<endl<<"d.mystery"<<endl;
                std::cin>>opt2;
                cout<<"---------------"<<endl;
                switch (opt2)
                {
                case 'a':
                    {genero="romance";}
                    break;
                case 'b':
                    {genero="non-fiction";}
                    break;
                case 'c':
                    {genero="sci-fiction";}
                    break;
                case 'd':
                    {genero="mystery";}
                    break;
                
                default:
                    cout<<"Dicho genero no existe "<<endl;
                    genero="no existe";
                    break;
                }

                if(genero=="no existe"){
                    break;
                }
                cout<<"Estos son los libros disponibles para su categoria"<<endl<<"---------------"<<endl;
                printByCategory(genero, booksList);
                cout<<"---------------"<<endl;

                do{
                cout<<"Ingrese el codigo del libro que desea alquilar: ";
                std::cin>>code;
                if(!repeatedBookCode(code, booksList, indice)){
                    cout<<"Ingrese un codigo existente!"<<endl;
                }
                }while(!repeatedBookCode(code, booksList));

                // modifica en lista(Book), file, usuario
                booksList[indice].bookStatus="alquilado";   //lista
                booksList[indice].by=actualUser.username;

                booksData.close();   

                //escribe en file books
                ofstream booksData1;
                booksData1.open("../assets/booksData.csv");

                booksData1<<"bookCode,bookTitle,author,genre,releaseYear,bookPrice,rentalPrice,bookStatus,by";
                
                int indexToWrite=0;
                while(booksList[indexToWrite].bookCode!=""){
                        booksData1<<endl<<booksList[indexToWrite].bookCode<<','<<booksList[indexToWrite].bookTitle<<','<<booksList[indexToWrite].author<<','<<booksList[indexToWrite].genre<<','<<booksList[indexToWrite].releaseYear<<','<<booksList[indexToWrite].bookPrice<<','<<booksList[indexToWrite].rentalPrice<<','<<booksList[indexToWrite].bookStatus<<','<<booksList[indexToWrite].by;
                        indexToWrite++;
                }                
                booksData1.close();
                booksData.open("../assets/booksData.csv");
                // termina en files book

                //escribe en usuario
                int indice1=0;
                repeatedUsername(actualUser.username,usersList, indice1);

                usersList[indice1].book=code;
                
                //escribe en file usuarios
                usersData.close();                      //cierre el flujo de entrada

                ofstream usersData1("../assets/usersData.csv");   // abra el de salida

                indexToWrite=0;
                usersData1<<"nombre,lastName,username,password,userStatus,userType,book";
                while(usersList[indexToWrite].name!=""){
                    usersData1<<endl<<usersList[indexToWrite].name<<','<<usersList[indexToWrite].lastName<<','<<usersList[indexToWrite].username<<','<<usersList[indexToWrite].password<<','<<usersList[indexToWrite].userStatus<<','<<usersList[indexToWrite].userType<<','<<usersList[indexToWrite].book;
                    indexToWrite++;
                }
                usersData1.close();                  //close ofstream
                usersData.open("../assets/usersData.csv");     //open ifstream
                //termina file usuarios
                // modificaciones
                cout<<"Alquiler procesado"<<endl;
            }
                break;
            case 'i':        //retornar libro de mi parte
                {
                    string code;
                    int indice3;    //indice para el libro
                    int indice4;    //indice para la persona
                    cout<<"Ingrese el codigo del libro que desea retornar: ";
                    std::cin>>code;
                    if(!repeatedBookCode(code, booksList, indice3)){
                        cout<<"El codigo no corresponde a ningún libro"<<endl;
                        break;                                                // el codigo esta
                    }else if(booksList[indice3].bookStatus=="disponible"){   //si se da que corresponde a un libro ya disponible
                            cout<<"El libro ya esta devuelto"<<endl;
                            break;
                    }else if(booksList[indice3].by!=actualUser.username){   // el libro en efecto esta alquilado
                        cout<<"El libro solo lo puede devolver la persona que lo alquilo"<<endl;    //corresponde a alguien que no soy yo
                        break;
                    }else{                                                   //el libro esta alquilado por mi

                    booksList[indice3].bookStatus="disponible";      //cambia estado de libro
                    booksList[indice3].by="nadie"; 
                    repeatedUsername(actualUser.username,usersList,indice4 );
                    usersList[indice4].book="none";

                    //escribelos

                    booksData.close();   
                    //escribe en file books
                    ofstream booksData1;
                    booksData1.open("../assets/booksData.csv");

                    booksData1<<"bookCode,bookTitle,author,genre,releaseYear,bookPrice,rentalPrice,bookStatus,by";
                
                    int indexToWrite=0;
                    while(booksList[indexToWrite].bookCode!=""){
                            booksData1<<endl<<booksList[indexToWrite].bookCode<<','<<booksList[indexToWrite].bookTitle<<','<<booksList[indexToWrite].author<<','<<booksList[indexToWrite].genre<<','<<booksList[indexToWrite].releaseYear<<','<<booksList[indexToWrite].bookPrice<<','<<booksList[indexToWrite].rentalPrice<<','<<booksList[indexToWrite].bookStatus<<','<<booksList[indexToWrite].by;
                            indexToWrite++;
                    }                
                    booksData1.close();
                    booksData.open("../assets/booksData.csv");

                    //en file users
                    usersData.close();                      //cierre el flujo de entrada

                    ofstream usersData1("../assets/usersData.csv");   // abra el de salida

                    indexToWrite=0;
                    usersData1<<"nombre,lastName,username,password,userStatus,userType,book";
                    while(usersList[indexToWrite].name!=""){
                        usersData1<<endl<<usersList[indexToWrite].name<<','<<usersList[indexToWrite].lastName<<','<<usersList[indexToWrite].username<<','<<usersList[indexToWrite].password<<','<<usersList[indexToWrite].userStatus<<','<<usersList[indexToWrite].userType<<','<<usersList[indexToWrite].book;
                        indexToWrite++;
                    }
                    usersData1.close();                  //close ofstream
                    usersData.open("../assets/usersData.csv");     //open ifstream
                    cout<<"Libro retornado con exito!"<<endl;
                    }
                }
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
        std::cin>>numAnswer;
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
        std::getline(file,lineInfo);

        if(lineInfo.substr(0,6)=="nombre"){
            continue;
        }

        s<<lineInfo;
        std::getline(s, user.name, ',');
        std::getline(s, user.lastName, ',');
        std::getline(s, user.username, ',');
        std::getline(s, user.password, ',');
        std::getline(s, user.userStatus, ',');
        std::getline(s, user.userType, ',');
        std::getline(s, user.book, ',');

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
        
        std::getline(file, lineInfo);
        if(lineInfo.substr(0,8)=="bookCode"){
            continue;
        }
        v<<lineInfo;

        std::getline(v, newBook.bookCode, ',');
        std::getline(v, newBook.bookTitle, ',');
        std::getline(v, newBook.author, ',');
        std::getline(v, newBook.genre, ',');
        std::getline(v, newBook.releaseYear, ',');
        std::getline(v, newBook.bookPrice, ',');
        std::getline(v, newBook.rentalPrice, ',');
        std::getline(v, newBook.bookStatus, ',');
        std::getline(v, newBook.by, ',');

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
void printByCategory(string category, booksInfo booksArray[]){

    int counter =0;
    std::cout<<setw(8)<<"bookCode"<<','<<setw(39)<<"bookTitle"<<','<<setw(22)<<"author"<<','<<setw(12)<<"genre"<<','<<setw(12)<<"releaseYear"<<','<<setw(10)<<"bookPrice"<<','<<setw(12)<<"rentalPrice"<<','<<setw(12)<<"bookStatus"<<endl;

    while(booksArray[counter].bookCode!=""){        
        if(booksArray[counter].genre==category){
            std::cout<<setw(8)<<booksArray[counter].bookCode<<','<<setw(39)<<booksArray[counter].bookTitle<<','<<setw(22)<<booksArray[counter].author<<','<<setw(12)<<booksArray[counter].genre<<','<<setw(12)<<booksArray[counter].releaseYear<<','<<setw(10)<<booksArray[counter].bookPrice<<','<<setw(12)<<booksArray[counter].rentalPrice<<','<<setw(12)<<booksArray[counter].bookStatus<<endl;
        }
        counter++;
    }

}