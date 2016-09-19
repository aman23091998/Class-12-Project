#include<stdio.h>
#include<fstream.h>
#include<conio.h>
#include<string.h>
#include<dos.h>
#include<stdlib.h>
#include<process.h>
#include<iomanip.h>

//macros
#define ld long double
#define ll long long
#define F(i,a,b) for(int i=(int)a;i<(int)b;++i)
#define g(a,b) gotoxy((int)a,(int)b)
typedef char string[101];

// global defaults
string loggedInUser = {NULL} ;
int isLoggedIn = 0 ;
int theme =  177 ;

//file handlers
ofstream fout;
ifstream fin;

//prototypes
class device;
class user;
void apilist();
void layout( char r = theme );
void getPassword( int , string );
void addDevice();
void outputDeviceDetails( ll , string );
ll countDevices();
void swapDevices ( device &device1 , device &device2 );
void sort(string);
void layoutMenu();
void getPassword(int , string ) ;
ll findDeviceDetails ( string type , string required );
ll findUserDetails ( string type , string required );
void modifyStock ( ll pos , int modification = -1 ) ;
void userSignIn();
void userSignUp();
void deleteFile(string type);
void displayAllDevices( string fileName);
void buyDevice();
void removeDevice(int);
void deleteFile(string filename);
void userMenu();
void sortMenu();
void companyMenu();
void mainMenu();
void boot();
void project();


void main (){
	project();
	getch();

}

class user {
	 string name , username  , password ;
	 public :
	 void signIn(){
			layout();
			g(10,7);cout<<"Enter Username ";
			g(10,8);gets(username) ;
			g(10,10);cout<<"Enter Password";
			g(10,11);getPassword ( 11 ,password ) ;
			}
		void signUp(){
			layout();
			g(10,7);cout<<"Enter Name";
			g(10,8);gets(name) ;
			g(10,10);cout<<"Enter Username ";
			g(10,11);gets(username) ;
			g(10,13);cout<<"Enter Password";
			g(10,14);getPassword ( 14 , password ) ;
		}
		char* getData( string req ){
			if(strcmpi ( req , "name" ) == 0 )return name ;
			else if(strcmpi ( req , "password" ) == 0 )return password ;
			else return username ;
		}

		int validatePassword(char* pass){
			return strcmp(pass,password);
		}
};

void getPassword(int y , string pass ){
	string password = {NULL} ;
	int k = 0;
	unsigned char m;
		do{
			m=getch();
			if(m=='\b' && 10+k >10){
				k--;
				g(10+k,y);cout<<" ";
				g(10+k,y);
				password[k]='\0';
			}
			else if(m!='\b' && m!='\r'){
				g(10+k,y);cout<<"*";
				password[k]=m;
				k++;
				g(10+k,y);
			}
		    else if(m=='\r' && strlen(password)==0){
				g(10,y);cout<<"                                        ";
				k=0;
				g(10,y+1);cout<<"Please enter a valid password.Hit a key to continue";
				getch();
				g(10,y+1);cout<<"                                                       ";
				g(10,y);
			}
		}while(m!='\r' || strlen(password)==0);
		strcpy(pass , password ) ;
}

class device {
	 string name , codename , vendor ;
	 //company , device and manufacturer's name ; codename is a specific name of a device so as distinguish it from all  other devices and even its own variants
	 ll quantity , api , price ;
	 //API is application program inteface-each api level has a link to an Android version

	public :
	void input(){
		clrscr();
		layout();
		g(20,4);cout<<"ADD NEW DEVICE\n";
		g(20,7);cout<<"Name of device\n";
		g(20,8);gets(name);
		g(20,9);cout<<"Enter Code Name\n";
		g(20,10);gets(codename);
		g(20,11);cout<<"Enter Vendor\n";
		g(20,12);gets(vendor);
		layout();
		g(20,5);cout<<"Enter Android Version\n";
		g(20,6);apilist();
		g(20,10);cout<<"Enter the api level";
		g(20,11);cin>>api;
		g(20,12);cout<<"Enter number of device(s) added to stock\n"   ;
		g(20,13);cin>>quantity;
		g(20,14);cout<<"Enter Price\n"     ;
		g(20,15);cin>>price;
	}

