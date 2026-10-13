#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

void findBlob(int r, int c, int rows, int cols,
             vector<vector<char>>& grid,
             int& pixelCount, long long& sumRow, long long& sumCol) {
    if (r < 0 || r >= rows || c < 0 || c >= cols) return;
    if (grid[r][c] != 'x') return;

    grid[r][c] = '.';
    pixelCount++;
    sumRow += r;
    sumCol += c;

    findBlob(r + 1, c, rows, cols, grid, pixelCount, sumRow, sumCol);
    findBlob(r - 1, c, rows, cols, grid, pixelCount, sumRow, sumCol);
    findBlob(r, c + 1, rows, cols, grid, pixelCount, sumRow, sumCol);
    findBlob(r, c - 1, rows, cols, grid, pixelCount, sumRow, sumCol);
}

struct BlobInfo{
    int id;
    int pixelCount;
    double comRow;
    double comCol;
};

int main() {
    /*** OPENING FILE ***/
    string filename;
    cout << "Enter filename: ";
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "File couldn't opened: " << filename << endl;
        return 1;
    }

    int rows=0, cols=0;
    file >> rows >> cols;

    string dummy;
    getline(file, dummy);

    vector<vector<char>> grid(rows, vector<char>(cols, ' '));
    for (int r = 0; r < rows; r++) {
        string line;
        if (getline(file, line)) {
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            for(int c = 0; c < cols && c < (int)(line.length()); c++) {
                grid[r][c] = line[c];
            }
        }
    }
    file.close();

    cout << " ";
    for (int c = 0; c < cols; ++c) {
        cout << (c % 10);
    }
    cout << "\n";

    cout << "+";
    for (int c = 0; c < cols; ++c) {
        cout << "-";
    }
    cout << "+\n";

    for (int r = 0; r < rows; ++r) {
        cout << (r % 10) << "|";
        for (int c = 0; c < cols; ++c) {
            cout << grid[r][c];
        }
        cout << "|" << (r % 10) << "\n";
    }

    cout << "+";
    for (int c = 0; c < cols; ++c) {
        cout << "-";
    }
    cout << "+\n";

    cout << " ";
    for (int c = 0; c < cols; ++c) {
        cout << (c % 10);
    }
    cout << "\n\n";


    vector<BlobInfo> blobs;
    int blobCount = 1;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] == 'x') {
                int pixelCount = 0;
                long long sumRow = 0;
                long long sumCol = 0;

                findBlob(r, c, rows, cols, grid, pixelCount, sumRow, sumCol);

                if (pixelCount > 0) {
                    BlobInfo b;
                    b.id = blobCount++;
                    b.pixelCount = pixelCount;
                    b.comRow = static_cast<double>(sumRow) / pixelCount;
                    b.comCol = static_cast<double>(sumCol) / pixelCount;
                    blobs.push_back(b);
                }
            }
        }
    }

    cout << "+------+------------+---------+---------+\n";
    cout << "| BLOB | NoOfPixels | Com Row | CoM Col |\n";
    cout << "+------+------------+---------+---------+\n";

    cout << fixed << setprecision(2);
    for (const auto& b : blobs) {
        cout << "| " << setw(4) << b.id << " | "
             << setw(10) << b.pixelCount << " | "
             << setw(7) << b.comRow << " | "
             << setw(7) << b.comCol << " |\n";
    }
    cout << "+------+------------+---------+---------+\n";

    return 0;
}
