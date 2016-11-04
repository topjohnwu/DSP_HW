#include "hmm.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

#define a model.transition
#define b model.observation
#define pi model.initial

int main(int argc, char const *argv[]) {
   if (argc != 5) {
      cerr << "Usage: " << argv[0] << " [iteration] [Initial Model file] [Input sequence file] [output file]" << endl;
      return 1;
   }

   HMM model;
   loadHMM(&model, argv[2]);
   int iteration = atoi(argv[1]), N = model.state_num, K = model.observ_num;
   vector<string> seq;
   ifstream seqFile(argv[3], ios::in);
   string line;

   while (getline(seqFile, line))
      seq.push_back(line);
   seqFile.close();

   int T = seq[0].size();
   int samples = seq.size();

   double alpha[MAX_SEQ][MAX_STATE], beta[MAX_SEQ][MAX_STATE], gamma[MAX_SEQ][MAX_STATE], epsilon[MAX_SEQ][MAX_STATE][MAX_STATE];
   double newPi[MAX_STATE], accuGamma[MAX_STATE], lastAccuGamma[MAX_STATE];
   double observGamma[MAX_OBSERV][MAX_STATE], accuEpsilon[MAX_STATE][MAX_STATE];

   while (iteration--) {

      for (int i = 0; i < N; ++i) {
         newPi[i] = 0.0;
         accuGamma[i] = 0.0;
         lastAccuGamma[i] = 0.0;
      }

      for (int i = 0; i < N; ++i) {
         for (int k = 0; k < N; ++k)
            observGamma[k][i] = 0.0;
         for (int j = 0; j < N; ++j)
            accuEpsilon[i][j] = 0.0;
      }

      for (int x = 0; x < samples; ++x) {
         line = seq[x];

         // Alpha
         for (int i = 0; i < N; ++i)
            alpha[0][i] = pi[i] * b[line[0] - 'A'][i];

         for (int t = 0; t < T - 1; ++t) {
            for (int j = 0; j < N; ++j) {
               alpha[t + 1][j] = 0.0;
               for (int i = 0; i < N; ++i)
                  alpha[t + 1][j] += alpha[t][i] * a[i][j];
               alpha[t + 1][j] *= b[line[t + 1] - 'A'][j];
            }
         }

         // Beta
         for (int t = T - 1; t >= 0; --t) {
            for (int i = 0; i < N; ++i) {
               if (t == T - 1) {
                  beta[t][i] = 1.0;
                  continue;
               }
               beta[t][i] = 0.0;
               for (int j = 0; j < N; ++j)
                  beta[t][i] += a[i][j] * b[line[t + 1] - 'A'][j] * beta[t + 1][j];
            }
               
         }

         // Gamma
         for (int t = 0; t < T; ++t) {
            double sum = 0.0;
            for (int i = 0; i < N; ++i) {
               gamma[t][i] = alpha[t][i] * beta[t][i];
               sum += gamma[t][i];
            }
            for (int i = 0; i < N; ++i)
               gamma[t][i] /= sum;
         }

         // Epsilon
         for (int t = 0; t < T - 1; ++t) {
            double sum = 0.0;
            for (int i = 0; i < N; ++i) {
               for (int j = 0; j < N; ++j) {
                  epsilon[t][i][j] = alpha[t][i] * a[i][j] * b[line[t + 1] - 'A'][j] * beta[t + 1][j];
                  sum += epsilon[t][i][j];
               }
            }
            for (int i = 0; i < N; ++i)
               for (int j = 0; j < N; ++j)
                  epsilon[t][i][j] /= sum;
         }

         // Accumulate gamma
         for (int i = 0; i < N; ++i) {
            for (int t = 0; t < T; ++t) {
               if (t == 0) newPi[i] += gamma[t][i];
               (t == T - 1 ? lastAccuGamma[i] : accuGamma[i]) += gamma[t][i];
               observGamma[line[t] - 'A'][i] += gamma[t][i];
            }
         }

         // Accumulate epsilon
         for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
               for (int t = 0; t < T - 1; ++t) 
                  accuEpsilon[i][j] += epsilon[t][i][j];
      }

      // Update parameters
      for (int i = 0; i < N; ++i) {
         pi[i] = newPi[i] / samples;
         for (int j = 0; j < N; ++j)
            a[i][j] = accuEpsilon[i][j] / accuGamma[i];
         for (int k = 0; k < K; ++k)
            b[k][i] = observGamma[k][i] / (accuGamma[i] + lastAccuGamma[i]);
      }
   }

   FILE* out = fopen(argv[4], "w");
   dumpHMM(out, &model);
   fclose(out);

   return 0;
}