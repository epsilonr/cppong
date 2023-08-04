#include <iostream>
#include <ctime>

int getRandomDirection()
{
    return (std::rand() % 2) ? -1 : 1;
}

int main()
{
    std::srand(std::time(0));

    // Example usage:
    for (int i = 0; i < 10; ++i)
    {
        int result = getRandomDirection();
        std::cout << result << " ";
    }

    return 0;
}
