#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class DocumentSearch {
private:
    struct Entry {
        std::string title;
        std::string content;
        std::vector<Entry> subEntries;
    };

    std::vector<Entry> chapters;

public:
void loadDocument(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file!" << std::endl;
        return;
    }

    std::string line;
    Entry* currentChapter = nullptr;
    Entry* currentSubChapter = nullptr;

    while (getline(file, line)) {
        if (line.empty()) continue;

        if (line.find("章节") == 0) {
            chapters.push_back({line});
            currentChapter = &chapters.back();
            currentSubChapter = nullptr;
        } else if (line.find(".") != std::string::npos && line.rfind(".") == line.find(".")) {
            if (currentChapter) {
                currentChapter->subEntries.push_back({line});
                currentSubChapter = &currentChapter->subEntries.back();
            }
        } else if (line.find(".") != line.rfind(".")) {
            if (currentSubChapter) {
                currentSubChapter->subEntries.push_back({line});
            }
        } else {
            if (currentChapter && !currentSubChapter) {
                currentChapter->content += line + "\n";
            } else if (currentSubChapter) {
                if (currentSubChapter->subEntries.empty()) {
                    currentSubChapter->content += line + "\n";
                } else {
                    currentSubChapter->subEntries.back().content += line + "\n";
                }
            }
        }
    }
}

std::vector<std::string> searchByKeyword(const std::string& keyword) {
    std::vector<std::string> foundSections;

    for (const auto& chapter : chapters) {
        for (const auto& subChapter : chapter.subEntries) {
            for (const auto& subSubChapter : subChapter.subEntries) {
                if (subSubChapter.title.find(keyword) != std::string::npos || subSubChapter.content.find(keyword) != std::string::npos) {
                    std::string result = chapter.title.substr(0, chapter.title.find(" ")) + " > " + 
                                         subChapter.title.substr(0, subChapter.title.find(" ")) + " > " + 
                                         subSubChapter.title + " \"" + subSubChapter.content.substr(0, subSubChapter.content.find("\n")) + "\"";
                    foundSections.push_back(result);
                }
            }
            
            if (subChapter.title.find(keyword) != std::string::npos || subChapter.content.find(keyword) != std::string::npos) {
                std::string result = chapter.title.substr(0, chapter.title.find(" ")) + " > " + 
                                     subChapter.title + " \"" + subChapter.content.substr(0, subChapter.content.find("\n")) + "\"";
                foundSections.push_back(result);
            }
        }

        if (chapter.title.find(keyword) != std::string::npos || chapter.content.find(keyword) != std::string::npos) {
            std::string result = chapter.title + " \"" + chapter.content.substr(0, chapter.content.find("\n")) + "\"";
            foundSections.push_back(result);
        }
    }

    return foundSections;
}
};

int main() {
    DocumentSearch ds;
    ds.loadDocument("sample.txt");

    std::cout << "Enter a keyword to search: ";
    std::string keyword;
    getline(std::cin, keyword);

    auto sections = ds.searchByKeyword(keyword);
    if (sections.empty()) {
        std::cout << "No sections found with the keyword!" << std::endl;
    } else {
        std::cout << "Found keyword in the following sections:" << std::endl;
        for (const auto& section : sections) {
            std::cout << section << std::endl;
        }
    }

    return 0;
}

