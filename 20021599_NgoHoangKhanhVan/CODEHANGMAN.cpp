#include<iostream>       // 20021599_NgoHoangKhanhVan
#include<fstream>
#include<cstdlib>
#include<ctime>
#include<algorithm>
#include<string>
#include<vector>
#include<thread>
#include<map>
// using for MODE 2
#include <set>
#include <cctype>
#include <stdexcept>
#include <chrono>
using namespace std;

#define MAX_GUESSES 7
#define consecutive_wrong 3

void hangpicture(int missword) {  //ve hangman
	string hang[] = {
"   ------------\n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        ",
"   ------------\n"
"   |          |\n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        ",
"   ------------\n"
"   |          |\n"
"   |          O\n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        ",
"   ------------\n"
"   |          |\n"
"   |          O\n"
"   |          |\n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        ",
"   ------------\n"
"   |          |\n"
"   |          O\n"
"   |         /|\n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        ",
"   ------------\n"
"   |          |\n"
"   |          O\n"
"   |         /|\\\n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        ",
"   ------------\n"
"   |          |\n"
"   |          O\n"
"   |         /|\\\n"
"   |         / \n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        ",
"   ------------\n"
"   |          |\n"
"   |          O\n"
"   |         /|\\\n"
"   |         / \\\n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        ",

	};
	cout << hang[missword] << endl;

}
void clearScreen()
{
#ifdef _WIN32
	system("cls");
#elif __linux__                                
	system("clear");
#endif // _WIN32
}                                              

string normalize(const string& s)                                    // HAM VIET THUONG TU
{
	string news = s;
	transform(s.begin(), s.end(), news.begin(), ::tolower);
	return news;
}

// ====================================================================== MODE 1 =======================================================================================//

int theme();                                                         // HAM CHON CHU DE
int level();                                                         // HAM CHON DO KHO ( dua theo do dai cua tu)
string pickword(const int& level, const int& theme);                 // HAM CHON TU ( dua theo level + theme )
char readword();                                                     // HAM NHAP TU
bool check(const string word, char c);                               // HAM TIM( tim chu cai minh nhap co trong findword khong )
// bien TOAN CUC dung cho cac ham duoi //
map <int, int> checkhint; // bien KIEM TRA SAI 3 LAN LIEN TIEP => HINT
int wintimes = 0;         // Dem SO LAN win.
int rewards = 0;          // Lan choi truoc WIN => +1 REWARDS => +1 HINT cho lan choi sau.

string update(string& guessword, const string word, char c);         // HAM UPDATE( neu nhap CHU CAI co trong findword => hien len )
void render(string guess_word, int miss_word, string wrongword);     // HAM IN ( sau khi thuc hien cac HAM KHAC )
bool TOTAL(string& word, string& guessword);                         // HAM DOAN 1 LAN LUON 
int Hint(string& findword, string& guessword);                       // HAM GOI Y ( sai 3 lan lien tiep moi duoc dung )
void printgameover(const string word, string& guessword);            // HAM GAMEOVER (in win / lose )
bool Replay();                                                       // HAM CHOI LAI
                    
//---------------------------------------------------------------DINH NGHIA CODE MODE 1 -------------------------------------------------------------------------------------//

int theme() { //Ham in chu de cho nguoi dung chon.
	int luachon;
	cout << "                           WARRIOR CHOOSE YOUR POWER  " << endl
		<< "              Man                             (press     1      )" << endl
		<< "              Plants                          (press     2      )" << endl
		<< "              Home sweet home                 (press     3      )" << endl
		<< "              The wild hunt                   (press     4      )" << endl
		<< "              Places and means of transport   (press     5      )" << endl
		<< "              Others                          (press     6      )" << endl
		<< "==================================================================================" << endl;
	cin >> luachon;
	while (luachon > 7) { cin >> luachon; }     // nhap chu de sai thi nhap lai den khi dung thi thoi.
	return luachon;                             // tra ve thu tu cua chu de duoc chon.
}

int level() { // Ham in do kho cho nguoi dung lua chon. 
	cout << "                       HOW YOU DARE TO FACE TO ME " << endl
		<< "              Story         -> press     1      " << endl
		<< "              Challenging   -> press     2      " << endl
		<< "              Death         -> press     3      " << endl;
	int luachon;
	cin >> luachon;
	srand((int)time(0));                                        // random khac nhau sau moi lan choi lai.
	if (luachon == 1) return 3;                                 // tra ve tu co 3 chu cai.
	else if (luachon == 2) return 4 + rand() % 2;               // tu co 4 hoac 5 chu cai.
	else if (luachon == 3) return 6 + rand() % 5;               // tu co 6 - 10 chu cai.

}

