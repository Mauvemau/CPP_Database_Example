#include "misc_utils.h"

#include <functional>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#ifndef _WIN32
int _kbhit(void) {
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if (ch != EOF) {
		ungetc(ch, stdin);
		return 1;
	}

	return 0;
}

int _getch(void) {
	struct termios oldt, newt;
	int ch;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}
#endif

static const vector<string> animalNames = {
	"Aardvark", "Albatross", "Alligator", "Alpaca", "Ant", "Anteater", "Antelope", "Ape", "Armadillo", "Donkey",
	"Baboon", "Badger", "Barracuda", "Bat", "Bear", "Beaver", "Bee", "Bison", "Boar", "Buffalo",
	"Butterfly", "Camel", "Capybara", "Caribou", "Cassowary", "Cat", "Caterpillar", "Cattle", "Chamois", "Cheetah",
	"Chicken", "Chimpanzee", "Chinchilla", "Chough", "Clam", "Cobra", "Cockroach", "Cod", "Cormorant", "Coyote",
	"Crab", "Crane", "Crocodile", "Crow", "Curlew", "Deer", "Dinosaur", "Dog", "Dogfish", "Dolphin",
	"Dotterel", "Dove", "Dragonfly", "Duck", "Dugong", "Dunlin", "Eagle", "Echidna", "Eel", "Eland",
	"Elephant", "Elk", "Emu", "Falcon", "Ferret", "Finch", "Fish", "Flamingo", "Fly", "Fox",
	"Frog", "Gaur", "Gazelle", "Gerbil", "Giraffe", "Gnat", "Gnu", "Goat", "Goldfinch", "Goldfish",
	"Goose", "Gorilla", "Goshawk", "Grasshopper", "Grouse", "Guanaco", "Gull", "Hamster", "Hare", "Hawk",
	"Hedgehog", "Heron", "Herring", "Hippo", "Hornet", "Horse", "Hummingbird", "Hyena", "Ibex",
	"Ibis", "Jackal", "Jaguar", "Jay", "Jellyfish", "Kangaroo", "Kingfisher", "Koala", "Kookaburra", "Kouprey",
	"Kudu", "Lapwing", "Lark", "Lemur", "Leopard", "Lion", "Llama", "Lobster", "Locust", "Loris",
	"Louse", "Lyrebird", "Magpie", "Mallard", "Manatee", "Mandrill", "Mantis", "Marten", "Meerkat", "Mink",
	"Mole", "Mongoose", "Monkey", "Moose", "Mosquito", "Mouse", "Mule", "Narwhal", "Newt", "Nightingale",
	"Octopus", "Okapi", "Opossum", "Oryx", "Ostrich", "Otter", "Owl", "Oyster", "Panther", "Parrot",
	"Partridge", "Peafowl", "Pelican", "Penguin", "Pheasant", "Pig", "Pigeon", "Pony", "Porcupine", "Porpoise",
	"Quail", "Quelea", "Quetzal", "Rabbit", "Raccoon", "Rail", "Ram", "Rat", "Raven", "Red deer",
	"Red panda", "Reindeer", "Rhino", "Rook", "Salamander", "Salmon", "Sand Dollar", "Sandpiper", "Sardine", "Scorpion",
	"Seahorse", "Seal", "Shark", "Sheep", "Shrew", "Skunk", "Snail", "Snake", "Sparrow", "Spider",
	"Spoonbill", "Squid", "Squirrel", "Starling", "Stingray", "Stork", "Swallow", "Swan", "Tapir",
	"Tarsier", "Termite", "Tiger", "Toad", "Trout", "Turkey", "Turtle", "Viper", "Vulture", "Wallaby",
	"Walrus", "Wasp", "Weasel", "Whale", "Wildcat", "Wolf", "Wolverine", "Wombat", "Woodcock", "Woodpecker",
	"Worm", "Wren", "Yak", "Zebra", "Antlion", "Aphid", "Armyworm", "Assassin Bug", "Backswimmer", "Bedbug", "Beetle", "Blister Beetle", "Booklouse", 
	"Boxelder Bug", "Bumblebee", "Caddisfly", "Carpenter Ant", "Carpenter Bee", "Carpet Beetle", "Caterpillar", "Cicada", 
	"Click Beetle", "Clothes Moth", "Cockchafer", "Crane Fly", "Cricket", "Damselfly", "Darkling Beetle", "Dobsonfly", "Dragonfly", 
	"Dung Beetle", "Earwig", "Flea", "Firefly", "Frit Fly", "Fruit Fly", "Fungus Gnat", "Giant Water Bug", "Glowworm", "Leafhopper", "Green Lacewing",
	"Ground Beetle", "Gypsy Moth", "Harlequin Bug", "Harvestman", "Head Louse", "Hoverfly", "House Cricket", "Housefly", "Ichneumon Wasp", "Japanese Beetle", 
	"June Bug", "Katydid", "Kissing Bug", "Lace Bug", "Ladybird Beetle", "Leaf Beetle", "Leafhopper", "Longhorn Beetle", "Lovebug", "Mayfly", "Mealybug", 
	"Mole Cricket", "Moth", "Mud Dauber", "Net-winged Beetle", "Oakworm", "Paper Wasp", "Pea Aphid", "Pill Bug", "Praying Mantis", "Puss Moth", "Reduviid Bug", 
	"Rove Beetle", "Sawfly", "Scarab Beetle", "Scorpionfly", "Seed Beetle", "Shield Bug", "Silverfish", "Soldier Beetle", "Green Stink Bug", "Spider Wasp",
	"Spittlebug", "Stag Beetle", "Stink Bug", "Stonefly", "Striped Beetle", "Ground Cricket", "Walkingstick", "Termite", "Caterpillar", "Tachinid Fly", 
	"Tarsonemid Mite", "Thrips", "Tiger Beetle", "Treehopper", "Tsetse Fly", "Vinegar Fly", "Walkingstick", "Water Strider", "Weevil", "Whirligig Beetle", 
	"Whitefly", "Yellowjacket", "Zebra Swallowtail", "Zorapteran", "Zygoptera", "Army Ant", "Argentine Ant", "Asian Lady Beetle", "Australian Cockroach", 
	"Bark Beetle", "Bat Bug", "Bean Leaf Beetle", "Bird Louse", "Black Vine Weevil", "Blue Bottle Fly", "Brazilian Cockroach", "Broad-headed Bug", 
	"Brown-banded Cockroach", "Bulb Mite", "Carrot Rust Fly", "Cherry Fruit Fly", "Chinch Bug", "Cicada Killer", "Clover Mite", "Coconut Crab", 
	"Furniture Beetle", "Corn Earworm", "Cucumber Beetle", "Cutworm", "Deathwatch Beetle", "Dermestid Beetle", "Corn Borer", "Eyed Click Beetle", 
	"Flat-headed Borer", "Tent Caterpillar", "Plant Bug", "Fungus Beetle", "Gall Wasp", "Garden Spider", "Tortoise Beetle", "Grape Berry Moth"
};