	void output(){
	clrscr();
	layout();
	g(20,4);cout<<"Name "<<name<<endl;
	delay(100);
	g(20,5);cout<<"Vendor "<<vendor<<endl;
	delay(100);g(20,6);cout<<"Codename "<<codename<<endl;
	delay(100);g(20,7);cout <<"Price " <<price<<endl;
	delay(100);g(20,8);cout<<"API Level "<<api<<endl;
	if(quantity==0){g(20,9);cout<<"Out of stock.Sorry for incoveniece\n";}
	else {g(20,9);cout<<"only "<<quantity<<" in stock\n";}
	}

	char* getStringData(string info){
		if(strcmpi( info , "name") == 0 ) return name ;
		else if (strcmpi( info , "codename" ) == 0) return codename ;
		else return vendor ;
	}
	ll getIntData(string info){
		if( strcmpi ("quantity" , info ) ==0 ) return quantity ;
		else if(strcmpi("api",info ) ==0 ) return   api ;
		return price;
	}

	void modifyQuantity ( ll modification){
		quantity += modification ;
	}
	void generateBill(int qty){
		clrscr();
	g(30,2);cout<<"AndroidX Pvt. Ltd.  ";
	g(5,4);cout<<"Device Name";
	g(20,4);cout<<"Vendor";
	g(30,4);cout<<"Quantity";
	g(42,4);cout<<"Rate(in Rupees)";
	g(60,4);cout<<"TOTAL";
	g(5,7);cout<<name;
	g(20,7);cout<<vendor;
	g(45,7);cout<<price;
	g(33,7);cout<<qty;
	g(60,7);cout<<qty*price;
	}
};

void addDevice(){
	fout.open("devices.txt" , ios::binary|ios::app);
	device phone ;
	phone.input();
	if( findDeviceDetails ( "codename" , phone.getStringData("codename") ) > -1 ) layout(),g(5,10),cout<<"Device with same codename found. Aborting...";
	else fout.write((char*)&phone , sizeof phone ) ;
	fout.close();
}

void outputDeviceDetails( ll position , string fileName){
	device phone;
	position--;
	fin.open(fileName , ios::binary|ios::in);
	fin.seekg(position * (sizeof phone),ios::beg);
	fin.read((char*)&phone , sizeof phone);
	phone.output();
	fin.close();
}

void displayAllDevices( string fileName){
	F( i , 1 , countDevices() + 1 )outputDeviceDetails( i , fileName) , delay(1000) ;
}

ll countDevices (){
	device size;
	fin.open("devices.txt" ,ios::binary|ios::in) ;
	fin.seekg(0 ,ios::end);
	ll totalSize = fin.tellg();
	fin.close();
	return (totalSize/(sizeof size));
}

void apilist(){
g(20,7);cout<<"Need Help?";
g(15,8);cout<<"Press 'h' to open api help else anyother to continue..." ;
unsigned char a;
a=getch();
if(a==104){
clrscr();
cout<<"Platform Version	      API Level	VERSION_CODE   \n";
cout<<"Android 6.0       	        23	MARSHMALLOW\n";
cout<<"Android 5.1.1       	        22	LOLLIPOP\n";
cout<<"Android 5.0       	        21	LOLLIPOP\n";
cout<<"Android 4.4 W	                20	KITKAT_WATCH \n";
cout<<"Android 4.4               	19 	KITKAT\n";
cout<<"Android 4.3	                18	JELLY_BEAN_MR2\n";
cout<<"Android 4.2, 4.2.2	        17	JELLY_BEAN_MR1\n";                             ;
cout<<"Android 4.1, 4.1.1	        16	JELLY_BEAN\n";
cout<<"Android 4.0.3, 4.0.4	        15	ICE_CREAM_SANDWICH_MR1\n";
cout<<"Android 4.0, 4.0.1, 4.0.2	14	ICE_CREAM_SANDWICH\n";
cout<<"Android 3.2      	        13	HONEYCOMB_MR2\n";
cout<<"Android 3.1.x	                12	HONEYCOMB_MR1\n";
cout<<"Android 3.0.x	                11	HONEYCOMB\n";
cout<<"Android 2.3.4\n";
cout<<"Android 2.3.3                   10	GINGERBREAD_MR1\n";
cout<<"Android 2.3.2\n";
cout<<"Android 2.3.1\n";
cout<<"Android 2.3      	         9 	GINGERBREAD\n";
cout<<"Android 2.2.x   	         8      FROYO\n";
cout<<"Android 2.1.x                    7	ECLAIR_MR1\n";
cout<<"Android 2.0.1                    6	ECLAIR_0_1\n";
cout<<"Android 2.0      	         5	ECLAIR\n";
cout<<"If done with help please hit any key";
getch();
}
layout();
}