string pickword(const int& level, const int& theme) { // ham chon tu theo chu de + do kho.
											 // CHU DE
	ifstream file;                           // doc file
	file.open("F:\\C++\\20021599_NgoHoangKhanhVan\\WORDOFHANGMAN.txt"); // tu SUA LAI duong dan theo may cua minh.
	vector<string> chosenwords;
	bool result = 0;

	while (!file.eof()) {                    // duyet den HET file               
		string line;
		getline(file, line);                 // doc file vao line // moi lan getline lay 1 dong
		if (line[0] - '0' == theme) {        // tim chu de ma nguoi dung chon cho line. 
											 // VD: neu nhap 2 thi line[0] phai == 2 => THEME 2  (chu de da duoc SAP XEP san trong file word)
			result = 1;
			continue;                        // duyet de khi nao chon DUNG CHU DE roi thi thoi.
		}
		else if (line[0] - '0' == theme + 1) // CHAN dau ra cua chu den. 
			break;                           // VD: tuc cac tu CHI TRONG chu de 1, khi duyet tu chu de 2 thi THOAT.

		if (result == 1)
		{
			chosenwords.push_back(normalize(line)); // lay cac phan tu trong chu de vao vector chosen
		}
	}
	// DO KHO 
	int	list = chosenwords.size();
	string findword = chosenwords[rand() % list];
	while (findword.size() != level) {
		findword = chosenwords[rand() % list];   // lay 1 tu trong vector words sao cho co size = level, neu khong thi lay lai. 
	}
	return findword;
}

char readword() { // Ham nhap chu cai.
	char c;
	cout << "Your guess: ";
	cin >> c;
	c = tolower(c);
	return c;
}

bool check(const string word, char c) {     // tim tu nguoi choi doan co trong findword khong.
	return (word.find_first_of(c) != string::npos);
}

string update(string& guessword, const string word, char c) { // neu dung thi hien chu nguoi choi doan dung ra.
	for (int i = word.size() - 1; i >= 0; i--)
	{
		if (c == word[i])
		{
			guessword[i] = c;
		}
	}
	return guessword;
}

void render(string guess_word, int miss_word, string wrongword) {
	// ham in man hinh
	clearScreen();
	hangpicture(miss_word);

	cout << "Mysterious word: " << guess_word << endl;
	cout << "FALIED TIMES: " << miss_word << "     " << "WRONG WORDS: " << wrongword << endl;
	cout << "--------------------------------------" << endl;
	cout << "Want guess all at ONCE ? -> press 0" << endl;

	if ( checkhint[consecutive_wrong] == 3) cout << "Need a hint ? -> press 5" << endl; // cu doan sai 3 lan LIEN TIEP thi 1 HINT
	if (rewards > 0) {
		cout << "You got " << rewards << " REWARDS -> press 9 to use " << endl;         // Neu win lan truoc => +1 HINT lan choi sau.
	}
}

bool TOTAL(string& word, string& guessword) { // Ham cho nguoi choi doan TOAN BO. 
											  // ham nay chay khi nguoi dung nhap '0'.
	cout << "Your ANSWER is : ";
	string a;
	cin >> a;
	if (a == word) {                          // Neu All IN dung thi gan luon guessword = word                             
		guessword = word;
	}
	return 1;
}

int Hint(string& findword, string& guessword) {   // Ham goi y neu nguoi choi doan sai 3 lan 
												  // de chay thi nguoi dung nhap ' 5 '.
	cout << "Hint in place: ";
	int place;
	cin >> place;
	guessword[place - 1] = findword[place - 1];     // show chu cai o place chi dinh
	return 1;
}

