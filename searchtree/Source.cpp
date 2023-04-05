#include <iostream>
#include <stack>
#include <vector>

int count_digits(int num) {
    if (num == 0) {
        return 1; // 0 считается одним символом
    }
    int count = num < 0 ? 1 : 0;

    // перебираем цифры числа и увеличиваем счетчик
    while (num != 0) {
        count++;
        num /= 10;
    }
    return count;
}

template <class T>
class Node {
private:
    T m_data;
    Node* m_left;
    Node* m_right;
    int m_height;
public:
    Node() {
        m_data = NULL;
        m_left = nullptr;
        m_right = nullptr;
        m_height = 0;
    }

    Node(T date) {
        m_data = date;
        m_left = nullptr;
        m_right = nullptr;
        m_height = 0;
    }

    ~Node() {}

    // Вставка узла
    void insert(T date) {
        if (this->m_data == NULL) {
            m_data = date;
            m_left = nullptr;
            m_right = nullptr;
        }

        // Проходим в нужное место и вставляет узел
        if (date < this->m_data) {
            if (m_left == nullptr) {
                m_left = new Node(date);
            }
            else {
                m_left->insert(date);
            }
        }
        else {
            if (m_right == nullptr) {
                m_right = new Node(date);
            }
            else {
                m_right->insert(date);
            }
        }
        updateHeight();
        Balance();
    }

    // Обход
    void inorder() {
        if (this != NULL) {
            // Обходим лево
            if (m_left != nullptr) {
                m_left->inorder();
            }

            // Обходим корень
            std::cout << m_data << "->";

            // Обходим право
            if (m_right != nullptr) {
                m_right->inorder();
            }
        }
    }

    void inorder_non_recursive() {
        std::stack<Node<T>*> stack;
        Node<T>* current = this;

        while (current != nullptr || !stack.empty()) {
            // Двигаемся влево до конца текущего поддерева
            while (current != nullptr) {
                stack.push(current);
                current = current->m_left;
            }

            // Берем узел с вершины стека и распечатываем его
            current = stack.top();
            std::cout << current->m_data << "->";
            stack.pop();

            // Переходим к правому поддереву
            current = current->m_right;
        }
    }
    // Метод Morris Traversal
    void inorder_non_stack_non_recursive() {
        Node<T>* current = this;
        while (current != nullptr) {
            if (current->m_left == nullptr) {
                // Распечатываем значение узла и идем к правому поддереву
                std::cout << current->m_data << "->";
                current = current->m_right;
            }
            else {
                // Ищем предшествующий узел
                Node<T>* predecessor = current->m_left;
                while (predecessor->m_right != nullptr && predecessor->m_right != current) {
                    predecessor = predecessor->m_right;
                }

                if (predecessor->m_right == nullptr) {
                    // Устанавливаем ссылку на текущий узел в правое поддерево
                    predecessor->m_right = current;
                    current = current->m_left;
                }
                else {
                    // Распечатываем значение узла и удаляем ссылку из предшествующего узла
                    std::cout << current->m_data << "->";
                    predecessor->m_right = nullptr;
                    current = current->m_right;
                }
            }
        }
    }

    Node* minValueNode() {
        Node* current = this;
        while (current && current->m_left != nullptr) { current = current->m_left; }
        return current;
    }

    // Удаление узла
    Node* deleteNode(T date) {
        // Возвращаем, если дерево пустое
        if (this == NULL) return this;

        // Ищем узел, который хотим удалить
        if (date < this->m_data) {
            this->m_left = this->m_left->deleteNode(date);
        }
        else if (date > this->m_data) {
            this->m_right = this->m_right->deleteNode(date);
        }
        else {
            // Если у узла один дочерний элемент или их нет
            if (this->m_left == NULL) {
                Node* temp = this->m_right;
                free(this);
                return temp;
            }
            else if (this->m_right == NULL) {
                Node* temp = this->m_left;
                free(this);
                return temp;
            }

            // Если у узла два дочерних элемента
            Node* temp = this->m_right->minValueNode();

            // Помещаем temp на место узла, который хотим удалить
            this->m_data = temp->m_data;

            // Удаляем
            this->m_right = deleteNode(temp->m_data);
        }
        if (this != nullptr) {
            updateHeight();
            Balance();
        }
        return this;
    }

    int height(Node<T>* root) {
        if (root == nullptr)
            return 0;
        return std::max(height(root->m_left), height(root->m_right)) + 1;
    }

    void updateHeight() {
        m_height = (m_left ? m_left->m_height : 0) + (m_right ? m_right->m_height : 0) + 1;
    }

    int balanceFactor() {
        if (this == nullptr) { return 0; }
        int l_height = height(this->m_left);
        int r_height = height(this->m_right);
        return r_height - l_height;
    }

    void swapData(Node<T>* node) {
        T temp = node->m_data;
        node->m_data = this->m_data;
        this->m_data = temp;
    }

    void RotateLeft() {
        this->swapData(this->m_right);
        Node<T>* buffer = this->m_left;
        this->m_left = this->m_right;
        if (this->m_left->m_right != nullptr) { this->m_right = this->m_left->m_right; }
        if (this->m_right->m_right != nullptr) { this->m_right->m_left = this->m_right->m_right; }
        this->m_left->m_right = this->m_left->m_left;
        this->m_left->m_left = buffer;
        this->m_left->updateHeight();
        this->updateHeight();
    }

    void RotateRight() {
        this->swapData(this->m_left);
        Node<T>* buffer = this->m_right;
        this->m_right = this->m_left;
        this->m_left = this->m_right->m_left;
        this->m_right->m_left = this->m_right->m_right;
        this->m_right->m_right = buffer;
        this->m_right->updateHeight();
        this->updateHeight();
    }

