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

int N = N_value_in();

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
	cout << "\x1B[2J\x1B[H";														// Clears the terminal window 
	for(int i=0;i<N;i++)															// and sets the pointer to the top left corner
	{
		cout << setw(3) << i+1 << ". "<<  a[i].name << " " << a[i].tally << endl;
	}
}

void del(vector<Data>& a,int i)
{
	a.erase(a.begin()+i-1);
	cout << "Position successfully deleted.";
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

void menu(vector<Data> &a)
{
	char c;
	do
	{
		cout << "\x1B[2J\x1B[H";
		cout << "       Menu       " << endl << setw(3) << "1. " << "Display" << endl << "2. " << "Add"
			 << endl << "3. " << "Add to position " << endl << "4. " << "Delete position." << endl << "5. "
			 << "Data wipe " << endl << "6. " << "Quit" << endl;
		 c=getch();
		 switch(c)
		 {
		 	case '1':
		 	{
		 		cout << "\x1B[2J\x1B[H";
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
		 		cout << "\x1B[2J\x1B[H";
		 		a.push_back(Data());
		 		int i = a.size()-1;
		 		a[i]=read();
		 		N++;
		 		break;
		 	}
		 	case '3':
		 	{
		 		cout << "\x1B[2J\x1B[H";								
		 		wys(a);
		 		int i;
		 		cout << "Choose the number to add to "; cin >> i;
		 		cin.get();			// Resetting the input stream. If not done the EOL char will be passed to the next line
		 		a[i-1].tally++;
		 		break;
		 	}
		 	case '4':
		 	{
				cout << "\x1B[2J\x1B[H";
		 		int i;
		 		wys(a);
		 		cout << "Which position would you like to delete? : "; cin >> i;
		 		cin.get();
		 		del(a,i);
		 		N--;
		 		break;
		 	}
		 	case '5':
		 	{
		 		cout << "\x1B[2J\x1B[H";
		 		wipe(a);
		 		break;
		 	}
		 	case '6':
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
	vector<Data> a;
	a = read_from_file();
	menu(a);
	save(a);
	cout << "\x1B[2J\x1B[H";
	return 0;
}