void printgameover(const string word, string& guessword) {

	clearScreen();
	if (word == guessword)
	{
		wintimes += 1;
		string hinhanh[] = {
"   -------------\n"
"   |            |\n"
"   |            O\n"
"   |           /|\\\n"
"   |           / \\\n"
"   |             \n"
"   |             \n"
"   |             \n"
"   |             \n"
"-------        ",
"   -------------\n"
"   |            |   \n"
"   |              \n"
"   |            O   \n"
"   |           /|\\   \n"
"   |           / \\ \n"
"   |             \n"
"   |             \n"
"   |             \n"
"-------        ",
"   -------------\n"
"   |            |      \n"
"   |                    \n"
"   |                    \n"
"   |            O    \n"
"   |           <|>   \n"
"   |           / \\  \n"
"   |                 \n"
"   |                 \n"
"-------        ",
"   -------------\n"
"   |            |  \n"
"   |              \n"
"   |               \n"
"   |               \n"
"   |           \\O/  \n"
"   |            | \n"
"   |           / \\ \n"
"   |                \n"
"-------        ",
"   -------------\n"
"   |            |  \n"
"   |               \n"
"   |                \n"
"   |                \n"
"   |                \n"
"   |           \\O/  \n"
"   |            |   \n"
"   |           / \\ \n"
"-------        ",
"   -------------\n"
"   |            |  \n"
"   |               \n"
"   |                \n"
"   |                \n"
"   |                \n"
"   |             O   \n"
"   |           ~~|   \n"
"   |           / \\ \n"
"-------        ",
"   -------------\n"
"   |            |  \n"
"   |               \n"
"   |                \n"
"   |                \n"
"   |                \n"
"   |           O    \n"
"   |           |~~   \n"
"   |           / \\ \n"
"-------        ",

		};
		int solan = 1;
		while (solan <= 7) // so lan lap lai hinh anh 
		{
			for (int i = 0; i < 7; i++)
			{
				clearScreen();
				cout << hinhanh[i];
				this_thread::sleep_for(chrono::milliseconds(12));
			}
			solan++;
			cout << "NOOOOOOOOOOOOOOOOOOOOO   " << endl;
		}
		cout << "       YOU WIN " << endl;
		cout << "CORRECT WORD: " << word << endl << endl;
		cout << "PLEASE MERCY !!! GIVE ME 1 MORE CHANCE" << endl << "TYPE BY YOURSELF: YES or NO " << endl << "YOUR CHOICE: ";


	}
	else
	{

		string hanged[] = {
"   ------------\n"
"   |         /\n"
"   |        O\n"
"   |       /|\\\n"
"   |       / \\\n"
"   |         \n"
"   |         \n"
"   |         \n"
"   |         \n"
"-------        ",
"   ------------\n"
"   |          |\n"
"   |          O\n"
"   |         /|\\\n"
"   |         / \\\n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |           \n"
"-------        ",
"   ------------\n"
"   |           \\\n"
"   |            O\n"
"   |           /|\\\n"
"   |           / \\\n"
"   |             \n"
"   |             \n"
"   |             \n"
"   |             \n"
"-------        ",
"   ------------\n"
"   |          |\n"
"   |          O\n"
"   |         /|\\\n"
"   |         / \\\n"
"   |           \n"
"   |           \n"
"   |           \n"
"   |   \n"
"-------        "
		};
		int solan = 1;
		while (solan <= 7)
		{
			for (int i = 0; i < 4; i++)
			{
				clearScreen();
				cout << hanged[i];
				this_thread::sleep_for(chrono::milliseconds(12));
			}
			solan++;
			cout << "HAAHAHAHAHAHAHAHA" << endl;
		}
		cout << "         YOU ARE DIED " << endl;
		cout << "CORRECT WORD: " << word << endl << endl;
		cout << "WANT TO REVENGE ? TYPE BY YOURSELF: YES or NO " << endl << "YOUR CHOICE: ";
	}
}
bool Replay() {
	string choilai;
	// Choi lai, nhap "no" thi out, nhap "yes" thi choi lai tiep
	cin >> choilai;
	if (choilai == "no" || choilai == "NO")
		return 0;
	else if (choilai == "YES" || choilai == "yes") {
		clearScreen();
		cout << "                     --- YOU WILL BE A DEAD MAN AGAIN ---            " << endl
			<< "TIMES YOU WON: " << wintimes << endl; // DEM so lan WIN.
		return 1;
	}
}
//=============================================================== MODE 2 (OPTIONAL) ===================================================================================== //
bool contains(string word, char guess);
int generateRandomNumber(int min, int max);
bool isCharInWord(char ch, string word);
vector<string> readWordListFromFile(const string& filePath);
bool isAllDash(const string& s);
bool isAllNotDash(const string& s);
set<char> getRemainingChars(const set<char>& previousGuesses);
char selectRandomChar(const set<char>& s);
char getVowelGuess(const set<char>& remainingChars);
bool isSuitableWord(const string& word, const string& secretWord, const set<char>& remainingChars);
vector<string> getSuitableWords(const vector<string>& wordList, const string& secretWord, const set<char>& remainingChars);
map<char, int> getOccurenceCount(const set<char>& remainingChars, const vector<string>& wordList);
char getMaxOccurenceChar(const set<char>& remainingChars, const map<char, int>& count);
char getNextGuess(const set<char>& previousGuesses, const string& secretWord);
string getDrawing(int incorrectGuess);
string getNextHangman();
string getNextStandingman();
int getUserWordLength();
void initialize(int& wordLength, string& secretWord, int& incorrectGuess, set<char>& previousGuesses, bool& stop);
string getUserAnswer(char guess);
bool isGoodMask(char guess, const string& mask, const string& secretWord);
void updateSecretWord(const string& mask, string& secretWord);
void update_mode2(char guess, const string& mask, int& incorrectGuess, set<char>& previousGuesses, string& secretWord, bool& stop);
void render_mode2(int incorrectGuess, const set<char>& previousGuesses, const string& secretWord);
void playAnimation(bool isLosing, const string& word);

