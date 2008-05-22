/* -------------------------------------------------------------------------- *
 *                      SimTK Core: SimTKcommon                               *
 * -------------------------------------------------------------------------- *
 * This is part of the SimTK Core biosimulation toolkit originating from      *
 * Simbios, the NIH National Center for Physics-Based Simulation of           *
 * Biological Structures at Stanford, funded under the NIH Roadmap for        *
 * Medical Research, grant U54 GM072970. See https://simtk.org.               *
 *                                                                            *
 * Portions copyright (c) 2008 Stanford University and the Authors.           *
 * Authors: Peter Eastman                                                     *
 * Contributors:                                                              *
 *                                                                            *
 * Permission is hereby granted, free of charge, to any person obtaining a    *
 * copy of this software and associated documentation files (the "Software"), *
 * to deal in the Software without restriction, including without limitation  *
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,   *
 * and/or sell copies of the Software, and to permit persons to whom the      *
 * Software is furnished to do so, subject to the following conditions:       *
 *                                                                            *
 * The above copyright notice and this permission notice shall be included in *
 * all copies or substantial portions of the Software.                        *
 *                                                                            *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    *
 * THE AUTHORS, CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,    *
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR      *
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE  *
 * USE OR OTHER DEALINGS IN THE SOFTWARE.                                     *
 * -------------------------------------------------------------------------- */

#include "SimTKcommon.h"

#include <iostream>
#include <vector>

#define ASSERT(cond) {SimTK_ASSERT_ALWAYS(cond, "Assertion failed");}

using std::cout;
using std::endl;
using namespace SimTK;
using namespace std;

class SetFlagTask : public ParallelExecutor::Task {
public:
    SetFlagTask(vector<int>& flags) : flags(flags) {
    }
    void execute(int index) {
        flags[index]++;
    }
private:
    vector<int>& flags;
};

void testParallelExecution() {
    const int numFlags = 100;
    vector<int> flags(numFlags);
    ParallelExecutor executor;
    SetFlagTask task(flags);
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < numFlags; ++j)
            flags[j] = 0;
        executor.execute(task, numFlags-10);
        for (int j = 0; j < numFlags; ++j)
            ASSERT(flags[j] == (j < numFlags-10 ? 1 : 0));
    }
}

void testSingleThreadedExecution() {
    const int numFlags = 100;
    vector<int> flags(numFlags);
    ParallelExecutor executor(1); // Specify only a single thread.
    SetFlagTask task(flags);
    for (int j = 0; j < numFlags; ++j)
        flags[j] = 0;
    executor.execute(task, numFlags-10);
    for (int j = 0; j < numFlags; ++j)
        ASSERT(flags[j] == (j < numFlags-10 ? 1 : 0));
}

int main() {
    try {
        testParallelExecution();
        testSingleThreadedExecution();
    } catch(const std::exception& e) {
        cout << "exception: " << e.what() << endl;
        return 1;
    }
    cout << "Done" << endl;
    return 0;
}