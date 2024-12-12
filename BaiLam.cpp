#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <sstream>
#include <algorithm>
#include <commdlg.h>  // Để sử dụng hộp thoại chọn file

// Định nghĩa các ID cho các thành phần giao diện
#define IDC_INPUT_TEXT 101
#define IDC_ALGO_COMBO 102
#define IDC_SORT_BUTTON 103
#define IDC_OUTPUT_TEXT 104
#define IDC_LOAD_FILE_BUTTON 105
#define IDC_SAVE_FILE_BUTTON 106
#define IDC_TIME_TEXT 107

// Các hàm sắp xếp
void BubbleSort(std::vector<int>& arr) {
    for (size_t i = 0; i < arr.size() - 1; i++) {
        for (size_t j = 0; j < arr.size() - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void QuickSort(std::vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int pivot = arr[(left + right) / 2];
    int i = left, j = right;
    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if (i <= j) {
            std::swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }
    QuickSort(arr, left, j);
    QuickSort(arr, i, right);
}

void SelectionSort(std::vector<int>& arr) {
    for (size_t i = 0; i < arr.size() - 1; i++) {
        int minIdx = i;
        for (size_t j = i + 1; j < arr.size(); j++) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }
        std::swap(arr[i], arr[minIdx]);
    }
}
void InsertionSort(std::vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}
void Merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> leftArr(n1), rightArr(n2);

    for (int i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (int i = 0; i < n2; i++)
        rightArr[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i];
            i++;
        }
        else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}

void MergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        MergeSort(arr, left, mid);
        MergeSort(arr, mid + 1, right);

        Merge(arr, left, mid, right);
    }
}

void Heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        Heapify(arr, n, largest);
    }
}

void HeapSort(std::vector<int>& arr) {
    int n = arr.size();

    for (int i = n / 2 - 1; i >= 0; i--)
        Heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);
        Heapify(arr, i, 0);
    }
}