void layout(char r){
clrscr();
F(a,1,25){
g(80,a);cout<<r;
g(79,a);cout<<r;
g(78,a);cout<<r;
g(1,a);cout<<r;
g(2,a);cout<<r;
g(3,a);cout<<r;
}
F(b,1,81){
g(b,2);cout<<r;
g(b,3);cout<<r;
g(b,24);cout<<r;
g(b,25);cout<<r;
}
}

void swapDevice( device &device1 , device &device2){
	device temp = device1;
	device1 = device2;
	device2 = temp ;
}

void sort(string basis){
	device temp;
	int deviceCount = countDevices();
	device *devices = new device[deviceCount+1];
	fin.open("devices.txt" , ios::binary|ios::in);
	int pos =0 ;
	while(fin.read((char*)&devices[ pos ] , sizeof temp) ) ++pos;
	fin.close();
	 F(i,0,deviceCount)
	   F(j,0,deviceCount-1)
	     if (devices[j].getIntData(basis) > devices[j+1].getIntData(basis))
			swapDevice( devices[j], devices[j+1]);

	fout.open("sortedDevices.txt" , ios::binary);
	i = 0 ;
	while(i < deviceCount){
		fout.write((char*)&devices[i] , sizeof(device));
		i++;
	}
	fout.close();
}

ll findDeviceDetails ( string type , string required ){
	fin.open("devices.txt" , ios::binary|ios::in);
	device temp ;
	int size = sizeof temp ;
	while(fin.read((char*)&temp , sizeof temp)){
		if(!strcmpi( temp.getStringData(type) , required )  ){
			int pos = fin.tellg()/size ;
			fin.close();
			return pos ;
		}
	}
	fin.close();
	return -1 ;

}

ll findUserDetails ( string type , string required ){
	fin.open("users.txt", ios::binary|ios::in);
	user newUser;
	int size  = sizeof newUser ;
	while(fin.read((char*)&newUser , sizeof newUser)){
		if(strcmpi( newUser.getData(type) , required ) == 0 ){
			int pos = fin.tellg()/size ;
			fin.close();
			return pos ;
		}
	}
	fin.close();
	return -1 ;
}

void userSignUp () {
	user registered ;
	registered.signUp();
	if ( findUserDetails("username" , registered.getData("username") ) > -1 ){
		 g( 10 , 16  ) ; cout<<" UserName already registered " ;
		 isLoggedIn = 0 ;
		 loggedInUser[0]='\0';
	}
	else {
		g( 10 , 16 );cout<<" Registered ";
		fout.open("users.txt" , ios::binary | ios:: app ) ;
		fout.write ( (char*)&registered , sizeof registered ) ;
		fout.close();
		strcpy(loggedInUser, registered.getData("name"));
		isLoggedIn = 1 ;
	}
}

void userSignIn(){
	user registered ;
	registered.signIn();
	char* password_entered =  registered.getData("password") ;
	int details = findUserDetails("username" , registered.getData("username") ) ;
	user _registered ;
	if ( details == -1 ){
		 g( 10 , 13  ) ; cout<<"User Not Found" ;
		 isLoggedIn = 0 ;
		 loggedInUser[0]='\0';
	}
	else {
		details--;
		fin.open("users.txt" , ios::binary | ios:: in ) ;
		fin.seekg( details * sizeof(user) , ios::beg );
		fin.read( (char*)&_registered , sizeof _registered );
		fin.close();

		if (!_registered.validatePassword(password_entered)){
		    g( 10 , 13  ) ;cout<<"Logged In" ;
		    strcpy(loggedInUser , _registered.getData("name")) ;
		    isLoggedIn =  1; }
		else {
		 g( 10 , 13  ) ;cout<<"Incorrect Password" ;
		 isLoggedIn =  0 ;
		 loggedInUser[0]='\0';
		  }
   }
}


void layoutMenu(){
 layout(theme);
 int mod = 0 ;
 int _theme = theme ;
 do{
    g(10,10);cout<<"  Press d to see next theme " ;
    g(10,11);cout<<"  Press a to see prev theme ";
    g(10,12);cout<<"  Press enter to select ";
   unsigned char a = getch () ;
   if ( a == 'd' || a == 'D' ) _theme++ ;
   if ( a == 'a' || a == 'A' ) _theme-- ;
   if ( a == '\r')break ;
   mod %= 10 ;
   _theme = (_theme  % 177 )%10 + 177  ;
   layout( _theme ) ;
   }while(1);
   ::theme = _theme ;
}

