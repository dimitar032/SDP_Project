#include <utility> // For std::move()
#include "../Header/BinaryTree.h"

BinaryTree::BinaryTree()
    : top(nullptr)
{ }

BinaryTree::~BinaryTree()
{
    clear(top);
}

BinaryTree::BinaryTree(const BinaryTree& source)
    : top(nullptr)
{
    copy(source.top, top);
}

BinaryTree::BinaryTree(BinaryTree&& source)
    : top(source.top)
{
    source.top = nullptr;
}

BinaryTree& BinaryTree::operator=(const BinaryTree& rhs)
{
    if(this != &rhs)
    {
        clear(top);
        copy(rhs.top, top);
    }

    return *this;
}

BinaryTree& BinaryTree::operator=(BinaryTree&& rhs)
{
    if(this != &rhs)
    {
        clear(top);
        top = rhs.top;
        rhs.top = nullptr;
    }
    return *this;
}

BinaryTree::BinaryTree(const Tree_Node& src)
    : top(nullptr)
{
    copy(&src, top);
}

BinaryTree::BinaryTree(Tree_Node&& src)
    : top(nullptr)
{
    top = new Tree_Node(src.weight, src.letter, src.left, src.right);

    src.left = nullptr;
    src.right = nullptr;
}

BinaryTree::BinaryTree(size_t weight, char letter)
    : top(nullptr)
{
    top = new Tree_Node(weight, letter);
}

bool BinaryTree::writeBits(BitVector& vect, size_t& pos, char what) const
{
    return writeBits(vect, pos, what, top);
}

char BinaryTree::extractBits(const BitVector& vect, size_t& pos) const
{
    return extractBits(vect, pos, top);
}

int BinaryTree::height(char ofWhat) const
{
    if(!top)
        return 0;

    return height(ofWhat, top);
}

int BinaryTree::height() const
{
    if(!top)
        return 0;

    return height(top);
}

size_t BinaryTree::weight() const
{
    return top ? top->weight : 0;
}

BinaryTree& BinaryTree::operator+=(const Tree_Node& rhs)
{
    Tree_Node* tmp = new Tree_Node(rhs);
    Tree_Node* new_top = new Tree_Node;

    new_top->left = top;
    new_top->right = tmp;

    if(top)
        new_top->weight += top->weight;

    new_top->weight += tmp->weight;

    top = new_top;

    return *this;
}

BinaryTree BinaryTree::operator+(const Tree_Node& rhs) const
{
    return BinaryTree(*this) += rhs;
}

BinaryTree& BinaryTree::operator+=(const BinaryTree& rhs)
{
    BinaryTree* right_node = new BinaryTree(rhs);
    Tree_Node* new_top = new Tree_Node;

    new_top->left = top;
    new_top->right = right_node->top;

    if(top)
        new_top->weight += top->weight;
    if(right_node->top)
        new_top->weight += top->weight;

    // Leaks a pointer (aka 4 bytes) into the memory if not done
    right_node->top = nullptr;
    delete right_node;
    //

    top = new_top;

    return *this;
}

BinaryTree BinaryTree::operator+(const BinaryTree& rhs) const
{
    return BinaryTree(*this) += rhs;
}

BinaryTree& BinaryTree::operator+=(BinaryTree&& rhs)
{
    Tree_Node* new_top = new Tree_Node;

    new_top->left = top;
    new_top->right = rhs.top;

    if(top)
        new_top->weight += top->weight;
    if(rhs.top)
        new_top->weight += rhs.top->weight;

    rhs.top = nullptr;
    top = new_top;

    return *this;
}

BinaryTree BinaryTree::operator+(BinaryTree&& rhs) const
{
    return BinaryTree(*this) += std::move(rhs);
}

bool BinaryTree::operator<(const BinaryTree& rhs) const
{
    if(!top && !rhs.top)
        return false;
    if(!top && rhs.top)
        return true;
    if(top && !rhs.top)
        return false;
    if(top->weight < rhs.top->weight)
        return true;

    return false;
}

bool BinaryTree::operator>(const BinaryTree& rhs) const
{
    if(!top && !rhs.top)
        return false;
    if(!top && rhs.top)
        return false;
    if(top && !rhs.top)
        return true;
    if(top->weight > rhs.top->weight)
        return true;

    return false;
}

bool BinaryTree::operator<=(const BinaryTree& rhs) const
{
    return !(*this > rhs);
}

bool BinaryTree::operator>=(const BinaryTree& rhs) const
{
    return !(*this < rhs);
}

void BinaryTree::BinaryExport(std::fstream& file) const
{
    bool topExists = top ? true : false;

    file.write((char*) &topExists, sizeof(bool));

    if(!topExists)
        return;

    bool leftExists = top->left ? true : false;
    bool rightExists = top->right ? true : false;

    // Write down the letter of the current node
    // Only write down the weight of the top
    // sizeof(char)*4 because of the padding
    file.write((char*)top, sizeof(char)*4 + sizeof(size_t));

    file.write((char*)&leftExists, sizeof(bool));
    BinaryExport(file, top->left);

    file.write((char*)&rightExists, sizeof(bool));
    BinaryExport(file, top->right);
}