string MiscUtils::getPetTypeString(PetType type) {
	string str = "";
	switch (type)
	{
	case PetType::FIRE:
		str = "FIRE";
		break;
	case PetType::WATER:
		str = "WATER";
		break;
	case PetType::GRASS:
		str = "GRASS";
		break;
	default:
		break;
	}
	return str;
}

string MiscUtils::getRandomAnimalName() {
	int index = getRandomInt(0, animalNames.size() - 1);
	return animalNames[index];
}

int MiscUtils::getRandomInt(int min, int max) {
	srand(static_cast<unsigned>(time(0)));
	if (min > max) {
		int aux = max;
		max = min;
		min = aux;
	}

	return (rand() % (abs(max - min) + 1) + min);
}

bool MiscUtils::isSafeCharacter(char input, bool allowSpecialChars) {
	if (isalnum(input)) {
		return true;
	}
	if (allowSpecialChars) {
		return true;
	}
	return false;
}

void MiscUtils::clearScreen()
{
#ifdef _WIN32
	HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { 0, 0 };
	DWORD dw;
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
	DWORD dwSize;

	cout.flush();
	GetConsoleScreenBufferInfo(outputHandle, &screenBufferInfo);

	dwSize = screenBufferInfo.dwSize.X * screenBufferInfo.dwSize.Y;

	FillConsoleOutputCharacter(outputHandle, ' ', dwSize, coord, &dw);
	FillConsoleOutputAttribute(outputHandle, screenBufferInfo.wAttributes, dwSize, coord, &dw);
	SetConsoleCursorPosition(outputHandle, coord);
#else
	printf("%s[2J%s[H", ESC, ESC);
	fflush(stdout);
#endif
}

