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

            if (line.find("Chapter:") == 0) {
                chapters.push_back({line});
                currentChapter = &chapters.back();
                currentSubChapter = nullptr;
            } else if (line.find(".") == line.rfind(".")) {
                if (currentChapter) {
                    currentChapter->subEntries.push_back({line});
                    currentSubChapter = &currentChapter->subEntries.back();
                }
            } else if (line.find(".") != line.rfind(".")) {
                if (currentSubChapter) {
                    currentSubChapter->subEntries.push_back({line});
                }
            } else {
                if (currentSubChapter && !currentSubChapter->subEntries.empty()) {
                    currentSubChapter->subEntries.back().content += line + "\n";
                } else if (currentChapter) {
                    currentChapter->content += line + "\n";
                }
            }
        }
    }

    std::vector<std::string> searchByKeyword(const std::string& keyword) {
        std::vector<std::string> foundSections;

        for (const auto& chapter : chapters) {
            bool chapterAdded = false;
            if (chapter.title.find(keyword) != std::string::npos) {
                foundSections.push_back(chapter.title);
                chapterAdded = true;
            }

            for (const auto& subChapter : chapter.subEntries) {
                bool subChapterAdded = false;
                if (subChapter.title.find(keyword) != std::string::npos || subChapter.content.find(keyword) != std::string::npos) {
                    foundSections.push_back(chapter.title + " > " + subChapter.title);
                    subChapterAdded = true;
                }

                for (const auto& subSubChapter : subChapter.subEntries) {
                    if (subSubChapter.title.find(keyword) != std::string::npos || subSubChapter.content.find(keyword) != std::string::npos) {
                        foundSections.push_back(chapter.title + " > " + subChapter.title + " > " + subSubChapter.title);
                    } else if (!subChapterAdded && subSubChapter.content.find(keyword) != std::string::npos) {
                        foundSections.push_back(chapter.title + " > " + subChapter.title);
                    } else if (!chapterAdded && subSubChapter.content.find(keyword) != std::string::npos) {
                        foundSections.push_back(chapter.title);
                    }
                }
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