void modifyStock ( ll pos , int modification){
	device temp ;
	pos--;
	fin.open("devices.txt" , ios::in | ios::binary );
	fin.seekg(pos*(sizeof temp) , ios::beg);
	fin.read((char*)&temp , sizeof(temp));
	fin.close();
	temp.modifyQuantity(modification);
	fout.open("devices.txt" , ios::ate| ios::binary );
	fout.seekp(pos*(sizeof temp) , ios::beg);
	fout.write((char*)&temp , sizeof(temp));
	fout.close();
}

void deleteFile(string filename){
	remove(filename) ;
}

device getDevice(int pos){
	device phone;
	pos--;
	fin.open("devices.txt" , ios::binary|ios::in);
	fin.seekg(pos * (sizeof phone),ios::beg);
	fin.read((char*)&phone , sizeof phone);
	fin.close();
	return phone;
}

void buyDevice(){
layout();
int flag = 0 ;
	do{
		g(5,5);cout<<"Need help in selecting the perfect device ?";
		g(5,6);cout<<"Press v to View list of all devices ";
		g(5,7);cout<<" or Press a to order for android version and p to order for price ";
		g(5,8);cout<<"else hit return";
		unsigned char t;
		g(5,9);t=getch();
		if      (t =='a')  flag = 1,sort("api")  ,displayAllDevices("sortedDevices.txt");
		else if (t =='p')  flag = 1,sort("price"),displayAllDevices("sortedDevices.txt");
		else if (t =='v')  flag = 1,displayAllDevices("devices.txt");
		else if (t =='\r') flag = 1;
		else {
			flag = 0 ;
			g(5,10);cout<<"Invalid Re-enter hit a  key to continue         ";
			getch();g(5,10);cout<<"                                             ";
			g(5,9);
		}
	}while(!flag);
	layout();
	g(5,5);cout<<"Enter codename of the  device you wish to buy";
	string codename ;
	int qty;
	g(5,6);gets(codename);
	int pos = findDeviceDetails( "codename", codename ) ;
	if (pos == -1 )g(5,7),cout<<"Device Not Found";
    else{
		g(5,7),cout<<"Enter Quantity to Buy",g(5,8),cin>>qty;
		if (getDevice(pos).getIntData("quantity") < qty )g(5,8),cout<<"Not Enough in Stock";
		else modifyStock( pos , -qty ),g(5,10),cout<<"Generating Bill...",delay(2000),getDevice(pos).generateBill(qty);
}
}

void removeDevice( int pos ){
	fin.open("devices.txt",ios::in|ios::binary);
	fout.open("new_devices.txt" , ios::binary );
	int index = 0 ;
	pos--;
	device temp;
	while( fin.read ( (char*)&temp , sizeof temp ) && index < pos)fout.write( (char*)&temp , sizeof temp ) , index ++ ;
	fin.seekg( sizeof temp , ios::cur );
	while( fin.read ( (char*)&temp , sizeof temp ))fout.write( (char*)&temp , sizeof temp ) ;
	fout.close();
	fin.close();
	remove("devices.txt");
	rename("new_devices.txt", "devices.txt");
}

void sortMenu(){

layout();

g(20,4);cout<<" Order devices on the basis of : " ;
int mod = 0 ;

g(10,7);cout<<"<..> Android Version ";
g(14,8);cout<<" Availability ";
g(14,9);cout<<" Price ";
g(14,10);cout<<" Back to previous Menu ";
g(4,15);cout<<"Press S to move down and W to move Up";
unsigned char in ;
do {
	in = getch() ;
	g(10 , 7 + mod);cout<<"    ";
	     if (in ==  0 || in == 0xE0)in=getch();
	else if (in == 'W'|| in == 'w' )mod-- ;
	else if (in == 'S'|| in == 's' )mod++ ;
	mod+=4, mod %= 4 ;
	g(10,7+mod);cout<<"<..>";
}while(in != '\r');
switch(mod){
  case 0 : sort("api")  ,displayAllDevices("sortedDevices.txt"); getch() ;break;
  case 1 : sort("quantity")  ,displayAllDevices("sortedDevices.txt");getch() ;break;
  case 2 : sort("price")  ,displayAllDevices("sortedDevices.txt");getch() ;break;
  case 3 : break;
}
}