int MiscUtils::getKey(bool wait)
{
	int key;

	if (wait || _kbhit())
	{
		key = _getch();

		if (key == 0 || key == 224)
			key += 256;
	}
	else
	{
		key = 0;
	}

	return key;
}

void MiscUtils::waitUserInput() {
	cout << "\nPress any key to continue.";
	getKey(true);
	cout << "\n";
}

string MiscUtils::SafeInput(bool allowAbort) {
	string returnString = "";
	char input;
	bool abort = false;

	do {
		input = getKey(true);

		switch (input) {
		case KEY_ESC:
			if (allowAbort) {
				returnString = MAGIC_STRING;
				cout << "\n";
				abort = true;
			}
			break;
		case KEY_ENTER:
			cout << "\n";
			break;
		case static_cast<char>(8):
			if (returnString.size() > 0) {
				cout << "\b \b";
				returnString.pop_back();
			}
			break;
		default:
			returnString.push_back(input);
			cout << input;
			break;
		}
	} while (!abort && input != KEY_ENTER);
	return returnString;
}

string MiscUtils::SafeInput(bool allowAbort, bool allowSpacing, bool allowSpecialChars, bool censorInput) {
	string returnString = "";
	char input;
	do {
		input = getKey(true);

		switch (input) {
		case KEY_ESC:
			if (allowAbort) {
				returnString = MAGIC_STRING;
				cout << "\n";
			}
			break;
		case KEY_ENTER:
			cout << "\n";
			break;
		case static_cast<char>(8):
			if (returnString.size() > 0) {
				cout << "\b \b";
				returnString.pop_back();
			}
			break;
		case ' ':
			if (allowSpacing) {
				returnString.push_back(input);
				if (censorInput) {
					cout << "*";
				}
				else {
					cout << input;
				}
			}
			break;
		default:
			if (isSafeCharacter(input, allowSpecialChars)) {
				returnString.push_back(input);
				if (censorInput) {
					cout << "*";
				}
				else {
					cout << input;
				}
			}
			break;
		}
	} while (input != KEY_ESC && input != KEY_ENTER);
	return returnString;
}

string MiscUtils::SafeInput(bool allowAbort, bool allowSpacing, bool allowSpecialChars, bool censorInput, int charLimit) {
	string returnString = "";
	char input;
	int charCount = 0;
	do {
		input = getKey(true);

		switch (input) {
		case KEY_ESC:
			if (allowAbort) {
				returnString = MAGIC_STRING;
				cout << "\n";
			}
			break;
		case KEY_ENTER:
			cout << "\n";
			break;
		case static_cast<char>(8):
			if (returnString.size() > 0) {
				cout << "\b \b";
				returnString.pop_back();
				charCount--;
			}
			break;
		case ' ':
			if (allowSpacing && returnString.size() < charLimit) {
				returnString.push_back(input);
				if (censorInput) {
					cout << "*";
				}
				else {
					cout << input;
				}
				charCount++;
			}
			break;
		default:
			if (returnString.size() < charLimit && isSafeCharacter(input, allowSpecialChars)) {
				returnString.push_back(input);
				if (censorInput) {
					cout << "*";
				}
				else {
					cout << input;
				}
				charCount++;
			}
			break;
		}
	} while (input != KEY_ESC && input != KEY_ENTER);
	return returnString;
}

string MiscUtils::safeInputLetters(bool allowAbort, bool censorInput) {
	string returnString = "";
	char input;
	do {
		input = getKey(true);

		switch (input) {
		case KEY_ESC:
			if (allowAbort) {
				returnString = MAGIC_STRING;
				cout << "\n";
			}
			break;
		case KEY_ENTER:
			cout << "\n";
			break;
		case static_cast<char>(8):
			if (returnString.size() > 0) {
				cout << "\b \b";
				returnString.pop_back();
			}
			break;
		case ' ':
			break;
		default:
			if (isalpha(input)) {
				returnString.push_back(input);
				if (censorInput) {
					cout << "*";
				}
				else {
					cout << input;
				}
			}
			break;
		}
	} while (input != KEY_ESC && input != KEY_ENTER);
	return returnString;
}

