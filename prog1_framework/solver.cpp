#include "solver.h"


/*************************** DECLARE YOUR HELPER FUNCTIONS HERE ************************/
void print_2D_vector(std::vector <std::vector <unsigned int>> solns) {
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


bool is_valid(std::vector <unsigned int> arr, int idx) {
    /*
        Checks if the input vector is a valid assignment
    */
    // Find the first occurance of -1 in arr
    // int idx = arr.size();
    // for(int i = 0; i < (int)arr.size(); i ++) {
    //     if(arr[i] == -1) {
    //         idx = i;
    //         break;
    //     }
    // }

    // if (idx == 0) {
    //     return true;
    // } else {
    //     idx --;     // represents the index of last assignment
    // }

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


void seq_solver(unsigned int n, unsigned int k, unsigned int exit_on_first, std::vector<std::vector<unsigned int> >& solns) {

    // TODO: Implement this function
    std::vector <unsigned int> arr (n, n);
    int start_value = 0;
    bool flag       = false;
    int i           = 0;
    while(i < (int)k && i >= 0) {
        flag = false;
        while(start_value < (int)n) {
            arr[i] = start_value;
            if(is_valid(arr, i)) {
                start_value = 0;
                flag        = true;
                if (i == (int)k - 1) {
                    // a partial solution of size k is created here
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

void solve_nqueens( std::vector <unsigned int> arr,
                    std::vector<std::vector<unsigned int> >& solns,
                    bool exit_on_first, int idx) {
    /*
        idx -> solved till this index
    */


    // find the occurance of -1
    // int idx = arr.size();
    // for (int i = 0; i < (int)arr.size(); i ++) {
    //     if (arr[i] == -1) {
    //         idx = i;
    //         break;
    //     }
    // }
    // if (idx == (int)arr.size()) {
    //     // cannot solve further
    //     solns.push_back(arr);
    //     return;
    // }

    // get all the solutions resulting from this partial solutions
    int start_value = 0;
    bool flag       = false;
    int i           = idx;
    int n           = (int)arr.size();

    while(i < (int)n && i >= 0) {
        flag = false;
        while(start_value < (int)n) {
            arr[i] = start_value;
            if(is_valid(arr, i)) {
                start_value = 0;
                flag        = true;
                if (i == (int)n - 1) {
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
                    std::vector<std::vector<unsigned int> >& solns) {
    

    // Master makes an initial list of partial solutions
    std::vector <std::vector <unsigned int> > partial_solns;
    seq_solver(n, k, exit_on_first, partial_solns);

    // If we have to find one solution
    if (exit_on_first) {
        while (!partial_solns.empty())
        {
            // send partial solutions
            MPI_Send(&partial_solns[0][0], n, MPI_UNSIGNED, 1, 4001, MPI_COMM_WORLD);
            break;
            // try to receive solution
                // break when one solution is received
        }
        // terminate all processes 
        return;

    }
    // if we need to find all solutions 
    else {
        while(!partial_solns.empty()) {
            MPI_Send(&partial_solns[0][0], n, MPI_UNSIGNED, 1, 4001, MPI_COMM_WORLD);
            break;
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

    std::vector < std::vector<unsigned int> > completed_sols; 

    std::vector <unsigned int> partial_sol{};
    partial_sol.resize(n);

    MPI_Status stat;
    while(true) {
        // Recieve a partial solution from master (blocking)
        // which has been solved till depth k
        MPI_Recv(&partial_sol[0], n, MPI_UNSIGNED, 0, 4001, MPI_COMM_WORLD, &stat);

        // Get a vector containing all solutions
        solve_nqueens(partial_sol, completed_sols, exit_on_first, k); 

        // Send the solutions back to the master

        print_2D_vector(completed_sols);


        break;

        /// wait for a message from master

     
        // if (message is a kill signal) {
        
                
        // }
    }


}



/*************************** DEFINE YOUR HELPER FUNCTIONS HERE ************************/







