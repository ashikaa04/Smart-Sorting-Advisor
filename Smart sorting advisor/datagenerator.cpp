#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

// ---------- Sorting Algorithms ----------
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
}

int partitionQS(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}
void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partitionQS(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void shellSort(vector<int>& arr) {
    int n = arr.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap)
                arr[j] = arr[j - gap];
            arr[j] = temp;
        }
    }
}

int getMax(vector<int>& arr) {
    return *max_element(arr.begin(), arr.end());
}
void countingSortRadix(vector<int>& arr, int exp) {
    int n = arr.size();
    vector<int> output(n);
    int count[10] = {0};
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
void radixSort(vector<int>& arr) {
    int m = getMax(arr);
    for (int exp = 1; m / exp > 0; exp *= 10)
        countingSortRadix(arr, exp);
}

// ---------- Utility Functions ----------
double getSortedness(const vector<int>& arr) {
    int count = 0;
    for (int i = 0; i < arr.size() - 1; i++)
        if (arr[i] <= arr[i + 1]) count++;
    return (double)count / (arr.size() - 1);
}
int getRange(const vector<int>& arr) {
    int minVal = *min_element(arr.begin(), arr.end());
    int maxVal = *max_element(arr.begin(), arr.end());
    return maxVal - minVal;
}

double timeAlgorithm(vector<int> arr, void (*sortFunc)(vector<int>&)) {
    auto start = high_resolution_clock::now();
    sortFunc(arr);
    auto end = high_resolution_clock::now();
    return duration<double, milli>(end - start).count();
}

// For quickSort variant
double timeQuickSort(vector<int> arr) {
    auto start = high_resolution_clock::now();
    quickSort(arr, 0, arr.size() - 1);
    auto end = high_resolution_clock::now();
    return duration<double, milli>(end - start).count();
}

// ---------- Main ----------
int main() {
    srand(time(0));
    ofstream file("training_data.csv");
    file << "size,sortedness,range,label\n";

    for (int trial = 0; trial < 200; trial++) {
        int n = rand() % 1000 + 10; // size between 10 and 1000
        vector<int> arr(n);
        for (int i = 0; i < n; i++)
            arr[i] = rand() % (n * 10); // values within some range

        double sortedness = getSortedness(arr);
        int rangeVal = getRange(arr);

        double t1 = timeAlgorithm(arr, bubbleSort);
        double t2 = timeQuickSort(arr);
        double t3 = timeAlgorithm(arr, shellSort);
        double t4 = timeAlgorithm(arr, radixSort);

        // Get fastest algorithm index
        double times[4] = {t1, t2, t3, t4};
        int label = min_element(times, times + 4) - times;

        file << n << "," << sortedness << "," << rangeVal << "," << label << "\n";
    }

    cout << "Training data saved to training_data.csv\n";
    return 0;
}