void userMenu(){
do{
layout();
if(isLoggedIn)g(5,2),cout<<"  Welcome  "<< loggedInUser<<"  ";
int mod = 0 ;

g(10,7);cout<<"<..> Sign In ";
g(14,8);cout<<" Sign Up ";
g(14,9);cout<<" Buy a device ";
g(14,10);cout<<" Order devices";
g(14,11);cout<<" Exit ";
g(4,15);cout<<" Press S to move down and W to move Up";
unsigned char in ;
do {
	in = getch() ;
	g(10 , 7 + mod);cout<<"    ";
	     if (in ==  0 || in == 0xE0) in=getch();
	else if (in == 'W'|| in == 'w' )mod-- ;
	else if (in == 'S'|| in == 's' )mod++ ;
	mod+=5, mod %= 5 ;
	g(10,7+mod);cout<<"<..>";
}while(in != '\r');
switch(mod){
  case 0 : userSignIn() , getch(); break;
  case 1 : userSignUp() , getch(); break;
  case 2 :
	   if(isLoggedIn)buyDevice() , getch();
	   else layout(),g(10,5),cout<<"Please Log In to Cotinue , hit any key to go back . " , getch();
	   break ;
  case 3 : sortMenu();break;
  case 4 : layout() , g(20,10) , cout<< " Come Back Soon ;)  " ; g ( 40 ,18 ) , cout << " - AndroidX " , delay(2000) ; exit(0);
}
}while(1);
}

void companyMenu(){
layout();
int mod = 0 ;

g(10,7) ;cout<<"<..> Add a Device ";
g(14,8) ;cout<<" View All Devices ";
g(14,9) ;cout<<" Change Theme ";
g(14,10);cout<<" Modify Stock ";
g(14,11);cout<<" Search for a device ";
g(14,12);cout<<" Order Devices";
g(14,13);cout<<" Delete a device ";
g(14,14);cout<<" Remove all registered Users ";
g(14,15);cout<<" Remove all Devices ";
g(14,16);cout<<" Exit" ;
g(14,19);cout<<" Press S to move down and W to move Up";
unsigned char in ;
do {
	in = getch() ;
	g(10 , 7 + mod);cout<<"    ";
	     if (in ==  0 || in == 0xE0) in=getch();
	else if (in == 'W'|| in == 'w' )mod-- ;
	else if (in == 'S'|| in == 's' )mod++ ;
	mod+=10, mod %= 10 ;
	g(10,7+mod);cout<<"<..>";
}while(in != '\r');
switch(mod){
  case 0 : addDevice();getch();break;
  case 1 : displayAllDevices("devices.txt");getch();break;
  case 2 : layoutMenu();break;
  case 3 :{
    layout();
	   g(10,5),cout<<"Enter codename for the device whose stock is to be modified ";
	   string codename ; g(10,6),gets(codename);
	   ll pos = findDeviceDetails("codename",codename);
	   device toModify = getDevice(pos);
	   int quantity = toModify.getIntData("quantity");
	   if ( pos == -1 ) g(10,8),cout<<"Device with codename " << codename << " not found " ;
	   else {
		layout();
		 g(10,8),cout<<" 1. Increase Stock";
		 g(10,9),cout<<" 2. Decrease Stock";
		 g(10,10),cout<<" Enter Choice";
		 int op ; g(11,11), cin>>op;
		 g(10,12),cout<<" Enter the Change";
		 ll modify ; g(11,13) , cin>>modify;
		 if( op == 1 )modifyStock( pos , modify),g(10,15),cout<< " Modified";
		 else if( op == 2 && quantity >= modify )modifyStock( pos , -modify),g(10,15),cout<< " Modified" ;
		 else g(10,15),cout<<" Cannot Modify ";
	   }
	   getch();
	   break;
   }
  case 4 :{
    layout();
	   g(10,5),cout<<"Enter codename for the device to be searched ";
	   string codename ; g(10,6),gets(codename);
	   ll pos = findDeviceDetails("codename",codename);
	   if(pos > -1)outputDeviceDetails(pos , "devices.txt"  );
	   else g(10,8),cout<<"Device with codename " << codename << " not found " ;
	   getch();
	   break;
   }
  case 5 : sortMenu();break;
  case 6 :{
   layout();
	   g(10,5),cout<<"Enter codename for the device to be deleted ";
	   string codename ; g(10,6),gets(codename);
	   ll pos = findDeviceDetails("codename",codename);
	   if(pos>-1){
	   outputDeviceDetails(pos, "devices.txt");
	   g(15,15),cout<<"Are you sure you want to delete this device ? Y/N";
	   char op ; g(15,16),cin>>op;
	   if( op == 'Y' || op == 'y' )removeDevice(pos);}
	   else g(10,8),cout<<"Device with codename " << codename << " not found " ;
	   break;
   }
  case 7 :{
	layout();
	   g(30,5); textcolor(RED + BLINK ) ;
	   cprintf("CAUTION ! !" ) ;
	   textcolor(WHITE);
	   g(10,7), cout<<" You are about delete all users " ;
	   g(10,8), cout<<" Are you sure ? Y/N " ;
	   char op ; g(11,9),cin>>op;
	   if( op == 'Y' || op == 'y' )deleteFile("users.txt");
	   break;
   }
   case 8 :{
	layout();
	   g(30,5); textcolor(RED + BLINK ) ;
	   cprintf("CAUTION ! !" ) ;
	   textcolor(WHITE);
	   g(10,7), cout<<" You are about delete all devices " ;
	   g(10,8), cout<<" Are you sure ? Y/N " ;
	   char op ; g(11,9),cin>>op;
	   if( op == 'Y' || op == 'y' )deleteFile("devices.txt");
	   break;
   }
   case 9 : layout() , g(20,10) , cout<< " Come Back Soon ;)  " ; g ( 40 ,18 ) , cout << " - AndroidX  " , delay(2000) ; exit(0);
}
}

