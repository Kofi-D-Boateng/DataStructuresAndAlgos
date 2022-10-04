#include <iostream>
#include <vector>

void StaticArray()
{
    int array[10];
    for (int i = 0; i < 10; i++)
    {
        array[i] = i * 2;
    }

    std::cout << "array[4] should equal 8" << std::endl;
    std::cout << array[4] << std::endl;
}

void VectorArray(std::string &word)
{
    std::vector<char> charVector;
    for (auto &c : word)
    {
        charVector.push_back(c);
    }

    for (auto &c : charVector)
        std::cout << c << " ";
}

int main(int argc, char const *argv[])
{
    std::string name = "Kofi";
    StaticArray();
    VectorArray(name);
    return 0;
}
