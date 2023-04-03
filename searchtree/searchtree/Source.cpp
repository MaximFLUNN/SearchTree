#include <iostream>

template <class T>
class Node {
private:
    T m_data;
    Node* m_left;
    Node* m_right;
public:
    Node(T date) {
        m_data = date;
        m_left = nullptr;
        m_right = nullptr;
    }

    ~Node() {}

    // ������� ����
    void insert(T date) {
        if (this->m_data == NULL) {
            m_data = date;
            m_left = nullptr;
            m_right = nullptr;
        }

        // �������� � ������ ����� � ��������� ����
        if (date < this->m_data)
            if (m_left == nullptr) {
                m_left = new Node(date);
            }
            else {
                m_left->insert(date);
            }
        else
            if (m_right == nullptr) {
                m_right = new Node(date);
            }
            else {
                m_right->insert(date);
            }
    }

    // �����
    void inorder() {
        if (this != NULL) {
            // ������� ����
            this->m_left->inorder();

            // ������� ������
            std::cout << this->m_data << " -> ";

            // ������� �����
            this->m_right->inorder();
        }
    }

    Node* minValueNode() {
        Node* current = this;
        while (current && current->m_left != NULL) { current = current->m_left; }
        return current;
    }

    // �������� ����
    Node* deleteNode(T date) {
        // ����������, ���� ������ ������
        if (this == NULL) return this;

        // ���� ����, ������� ����� �������
        if (date < this->m_data) {
            this->m_left = this->m_left->deleteNode(date);
        }
        else if (date > this->m_data) {
            this->m_right = this->m_right->deleteNode(date);
        }
        else {
            // ���� � ���� ���� �������� ������� ��� �� ���
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

            // ���� � ���� ��� �������� ��������
            Node* temp = this->m_right->minValueNode();

            // �������� temp �� ����� ����, ������� ����� �������
            this->m_data = temp->m_data;

            // �������
            this->m_right = deleteNode(temp->m_data);
        }
        return this;
    }
};

int main() {
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
}