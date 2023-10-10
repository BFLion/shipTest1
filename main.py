class DocumentSearch:
    class Entry:
        def __init__(self):
            self.title = ""
            self.content = ""
            self.subEntries = []

    def __init__(self):
        self.chapters = []

    def loadDocument(self, filename):
        with open(filename, 'r', encoding='utf-8') as file:
            lines = file.readlines()

        currentChapter = None
        currentSubChapter = None
        for line in lines:
            line = line.strip()
            if not line:
                continue

            if "章节" in line:
                self.chapters.append(self.Entry())
                currentChapter = self.chapters[-1]
                currentSubChapter = None
                currentChapter.title = line
            elif "." in line and line.count(".") == 1:
                if currentChapter:
                    currentChapter.subEntries.append(self.Entry())
                    currentSubChapter = currentChapter.subEntries[-1]
                    currentSubChapter.title = line
            elif line.count(".") > 1:
                if currentSubChapter:
                    currentSubChapter.subEntries.append(self.Entry())
                    currentSubChapter.subEntries[-1].title = line
            else:
                if currentChapter and not currentSubChapter:
                    currentChapter.content += line + "\n"
                elif currentSubChapter:
                    if not currentSubChapter.subEntries:
                        currentSubChapter.content += line + "\n"
                    else:
                        currentSubChapter.subEntries[-1].content += line + "\n"

    def searchByKeyword(self, keyword):
        foundSections = []

        for chapter in self.chapters:
            for subChapter in chapter.subEntries:
                for subSubChapter in subChapter.subEntries:
                    if keyword in subSubChapter.title or keyword in subSubChapter.content:
                        result = f"{chapter.title.split(' ')[0]} > {subChapter.title.split(' ')[0]} > {subSubChapter.title} \"{subSubChapter.content.split('n')[0]}\""
                        foundSections.append(result)

                if keyword in subChapter.title or keyword in subChapter.content:
                    result = f"{chapter.title.split(' ')[0]} > {subChapter.title} \"{subChapter.content.split('n')[0]}\""
                    foundSections.append(result)

            if keyword in chapter.title or keyword in chapter.content:
                result = f"{chapter.title} \"{chapter.content.split('n')[0]}\""
                foundSections.append(result)

        return foundSections


if __name__ == "__main__":
    ds = DocumentSearch()
    #ds.loadDocument("sample.txt")
    ds.loadDocument("handbook.txt")
    keyword = input("Enter a keyword to search: ")

    sections = ds.searchByKeyword(keyword)
    if not sections:
        print("No sections found with the keyword!")
    else:
        print("Found keyword in the following sections:")
        for section in sections:
            print(section)