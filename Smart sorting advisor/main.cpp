#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "decision_tree.h" // ML model's predictAlgorithm() function

using namespace std;

// ---------------- Sorting Algorithms ----------------
void bubbleSort(vector<int>& arr) {
    for (size_t i = 0; i < arr.size() - 1; i++) {
        for (size_t j = 0; j < arr.size() - i - 1; j++) {
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }
    }
}

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void shellSort(vector<int>& arr) {
    for (int gap = arr.size() / 2; gap > 0; gap /= 2) {
        for (size_t i = gap; i < arr.size(); i++) {
            int temp = arr[i];
            size_t j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

void countingSortForRadix(vector<int>& arr, int exp) {
    vector<int> output(arr.size());
    int count[10] = {0};

    for (size_t i = 0; i < arr.size(); i++)
        count[(arr[i] / exp) % 10]++;

    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (int i = arr.size() - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    for (size_t i = 0; i < arr.size(); i++)
        arr[i] = output[i];
}

void radixSort(vector<int>& arr) {
    int maxVal = *max_element(arr.begin(), arr.end());
    for (int exp = 1; maxVal / exp > 0; exp *= 10)
        countingSortForRadix(arr, exp);
}

// ---------------- Feature Extraction ----------------
double calculateSortedness(const vector<int>& arr) {
    int count = 0;
    for (size_t i = 0; i < arr.size() - 1; i++) {
        if (arr[i] <= arr[i + 1]) count++;
    }
    return static_cast<double>(count) / (arr.size() - 1);
}

int calculateRange(const vector<int>& arr) {
    return *max_element(arr.begin(), arr.end()) - *min_element(arr.begin(), arr.end());
}

// ---------------- Random Array Generator ----------------
vector<int> generateRandomArray(int n, int minVal, int maxVal) {
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        arr[i] = minVal + rand() % (maxVal - minVal + 1);
    }
    return arr;
}

// ---------------- Main Program ----------------
int main() {
    srand(time(0));

    int choice;
    cout << "Choose input method:\n1. Manual entry\n2. Random array\nEnter choice: ";
    cin >> choice;

    vector<int> arr;
    int n;

    if (choice == 1) {
        cout << "Enter number of elements: ";
        cin >> n;
        arr.resize(n);
        cout << "Enter " << n << " integers:\n";
        for (int i = 0; i < n; i++) {
            cin >> arr[i];
        }
    } 
    else if (choice == 2) {
        int minVal, maxVal;
        cout << "Enter number of elements: ";
        cin >> n;
        cout << "Enter min value: ";
        cin >> minVal;
        cout << "Enter max value: ";
        cin >> maxVal;
        arr = generateRandomArray(n, minVal, maxVal);
        cout << "Generated array: ";
        for (int num : arr) cout << num << " ";
        cout << "\n";
    } 
    else {
        cout << "Invalid choice!\n";
        return 1;
    }

    int size = arr.size();
    double sortedness = calculateSortedness(arr);
    int range = calculateRange(arr);

    int algo = stoi(predictAlgorithm(size, sortedness, range));

    cout << "Predicted algorithm: ";
    switch (algo) {
        case 0: cout << "Bubble Sort\n"; bubbleSort(arr); break;
        case 1: cout << "Quick Sort\n"; quickSort(arr, 0, arr.size() - 1); break;
        case 2: cout << "Shell Sort\n"; shellSort(arr); break;
        case 3: cout << "Radix Sort\n"; radixSort(arr); break;
        default: cout << "Unknown (defaulting to Quick Sort)\n"; quickSort(arr, 0, arr.size() - 1); break;
    }

    cout << "Sorted array: ";
    for (int num : arr) cout << num << " ";
    cout << endl;

    return 0;
}
