#include <bits/stdc++.h>
using namespace std;
using namespace chrono;

// Sorting algorithms implementations

void bubbleSort(vector<int>& arr) {
    int n = (int)arr.size();
    for (int i = 0; i < n-1; i++) {
        bool swapped = false;
        for (int j = 0; j < n-1-i; j++) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

void insertionSort(vector<int>& arr) {
    int n = (int)arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i], j = i-1;
        while (j >= 0 && arr[j] > key) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
}

void selectionSort(vector<int>& arr) {
    int n = (int)arr.size();
    for (int i = 0; i < n-1; i++) {
        int min_idx = i;
        for (int j = i+1; j < n; j++) {
            if (arr[j] < arr[min_idx])
                min_idx = j;
        }
        swap(arr[i], arr[min_idx]);
    }
}

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low-1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i+1], arr[high]);
    return i+1;
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int p = partition(arr, low, high);
        quickSort(arr, low, p-1);
        quickSort(arr, p+1, high);
    }
}

void shellSort(vector<int>& arr) {
    int n = (int)arr.size();
    for (int gap = n/2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j-gap] > temp; j -= gap)
                arr[j] = arr[j-gap];
            arr[j] = temp;
        }
    }
}

void countingSort(vector<int>& arr) {
    if (arr.empty()) return;
    int min_val = *min_element(arr.begin(), arr.end());
    int max_val = *max_element(arr.begin(), arr.end());
    int range = max_val - min_val + 1;
    vector<int> count(range, 0);
    for (int x : arr) count[x - min_val]++;
    int index = 0;
    for (int i = 0; i < range; i++) {
        while (count[i]-- > 0) {
            arr[index++] = i + min_val;
        }
    }
}

int getMax(const vector<int>& arr) {
    return *max_element(arr.begin(), arr.end());
}

void countingSortForRadix(vector<int>& arr, int exp) {
    int n = (int)arr.size();
    vector<int> output(n);
    int count[10] = {0};

    for (int i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (int i = n-1; i >= 0; i--) {
        int digit = (arr[i] / exp) % 10;
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }

    for (int i = 0; i < n; i++)
        arr[i] = output[i];
}

void radixSort(vector<int>& arr) {
    if (arr.empty()) return;
    // Radix sort normally works for non-negative ints, so offset by min if negatives present
    int min_val = *min_element(arr.begin(), arr.end());
    if (min_val < 0) {
        for (auto& x : arr) x -= min_val;
    }

    int max_val = getMax(arr);

    for (int exp = 1; max_val/exp > 0; exp *= 10) {
        countingSortForRadix(arr, exp);
    }

    if (min_val < 0) {
        for (auto& x : arr) x += min_val;
    }
}

// For bucket sort, we map elements into buckets and sort each bucket (using std::sort here)

void bucketSort(vector<int>& arr) {
    if (arr.empty()) return;

    int min_val = *min_element(arr.begin(), arr.end());
    int max_val = *max_element(arr.begin(), arr.end());
    int n = (int)arr.size();
    int bucketCount = max(1, n/5); // heuristic bucket count

    vector<vector<int>> buckets(bucketCount);

    for (int x : arr) {
        int idx = (int)((long long)(x - min_val) * (bucketCount - 1) / (max_val - min_val));
        buckets[idx].push_back(x);
    }

    arr.clear();

    for (auto& bucket : buckets) {
        sort(bucket.begin(), bucket.end());
        for (int x : bucket) arr.push_back(x);
    }
}

// Overload 1: for sorting functions with signature void f(vector<int>&)
template<typename Func>
void testAlgorithm(Func sortFunc, const string& name, const vector<int>& arr, vector<pair<string, double>>& results) {
    vector<int> temp = arr;
    auto start = high_resolution_clock::now();
    sortFunc(temp);
    auto end = high_resolution_clock::now();
    double duration = duration_cast<microseconds>(end - start).count();
    results.push_back({name, duration});
}

// Overload 2: for sorting functions with signature void f(vector<int>&, int, int)
template<typename Func>
void testAlgorithm(Func sortFunc, const string& name, const vector<int>& arr, vector<pair<string, double>>& results, bool) {
    vector<int> temp = arr;
    auto start = high_resolution_clock::now();
    sortFunc(temp, 0, (int)temp.size() - 1);
    auto end = high_resolution_clock::now();
    double duration = duration_cast<microseconds>(end - start).count();
    results.push_back({name, duration});
}

// Helper to parse one line CSV to vector<int>
vector<int> parseArray(const string& line) {
    vector<int> arr;
    stringstream ss(line);
    string numStr;
    while (getline(ss, numStr, ',')) {
        if(!numStr.empty()) {
            arr.push_back(stoi(numStr));
        }
    }
    return arr;
}

// Convert vector<int> to CSV string
string arrayToString(const vector<int>& arr) {
    stringstream ss;
    for (size_t i = 0; i < arr.size(); i++) {
        if (i > 0) ss << ",";
        ss << arr[i];
    }
    return ss.str();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream fin("arrays_dataset.csv");
    ofstream fout("labeled_arrays.csv");
    if (!fin) {
        cerr << "Error opening input_arrays.csv\n";
        return 1;
    }

    // Write header to output CSV
    fout << "Array,BestSort\n";

    string line;
    int lineCount = 0;
    while (getline(fin, line)) {
        if (line.empty()) continue;

        vector<int> arr = parseArray(line);
        if (arr.empty()) continue;

        vector<pair<string, double>> results;

        // Test all sorting algorithms:

        testAlgorithm(bubbleSort, "BubbleSort", arr, results);
        testAlgorithm(insertionSort, "InsertionSort", arr, results);
        testAlgorithm(selectionSort, "SelectionSort", arr, results);
        testAlgorithm(quickSort, "QuickSort", arr, results, true);
        testAlgorithm(shellSort, "ShellSort", arr, results);
        testAlgorithm(countingSort, "CountingSort", arr, results);
        testAlgorithm(radixSort, "RadixSort", arr, results);
        testAlgorithm(bucketSort, "BucketSort", arr, results);

        // Find best (minimum time) algorithm
        auto best = min_element(results.begin(), results.end(),
            [](const auto& a, const auto& b) { return a.second < b.second; });

        // Write array and best sort to output CSV
        fout << "\"" << arrayToString(arr) << "\"," << best->first << "\n";

        lineCount++;
        if (lineCount % 100 == 0) {
            cout << lineCount << " arrays processed...\n";
        }
    }

    cout << "Done labeling " << lineCount << " arrays.\n";
    fin.close();
    fout.close();

    return 0;
}