// ===================================================================MAIN============================================================================================== //

int main() {
	cout <<"                                 WELCOME TO HELL               " << endl
		<< "                                  HANGMAN GAME           " << endl
		<< "                      -----------------------------------" << endl;
	cout << "                         WHAT MODE DO YOU WANT TO PLAY ? " << endl << endl
		<< "                           YOU GUESS MY WORD => press +" << endl
		<< "                           I GUESS YOUR WORD => press -" << endl
		<< "                      ===================================" << endl;
	char mychoice;
	cin >> mychoice;
//=================================================================MAIN MODE 1============================================================================================//
	if (mychoice == '+') {
		while (true) {
			srand((int)time(0));
			//chooselevel;
			string findword = pickword(level(), theme());        // tu can tim dua theo DO KHO + CHU DE nguoi dung da chon.
			
            int miss_word = 0;
			rewards = wintimes;                                  // Neu win lan truoc => +1 HINT cho lan choi sau.

			string guessword = string(findword.size(), '-');     // luc moi choi guessword se co do dai cua word nhung tat ca la dau '-'
			string wrongword;                // in chu cai da doan sai.
			string checktotal = "NO";
			checkhint[consecutive_wrong] = 0;// map <int, int> checkhint;

			int initial = 0;
			do {
				if (initial == 0) {// ve hinh cot khi moi BAT DAU choi
					hangpicture(0);
					initial = 1;
				}

				render(guessword, miss_word, wrongword);
				char letter = readword();                       // nhap chu cai 
																// ham TOTAL se chay neu nhap vao ' 1 '.
				if (letter == '0') {
					TOTAL(findword, guessword);
					checktotal = "YES";                         // Mau chot de biet ham TOTAL co duoc su dung hay khong trong dieu kien thoat khi THUA.
					break;
				}
				else if (letter == '5' || letter == '9') { // 5 - HINT ; 9- REWARDS
					if (letter == '5') {

						if (checkhint[consecutive_wrong] == 3) {    // sai 3 lan LIEN TIEP => HINT
							Hint(findword, guessword);
							hangpicture(miss_word);               // dua ra goi y ma KHONG mat them 1 mang cua nguoi choi.	
							checkhint[consecutive_wrong] = 0;     // dung xong HINT => reset lai SAI LIEN TIEP = 0.

						}
					}
					else if (letter == '9') {   // Neu win lan truoc => +1 HINT cho lan choi sau.
						if (rewards > 0) {
							Hint(findword, guessword);
							hangpicture(miss_word);
							rewards--;
						}
					}
				}
				else if (check(findword, letter) == 1) {
					guessword = update(guessword, findword, letter);
					checkhint[consecutive_wrong] = 0; // doan DUNG => KHONG sai lien tiep => reset lai bien dem sai lien tiep ve 0.
				}
				else {
					miss_word++;
					wrongword += letter;
					checkhint[consecutive_wrong]++;  // +1 lan neu sai lien tiep
				}
			} while (miss_word < MAX_GUESSES && findword != guessword || checktotal == "YES" && TOTAL(findword, guessword) == 1 && findword != guessword); // toi da 7 lan doan sai HOAC ALL IN sai. 

			printgameover(findword, guessword);
			if (Replay() == 0) {
				break;
			}
		}
	} 
 //================================================================== MAIN MODE 2=============================================================================================//
	else if (mychoice == '-') { 
        string fileName = "F:\\C++\\20021599_NgoHoangKhanhVan\\WORDOFHANGMAN.txt";
            int wordLength;
            string secretWord;
            int incorrectGuess;
            set<char> previousGuesses;
            bool stop;
            //Thiết lập trạng thái đầu của cuộc chơi
            initialize(wordLength, secretWord, incorrectGuess, previousGuesses, stop);

            //Vẽ trạng thái đầu
            render_mode2(incorrectGuess, previousGuesses, secretWord);
            do {
                char guess = getNextGuess(previousGuesses, secretWord);
                if (guess == 0) {
                    cout << "I give up, hang me" << endl;
                    break;
                }
                do {
                    try {
                        string mask = getUserAnswer(guess);
                        update_mode2(guess, mask, incorrectGuess, previousGuesses, secretWord, stop);
                        break;
                    }
                    catch (invalid_argument e) {
                        cout << "Invalid mask, try again" << endl;
                    }
                } while (true);
                render_mode2(incorrectGuess, previousGuesses, secretWord);
            } while (!stop);
            playAnimation(incorrectGuess == MAX_GUESSES, secretWord);
            return 0;
        }
	return 0;
}
// ===============================================================CAC HAM CUA MODE 2=======================================================================================//


