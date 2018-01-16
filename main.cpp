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

void clear();
int N_value_in();
int N = N_value_in();
void N_value_out(int a);
vector<Data> read_from_file();
void save(vector<Data> a);
Data read();
void wys(vector<Data> a);
void scroll(vector<Data> a);
void del(vector<Data>& a,int i);
void wipe(vector<Data> &a);
void menu();
void sub_menu_add(vector<Data> &a);
void sub_menu_subtract(vector<Data> &a);
void modify(vector<Data> &a);



int main()
{
	char c;
	vector<Data> a;
	a = read_from_file();
	do
	{
		menu();
		c=getch();
		switch(c)
		{
		 	case '1':						// Display
		 	{
		 		clear();
		 		if(a.size()==0)
		 		{
		 			cout << "There's nothing to display.";
		 			getch();
		 		}
		 		else 
		 		{
		 			scroll(a);
		 		}
		 		break;
		 	}
		 	case '2':								// Add new position
		 	{
		 		clear();
		 		a.push_back(Data());
		 		int i = a.size()-1;
		 		a[i]=read();
		 		N++;	
		 		break;
		 	}
		 	case '3':								// Modify position
		 	{
		 		modify(a);
		 		break;
		 	}			
		 	case '4':								// Wipe data
		 	{
		 		clear();
		 		wipe(a);
		 		break;
		 	}
		 	case 'q':								// Quit
		 	{
		 		clear();
		 		char z;
		 		cout << "Would you like to save?[y/N]   ";
		 		cin >> z;
		 		cin.get();
		 		if(z == 'y' || z == 'Y')
		 		{
		 			c='q';
		 			save(a);
		 			N_value_out(N);
		 		}
		 		else if(z == 'n' || z == 'N') c = 'q';
		 		break;
		 	}
		 }
	}while(c!='q');
	clear();
	return 0;
}

void clear()
{
	cout << "\x1B[2J\x1B[H";				// Clears the screen and sets the pointer to the left top corner of the terminal
}

int N_value_in()							// Setting the N value from the config file
{											// It is used to determine the size of the table
	int N;
	ifstream plik;
	plik.open("n.txt",ios_base::in);			
	if(plik.good())							// Checking if the file exists
	{
		plik >> N;
		plik.close();
	}
	else N=0;								// If no the the value is set by default to 0
	return N;
}

void N_value_out(int a)						// Saving the N value out to the config file
{
	ofstream plik;
	plik.open("n.txt",ios_base::out);
	plik << a;
	plik.close();
}

vector<Data> read_from_file()				// Inputting the vector from the data in the data.txt file
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

void save(vector<Data> a)					// Saving the vector to the config file
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
	cout << "Input name: "; cin.getline(a.name,32);
	a.tally=0;
	return a;
}

void wys(vector<Data> a)
{
	clear();
	for(int i=0;i<N;i++)
	{
		cout << i+1 << ". " << a[i].name << " " << a[i].tally << endl;
	}
}

void scroll(vector<Data> a)
{
	clear();
	char c;
	int i,k=0,l=10;
	if( N<10 )
	{
		do
		{
			clear();
			for(i=0;i<N;i++)
			{
				cout << i+1 << ". " << a[i].name << " " << a[i].tally << endl;
			}
			c = getch();
		}while(c != 'q');
	}
	else
	{
		for(i=0;i<10;i++)
		{
			cout << i+1 << ". " << a[i].name << " " << a[i].tally << endl;
		}
		do
		{
			c = getch();
			if( c != 'q')
			{
				if( c == 's' && l<N)
				{
					k++;
					l++;
				}
				else if( c == 'w' && k>0)
				{
					k--;
					l--;
				}
			}
			clear();
			for(int i=k;i<l;i++)
			{
				cout << i+1 << ". " << a[i].name << " " << a[i].tally << endl;
			}
		}while( c != 'q');
	}
}


void del(vector<Data>& a,int i)
{
	a.erase(a.begin()+i-1);
	cout << "Position successfully deleted.";
	N--;
	getch();
}

void wipe(vector<Data> &a)
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

void menu()
{
	clear();
	cout << "       Menu       " << endl << setw(3) << "1. " << "Display" << endl << "2. " << "Add"
		 << endl << "3. " << "Modify position. " << endl << "4. "
			 << "Data wipe " << endl << "Quit. (q)" << endl;
}

void sub_menu_add(vector<Data>& a)
{
	clear();
	cout << setw(3) <<  "Select if you'd like to add one or more: " << endl << "1. Add one. " << endl
		 << "2. Add more. " << endl << "3. Go back.";
	char c = getch();
	switch(c)
	{
		case '1':
		{
			int i;
			wys(a);
		 	cout << "Choose the number to add to: "; cin >> i;
		 	cin.get();			// Resetting the input stream. If not done the EOL char will be passed to the next line
		 	a[i-1].tally++;
		 	break;
		}
		case '2':
		{
			int i,j;
			wys(a);
	 		cout << "Choose the number to add to: "; cin >> i;
			cout << "Define how much you'd like to add: "; cin >> j;
			cin.get();			// Resetting the input stream. If not done the EOL char will be passed to the next line
			a[i-1].tally+=j;
			break;
		}
		case '3': break;
	}
}

void sub_menu_subtract(vector<Data>& a)
{
	clear();
	cout
	<< "1. Subtract one from position" << endl << "2. Subtract more from position."
	<< endl << "3. Delete position" << endl << "4. Go back.";
	char c = getch();
	switch(c)
	{
		case '1':
		{
			clear();
			int i;
			wys(a);
			cout << "Choose position you'd like to subtract from: "; cin >> i;
			if(a[i-1].tally>0) a[i-1].tally--;
			else
			{
				cout << "The data can't have negative values.";
				getch();
				sub_menu_subtract(a);
				break;
			}
			cin.get();
			break;
		}
		case '2':
		{
			clear();
			int i,j;
			wys(a);
			cout << "Choose position you'd like to subtract from: "; cin >> i;
			cout << "How much you'd like to subtract? : "; cin >> j;
			cin.get();
			int tmp = a[i-1].tally;
			for(int k=j;k>=0;k--)
			{
				if(a[i-1].tally>=0) a[i-1].tally--;
				else 
				{
					a[i-1].tally=tmp;
					cout << "The data can't have negative values." << endl << "Restoring to previous value: " << a[i-1].tally;
					getch();
					sub_menu_subtract(a);
					break;
				}
			}
			break;
		}
		case '3':
		{
			clear();
			int i;
			wys(a);
			cout << "Choose position to delete: "; cin >> i;
			del(a,i);
			cin.get();
			break;
		}
		case '4': break;
	}
}

void modify(vector<Data> &a)
{
	clear();
	char c;
	cout << "1. Add to position." << endl << "2. Subtract from position. " << endl 
	<< "3. Delete position. " << endl <<"4. Go back. "<<  endl <<"What would you like to do?: ";
	c = getch();
	switch(c)
	{
		case '1':
		{
			sub_menu_add(a);
			break;
		}
		case '2':
		{
			sub_menu_subtract(a);
			break;
		}
		case '3':
		{
			clear();
			int i;
			wys(a);
			cout << "Choose position to delete: "; cin >> i;
			del(a,i);
			cin.get();
			break;
		}
		case '4':
		{
			break;
		}
	}
}