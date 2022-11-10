

#include "Parser/LLPredictor.h"

using namespace std;


int main() {
    Token token;
    char test_f[] = "testfile.txt";
    Scanner scanner{test_f};
    LLPredictor predictor(scanner, token);
    KW_INIT();
    predictor.Program();
    return 0;
}
