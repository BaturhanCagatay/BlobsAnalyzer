// 152120211060 Baturhan Çağatay
// 152120211090 Emre Güner
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

// Structure to represent a pixel
struct Pixel {
    int x;        // X-coordinate
    int y;        // Y-coordinate
    int connected; // Indicates whether the pixel is connected to a blob. 
};

// Class to analyze and process the image file
class BlobAnalyzer {
public:
    BlobAnalyzer(const string& fileName) : inputFileName(fileName) {}

    // Process the image file to find and analyze blobs
    // This function checks the existence of the file, reads the number of rows and columns from the file, 
    // stores pixel information in a 2D vector, and determines the connected components.
    void processFile() {
        ifstream inputFile(inputFileName);
        if (!inputFile.is_open()) {
            cout << "File cannot be opened." << endl;
            return;
        }

        inputFile >> numRows >> numCols;
        inputFile.ignore(numCols, '\n'); // It aims to ignore numCols characters or the '\n' (newline) character from an input file rather than reading them.

        // Create a 2D vector to store pixel information
        pixelArray.resize(numRows, vector<Pixel>(numCols));

        connectedComponents = 0;

        // Read the x or empty characters from the file
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                char pixelValue;
                inputFile.get(pixelValue);

                if (pixelValue == 'x') {
                    pixelArray[i][j] = { i, j, 0 }; // Initialize pixel as part of an unknown blob
                }
                else {
                    pixelArray[i][j] = { i, j, -1 }; // Initialize pixel as part of no blob
                }
            }
            inputFile.get(); // Read the newline character
        }

        // Find and mark connected components (blobs)
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                if (pixelArray[i][j].connected == 0) {
                    connectedComponents++;
                    findBlobs(i, j);
                }
            }
        }

        inputFile.close();
    }

    // Print the image with connected components
    void printResults() {
        printImage();
        printBlobProperties();
    }

private:
    const string inputFileName;
    int numRows;
    int numCols;
    vector<vector<Pixel>> pixelArray;
    int connectedComponents;

    // Recursive function to find connected pixels and mark them
    // The findBlobs function of the BlobAnalyzer class is used to determine the connected component (blob) to which 
    // a pixel belongs. This function scans the other connected pixels around the pixel.
    void findBlobs(int x, int y) {
        if (x < 0 || y < 0 || x >= numRows || y >= numCols || pixelArray[x][y].connected != 0) {
            // If x or y is negative, we have reached a pixel outside the image boundaries and ignores this situation.
            // If x or y is outside the number of rows or columns, this will be ignored.
            // If the pixel already belongs to a connected component (blob), the processing does not continue and returns.
            return;
        }

        // Mark the pixel to the connected component (blob).
        pixelArray[x][y].connected = connectedComponents;

        // Recursively check neighboring pixels
        findBlobs(x + 1, y);
        findBlobs(x - 1, y);
        findBlobs(x, y + 1);
        findBlobs(x, y - 1);
    }

    // Print the image with connected components
    void printImage() {
        cout << " " << inputFileName << endl;

        // Print formatted table
        cout << "  ";
        for (int i = 0; i < numCols; i++) {
            cout << i % 10;
        }
        cout << endl;

        cout << " +";
        for (int i = 0; i < numCols; i++) {
            cout << "-";
        }
        cout << "+" << endl;

        // Print the input file and the identified blobs
        for (int i = 0; i < numRows; i++) {
            cout << i % 10 << "|";
            for (int j = 0; j < numCols; j++) {
                cout << (pixelArray[i][j].connected == -1 ? ' ' : 'x'); // Print 'x' for pixels in a blob
            }
            cout << "|" << i % 10 << endl;
        }

        cout << " +";
        for (int i = 0; i < numCols; i++) {
            cout << "-";
        }
        cout << "+" << endl;

        cout << "  ";
        for (int i = 0; i < numCols; i++) {
            cout << i % 10;
        }
        cout << endl << endl;
    }

    // Calculate the size of a blob
    int calculateBlobSize(int component) {
        int size = 0;
        // Scans all pixels
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                // If the pixel belongs to a specific component (blob), it increases the size.
                if (pixelArray[i][j].connected == component) {
                    size++;
                }
            }
        }
        return size;
    }

    // Calculate the center of mass in the row direction for a blob
    double calculateCenterOfMassRow(int component) {
        int sumX = 0;
        int size = 0;
        // Scans all pixels
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                // If the pixel belongs to a specific component (blob), it collects the row position and increases the size.
                if (pixelArray[i][j].connected == component) {
                    sumX += i;
                    size++;
                }
            }
        }
        return double(sumX) / size;
    }

    // Calculate the center of mass in the column direction for a blob
    double calculateCenterOfMassCol(int component) {
        int sumY = 0;
        int size = 0;
        // Scans all pixels
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                // If the pixel belongs to a specific component (blob), the column sums its position and increases the size.
                if (pixelArray[i][j].connected == component) {
                    sumY += j;
                    size++;
                }
            }
        }
        return double(sumY) / size;
    }

    // Print the properties of all blobs
    void printBlobProperties() {
        cout << "Blob Properties:" << endl;
        cout << "+------+------------+---------+------------+" << endl;
        cout << "| BLOB | NoOfPixels | CoM Row | CoM Column |" << endl;
        cout << "+------+------------+---------+------------+" << endl;

        // Print properties for each identified blob
        for (int i = 1; i <= connectedComponents; ++i) {
            cout << "| " << setw(4) << i << " | " << setw(10) << calculateBlobSize(i) << " | "
                << setw(7) << fixed << setprecision(2) << calculateCenterOfMassRow(i) << " | "
                << setw(10) << fixed << setprecision(2) << calculateCenterOfMassCol(i) << " |" << endl;
            // By using the setw function, we determined the width of the expression we want to display, 
            // and with the setprecision function, we specified how many decimal places the decimal number should be 
            // displayed, ensuring a neat and readable presentation.
        }

        cout << "+------+------------+---------+------------+" << endl;
    }
};

int main() {
    string userFileName;
    cout << "Enter the filename: ";
    cin >> userFileName;

    BlobAnalyzer blobAnalyzer(userFileName); // Gets the file name from the user and instantiates the BlobAnalyzer class.
    blobAnalyzer.processFile(); // Processes the file, identifies linked components (blobs) and calculates information.
    blobAnalyzer.printResults(); // // Prints the processed image and the properties of the connected components to the screen.

    return 0;
}
