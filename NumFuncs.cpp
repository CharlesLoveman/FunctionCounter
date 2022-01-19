#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <map>
#include <fstream>
using namespace llvm;

#define DEBUG_TYPE "numfuncs"

namespace {
  static int FuncCounter = 0;
  static int BBCounter = 0;
  static std::map<int, int> map;
  struct NumFuncs : public FunctionPass {
    static char ID;
    NumFuncs() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
      ++FuncCounter;
      BBCounter += F.size();
      for (auto iter = F.begin(); iter != F.end(); ++iter) {
        int n = iter->sizeWithoutDebug();
        if (map.find(n) != map.end()) {
          map[n] = map.find(n)->second + 1;
        } else {
          map[n] = 1;
        }
      }
      return false;
    }

    void plotHistogram() {
      std::ofstream dataf{"data.txt"};
      std::ofstream plotf{"plot.plt"};
      plotf << "set style data histograms\n";
      plotf << "set style fill solid\n";
      plotf << "set terminal pdf\n";
      plotf << "set output 'histogram.pdf'\n";
      plotf << "plot 'data.txt' using 2:xtic(1) title 'Lines per BasicBlock'\n";
      plotf.close();
      for (const auto& iter : map) {
        dataf << iter.first << " " << iter.second << "\n";
      }
      dataf.close();
      system("gnuplot plot.plt");
      system("rm data.txt plot.plt");
    }

    bool doFinalization(Module &M) override {
      errs() << "The program has a total of " << FuncCounter << " functions";
      errs() << " and " << BBCounter << " basic blocks.\n";
      plotHistogram();
      return false;
    }
  };
}

char NumFuncs::ID = 0;
static RegisterPass<NumFuncs> X("numfuncs", "Function Counter Pass", false, false);