bool contains(string word, char guess)
{
    return (word.find(guess) != string::npos);
}


int generateRandomNumber(int min, int max)
{
    return rand() % (max - min) + min;
}

bool isCharInWord(char ch, string word)
{
    return (word.find_first_of(ch) != string::npos);
}

vector<string> readWordListFromFile(const string& filePath)
{
    vector<string> wordList;
    string word;
    ifstream wordFile(filePath);
    if (wordFile.is_open()) {
        while (wordFile >> word) {
            wordList.push_back(word);
        }
        wordFile.close();
        return wordList;
    }
    else {
        throw domain_error("Error: Unable to open vocabulary file " + filePath);
    }
}

bool isAllDash(const string& s)
{
    for (char c : s)
        if (c != '-') return false;
    return true;
}

bool isAllNotDash(const string& s)
{
    for (char c : s)
        if (c == '-') return false;
    return true;
}
set<char> getRemainingChars(const set<char>& previousGuesses)
{
    set<char> remainingChars;
    for (char c = 'a'; c <= 'z'; c++)
        remainingChars.insert(c);
    for (char c : previousGuesses)
        remainingChars.erase(c);
    return remainingChars;
}

char selectRandomChar(const set<char>& s) {
    int r = rand() % s.size();
    set<char>::iterator it = s.begin();
    for (; r != 0; r--) it++;
    return *it;
}

char getVowelGuess(const set<char>& remainingChars)
{
    char vowel[] = { 'e', 'a', 'o', 'i', 'u' };
    for (char c : vowel) {
        if (remainingChars.find(c) != remainingChars.end())
            return c;
    }
    return 0;
}

bool isSuitableWord(const string& word, const string& secretWord, const set<char>& remainingChars)
{
    if (word.length() != secretWord.length()) return false;
    for (unsigned int i = 0; i < word.length(); i++) {
        if (secretWord[i] != '-') {
            if (tolower(word[i]) != tolower(secretWord[i])) return false;
        }
        else if (remainingChars.find(word[i]) == remainingChars.end())
            return false;
    }
    return true;
}

vector<string> getSuitableWords(const vector<string>& wordList,const string& secretWord,const set<char>& remainingChars)
{
    vector<string> result;
    for (const string& word : wordList)
        if (isSuitableWord(word, secretWord, remainingChars))
            result.push_back(word);
    return result;
}

map<char, int> getOccurenceCount(const set<char>& remainingChars, const vector<string>& wordList)
{
    map<char, int> count;
    for (char c : remainingChars) count[c] = 0;

    for (const string& word : wordList) {
        for (char c : word)
            if (count.find(c) != count.end())
                count[c]++;
    }
    return count;
}

