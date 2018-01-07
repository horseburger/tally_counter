#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>


#include "getch.h"


using namespace std;

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
	plik >> N;
	plik.close();
	return N;
}

int N=N_value_in();

void N_value_out(int N)
{
	ofstream plik;
	plik.open("n",ios_base::out);
	plik << N;
	plik.close();
}

void read_from_file(Data a[])
{
	ifstream plik;
	plik.open("data.txt",ios_base::in);
	for(int i=0;i<N;i++)
	{
		plik >> a[i].name >> a[i].tally;
		cout << endl;
	}
	plik.close();
}

void save(Data a[])
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
	cout << "Podaj nazwe: ";cin.getline(a.name,32);
	a.tally=0;
	return a;
}


void wys(Data a[])
{
	system("clear");
	for(int i=0;i<N;i++)
	{
		cout << setw(3) << i+1 << ". "<<  a[i].name << " " << a[i].tally << endl;
	}
}

void wipe()
{
	ofstream data,n;
	data.open("data.txt",ios_base::out);
	data << "";
	data.close();
	N=0;
	cout << "Data wipe successful.";
	getch();
}

void menu(Data a[])
{
	char c;
	do
	{
		read_from_file(a);
		system("clear");
		cout << "       Menu       " << endl << setw(3) << "1. " << "Pokaz." << endl << "2. " << "Dodaj."
			 << endl << "3. " << "Dodaj do numeru. " << endl << "4. " << "Data wipe. " << endl << "5. " << "Wyjdz." << endl;
		 c=getch();
		 switch(c)
		 {
		 	case '1':
		 	{
		 		wys(a);
		 		getch();
		 		break;
		 	}
		 	case '2':
		 	{
		 		system("clear");
		 		N++;

		 		a[N-1]=read();
		 		save(a);
		 		break;
		 	}
		 	case '3':
		 	{
		 		system("clear");
		 		wys(a);
		 		int i;
		 		cout << "Gdzie chcesz dodac: "; cin >> i;
		 		a[i-1].tally++;
		 		save(a);
		 		break;
		 	}
		 	case '4':
		 	{
		 		system("clear");
		 		wipe();
		 		break;
		 	}
		 }
	}while(c!='5');
}


int main()
{
	Data* a = new Data[1024];
	menu(a);
	N_value_out(N);
	system("clear");
	delete [] a;
	return 0;
}