void CountSort(std::vector<int>& arr, int exp) {
    int n = arr.size();
    std::vector<int> output(n);
    int count[10] = { 0 };

    for (int i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    for (int i = 0; i < n; i++)
        arr[i] = output[i];
}

void RadixSort(std::vector<int>& arr) {
    int maxVal = *std::max_element(arr.begin(), arr.end());

    for (int exp = 1; maxVal / exp > 0; exp *= 10)
        CountSort(arr, exp);
}
void ShellSort(std::vector<int>& arr) {
    int n = arr.size();

    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}


// Hàm chuyển chuỗi thành vector số nguyên
std::vector<int> ParseInput(const std::wstring& input) {
    std::vector<int> arr;
    std::wstringstream ss(input);
    int num;
    while (ss >> num) {
        arr.push_back(num);
    }
    return arr;
}

// Hàm đọc dữ liệu từ file
bool LoadDataFromFile(const std::wstring& filename, std::vector<int>& data) {
    std::wifstream file(filename);
    if (!file.is_open()) return false;

    int num;
    while (file >> num) {
        data.push_back(num);
    }
    return true;
}

// Hàm lưu dữ liệu vào file
bool SaveDataToFile(const std::wstring& filename, const std::vector<int>& data) {
    std::wofstream file(filename);
    if (!file.is_open()) return false;

    for (int num : data) {
        file << num << L" ";
    }
    return true;
}

// Hàm xử lý sự kiện cửa sổ
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static HWND hInput, hAlgoCombo, hSortButton, hOutput, hLoadFile, hSaveFile, hTimeText;
    static std::vector<int> data;

    switch (message) {
    case WM_CREATE: {
        // Ô nhập dữ liệu
        hInput = CreateWindow(L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE,
            20, 20, 300, 50,
            hWnd, (HMENU)IDC_INPUT_TEXT, NULL, NULL);

        hAlgoCombo = CreateWindow(L"COMBOBOX", NULL,
            CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE,
            20, 90, 200, 300, // Tăng chiều cao để hiển thị đủ
            hWnd, (HMENU)IDC_ALGO_COMBO, NULL, NULL);

        SendMessage(hAlgoCombo, CB_ADDSTRING, 0, (LPARAM)L"Bubble Sort");
        SendMessage(hAlgoCombo, CB_ADDSTRING, 0, (LPARAM)L"Quick Sort");
        SendMessage(hAlgoCombo, CB_ADDSTRING, 0, (LPARAM)L"Selection Sort");
        SendMessage(hAlgoCombo, CB_ADDSTRING, 0, (LPARAM)L"Insertion Sort");
        SendMessage(hAlgoCombo, CB_ADDSTRING, 0, (LPARAM)L"Merge Sort");
        SendMessage(hAlgoCombo, CB_ADDSTRING, 0, (LPARAM)L"Heap Sort");
        SendMessage(hAlgoCombo, CB_ADDSTRING, 0, (LPARAM)L"Radix Sort");
        SendMessage(hAlgoCombo, CB_ADDSTRING, 0, (LPARAM)L"Shell Sort");

        // Nút sắp xếp
        hSortButton = CreateWindow(L"BUTTON", L"Sắp xếp",
            WS_CHILD | WS_VISIBLE,
            240, 90, 80, 30,
            hWnd, (HMENU)IDC_SORT_BUTTON, NULL, NULL);

        // Nút tải file
        hLoadFile = CreateWindow(L"BUTTON", L"Tải từ file",
            WS_CHILD | WS_VISIBLE,
            340, 20, 100, 30,
            hWnd, (HMENU)IDC_LOAD_FILE_BUTTON, NULL, NULL);

        // Nút lưu file
        hSaveFile = CreateWindow(L"BUTTON", L"Ghi ra file",
            WS_CHILD | WS_VISIBLE,
            340, 60, 100, 30,
            hWnd, (HMENU)IDC_SAVE_FILE_BUTTON, NULL, NULL);

        // Ô hiển thị kết quả
        hOutput = CreateWindow(L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_READONLY,
            20, 140, 420, 100,
            hWnd, (HMENU)IDC_OUTPUT_TEXT, NULL, NULL);

        // Hiển thị thời gian thực thi
        hTimeText = CreateWindow(L"STATIC", L"Thời gian thực thi: 0 ms",
            WS_CHILD | WS_VISIBLE,
            20, 260, 300, 20,
            hWnd, (HMENU)IDC_TIME_TEXT, NULL, NULL);
        break;
    }

    case WM_COMMAND: {
        if (LOWORD(wParam) == IDC_SORT_BUTTON) {
            // Lấy dữ liệu từ ô nhập
            wchar_t inputText[1024];
            GetWindowText(hInput, inputText, 1024);

            // Chuyển đổi chuỗi thành vector số nguyên
            data = ParseInput(inputText);

            // Kiểm tra dữ liệu đầu vào
            if (data.empty()) {
                MessageBox(hWnd, L"Vui lòng nhập dữ liệu hợp lệ (các số cách nhau bởi dấu cách)!", L"Lỗi", MB_OK);
                return 0;
            }

            // Lấy thuật toán được chọn
            int algoIndex = SendMessage(hAlgoCombo, CB_GETCURSEL, 0, 0);
            if (algoIndex == CB_ERR) {
                MessageBox(hWnd, L"Vui lòng chọn thuật toán!", L"Lỗi", MB_OK);
                return 0;
            }

            auto start = std::chrono::high_resolution_clock::now();

            // Thực hiện thuật toán sắp xếp
            if (algoIndex == 0) {
                BubbleSort(data);
            }
            else if (algoIndex == 1) {
                QuickSort(data, 0, data.size() - 1);
            }
            else if (algoIndex == 2) {
                SelectionSort(data);
            }
            else if (algoIndex == 3) {
                InsertionSort(data);
            }
            else if (algoIndex == 4) {
                MergeSort(data, 0, data.size() - 1);
            }
            else if (algoIndex == 5) { // Heap Sort
                HeapSort(data);
            }
            else if (algoIndex == 6) { // Radix Sort
                RadixSort(data);
            }
            else if (algoIndex == 7) { // Shell Sort
                ShellSort(data);
            }

            auto end = std::chrono::high_resolution_clock::now();

            // Tính thời gian chạy (micro giây)
            int64_t elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            // Chuyển đổi sang chuỗi hiển thị (microseconds)
            std::wstring timeText = L"Thời gian thực thi: " + std::to_wstring(elapsedTime) + L" µs";
            SetWindowText(hTimeText, timeText.c_str());

            // Hiển thị kết quả sắp xếp
            std::wstring result;
            for (int num : data) {
                result += std::to_wstring(num) + L" ";
            }
            SetWindowText(hOutput, result.c_str());
        }
        // Tải dữ liệu từ file
        else if (LOWORD(wParam) == IDC_LOAD_FILE_BUTTON) {
            OPENFILENAME ofn;       // cấu trúc để lưu thông tin file
            wchar_t szFile[260];     // Dùng để lưu tên file

            // Thiết lập các thông số cho hộp thoại mở file
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hWnd;
            ofn.lpstrFile = szFile;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(szFile);
            ofn.lpstrFilter = L"Text Files\0*.TXT\0All Files\0*.*\0";
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            if (GetOpenFileName(&ofn) == TRUE) {
                // Tải dữ liệu từ file vào vector
                data.clear();
                if (LoadDataFromFile(ofn.lpstrFile, data)) {
                    // Cập nhật ô nhập liệu với dữ liệu từ file
                    std::wstring fileContent;
                    for (int num : data) {
                        fileContent += std::to_wstring(num) + L" ";
                    }
                    SetWindowText(hInput, fileContent.c_str());
                }
                else {
                    MessageBox(hWnd, L"Không thể tải dữ liệu từ file!", L"Lỗi", MB_OK);
                }
            }
        }
        // Lưu dữ liệu vào file
        else if (LOWORD(wParam) == IDC_SAVE_FILE_BUTTON) {
            OPENFILENAME ofn;       // cấu trúc để lưu thông tin file
            wchar_t szFile[260];     // Dùng để lưu tên file

            // Thiết lập các thông số cho hộp thoại lưu file
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hWnd;
            ofn.lpstrFile = szFile;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(szFile);
            ofn.lpstrFilter = L"Text Files\0*.TXT\0All Files\0*.*\0";
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

            if (GetSaveFileName(&ofn) == TRUE) {
                if (!SaveDataToFile(ofn.lpstrFile, data)) {
                    MessageBox(hWnd, L"Không thể lưu dữ liệu vào file!", L"Lỗi", MB_OK);
                }
            }
        }
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Hàm chính
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"SortingApp";

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClassEx(&wc)) return 0;

    HWND hWnd = CreateWindowEx(
        0, CLASS_NAME, L"Sorting Application",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 350,
        NULL, NULL, hInstance, NULL);

    if (!hWnd) return 0;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