void mainMenu(){
do{
	layout(177);
	randomize();
	int pin = random(900) + 100 ;
	int mod = 0 ;
	g(20,10) , cout << " One Time Password  : " << pin ;
	delay(3000);
	layout(177);
	g(10,7) ;cout<<"<..> Comapny Official ";
	g(14,8) ;cout<<" User Menu ";
	g(14,9) ;cout<<" Re-generate pin for company login ";
	g(14,10);cout<<" Exit ";
	g(14,18);cout<<" Press S to move down and W to move Up";
	unsigned char in ;
	do {
		in = getch() ;
		g(10 , 7 + mod);cout<<"    ";
		     if (in ==  0 || in == 0xE0) in=getch();
		else if (in == 'W'|| in == 'w' )mod-- ;
		else if (in == 'S'|| in == 's' )mod++ ;
		mod+=4, mod %= 4 ;
		g(10,7+mod);cout<<"<..>";
	}while(in != '\r');
	switch(mod){
		case 0:
		layout();
		g(10,10), cout<<"Enter the high secure OTP";
		int _pin  ; g (10,11); cin >> _pin;
		if ( pin == _pin )companyMenu();
		else g ( 10, 14) , cout<<"Acess Denied " , delay(750) ;
		break ;
		case 1: userMenu(); break ;
		case 2: break;
		case 3 : layout() , g(20,10) , cout<< " Come Back Soon ;)  " ; g ( 40 ,18 ) , cout << " - AndroidX " , delay(2000) ; exit(0);
	}
}while(1);
}

void boot(){
	string prct[10]={"Setting up the Sorting Machine","Appointing Salemen","Installing Software","Setting the sorter","Creating Cash Counter","Testing New Firmaware","Contacting Dealers","Creating Accounts Department"};
	clrscr();
	textattr(RED + BLINK)   ;
	g(36,7);cprintf("LOADING");
	textattr(WHITE);
	randomize();
	F(perc,0,101){
	  g(15,10);
		F(perc_1,1,perc+1){
	      cout<<"|";
		  perc_1+=1;
		}
		delay(300);
	 g(25,12);cout<<"                                                                        ";
	 g(25,12);cout<<prct[random(10)];
	 g(36,11);cout<<perc<<"%";
	 if(perc<95)perc+=random(5);
	 delay(80);
	}

	g(20,12);cout<<"                                                     ";
	textattr(RED + BLINK)   ;
	g(33,11);cprintf("Finishing Boot");
	delay(1000);
	int r,menu;
	clrscr();
	textattr(WHITE);
}

void project(){
	boot();
	mainMenu();
}