void BinaryTree::BinaryImport(std::fstream& file)
{
    bool topExists;

    file.read((char*) &topExists, sizeof(bool));

    Tree_Node* newTop = nullptr;

    if(!topExists)
    {
        clear(top);
        top = newTop;
        return;
    }

    newTop = new Tree_Node;

    // sizeof(char)*4 because of the padding
    file.read((char*)newTop, sizeof(char)*4 + sizeof(size_t));

    bool leftExists;
    bool rightExists;

    file.read((char*)&leftExists, sizeof(bool));
    if(leftExists)
        BinaryImport(file, newTop->left);

    file.read((char*)&rightExists, sizeof(bool));
    if(rightExists)
        BinaryImport(file, newTop->right);

    clear(top);
    top = newTop;
}

bool BinaryTree::CreatePattern(char*& holder, char target) const
{
    int holderLen = height(target);
    
    if(holderLen == -1)
        return false;

    holder = new char[holderLen + 1];
    holder[holderLen] = '\0';

    if(!CreatePattern(holder, target, top, holderLen, 0))
    {
        delete[] holder;
        holder = nullptr;
        return false;
    }

    return true;
}

int BinaryTree::height(char ofWhat, const Tree_Node* cur) const
{
    if(!cur)
        return -1;

    if(leaf(cur) && cur->letter == ofWhat)
        return 0;

    if(leaf(cur) && cur->letter != ofWhat)
        return -1;

    int leftHeight = height(ofWhat, cur->left);

    if(leftHeight != -1)
        return 1 + leftHeight;

    int rightHeight = height(ofWhat, cur->right);

    if(rightHeight != -1)
        return 1 + rightHeight;

    return -1;
}

int BinaryTree::height(const Tree_Node* cur) const
{
    if(!cur)
        return -1;

    int leftHeight = height(cur->left);
    int rightHeight = height(cur->right);

    if(leftHeight > rightHeight)
        return leftHeight + 1;
    else
        return rightHeight + 1;
}

bool BinaryTree::CreatePattern(char* holder, char target, const Tree_Node* cur, size_t len, size_t counter) const
{
    if(!cur)
        return false;

    if(leaf(cur) && cur->letter == target)
        return true;

    if(leaf(cur) && cur->letter != target)
        return false;

    if(counter >= len)
        return false;

    *holder = '0';
    if(!CreatePattern(holder+1, target, cur->left, len, counter+1))
    {
        *holder = '1';
        if(!CreatePattern(holder+1, target, cur->right, len, counter + 1))
            return false;
        else
            return true;
    }
    else 
        return true;
}

char BinaryTree::extractBits(const BitVector& vect, size_t& pos, const Tree_Node* cur) const
{
    if(leaf(cur))
        return cur->letter;

    if(cur == nullptr)
        return -1;

    try
    {
        if(vect.GetBit(pos))
        {
            pos++;
            return extractBits(vect, pos, cur->right);
        }
        else
        {
            pos++;
            return extractBits(vect, pos, cur->left);
        }
    }
    catch(BIT_VECTOR_ERRORS& err)
    {
        if(err == INVALID_BIT_INDEX)
            return -1;
        else
            throw err;
    }
}

void BinaryTree::BinaryExport(std::fstream& file, const Tree_Node* cur) const
{
    if(cur == nullptr)
        return;

    bool leftExists = cur->left ? true : false;
    bool rightExists = cur->right ? true : false;

    // Write down the letter of the current node
    // We skip the weight because we do not need it in the archive
    file.write((char*) cur, sizeof(char)); 

    file.write((char*) &leftExists, sizeof(bool));
    BinaryExport(file, cur->left);

    file.write((char*) &rightExists, sizeof(bool));
    BinaryExport(file, cur->right);
}

void BinaryTree::BinaryImport(std::fstream& file, Tree_Node*& cur)
{
    cur = new Tree_Node;

    file.read((char*) cur, sizeof(char));

    bool leftExists;
    bool rightExists;

    file.read((char*) &leftExists, sizeof(bool));
    if(leftExists)
        BinaryImport(file, cur->left);

    file.read((char*) &rightExists, sizeof(bool));
    if(rightExists)
        BinaryImport(file, cur->right);
}

bool BinaryTree::writeBits(BitVector& vect, size_t& pos, char what, const Tree_Node* cur) const
{
    if(!cur)
        return false;

    if(leaf(cur))
        if(cur->letter == what)
            return true;
        else
            return false;

    size_t oldPos = pos;
    ++pos;

    if(writeBits(vect, pos, what, cur->left))
    {
        vect.change(oldPos, 0);
        return true;
    }
    else if(writeBits(vect, pos, what, cur->right))
    {
        vect.change(oldPos, 1);
        return true;
    }
    else
    {
        --pos;
        return false;
    }
}

void BinaryTree::copy(const Tree_Node* what, Tree_Node*& into)
{
    if(!what)
        return;

    into = new Tree_Node(what->weight, what->letter);
    copy(what->left, into->left);
    copy(what->right, into->right);
}

void BinaryTree::clear(Tree_Node* cur)
{
    if(!cur)
        return;

    clear(cur->left);
    clear(cur->right);
    delete cur;
}

bool BinaryTree::leaf(const Tree_Node * node) const
{
    return !node->left && !node->right;
}







