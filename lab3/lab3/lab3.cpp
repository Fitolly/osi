#include <iostream>
#include <vector>
#include <windows.h>
#include <cstdlib>
#include <ctime>

std::vector<int> arr;
int markers_count;
std::vector<HANDLE> hMarkers;
std::vector<HANDLE> hStartEvents;
std::vector<HANDLE> hStopEvents;
std::vector<HANDLE> hContinueEvents;

DWORD WINAPI MarkerThread(LPVOID lpParam)
{
    int markerId = (int)(intptr_t)lpParam;
    int marked_count = 0;
    std::vector<int> marked_indices;

    srand(markerId);

    WaitForSingleObject(hStartEvents[markerId - 1], INFINITE);

    while (true)
    {
        int rand_val = rand();
        int index = rand_val % arr.size();

        if (arr[index] == 0)
        {
            Sleep(5);
            arr[index] = markerId;
            marked_indices.push_back(index);
            marked_count++;
            Sleep(5);
        }
        else
        {
            std::cout << "\nMarker " << markerId << " cannot continue.\n";
            std::cout << "Marked elements: " << marked_count << "\n";
            std::cout << "Failed index: " << index << "\n";

            SetEvent(hStopEvents[markerId - 1]);

            HANDLE events[2] = { hContinueEvents[markerId - 1], hStopEvents[markers_count + markerId - 1] };
            DWORD waitResult = WaitForMultipleObjects(2, events, FALSE, INFINITE);

            if (waitResult == WAIT_OBJECT_0 + 1)
            {
                for (int idx : marked_indices)
                {
                    arr[idx] = 0;
                }
                return 0;
            }
        }
    }
    return 0;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int size;
    std::cout << "Enter array size: ";
    std::cin >> size;

    arr.assign(size, 0);

    std::cout << "Enter number of marker threads: ";
    std::cin >> markers_count;

    hMarkers.resize(markers_count);
    hStartEvents.resize(markers_count);
    hStopEvents.resize(markers_count * 2);
    hContinueEvents.resize(markers_count);

    for (int i = 0; i < markers_count; ++i)
    {
        hStartEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        hStopEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        hStopEvents[markers_count + i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        hContinueEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);

        hMarkers[i] = CreateThread(NULL, 0, MarkerThread, (LPVOID)(intptr_t)(i + 1), 0, NULL);
    }

    for (int i = 0; i < markers_count; ++i)
    {
        SetEvent(hStartEvents[i]);
    }

    int active_markers = markers_count;

    while (active_markers > 0)
    {
        WaitForMultipleObjects(markers_count, hStopEvents.data(), TRUE, INFINITE);

        std::cout << "\nCurrent array state:\n";
        for (int i = 0; i < size; ++i)
        {
            std::cout << arr[i] << " ";
        }
        std::cout << "\n";

        int stop_id;
        std::cout << "Enter marker ID to stop: ";
        std::cin >> stop_id;

        SetEvent(hStopEvents[markers_count + stop_id - 1]);

        WaitForSingleObject(hMarkers[stop_id - 1], INFINITE);
        CloseHandle(hMarkers[stop_id - 1]);

        std::cout << "\nArray after stopping marker " << stop_id << ":\n";
        for (int i = 0; i < size; ++i)
        {
            std::cout << arr[i] << " ";
        }
        std::cout << "\n";

        active_markers--;

        for (int i = 0; i < markers_count; ++i)
        {
            if (i != stop_id - 1 && arr.size() > 0)
            {
                ResetEvent(hStopEvents[i]);
                SetEvent(hContinueEvents[i]);
            }
        }
    }

    std::cout << "\nAll marker threads finished.\n";

    for (int i = 0; i < markers_count; ++i)
    {
        CloseHandle(hStartEvents[i]);
        CloseHandle(hStopEvents[i]);
        CloseHandle(hStopEvents[markers_count + i]);
        CloseHandle(hContinueEvents[i]);
    }

    return 0;
}