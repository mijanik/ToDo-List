//*****************************************//
// Autorzy: Miłosz Janik i Antoni Kijania  //
//    Języki Programowania Obiektowego     //
//   AGH - Elektronika i Telekomunikacja   //
//     Prowadzący: dr. Rafał Frączek       //
//*****************************************//

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
using namespace std;

// ----------KLASA ITEM---------------------------------------------------------

class Item{
private:
	string _text;           //Pole - treść zadania
	bool _done;             //Pole - stan wykonania zadania

public:
	Item(){                 //Konstruktor domyślny
	  _text = "";
	  _done = false;
  }

	Item(string);           //Konstruktor parametryczny Item

	~Item(){}               //Destruktor domyślny

	string text() const{    //Getter _text
	  return _text;
  }
 
	void done(){            //Setter _done (tylko w 1 stronę)
	  _done = true;
  }
  
	bool is_done() const{   //Getter _done
	  return _done;
  }

};

//---------METODY ITEM-------------------------------------------------------

//Konstruktor parametryczny Item - przyjmuje tresc zadania do wykonania (stan ustala sam na niewykonane)
Item::Item(string item){

  //  W przypadku odczytywania z pliku, Konstruktor przyjmuje cała linijke z pliku i dzieki
  //  funkcjom ponizej odczytuje rowniez stan wykonania zadania
	int startf = item.find("false");
	int startt = item.find("true");
	string str;

  //sprawdzanie czy zadanie jest wykonane - odnaleziono "true"
  if (startt > -1 && startf == -1) {
		str = item.substr(0, startt - 1);
		_done = true;
	} 

  //sprawdzanie czy zadanie jest niewykonane - odnaleziono "false"
	else if (startf > -1 && startt == -1){
		str = item.substr(0, startf - 1);
		_done = false;
  } 

  //Po stworzeniu nowego obiektu - nie z pliku:
  //nie odnaleziono ani "true" ani "false" - domyślnie nie wykonano = "false"
  else if (startf == -1 && startt == -1) {
		str = item;
		_done = false;
	}
	
	_text = str;
}


// ----------KLASA TODOLIST-------------------------------------------------

class TodoList {
private:
	const char* filename;       //Pole - Lokalizacja pliku listy
	vector<Item> list;          //Pole - Wektor zadań do wykonania

public:
  //Konstruktor domyślny  
	TodoList(){                                  
	  filename = nullptr;
  }

  //Konstruktor parametryczny - przyjmuje nazwa pliku jako parametr
	TodoList(const char* filename){             
	  this->filename = filename;
  }

  //Setter filename
  void set_filename(const char* filename){    
    this->filename = filename;
  }

  //Destruktor domyślny
	~TodoList(){}                               
	
  //Metody
  void create();
	void read();
	void show();
	void save();
	void add();
  void check();
	void destroy_file();

	int get_count();
};

//----METODY TODOLIST--------------------------------------------------------

//metoda tworząca nową listę (czyści bieżącą)
void TodoList::create(){
	bool finished = false;
	int count = 0;
	string task;
	list.clear();

	while(!finished){	
		cout << ++count << ": ";
		
		getline(cin, task);
		if (task == "") finished = true; //jeżeli wprowadzono pustą linikę - zakończ tworzenie

    //Wyszukiwanie zabronionej nazwy w podanej treści zadania:
    int startf = task.find("false");
    int startt = task.find("true");
    if (startf != string::npos || startt != string::npos){
      cout << "Zabroniona nazwa. Wprowadź inną."<<endl;
      count--;
      continue;
    }

		Item item(task);      //tworzenie obiektu zadania
		list.push_back(item); //dodawanie go do wektora zadań
	}
}


//Metoda odczytująca zawartość pliku po uruchomieniu programu
void TodoList::read(){
	fstream fs(filename, fstream::in);
	string line;

	list.clear();

  //Wczytywanie zadań z pliku do wektora zadań
	while(getline(fs, line)){
		if (line == "") continue;
		Item item(line);
		list.push_back(item);
	}
	fs.close();
}


//Metoda wyświetlająca zawartość listy zadań
void TodoList::show(){
	cout << endl << "Aktualna lista zadań: " << endl << endl;
	cout << "   " << setw(35) << left << "ZADANIE" << "UKOŃCZONO" << endl;
	cout << "   " << setw(35) << left << "-------" << "---------" << endl;
	for (int i = 0; i < list.size(); ++i)
		cout << i + 1 << ") " << setw(35) << left << list[i].text() << (list[i].is_done() ? "TAK" : "" ) << endl;	
  
  cout << endl;
}


//Metoda zapisująca stan zadania
void TodoList::save(){
	fstream fs(filename, fstream::out);
	
  //Wpisywanie kolejnych zadań jako nowe linie do pliku
	for (Item item : list) {
		if (item.text().empty()) continue;
		fs << item.text() << " " << (item.is_done() ? "true" : "false") << endl;
	}

	fs.close();
}

//Metoda zwracająca liczbę zadań do wykonania
int TodoList::get_count(){
	return list.size();       //rozmiar wektora zadań
}