char getMaxOccurenceChar(const set<char>& remainingChars, const map<char, int>& count)
{
    char best = 0;
    int best_count = 0;
    for (auto p : count)
        if (p.second > best_count) {
            best = p.first;
            best_count = p.second;
        }
    return best;
}

char getNextGuess(const set<char>& previousGuesses, const string& secretWord)
{
    static vector<string> wordList = readWordListFromFile("F:\\C++\\20021599_NgoHoangKhanhVan\\WORDOFHANGMAN.txt");
    set<char> remainingChars = getRemainingChars(previousGuesses);
    if (remainingChars.size() == 0)
        return 0;

    if (isAllDash(secretWord))
        return getVowelGuess(remainingChars);

    vector<string> filteredWordList = getSuitableWords(wordList, secretWord, remainingChars);
    map<char, int> occurenceCount = getOccurenceCount(remainingChars, filteredWordList);
    return getMaxOccurenceChar(remainingChars, occurenceCount);
}

string getDrawing(int incorrectGuess)
{
    static const string figure[] = {
        "   -------------    \n"
        "   |                \n"
        "   |                \n"
        "   |                \n"
        "   |                \n"
        "   |     \n"
        " -----   \n",
        "   -------------    \n"
        "   |           |    \n"
        "   |                \n"
        "   |                \n"
        "   |                \n"
        "   |     \n"
        " -----   \n",
        "   -------------    \n"
        "   |           |    \n"
        "   |           O    \n"
        "   |                \n"
        "   |                \n"
        "   |     \n"
        " -----   \n",
        "   -------------    \n"
        "   |           |    \n"
        "   |           O    \n"
        "   |           |    \n"
        "   |                \n"
        "   |     \n"
        " -----   \n",
        "   -------------    \n"
        "   |           |    \n"
        "   |           O    \n"
        "   |          /|    \n"
        "   |                \n"
        "   |     \n"
        " -----   \n",
        "   -------------    \n"
        "   |           |    \n"
        "   |           O    \n"
        "   |          /|\\  \n"
        "   |                \n"
        "   |     \n"
        " -----   \n",
        "   -------------    \n"
        "   |           |    \n"
        "   |           O    \n"
        "   |          /|\\  \n"
        "   |          /     \n"
        "   |     \n"
        " -----   \n",
        "   -------------    \n"
        "   |           |    \n"
        "   |           O    \n"
        "   |          /|\\  \n"
        "   |          / \\  \n"
        "   |     \n"
        " -----   \n",
    };

    const int NUMBER_OF_FIGURES = sizeof(figure) / sizeof(string);
    return figure[incorrectGuess % NUMBER_OF_FIGURES];
}

string getNextHangman()
{
    static string figure[] = {
          "   ------------+    \n"
          "   |          /     \n"
          "   |         O      \n"
          "   |        /|\\    \n"
          "   |        / \\    \n"
          "   |        \n"
          " -----      \n" ,
          "   ------------+     \n"
          "   |           |     \n"
          "   |           O     \n"
          "   |          /|\\   \n"
          "   |          / \\   \n"
          "   |        \n"
          " -----      \n",
          "   ------------+      \n"
          "   |            \\    \n"
          "   |            O     \n"
          "   |           /|\\   \n"
          "   |           / \\   \n"
          "   |      \n"
          " -----    \n",
          "   ------------+     \n"
          "   |           |     \n"
          "   |           O     \n"
          "   |          /|\\   \n"
          "   |          / \\   \n"
          "   |        \n"
          " -----      \n",
    };
    const int NUMBER_OF_FIGURES = sizeof(figure) / sizeof(string);
    static int currentFigure = 0;
    return figure[(currentFigure++) % NUMBER_OF_FIGURES];
}

string getNextStandingman()
{
    static string figure[] = {
          "     O     \n"
          "    /|\\   \n"
          "    | |    \n",
          "     O     \n"
          "    /|\\   \n"
          "    / \\   \n",
          "   _O_   \n"
          "     |     \n"
          "    / \\   \n",
          "    \\O/   \n"
          "     |     \n"
          "    / \\   \n",
          "   _O_   \n"
          "     |     \n"
          "    / \\   \n",
          "     O     \n"
          "    /|\\   \n"
          "    / \\   \n" ,
          "    O     \n"
          "    /|\\   \n"
          "    / \\   \n" ,
          "     O     \n"
          "    /|\\   \n"
          "    / \\   \n" ,
          "      O     \n"
          "    /|\\   \n"
          "    / \\   \n" ,
          "     O     \n"
          "    /|\\   \n"
          "    / \\   \n" ,
    };
    const int NUMBER_OF_FIGURES = sizeof(figure) / sizeof(string);
    static int currentFigure = 0;
    return figure[(currentFigure++) % NUMBER_OF_FIGURES];
}

