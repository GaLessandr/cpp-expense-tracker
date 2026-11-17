#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <fstream>
#include <sstream>

using namespace std;

//Rappresenta una singola Exspense
class Exspense {
  private:
    int id;
    string category;
    string description;
    double amount;

  public:
    //Costruttore per inizializzare la spesa 
    Exspense(int id, string category, string description, double amount):
      id(id), category(category), description(description), amount(amount){}

    //metodi "getter" per leggere i dati protetti
    int getId() const{ return id; }
    string getCategory() const { return category; }
    string getDescription() const { return description; }
    double getAmount() const { return amount; }
    
    //converte la spesa in una riga CSV (es: "1,Cibo,Pizza,12,50")
    string toCSV() const {
      return to_string(id) + "," + category + "," + description + "," + to_string(amount);
    }
};

class ExpenseManager {
  private:
    vector<Exspense> exspenses; // Lista dinamica di tutte le spese 
    string filename; 
    int nextId = 1;            // Contatore automatico per l'ID 
    
    //Salvo le spese in un file CSV
    void saveToFile() const {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Errore: impossibile salvare su " << filename << "\n";
            return;
        }
        //inserisco la spesa nel file CSV
        for (const auto& exp : exspenses) {
          file << exp.toCSV() << "\n";
        }
        file.close();
    }

    void loadFromFile() {
      ifstream file(filename);
      if (!file.is_open()) return;

      string line;
      while (getline(file, line)) {
        stringstream ss(line);
        string idStr, category, description, amountStr;

        if (getline(ss, idStr, ',') &&
          getline(ss, category, ',') &&
          getline(ss, description, ',') &&
          getline(ss, amountStr, ',')) {
            int id = stoi(idStr);
            double amount = stod(amountStr);
            exspenses.emplace_back(id, category, description, amount);

            if (id >= nextId) nextId = id + 1;
        }
    }
      file.close();
  }    
  public:
    ExpenseManager(string file) : filename(file) {
      loadFromFile();
    }
 //aggiunge una nuova spesa all'elenco 
     void addExspense(string category, string description, double amount){
       //emplace_back crea l'oggetto direttamente dentro il vector 
       exspenses.emplace_back(nextId++, category, description, amount);
       saveToFile();
       cout << "Spesa aggiounta con successo!\n";
     }

     //stampa tutte le spese in forma di tabella
     void listExspenses() const{
       if (exspenses.empty()) {
         cout << "Nessuna spesa registrata.\n";
         return;
       } 

     cout << "\n--------------------------------------------------------------\n";
     cout << left << setw(6) << "ID"
          << setw(15) << "Category"
          << setw(25) << "Description"
          << right << setw(12) << "Amount\n";
     cout << "\n--------------------------------------------------------------\n"; 
     
     double total = 0.0;
        for (const auto& exp: exspenses) {
            cout << left << setw(6) << exp.getId()
                 << setw(15) << exp.getCategory()
                 << setw(25) << exp.getDescription()
                 << right << setw(11) << fixed << setprecision(2) << exp.getAmount() << "\n";
            total += exp.getAmount();     
     }
     cout << "--------------------------------------------------------------\n";
     cout << left << setw(46) << "TOTALE:"
          << right << setw(11) << setprecision(2) << total << "\n\n";
  }

    bool removeExspense(int id) {
      for (auto it = exspenses.begin(); it != exspenses.end(); ++it) {
          if(it->getId() == id) {
            exspenses.erase(it);
            saveToFile();
            return true;
          }      
      }
      return false;
    } 
};

  //Pulisce la tastiera dall'ultimo input assicurandomi che quest'ultimo sia vuoto
  void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }
  
  //Controlla se uno o piu' attributi della spesa sono vuoti 
  string readNonEmptyLine(const string& prompt) {
    string input;
    while (true) {
      cout << prompt;
      getline(cin, input);

      //controlla se ci sono caratteri diversi da spazi o tabulazioni
      size_t first = input.find_first_not_of(" \t\r\n");
      if (first != string::npos) {
        //Rimuove spazi inziali e finali 
        size_t last = input.find_last_not_of(" \t\r\n");
        input = input.substr(first, (last - first + 1));

        //Sostituisce eventuali virgole per non corrompere il CSV
        for (char& c : input) {
          if (c == ',') c = ';';
        }
        return input;
      }
      cout << "Errore: il campo non puo' essere vuoto. Riprova.\n";
    }
  }
  
int main(){
    ExpenseManager manager("exspenses.csv");
    int choice = 0;
    while (true) {
      cout << "\n=== CLI Tracker ===\n";
      cout << "1. Aggiungi Spesa\n";
      cout << "2. Visualizza Spese\n";
      cout << "3. Elimina Spesa\n";
      cout << "4. Esci\n";
      cout << "Scegli un'opzione: ";

      if (!(cin >> choice)) {
        cout << "Input non valido. Inserisci un numero.\n";
        clearInputBuffer();
        continue;
      }
      clearInputBuffer();

      if (choice == 1) {
        string category = readNonEmptyLine("Categoria (es. Food, Tech): ");
        string description = readNonEmptyLine("Descrizione: ");  
        double amount;

        while (true) {
          cout << "Importo: ";
          if (cin >> amount && amount > 0) {
            clearInputBuffer();
            break;
          }
          cout << "Importo non valido. Inserisci un numero maggiore di 0.\n";
          clearInputBuffer();
        }

        manager.addExspense(category, description, amount);
    }else if (choice == 2) {
      manager.listExspenses();
    }else if (choice == 3) {
      int id;
      cout << "Inserisci l'ID della spesa da eliminare: ";
      if (cin >> id) {
        if (manager.removeExspense(id)) {
          cout << "Spesa eliminata.\n";
        }else {
          cout << "ID non trovato.\n";
        }
      }
      clearInputBuffer();

    }else if (choice ==4) {
      cout << "Uscita...\n";
      break;
    }else {
      cout << "Opzione nopn valida.\n";
    }
  }
    return 0;
}
