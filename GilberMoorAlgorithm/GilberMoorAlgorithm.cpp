#include <fstream>
#include <iostream>

#include "GilberMoorAlgorithm.h"

#include <QtMath>
#include <QString>

std::vector<bool> ReadFromBinaryFile(const std::string& file_path)
{
    std::ifstream file(QString::fromStdString(file_path).toLocal8Bit().toStdString(), std::ios::binary);

    int bits_count;

    file.read(reinterpret_cast<char*>(&bits_count), sizeof(bits_count));

    int bytes_count = bits_count / 8;

    if (bits_count % 8 != 0)
        ++bytes_count;

    std::vector<char> bytes(bytes_count, '\0');

    file.read(&bytes[0], bytes_count);

    std::vector<bool> res;
    bool bit;
    int bit_index = 7;
    int byte_index = 0;

    for (int i = 0; i < bits_count; ++i)
    {
        bit = bytes[byte_index] & (1 << bit_index--);
        res.push_back(bit);

        if (bit_index == -1)
        {
            bit_index = 7;
            ++byte_index;
        }
    }

    return res;
}

void WriteToBinaryFile(const std::string& file_path, const std::vector<bool>& encode)
{
    int bytes_count = encode.size() / 8;

    if (encode.size() % 8 != 0)
        ++bytes_count;

    std::vector<char> bytes(bytes_count, '\0');

    int byte_index = 0;
    int bit_index = 0;

    for (const auto& bit : encode)
    {
        if (bit)
            bytes[byte_index] |= 1 << (7 - bit_index);

        ++bit_index;

        if (bit_index == 8)
        {
            ++byte_index;
            bit_index = 0;
        }
    }

    int size = encode.size();

    std::ofstream file(file_path, std::ios::binary);

    file.write(reinterpret_cast<char*>(&size), sizeof(size));
    file.write(&bytes[0], bytes_count);

    file.close();

    return;
}

GilberMoorEncoder::GilberMoorEncoder(const std::string& file_path) : l(0.0), r(0.0), kraftInequality(0.0), max_l_alphabet(0)
{
    std::ifstream file(QString::fromStdString(file_path).toLocal8Bit().toStdString());

    if (!file.is_open())
    {
        std::string err = "file " + file_path + " cannot be opened";
        throw err.c_str();
    }

    std::string symbol;
    double p_k = 0.0;
    int l_k = 0;
    double q_k = 0.0;
    double sigma_k = 0.0;
    double entropy = 0.0;

    while (!file.eof())
    {
        file >> symbol >> p_k;

        if (p_k == 0.0)
            throw "Creating codes for symbols error: incorrect alphabet or file";

        if (symbol.size() > this->max_l_alphabet)
            this->max_l_alphabet = symbol.size();

        sigma_k = q_k + p_k / 2;

        // the sum of the probabilities of occurrence of the symbols of the alphabet
        q_k += p_k;

        // k-th symbol length
        l_k = std::ceil(-std::log2(p_k / 2));

        this->codes[symbol] = DoubleToBinary(sigma_k, l_k);

        entropy += p_k * std::log2(p_k);

        // codes average length
        this->l += l_k * p_k;

        this->kraftInequality += std::pow(2, -l_k);
    }

    this->r = this->l + entropy;

    file.close();
}

GilberMoorEncoder::~GilberMoorEncoder()
{
}

std::vector<bool> GilberMoorEncoder::EncodeFromTxtFile(const std::string& file_path) const
{
    std::ifstream file(QString::fromStdString(file_path).toLocal8Bit().toStdString(), std::ios::ate);

    if (!file.is_open())
    {
        std::string err = "file " + file_path + " cannot be opened";
        throw err.c_str();
    }

    auto size = file.tellg();

    std::string encoding_str(size, '\0');

    file.seekg(0);

    file.read(&encoding_str[0], size);

    return EncodeString(encoding_str);
}

std::vector<bool> GilberMoorEncoder::EncodeString(const std::string& encoding_str) const
{
    std::vector<bool> res;

    std::string cur;
    int j = this->max_l_alphabet;

    for (int i = 0; i < encoding_str.size() && j >= 0; ++i, --j)
    {
        cur += encoding_str[i];

        if (this->codes.count(cur))
        {
            for (const auto& bit_in_code : this->codes.at(cur))
                res.push_back(bit_in_code);

            cur = "";
            j = this->max_l_alphabet;
        }
    }

    if (j == 0 || cur != "")
            throw "Encoding error: incorrect symbol in sequence or file";

    return res;
}

const std::map<std::string, std::vector<bool>>& GilberMoorEncoder::getCodes() const
{
    return this->codes;
}

std::vector<bool> GilberMoorEncoder::DoubleToBinary(double p, const int& l) const
{
    std::vector<bool> res;

    for (int i = 0; i < l; ++i)
    {
        p *= 2;
        res.push_back(int(p));
        p -= int(p);
    }

    return res;
}

double GilberMoorEncoder::getL() const
{
    return this->l;
}

double GilberMoorEncoder::getR() const
{
    return this->r;
}

double GilberMoorEncoder::getKraftInequality() const
{
    return this->kraftInequality;
}

GilberMoorDecoder::GilberMoorDecoder(const GilberMoorEncoder& encoder) : HaffmanTree(new Node())
{
    auto codes = encoder.getCodes();

    for (const auto& key_value : codes)
    {
        Node* cur = HaffmanTree;

        for (const auto& bit : key_value.second)
        {
            if (bit)
            {
                if(cur->r == nullptr)
                    cur->r = new Node();

                cur = cur->r;
            }
            else
            {
                if (cur->l == nullptr)
                    cur->l = new Node();

                cur = cur->l;
            }
        }

        cur->symbol = key_value.first;
    }
}

GilberMoorDecoder::~GilberMoorDecoder()
{
    delete this->HaffmanTree;
}

std::string GilberMoorDecoder::DecodeFromBinaryFile(const std::string& file_path) const
{
    auto decoded_sequence = ReadFromBinaryFile(file_path);

    return DecodeBinarySequence(decoded_sequence);
}

std::string GilberMoorDecoder::DecodeBinarySequence(const std::vector<bool>& bits) const
{
    Node* cur = this->HaffmanTree;
    std::string res;

    for (const auto& bit : bits)
    {
        if (bit)
            cur = cur->r;
        else
            cur = cur->l;

        if (cur == nullptr)
            throw "Decoding error: incorrect sequence";

        if (cur->symbol != "")
        {
            res += cur->symbol;
            cur = this->HaffmanTree;
        }
    }

    return res;
}
