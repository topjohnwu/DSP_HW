#include "hmm.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
using namespace std;

#define a(n) models[n].transition
#define b(n) models[n].observation
#define pi(n) models[n].initial

void update_max(double &i, double j) {
   if (j > i) i = j;
}

int main(int argc, char const *argv[]) {
   if (argc != 4) {
      cerr << "Usage: " << argv[0] << " [Model list] [Testing Sequence file] [output file]" << endl;
      return 1;
   }
   HMM models[5];
   load_models(argv[1], models, 5);
   vector<string> seq;
   ifstream seqFile(argv[2], ios::in);
   ofstream output(argv[3], ios::out);
   string line;

   while (getline(seqFile, line))
      seq.push_back(line);
   seqFile.close();

   int samples = seq.size(), T = seq[1].size(), maxIndex;
   double delta[MAX_SEQ][MAX_STATE], max, maxProb;

   for (int x = 0; x < samples; ++x) {
      line = seq[x];
      maxProb = 0.0;
      for (int n = 0; n < 5; ++n) {
         int N = models[n].state_num;
         for (int i = 0; i < N; ++i)
            delta[0][i] = pi(n)[i] * b(n)[line[0] - 'A'][i];
         for (int t = 0; t < T - 1; ++t) {
            for (int j = 0; j < N; ++j) {
               max = 0.0;
               for (int i = 0; i < N; ++i)
                  update_max(max, delta[t][i] * a(n)[i][j]);
               delta[t + 1][j] = max * b(n)[line[t + 1] - 'A'][j];
            }
         }
         max = 0.0;
         for (int i = 0; i < N; ++i)
            update_max(max, delta[T -1][i]);
         if (max > maxProb) {
            maxProb = max;
            maxIndex = n;
         }
      }

      output << models[maxIndex].model_name << " " << setprecision(6) << scientific << maxProb << endl;
   }
   output.close();
   return 0;
}