#include "solver.h"


/*************************** DECLARE YOUR HELPER FUNCTIONS HERE ************************/
void print_2D_vector(std::vector <std::vector <int>> solns) {
    /*
        Prints a 2D vector
    */
    for(int i = 0; i < (int)solns.size(); i ++) {
        printf("{");
        for (int j = 0; j < (int)solns[i].size(); j ++) {
            printf("%d ", solns[i][j]);
        }
        printf("}\n");
    }
}


bool is_valid(std::vector <int> arr) {
    /*
        Checks if the input vector is a valid assignment
    */
    // Find the first occurance of -1 in arr
    int idx = arr.size();
    for(int i = 0; i < (int)arr.size(); i ++) {
        if(arr[i] == -1) {
            idx = i;
            break;
        }
    }

    if (idx == 0) {
        return true;
    } else {
        idx --;     // represents the index of last assignment
    }

    // Check if the last assigned variable is consistent with
    // the ones assigned before it
    for(int i = 0; i < idx; i ++) {
        if (arr[i] == arr[idx] || abs(i - idx) == abs((int)(arr[i] - arr[idx]))) {
            return false;
        }
    }
    return true;
}



/*************************** solver.h functions ************************/


void seq_solver(unsigned int n, unsigned int k, unsigned int exit_on_first, std::vector<std::vector<int> >& solns) {

    // TODO: Implement this function
    std::vector <int> arr (n, -1);
    int start_value = 0;
    bool flag       = false;
    int i           = 0;
    while(i < (int)k && i >= 0) {
        flag = false;
        while(start_value < (int)n) {
            arr[i] = start_value;
            if(is_valid(arr)) {
                start_value = 0;
                flag        = true;
                if (i == (int)k - 1) {
                    // a partial solution of size k is created here
                    // send it to an idle worker
                    solns.push_back(arr);
                    if (exit_on_first) {
                        return;
                    }
                    flag = false;
                }
                break;
            } else {
                start_value ++;
            }
        }
        if (flag) {
            i ++;
            continue;
        }
        arr[i] = -1;
        i      = i - 1;
        start_value = arr[i] + 1;
    }

}


void solve_nqueens( std::vector <int> arr,
                    std::vector<std::vector<int> >& solns,
                    bool exit_on_first) {
    // find the occurance of -1
    int idx = arr.size();
    for (int i = 0; i < (int)arr.size(); i ++) {
        if (arr[i] == -1) {
            idx = i;
            break;
        }
    }
    if (idx == (int)arr.size()) {
        // cannot solve further
        solns.push_back(arr);
        return;
    }

    // get all the solutions resulting from this partial solutions
    int start_value = 0;
    bool flag       = false;
    int i           = idx;
    int n           = (int)arr.size();
    int k           = n;

    while(i < (int)k && i >= 0) {
        flag = false;
        while(start_value < (int)n) {
            arr[i] = start_value;
            if(is_valid(arr)) {
                start_value = 0;
                flag        = true;
                if (i == (int)k - 1) {
                    // a partial solution of size k is created here
                    // send it to an idle worker
                    solns.push_back(arr);
                    if (exit_on_first) {
                        return;
                    }
                    flag = false;
                }
                break;
            } else {
                start_value ++;
            }
        }
        if (flag) {
            i ++;
            continue;
        }
        arr[i] = -1;
        i      = i - 1;
        start_value = arr[i] + 1;
    }

}




void nqueen_master( unsigned int n,
                    unsigned int k,
                    unsigned int exit_on_first,
                    std::vector<std::vector<int> >& solns) {

    // Master makes an initial list of partial solutions
    std::vector <std::vector <int> > partial_solns;
    seq_solver(n, k, exit_on_first, partial_solns);

    for (int i = 0; i < (int)partial_solns.size(); i ++) {
        solns.push_back(partial_solns[i]);
    }
    return; 

    // If we have to find one solution
    if (exit_on_first) {
        while (!partial_solns.empty())
        {
            // send partial solutions
            // try to receive solution
                // break when one solution is received
        }
        // terminate all processes 
        return;

    }
    // if we need to find all solutions 
    else {
        while(!partial_solns.empty()) {
            // send partial solutions to any processor 
            // try to receive solutions from processors who are finished
                // append to solns
        }
    }



    // TODO: Implement this function

    /* Following is a general high level layout that you can follow
     (you are not obligated to design your solution in this manner.
      This is provided just for your ease). */


    /******************* STEP 1: Send one partial solution to each worker ********************/
    /*
     * for (all workers) {
     *      - create a partial solution.
     *      - send that partial solution to a worker
     * }
     */


    /******************* STEP 2: Send partial solutions to workers as they respond ********************/
    /*
     * while() {
     *      - receive completed work from a worker processor.
     *      - create a partial solution
     *      - send that partial solution to the worker that responded
     *      - Break when no more partial solutions exist and all workers have responded with jobs handed to them, or if exiting on first solution
     * }
     */

    /********************** STEP 3: Terminate **************************
     *
     * Send a termination/kill signal to all workers.
     *
     */





}

void nqueen_worker( unsigned int n,
                    unsigned int k,
                    unsigned int exit_on_first) {



    // TODO: Implement this function

    // Following is a general high level layout that you can follow (you are not obligated to design your solution in this manner. This is provided just for your ease).

    /*******************************************************************
     *
     * while() {
     *
     *      wait for a message from master
     *
     *      if (message is a partial job) {
     *              - finish the partial solution
     *              - send all found solutions to master
     *      }
     *
     *      if (message is a kill signal) {
     *
     *              quit
     *
     *      }
     *  }
     */


}



/*************************** DEFINE YOUR HELPER FUNCTIONS HERE ************************/