    void Balance() {
        this->updateHeight();
        if (this->balanceFactor() == 2) {
            if (this->m_right->balanceFactor() == -1) { this->m_right->RotateRight(); }
            this->RotateLeft();
        }
        else if (this->balanceFactor() == -2) {
            if (this->m_left->balanceFactor() == 1) { this->m_left->RotateLeft(); }
            this->RotateRight();
        }
    }

    int getcol(int h) {
        if (h == 1)
            return 1;
        return getcol(h - 1) + getcol(h - 1) + 1;
    }

    void printTree(int** M, Node<T>* root, int col, int row, int height, int** N) {
        if (root == nullptr)
            return;
        M[row][col] = root->m_data;
        N[row][col] = 1;
        printTree(M, root->m_left, col - pow(2, height - 2), row + 1, height - 1, N);
        printTree(M, root->m_right, col + pow(2, height - 2), row + 1, height - 1, N);
    }

    void TreePrinter() {
        int h = height(this);
        int col = getcol(h);
        int** M = new int* [h];
        int** N = new int* [h];
        for (int i = 0; i < h; i++) {
            M[i] = new int[col];
            N[i] = new int[col];
        }
        printTree(M, this, col / 2, 0, h, N);
        for (int i = 0; i < h; i++) {
            std::cout << "  ";
            std::vector<int> vec;
            std::string out = "";
            bool not_first = false;
            bool not_space = false;
            int last = 0;
            for (int j = 0; j < col; j++) {
                if (M[i][j] == 0 && N[i][j] != 1) {
                    std::cout << " " << " ";
                }
                else {
                    if (N[i][j] == 1) {
                        vec.push_back(M[i][j]);
                        std::cout << M[i][j] << " ";
                        if (!not_first) {
                            out += " ";
                        }
                        out += "/";
                        int counts_tens = count_digits(M[i][j]);
                        for (; counts_tens > 0; counts_tens--) {
                            out += " ";
                        }
                        out += "\\";
                        not_first = true;
                        out.resize(out.size() - 1);
                        out += "\\";
                    }
                    else {
                        std::cout << " " << " ";
                        out += "  ";
                        if (last != vec.size()) {
                            out.resize(out.size() - 1);
                            last = vec.size();
                        }
                    }
                }
            }
            std::cout << "\n" << out;
            std::cout << std::endl;
            vec.clear();
        }
    }
};

int main() {
    std::cout << "||||||||||||||||||||||||||||||||||\n|||     New gen (recursive)    |||\n||||||||||||||||||||||||||||||||||\n";

    Node<int> rt(8);
    rt.insert(3);
    rt.insert(1);
    rt.insert(6);
    rt.insert(7);
    rt.insert(10);
    rt.insert(14);
    rt.insert(4);

    std::cout << "Tree: ";
    rt.inorder();

    std::cout << "\nDel 10\n";
    rt.deleteNode(10);

    std::cout << "Tree: ";
    rt.inorder();

    std::cout << "\n||||||||||||||||||||||||||||||||||\n|||   New gen (non recursive)  |||\n||||||||||||||||||||||||||||||||||\n";

    Node<int> rt2(8);
    rt2.insert(3);
    rt2.insert(1);
    rt2.insert(6);
    rt2.insert(7);
    rt2.insert(10);
    rt2.insert(14);
    rt2.insert(4);

    std::cout << "Tree: ";
    rt2.inorder_non_recursive();

    std::cout << "\nDel 10\n";
    rt2.deleteNode(10);

    std::cout << "Tree: ";
    rt2.inorder_non_recursive();

    std::cout << "\n||||||||||||||||||||||||||||||||||\n|||  New gen (Morris Traversal)  |||\n||||||||||||||||||||||||||||||||||\n";

    Node<int> rt3(8);
    rt3.insert(3);
    rt3.insert(1);
    rt3.insert(6);
    rt3.insert(7);
    rt3.insert(10);
    rt3.insert(14);
    rt3.insert(4);

    std::cout << "Tree: ";
    rt3.inorder_non_recursive();

    std::cout << "\nDel 10\n";
    rt3.deleteNode(10);

    std::cout << "Tree: ";
    rt3.inorder_non_stack_non_recursive();

    std::cout << "\n";
    system("pause");
    system("cls");

    std::cout << "\n||||||||||||||||||||||||||||||||||\n|||  New gen (Vertical print)  |||\n||||||||||||||||||||||||||||||||||\n";
     
    Node<int> rt4(8);
    /*rt4.insert(3);
    rt4.insert(1);
    rt4.insert(6);
    rt4.insert(7);
    rt4.insert(10);
    rt4.insert(14);
    rt4.insert(4);
    rt4.insert(13);
    rt4.insert(16);
    rt4.insert(18);
    rt4.insert(15);
    rt4.insert(-1);
    rt4.insert(2);
    rt4.insert(5);*/
    rt4.insert(9);
    rt4.insert(10);
    rt4.insert(11);
    rt4.insert(12);
    rt4.insert(13);
    rt4.insert(14);
    /*rt4.insert(5);
    rt4.insert(4);
    rt4.insert(3);
    rt4.insert(2);
    rt4.insert(1);*/

    /*std::cout << "Tree: ";
    rt4.inorder_non_stack_non_recursive();

    std::cout << "\nDel 10\n";
    rt4.deleteNode(10);

    std::cout << "Tree: ";
    rt4.inorder_non_stack_non_recursive();*/

    std::cout << "\n";
    system("pause");
    system("cls");
    rt4.TreePrinter();
    std::cout << "\n|||||||||||||||||||||||||||||||||||\n";
    rt4.deleteNode(10);
    //rt4.deleteNode(13);

    rt4.TreePrinter();
}