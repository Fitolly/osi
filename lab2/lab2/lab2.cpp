#include <iostream>
#include <vector>
#include <windows.h>
#include <iomanip>

std::vector<int> arr;
int min_val, max_val;
double avg_val;

DWORD WINAPI MinMaxThread(LPVOID lpParam)
{
    std::cout << "\n[min_max Thread] Started searching for Min and Max...\n";

    min_val = arr[0];
    max_val = arr[0];

    for (size_t i = 1; i < arr.size(); ++i)
    {
        if (arr[i] < min_val)
            min_val = arr[i];

        Sleep(7);

        if (arr[i] > max_val)
            max_val = arr[i];

        Sleep(7);
    }

    std::cout << "[min_max Thread] Min element: " << min_val << "\n";
    std::cout << "[min_max Thread] Max element: " << max_val << "\n";
    std::cout << "[min_max Thread] Finished.\n";

    return 0;
}

DWORD WINAPI AverageThread(LPVOID lpParam)
{
    std::cout << "\n[average Thread] Started calculating Average...\n";

    double sum = 0;

    for (size_t i = 0; i < arr.size(); ++i)
    {
        sum += arr[i];
        Sleep(12);
    }

    avg_val = sum / arr.size();

    std::cout << "[average Thread] Average value: " << std::fixed << std::setprecision(2) << avg_val << "\n";
    std::cout << "[average Thread] Finished.\n";

    return 0;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int size;
    std::cout << "Enter array size: ";
    std::cin >> size;

    arr.resize(size);
    std::cout << "Enter " << size << " integer elements:\n";
    for (int i = 0; i < size; ++i)
    {
        std::cin >> arr[i];
    }

    std::cout << "Original array: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";

    HANDLE hMinMax = CreateThread(NULL, 0, MinMaxThread, NULL, 0, NULL);
    HANDLE hAverage = CreateThread(NULL, 0, AverageThread, NULL, 0, NULL);

    if (hMinMax == NULL || hAverage == NULL)
    {
        std::cerr << "Failed to create threads. Error: " << GetLastError() << "\n";
        return 1;
    }

    WaitForSingleObject(hMinMax, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);

    CloseHandle(hMinMax);
    CloseHandle(hAverage);

    int avg_int = static_cast<int>(avg_val);

    for (size_t i = 0; i < arr.size(); ++i)
    {
        if (arr[i] == min_val || arr[i] == max_val)
        {
            arr[i] = avg_int;
        }
    }

    std::cout << "\nFinal results\n";
    std::cout << "Calculated average (rounded to int): " << avg_int << "\n";
    std::cout << "Array after replacing min and max: ";
    for (int x : arr)
    {
        std::cout << x << " ";
    }
    std::cout << "\n";

    std::cout << "Program execution completed.\n";
    return 0;
}