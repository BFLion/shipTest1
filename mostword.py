import jieba
from collections import Counter

def get_high_freq_words(text, top_n=10):
    words = [word for word in jieba.cut(text) if len(word) > 1]  # 过滤掉长度1
    word_freq = Counter(words)
    return word_freq.most_common(top_n)

if __name__ == "__main__":
    with open("handbook.txt", "r", encoding="utf-8") as file:#修改对应文档名
        content = file.read()
    high_freq_words = get_high_freq_words(content, 20)#展示前20
    for word, freq in high_freq_words:
        print(f"{word}: {freq}")
