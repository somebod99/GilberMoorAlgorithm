#ifndef GILBERMOORALGORITHM_H
#define GILBERMOORALGORITHM_H

#include <string>
#include <map>
#include <vector>

class GilberMoorEncoder
{
public:
    GilberMoorEncoder(const std::string&);
    ~GilberMoorEncoder();

    std::vector<bool> EncodeFromTxtFile(const std::string&) const;
    std::vector<bool> EncodeString(const std::string&) const;

    const std::map<std::string, std::vector<bool>>& getCodes() const;

    double getL() const;
    double getR() const;
    double getKraftInequality() const;

private:
    std::vector<bool> DoubleToBinary(double, const int&) const;

    std::map<std::string, std::vector<bool>> codes;
    double l;
    double r;
    double kraftInequality;
    int max_l_alphabet;
};

class Node
{
public:
    Node* r;
    Node* l;
    std::string symbol;

    Node() : r(nullptr), l(nullptr) {}
    ~Node()
    {
        delete r;
        delete l;
    }
};

class GilberMoorDecoder
{
public:
    GilberMoorDecoder(const GilberMoorEncoder&);
    ~GilberMoorDecoder();

    std::string DecodeFromBinaryFile(const std::string&) const;
    std::string DecodeBinarySequence(const std::vector<bool>&) const;

private:
    Node* HaffmanTree;
};

std::vector<bool> ReadFromBinaryFile(const std::string&);
void WriteToBinaryFile(const std::string&, const std::vector<bool>&);

#endif // GILBERMOORALGORITHM_H
