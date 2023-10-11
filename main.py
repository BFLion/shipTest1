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
            
            if line.count(".") == 1:
                currentChapter = self.Entry()
                self.chapters.append(currentChapter)
                currentSubChapter = None
                currentChapter.title = line
            elif line.count(".") == 2:
                currentSubChapter = self.Entry()
                currentChapter.subEntries.append(currentSubChapter)
                currentSubChapter.title = line
            elif "、" in line and "？" in line:  
                if currentSubChapter:
                    currentSubChapter.subEntries.append(self.Entry())
                    currentSubChapter.subEntries[-1].title = line
            else:
                if currentSubChapter and currentSubChapter.subEntries:
                    currentSubChapter.subEntries[-1].content += line + "\n"
                elif currentSubChapter:
                    currentSubChapter.content += line + "\n"
                elif currentChapter:
                    currentChapter.content += line + "\n"

    def searchByKeyword(self, keyword):
        foundSections = []

        for chapter in self.chapters:
            for subChapter in chapter.subEntries:
                for subSubChapter in subChapter.subEntries:
                    if keyword in subSubChapter.title or keyword in subSubChapter.content:
                        result = f"{chapter.title.split(' ')[0]} > {subChapter.title.split(' ')[0]} > {subSubChapter.title}{subSubChapter.content}"
                        foundSections.append(result)

                if keyword in subChapter.title or keyword in subChapter.content:
                
                    if not subChapter.subEntries:
                        result = f"{chapter.title.split(' ')[0]} > {subChapter.title.split(' ')[0]}、{subChapter.title}{subChapter.content}"
                    else:
                        result = f"{chapter.title.split(' ')[0]} > {subChapter.title}{subChapter.content}"
                    foundSections.append(result)

            if keyword in chapter.title or keyword in chapter.content:
                result = f"{chapter.title}{chapter.content}"
                foundSections.append(result)

        return foundSections

if __name__ == "__main__":
    ds = DocumentSearch()
    ds.loadDocument("handbook.txt")
    #ds.loadDocument("sample.txt")
    keyword = input("您想查询的关键词: ")

    sections = ds.searchByKeyword(keyword)
    if not sections:
        print("没有结果!") 
    else:
        print("关键词出现在以下章节：")
        for section in sections:
            print(section)