string MiscUtils::safeInputLetters(bool allowAbort, bool censorInput, int charLimit) {
	string returnString = "";
	char input;
	int charCount = 0;
	do {
		input = getKey(true);

		switch (input) {
		case KEY_ESC:
			if (allowAbort) {
				returnString = MAGIC_STRING;
				cout << "\n";
			}
			break;
		case KEY_ENTER:
			cout << "\n";
			break;
		case static_cast<char>(8):
			if (returnString.size() > 0) {
				cout << "\b \b";
				returnString.pop_back();
				charCount--;
			}
			break;
		case ' ':
			break;
		default:
			if (returnString.size() < charLimit && isalpha(input)) {
				returnString.push_back(input);
				if (censorInput) {
					cout << "*";
				}
				else {
					cout << input;
				}
				charCount++;
			}
			break;
		}
	} while (input != KEY_ESC && input != KEY_ENTER);
	return returnString;
}

string MiscUtils::safeInputNumbers(bool allowAbort, bool censorInput) {
	string returnString = "";
	char input;
	do {
		input = getKey(true);

		switch (input) {
		case KEY_ESC:
			if (allowAbort) {
				returnString = MAGIC_STRING;
				cout << "\n";
			}
			break;
		case KEY_ENTER:
			cout << "\n";
			break;
		case static_cast<char>(8):
			if (returnString.size() > 0) {
				cout << "\b \b";
				returnString.pop_back();
			}
			break;
		case ' ':
			break;
		default:
			if (isdigit(input)) {
				returnString.push_back(input);
				if (censorInput) {
					cout << "*";
				}
				else {
					cout << input;
				}
			}
			break;
		}
	} while (input != KEY_ESC && input != KEY_ENTER);
	return returnString;
}

string MiscUtils::safeInputNumbers(bool allowAbort, bool censorInput, int charLimit) {
	string returnString = "";
	char input;
	int charCount = 0;
	do {
		input = getKey(true);

		switch (input) {
		case KEY_ESC:
			if (allowAbort) {
				returnString = MAGIC_STRING;
				cout << "\n";
			}
			break;
		case KEY_ENTER:
			cout << "\n";
			break;
		case static_cast<char>(8):
			if (returnString.size() > 0) {
				cout << "\b \b";
				returnString.pop_back();
				charCount--;
			}
			break;
		case ' ':
			break;
		default:
			if (returnString.size() < charLimit && isdigit(input)) {
				returnString.push_back(input);
				if (censorInput) {
					cout << "*";
				}
				else {
					cout << input;
				}
				charCount++;
			}
			break;
		}
	} while (input != KEY_ESC && input != KEY_ENTER);
	return returnString;
}

string MiscUtils::toLowerCase(string input) {
	string result = input;
	for (char& c : result) {
		c = std::tolower(c);
	}
	return result;
}

bool MiscUtils::containsSpace(const char input[]) {
	for (size_t i = 0; input[i] != '\0'; i++) {
		if (input[i] == ' ') {
			cout << "\nInput provided contains a space! Please input a value without spaces!\n> ";
			return true;
		}
	}
	return false;
}

void MiscUtils::validateCharInput(char input[], size_t size) {
	bool validInput = false;
	while (!validInput) {
		cin.getline(input, size);

		if (input[0] != '\0' && input[0] != ' ') {
			validInput = true;
		}
	}
}

template <typename T>
T MiscUtils::validateInput() {
	T input;
	cin >> input;
	while (cin.fail()) {
		cout << "[Input failed!] Please input a value of type" << typeid(input).name() << ".\n> ";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> input;
	}
	return input;
}

string MiscUtils::hashString(string inputString) {
	hash<string> hasher;
	return to_string(hasher(inputString));
}

string MiscUtils::usernameInput() {
	string input;
	input = SafeInput(true, false, false, false, 20);
	return input;
}

string MiscUtils::passwordInput() {
	string input;
	input = SafeInput(true, true, true, true, 20);
	if (input != MAGIC_STRING) {
		return hashString(input);
	}
	else {
		return input;
	}
}

map<string, string> MiscUtils::parseDatabaseConfigFile(string path) {
	map<string, string> config;
	try {
		ifstream inputFile(path);
		if (!inputFile.is_open()) {
			throw runtime_error("Could not open configuration file");
		}

		std::string line;
		while (std::getline(inputFile, line)) {
			istringstream is_line(line);
			string key;
			if (getline(is_line, key, '=')) {
				std::string value;
				if (getline(is_line, value)) {
					config[key] = value;
				}
			}
		}

		inputFile.close();
	}
	catch (const exception& e) {
		cerr << "Error parsing configuration file: " << e.what() << std::endl;
		throw;
	}
	return config;
}