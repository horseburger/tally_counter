#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <vector>

using namespace std;

// getch for linux 
// If you're using Windows just uncomment the next line and delete the function;

// #include <conio.h>

#include <unistd.h>
#include <termios.h>

char getch()
{
    /*#include <unistd.h>   //_getch*/
    /*#include <termios.h>  //_getch*/
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    return buf;
 }
 

struct Data
{
	char name[32];
	int tally;
};


int N_value_in()
{
	int N;
	ifstream plik;
	plik.open("n",ios_base::in);
	if(plik.good())
	{
		plik >> N;
		plik.close();
	}
	else N=0;
	return N;
}

int N = N_value_in();

void N_value_out(int a)
{
	ofstream plik;
	plik.open("n",ios_base::out);
	plik << a;
	plik.close();
}

vector<Data> read_from_file()
{
	vector<Data> a;
	ifstream plik;
	plik.open("data.txt",ios_base::in);
	for(int i=0;i<N;i++)
	{
		a.push_back(Data());
		plik >> a[i].name >> a[i].tally;
		cout << endl;
	}
	plik.close();
	return a;
}

void save(vector<Data> a)
{
	ofstream plik;
	plik.open("data.txt",ios_base::out);
	for(int i=0;i<N;i++)
	{
		plik << a[i].name << " " << a[i].tally << endl;
	}
	plik.close();
}

Data read()
{
	Data a;
	cout << "Podaj nazwe: "; cin.getline(a.name,32);
	a.tally=0;
	return a;
}


void wys(vector<Data> a)
{
	system("clear");
	for(int i=0;i<N;i++)
	{
		cout << setw(3) << i+1 << ". "<<  a[i].name << " " << a[i].tally << endl;
	}
}

void wipe(vector<Data> a)
{
	ofstream data,n;
	data.open("data.txt",ios_base::out);
	data << "";
	data.close();
	for(int i=0;i<N;i++)
	{
		a.pop_back();
	}
	N=0;
	cout << "Data wipe successful.";
	getch();
}

void menu(vector<Data> a)
{
	char c;
	do
	{
		a = read_from_file();
		system("clear");
		cout << "       Menu       " << endl << setw(3) << "1. " << "Pokaz." << endl << "2. " << "Dodaj."
			 << endl << "3. " << "Dodaj do numeru. " << endl << "4. " << "Data wipe. " << endl << "5. " << "Wyjdz." << endl;
		 c=getch();
		 switch(c)
		 {
		 	case '1':
		 	{
		 		system("clear");
		 		if(a.size()==0)
		 		{
		 			cout << "There's nothing to display.";
		 			getch();
		 		}
		 		else 
		 		{
		 			wys(a);
		 			getch();
		 		}
		 		break;
		 	}
		 	case '2':
		 	{
		 		system("clear");
		 		a.push_back(Data());
		 		int i = a.size()-1;
		 		a[i]=read();
		 		N++;
		 		save(a);
		 		break;
		 	}
		 	case '3':
		 	{
		 		system("clear");								
		 		wys(a);
		 		int i;
		 		cout << "Gdzie chcesz dodac: "; cin >> i;
		 		cin.get();
		 		a[i-1].tally++;
		 		save(a);
		 		break;
		 	}
		 	case '4':
		 	{
		 		system("clear");
		 		wipe(a);
		 		break;
		 	}
		 	case '5':
		 	{
		 		N_value_out(N);
		 		c='q';
		 		break;
		 	}
		 }
	}while(c!='q');
}


int main()
{
	// Data* a = new Data[1024];
	vector<Data> a;
	menu(a);
	system("clear");
	return 0;
}
