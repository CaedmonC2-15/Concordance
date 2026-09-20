//Made by Caedmon Capron, but certain features were via AI
//the major feature from AI is the line length resizing feature that makes the text look good
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#if defined(_WIN32)
    #include <windows.h>
#elif defined(__linux__) || defined(__ANDROID__)
    #include <sys/ioctl.h>
    #include <unistd.h>
#endif

size_t get_terminal_width() { // strait from AI
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        // Calculate width from the window coordinates
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
#elif defined(__linux__) || defined(__ANDROID__)
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        return w.ws_col;
    }
#endif
    return 80; // Default fallback if detection fails
}

bool targetFile(const std::string& filename, std::ifstream& fileStream) {
    fileStream.open(filename, std::ios::binary); //opens the file
    return fileStream.is_open();
}
size_t getFileCharCount(std::ifstream& fileStream) {
    if (!fileStream.is_open()) return 0; //this gives errors
    fileStream.seekg(0, std::ios::end); //this fills up the variable used later with the appropriate size
    size_t charCount = fileStream.tellg(); //this accesses the prefilled variable 
    fileStream.clear(); //this clears all of the work done on the filestream
    fileStream.seekg(0, std::ios::beg); //this resets the cursor to the bottom of the file

    return charCount;
}
void fillVectorWithExactChars(std::ifstream& fileStream, std::vector<char>& outVector, size_t exactSize) {
	if (!fileStream.is_open() || exactSize == 0) return; //this returns errors
	outVector.resize(exactSize); // this resizes the vector to the appropriate size
	fileStream.read(&outVector[0], exactSize); //this fills the vector with the text file
}
void getSentences(std::vector<std::string>& output, std::vector<char>& input) {
	size_t size = input.size();
	size_t length = 0;
	output.push_back("");
	size--;
	for(size_t i = 0; i < size; i++) {
	switch(input[i]) {
		case '\n':
		length++;
		output.push_back("");
		break;
		default:
		output[length] += input[i];
		break;
	}
	}
}
void toLowercase(std::vector<char>& output) {
	for(size_t a = 0; a < output.size(); a++) {
		if(output[a] > 64 && output[a] < 91) {
			output[a] += 32;
		}
	}
}
void getText(std::vector<std::string>& output, std::vector<char>& input) {
	size_t size = input.size();
	size_t length = 0;
	std::string manage = "";
	bool inputing = true;
	for(size_t i = 0; i < size; i++) {
		switch(input[i]) {
			case '\n':
				inputing = false;
			break;
			case ':':
				manage += "Example:";
				inputing = true;
			break;
			default:
				if(std::isdigit(input[i])) {
					if(inputing == false) {
						output.push_back(manage);
						length++;
						manage = "";
					}
					manage += input[i];
				}
				else {
					if(inputing) {
						manage += input[i];
					}
				}
			break;
		}
	}
}
size_t getPlace(std::string word, std::vector<std::string>& input) {
	size_t size = input.size();
		size_t start = 0;
		size_t end = size;
		while(true) {
			if(end-start < 5) {
				for(size_t b = start; b < end; b++) {
					if(word <= input[b]) {
						return b;
					}
				}
				return end;
			}
			size_t target = (start+end)/2;
			bool result = (word < input[target]);
			if(result) {
				if(end == target) {
					return target;
				}
				end = target;
			}
			else {
				if(start == target) {
					return target;
				}
				start = target;
			}
		}
		return 0;
}
void getWords(std::vector<std::string>& output, std::vector<char>& input) {
	size_t size = input.size();
	size_t length = 0;
	output.push_back("");
	for(size_t i = 0; i < size; i++) {
	switch(input[i]) {
		case 32:
		length++;
		output.push_back(" ");
		break;
		default:
		output[length] += input[i];
		break;
	}
	}
}
void getUniqueWords(std::vector<std::string>& output, std::vector<std::string>& input) {
	size_t size = input.size();
	size_t length = 0;
	for(size_t a = 0; a < size; a++) {
		size_t location = getPlace(input[a], output);
		if(location < length) {
			if(output[location] == input[a]) {
				continue;
			}
		}
		output.insert(output.begin()+location, input[a]);
		length++;
	}
}
void extractBrackets(std::vector<std::string>& output, std::vector<char>& input) {

	size_t size = input.size();
	for(size_t a = 0; a < size; a++) {
		if(input[a] == '[') {
			std::string remover = "";
			input.erase(input.begin()+a);
			bool looping = true;
			while(looping) {
				if(input[a] == ']') {
					input.erase(input.begin()+a);
					input.erase(input.begin()+a);
					break;
				}
				remover += input[a];
				input.erase(input.begin()+a);
			}
			std::cout << a << '\n';
			output.push_back(remover);
		}
	}
}
void removeCharacters(std::vector<char>& output, std::string list) {
	size_t size = output.size();
	size_t length = list.length();
	for(size_t a = 0; a < size; a++) {
		for(size_t b = 0; b < length; b++) {
			if(output[a] == list[b]) {
				output.erase(output.begin()+a);
				a--;
				size--;
				break;
			}
		}
	}
}
void quickSift(std::vector<std::string>& input) {
	size_t size = input.size();
	for(size_t a = 0; a < size; a++) {
		std::string checker = input[a];
		for(size_t b = a+1; b < size; b++) {
			if(checker == input[b]) {
				input.erase(input.begin()+b);
				size--;
				b--;
				continue;
			}
		}
	}
}
void getVerses(std::vector<std::string>& output, std::vector<char>& input) {
	size_t size = input.size();
	size_t length = 0;
	output.push_back("");
	for(size_t a = 0; a < size; a++) {
		if(input[a] == '\n') {
			output.push_back("");
			length++;
		}
		else {
			output[length]+=input[a];
		}
		
	}
}
void useConcordance(std::vector<std::string>& output, std::vector<std::string>& input, std::string phrase) {
	size_t size = input.size();
	for(size_t a = 0; a < size; a++) {
		if(input[a].find(phrase) != std::string::npos) {
			output.push_back(input[a]);
		}
	}
}
void replaceSymbol(std::vector<char>& outputText, const std::vector<char>& inputText, char targetChar, const std::string& replacementStr) {
    
    // Clear the output vector first to ensure it's a fresh buffer
    outputText.clear();

    // Loop through the input vector character by character
    for (char c : inputText) {
        if (c == targetChar) {
            // If we matchs the target char, insert the whole string into the output vector
            outputText.insert(outputText.end(), replacementStr.begin(), replacementStr.end());
        } else {
            // If it doesn't match, keep the original character untouched
            outputText.push_back(c);
        }
    }
}
void combineVectors(std::string& output, std::vector<std::string>& input) {
	size_t size = input.size();
	for(size_t a = 0; a < size; a++) {
		output+=input[size];
	}
}
void replaceSymbols(std::vector<char>& output, std::vector<char>& input, char find, std::string replace) {
	size_t size = input.size();
	for(size_t a = 0; a < size; a++){
		if(input[a] == find) {
			output.insert(output.end(), replace.begin(), replace.end()); // from Gemini
		}
		else {
			output.push_back(input[a]);
		}
	}
}
void massSeperation(std::vector<std::string>& output, std::string& input, char choice) {
	size_t size = input.size();
	size_t index = 0;
	output.push_back("");
	for(size_t a = 0; a < size; a++) {
		if(input[a] == choice) {
			output.push_back("");
			index++;
		}
		else {
			output[index]+=input[a];
		}
	}
}
bool startsWith(std::string& compare, std::string& field) {
	size_t size = compare.size();
	for(size_t a = 0; a+1 < size; a++) {
		if(field[a] != compare[a]) {
			return false;
		}
	}
	return true;
}
void formatExamples(std::vector<std::string>& output, std::vector<std::string>& input) {
	size_t size = input.size();
	bool skipped = false;
	size_t index = 0;
	for(size_t a = 0; a < size; a++) {
		std::string checker = "Example:";
		if(startsWith(checker, input[a])) {
			output[index] += "\n"+input[a];
		}
		else {
			if(skipped) {
				output[index] += "\n";
				index++;
			}
			else {
				skipped = true;
			}
			output.push_back("");
			output[index] += input[a];
		}
	}
}
void removeNumbers(std::vector<std::string>& output) {
	
}
size_t getIndex(size_t current, std::string& field) {
	for(size_t a = current; a >= 0; a--) {
		if(field[a] == ' ') {
			return a;
		}
	}
	std::cout << "big time error" << "\n" << "\n" << "\n";
	return -1;
}
void addBreaks(std::vector<std::string>& output, size_t maxLength) {
	size_t size = output.size();
	for(size_t a = 0; a < size; a++) {
		size_t index = output[a].length();
		size_t currentLength = 0;
		for(size_t b = 0; b < index; b++) {
			currentLength++;
			if(output[a][b] == '\n') {
				currentLength = 0;
			}
			else {
				if(currentLength >= maxLength) {
					size_t location = getIndex(b, output[a]);
					output[a][location] = '\n';
					currentLength = b-location;
				}
			}
		}
	}
}
int main() {
    std::ifstream myFile; //this is the filesystem manager
    std::string filename = "bible.txt"; // this is my string that I am targeting

    if (!targetFile(filename, myFile)) { // this targets the file
        std::cerr << "Failed to open file.\n"; //this gives an error
        return 1;
    }
    size_t totalChars = getFileCharCount(myFile); // this gets the character amount
    std::vector<char> modernCharVector; // this is making the vector to store the data without sizing it
    fillVectorWithExactChars(myFile, modernCharVector, totalChars); //this fills the vector up
    std::vector<std::string> verses;
	getVerses(verses, modernCharVector);
	std::vector<std::string> formatted;
	formatExamples(formatted, verses);
	size_t maxLength = get_terminal_width();
	addBreaks(formatted, maxLength);
	std::cout << "Please type in your phrase that you want to search for: ";
	std::string choice;
	while(std::getline(std::cin, choice)) {
		std::vector<std::string> results;
		useConcordance(results, formatted, choice);
		std::cout << "\n";
		for(size_t i = 0; i < results.size(); i++) {
			std::cout << "\n" << results[i];
		}
		std::cout << "\n" << "The phrase that you looked for was found " << results.size() << " times!";
		std::cout << "\n" << "Please type in your phrase that you want to search for: ";
	}
	myFile.close();
    return 0;
}
