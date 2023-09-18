#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Стуктура элемента списка 
struct Node
{
    string str = "";
    Node* next = nullptr;
    Node(string data)
    {
        str = data;
    }
};

// Класс списка 
class list
{
public:
    list() : head(nullptr), tail(nullptr), _size(0)
    {

    }
    ~list()
    {
        if (head == nullptr)
        {
            return;
        }
        
        Node* current = head->next;
        while (current->next)
        {
            Node* deletingElement = current;
            current = current->next;
            delete deletingElement;
        }

        delete head;
    }

    // Добавление элемента в конец списка
    void push_back(const string& data)
    {
        if (head == nullptr)
        {
            head = new Node(data);
            tail = head;
        }
        else
        {
            tail->next = new Node(data);
            tail = tail->next;
        }
        ++_size;
    }

    // Обращение к первому элементу списка
    Node* get_head() { return head; }
    // Возвращает текущий размер списка
    int size() { return _size; }

private:
    Node* head;
    Node* tail;
    int _size;
};



int main() {
    setlocale(LC_ALL, "RU");
    string sourceFileName = "input.txt";
    string commentFileName = "output.txt";
    string line;

    list codeLines;
    list commentLines;

    // Чтение исходного файла построчно
    ifstream sourceFile(sourceFileName);
    // Если файл удачно открылся
    if (sourceFile.is_open()) 
    {
        // Построчно заполняем список
        while (getline(sourceFile, line)) 
        {
            codeLines.push_back(line);
        }
        sourceFile.close();
    }
    else 
    {
        cout << "Не удалось открыть исходный файл." << endl;
        return 0;
    }

    // Поиск и удаление комментариев и запись их в список комментариев
    bool blockComment = false;

    Node* currentItem = codeLines.get_head();
    for (auto it = 0; it < codeLines.size() - 1; ++it)
    {
        string currentLine = currentItem->str;

        size_t singleCommentPos = currentLine.find("//");
        size_t blockCommentStartPos = currentLine.find("/*");
        size_t blockCommentEndPos = currentLine.find("*/");

        // Если найден символ однострочного комментария
        if (singleCommentPos != string::npos) 
        {
            // Добавляем комментарий в отдельный список и удаляем из исходного списка
            commentLines.push_back(currentLine.substr(singleCommentPos));
            currentLine = currentLine.substr(0, singleCommentPos);
        }
        // Если найден открывающий символ многострочного комментария
        else if (blockCommentStartPos != string::npos) 
        {
            // Если также найден закрывающий символ многострочного комментария
            if (blockCommentEndPos != string::npos) 
            {
                // Полностью добавляем комментарий в отдельный список и удаляем из исходного списка
                commentLines.push_back(currentLine.substr(blockCommentStartPos, blockCommentEndPos - blockCommentStartPos + 2));
                currentLine = currentLine.substr(0, blockCommentStartPos) + currentLine.substr(blockCommentEndPos + 2);
            }
            else 
            {
                // Частично добавляем комментарий в отдельный список и удаляем из исходного списка
                commentLines.push_back(currentLine.substr(blockCommentStartPos));
                currentLine = currentLine.substr(0, blockCommentStartPos);
                blockComment = true;
            }
        }
        // Если найден закрывающий символ многострочного комментария
        else if (blockCommentEndPos != string::npos) 
        {
            // Частично добавляем комментарий в отдельный список и удаляем из исходного списка
            commentLines.push_back(currentLine.substr(0, blockCommentEndPos + 2));
            currentLine = currentLine.substr(blockCommentEndPos + 2);
            blockComment = false;
        }
        // Если в предыдущих строках не был закрыт многострочный комментарий
        else if (blockComment) 
        {
            // Добавляем всю строку в отдельный список и удаляем из исходного
            commentLines.push_back(currentLine);
            currentLine = "";
        }
        currentItem = currentItem->next;
    }

    // Запись кода без комментариев в исходный файл
    ofstream updatedSourceFile(sourceFileName);
    if (updatedSourceFile.is_open()) 
    {
        Node* currentItem = codeLines.get_head();
        for (size_t i = 0; i < codeLines.size() - 1; i++)
        {
            string currentLine = currentItem->str;
            updatedSourceFile << currentLine << endl;

            currentItem = currentItem->next;
        }
        updatedSourceFile.close();
    }
    else 
    {
        cout << "Не удалось открыть исходный файл для записи." << endl;
        return 0;
    }

    // Запись комментариев в отдельный файл
    ofstream commentFile(commentFileName);
    if (commentFile.is_open()) 
    {
        Node* currentItem = commentLines.get_head();
        for (size_t i = 0; i < commentLines.size() - 1; i++)
        {
            string currentLine = currentItem->str;

            commentFile << currentLine << endl;

            currentItem = currentItem->next;
        }
        commentFile.close();
        cout << "Комментарии успешно записаны в файл: " << commentFileName << endl;
    }
    else 
    {
        cout << "Не удалось открыть файл для записи комментариев." << endl;
        return 0;
    }

    return 0;
}