//Lấy chiều dài từ của người chơi
int getUserWordLength()
{
    int wordLength;
    cout << endl << "Enter your word length: ";
    cin >> wordLength;
    return wordLength;
}

//Khởi tạo các thông tin ban đầu trống lúc bắt đầu đoán
void initialize(int& wordLength, string& secretWord,int& incorrectGuess, set<char>& previousGuesses,bool& stop)
{
    wordLength = getUserWordLength();       //Độ dài từ
    secretWord = string(wordLength, '-');   //Tạo dòng trống ban đầu
    incorrectGuess = 0;                     //Lỗi của máy
    previousGuesses = set<char>();          //Ban đầu không có ký tự nào đã được đoán
    stop = false;                           //Khi sai gặp biến 'stop' sẽ dừng thao tác
}

//Yêu cầu người chơi phản hồi đối với câu trả lời của máy
string getUserAnswer(char guess)
{
    string answerMask;

    //Máy thông báo dự đoán của mình, người chơi nhập mask để phản hồi
    cout << endl << "I guess " << guess << ", please enter your mask: ";
    cin >> answerMask;

    //Biến các ký tự hoa thành thường
    transform(answerMask.begin(), answerMask.end(), answerMask.begin(), ::tolower);
    return answerMask;
}

//Đối chiếu phần phản hồi của người dùng
bool isGoodMask(char guess, const string& mask, const string& secretWord)
{
    //Kiểm tra xem độ dài của phản hồi có phù hợp với câu hỏi ban đầu
    if (mask.length() != secretWord.length())
        return false;

    //Đối chiếu phỏng đoán
    for (unsigned int i = 0; i < secretWord.length(); i++)
        if (mask[i] != '-')         //Tìm đến chỗ đã để lộ
        {
            if (mask[i] != guess)   //Nếu chỗ đoán
                return false;
            if (secretWord[i] != '-' && secretWord[i] != mask[i])
                return false;
        }
    return true;
}

void updateSecretWord(const string& mask, string& secretWord)
{
    for (unsigned int i = 0; i < secretWord.length(); i++)
        if (mask[i] != '-')
            secretWord[i] = mask[i];
}

void update_mode2(char guess, const string& mask,int& incorrectGuess, set<char>& previousGuesses,string& secretWord, bool& stop)
{
    if (!isGoodMask(guess, mask, secretWord))
        throw invalid_argument("mistake entering answer");

    previousGuesses.insert(guess);
    if (isAllDash(mask)) {
        incorrectGuess++;
        if (incorrectGuess == MAX_GUESSES) stop = true;
    }
    else {
        updateSecretWord(mask, secretWord);
        if (isAllNotDash(secretWord)) stop = true;
    }
}

void render_mode2(int incorrectGuess, const set<char>& previousGuesses,const string& secretWord)
{
    clearScreen();
    cout << endl << "Incorrect guess = " << incorrectGuess
        << "   previous guesses = ";
    for (char c : previousGuesses)
        cout << c;
    cout << "   secretWord = " << secretWord << endl;
    cout << getDrawing(incorrectGuess) << endl;
}

void playAnimation(bool isLosing, const string& word)
{
    clearScreen();
    while (true) {
        if (isLosing)
            cout << endl << "I lost 🙁. My best word is: " << word << endl;
        else
            cout << endl << "Haha, I win 😃. The word is: " << word << endl;
        cout << (isLosing ? getNextHangman() : getNextStandingman());
        this_thread::sleep_for(chrono::milliseconds(12));
        clearScreen();
    }
}

/* CAC YEU CAU DA HOAN THANH
1. lua chon tu theo chu de
2. them goi y neu sai 3 lan LIEN TIEP.
3. do kho theo do dai ngan cua tu
4. sai nhieu lan co the tang toc do hangman.
5. replay
6. thang thi + 1 reward mua goi y lan choi sau
7. doan luon neu biet tu la gi (ALL IN)
8. (optional) nguoi nghi may doan 
*/