//Metoda dodawania kolejnych zadań
void TodoList::add(){
  cout << "Wpisz kolejne zadania" << endl;
  cout << "Wprowadź puste pole aby zakończyć wpisywanie" << endl;
	int index = list.size();

  //Wprowadzanie nowych zadań:
	while (true){
		cout << ++index << ": ";
		
		string task;
		getline(cin, task);
		if (task == "") break;     //jeżeli wprowadzono pustą linikę - zakończ tworzenie

    //Sprawdzanie czy nazwa jest zabroniona
    int startf = task.find("false");
    int startt = task.find("true");
    if (startf != string::npos || startt != string::npos){
      cout << "Zabroniona nazwa, wprowadź inną"<<endl;
      index--;
      continue;
    }

		Item item(task);          //tworzenie obiektu zadania
		list.push_back(item);     //dodawanie go do wektora zadań
	}

	save();
}

//Metoda ustawiająca stan zadania na wykonane
void TodoList::check(){
	cout << "Podaj numer ukończonego zadania: ";
	string choice;
	getline(cin, choice);
	
  //jeżeli zostanie wprowadzone puste - wyjdz
	if (choice.empty()) return;
  //jezeli zostanie wprowadzona litera - wyjdz
	for (char c : choice) {
    if (isalpha(c)){
      cout << "Niepoprawny wybór, należy podać liczbę."<<endl;
      return;
    }
  }
	
  //jeżeli liczba jest poza zakresem listy - wyjdz
	int index = stoi(choice) - 1;
	if (index > list.size()){
    cout << "Niepoprawny numer zadania"<<endl;
    return;
  }

  //Jeżeli użytkownik poda poprawną wartość - potwierdź wykonanie zadania
	list[index].done();

	save();
}

//Czyszczenie
void TodoList::destroy_file(){
	list.clear();     //opróżnianie wektora zadań do wykonania
	remove(filename); //usuwanie pliku
}	



//-----------MAIN------------------------------------------------------------

int main(){

  //Obsługa polskich znaków
  setlocale( LC_ALL, "pl_PL" );
  setlocale( LC_ALL, "polish" );
	
  string path;
  string decision = "n";
  cout << "Użyć pliku innego niż domyślny? [y/n]" << endl;
  getline(cin, decision);
  
  TodoList tasks("todo_default.txt"); //Tworzenie obiektu listy
  
  if(decision =="y" || decision == "Y"){
    cout << "Podaj nazwę nowego lub istniejącego pliku [nazwa_pliku.txt]: ";
    getline(cin, path);
    tasks.set_filename(path.c_str()); //zmiana nazwy pliku listy
  }

  else if (decision =="n" || decision == "N"){
    cout << "Otwieranie pliku domyślnego todo_default.txt"<<endl;
  }

  else {
    cout << "Błędny wybór"<<endl;
    cout << "Otwieranie pliku domyślnego todo_default.txt"<<endl;
  }

	while(true){

    
		tasks.read();   //Wczytanie zadań z pliku do wektora zadań przypisanego do obiektu listy

    //Jeżeli rozmiar wektora zadań przypisanego do danej listy >0 - to nasza lista ma zadania i można je edytować
		if (tasks.get_count() > 0){
			
      //wyświetlenie listy zadań
      tasks.show();

      //wyświetlenie menu wyboru i wybór opcji
			cout << "-------------------MENU------------------" << endl;
			cout << " -1-  Dodaj zadanie                      " << endl;
			cout << " -2-  Potwierdź ukończenie zadania       " << endl;
			cout << " -3-  Wyczyść listę                      " << endl;
      cout << " -4-  Zakończ i wyjdź                    " << endl;
      cout << "-----------------------------------------" << endl;
			cout << "Wybierz opcję: ";
			string choice;
			getline(cin, choice);
			
      //Dodanie kolejnych zadań do bieżącej listy
			if (choice == "1"){
				tasks.add();
			}

      //Zaznaczenie wykonania konkretnych czynności
      else if (choice == "2") {
        tasks.check();
			} 

      //Wyczyszczenie listy, usunięcie pliku
      else if (choice == "3") {
        tasks.destroy_file();
			} 
      else if (choice == "4") {
        cout << "Zapisano zmiany, zamykanie programu..." << endl;
				break;
      }
      else {
        cout << "Niepoprawny wybór. Zamykanie programu..." << endl;
        break;
      }
		} 
    
    //Jeżeli brak zadań - lista zadań nie istnieje. Tworzenie nowej listy:
    else{
      cout << endl << "Lista nie istnieje" << endl;
			cout << "Utworzono nową listę" << endl;
      cout << "Wpisz nowe zadania" << endl;
      cout << "Wprowadź puste pole aby zakończyć wpisywanie" << endl;
			
      //Uruchomienie kreatora tworzenia listy zadań
      tasks.create();
			cout << endl; 

      //zapisanie do pliku bieżącej listy - każdego elementu wektora zadań wraz z stanem wykonania
			tasks.save();
		}
	}
	return 0;
}