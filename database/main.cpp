#include <iostream>

using namespace std;
typedef struct{ 
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;
}InputBuffer;
int main(int argc,char* argv[]){
    int n;
    cin >> n;
    cout << n << "\n";
}