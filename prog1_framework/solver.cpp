#include "solver.h"


/*************************** DECLARE YOUR HELPER FUNCTIONS HERE ************************/
void print_1D_vector(std::vector <unsigned int> vec) {
    for(int i = 0; i < (int)vec.size(); i ++) {
        printf("%d ", vec[i]);
    }
    printf("\n");
}

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
                if (i == (int)k - 1) {
                    // a partial solution of size k is created here
                    solns.push_back(arr);
                    start_value ++;
                    continue;
                    if (exit_on_first) {
                        return;
                    }
                }
                start_value = 0;
                flag = true;
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
        idx -> start from this index
    */

    // get all the solutions resulting from this partial solutions

    bool flag       = false;
    int i           = idx;
    int n           = (int)arr.size();
    int start_value = 0;

    while(i < (int)n && i >= idx) {
        flag = false;
        while(start_value < (int)n) {
            arr[i] = start_value;
            if(is_valid(arr, i)) {
                if (i == (int)n - 1) {
                    // a partial solution of size k is created here
                    // send it to an idle worker
                    solns.push_back(arr);
                    start_value ++;
                    continue;
                    if (exit_on_first) {
                        return;
                    }
                }
                start_value = 0;
                flag = true;
                break;
            } else {
                start_value ++;
            }
        }
        if (flag) {
            i ++;
            continue;
        }
        // arr[i] = -1;
        arr[i] = 0;
        i      = i - 1;
        start_value = arr[i] + 1;
    }

}

void nqueen_master( unsigned int n,
                    unsigned int k,
                    unsigned int exit_on_first,
                    std::vector<std::vector<unsigned int> >& solns) {

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
    

    // generate a list of all partial solutions
    std::vector <std::vector <unsigned int> > partial_solns;

    seq_solver(n, k, 0, partial_solns);

    int num_procs;
    MPI_Status stat;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    std::vector <unsigned int> dummy(n, n);

    for(int i = 1; i < num_procs; i ++) {
        if (partial_solns.empty()) {
            break;
        }
        MPI_Send(&partial_solns[partial_solns.size() - 1][0], n, MPI_UNSIGNED, i, SEND_TAG, MPI_COMM_WORLD);
        partial_solns.pop_back();
    }


    // receive from the processor which is done and send to it
    // expect to recieve at least something from counter number of processors
    int new_counter = 0;
    unsigned int sol_size;
    std::vector <unsigned int> ans;
    ans.resize(n);
    while(new_counter != (num_procs - 1)) {

        // first receive size from any source
        MPI_Recv(&sol_size, 1, MPI_UNSIGNED, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);

        if (stat.MPI_TAG == NO_SOLUTION) {
            // get the dummy solution
            MPI_Recv(&ans[0], n, MPI_UNSIGNED, stat.MPI_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);

            // give it a partial solution to work on 
            if (partial_solns.empty()) {
                new_counter ++;
            } else {
                MPI_Send(&partial_solns[partial_solns.size() -1][0], n, MPI_UNSIGNED, stat.MPI_SOURCE, SEND_TAG, MPI_COMM_WORLD);
                partial_solns.pop_back();
            }
        } else {

            // iteratively get the solutions from the source from which size was acquired
            for(int i = 0; i < sol_size; i ++) {
                MPI_Recv(&ans[0], n, MPI_UNSIGNED, stat.MPI_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
                solns.push_back(ans);
            }

            if (partial_solns.empty()) {
                new_counter ++;
            } else {
                MPI_Send(&partial_solns[partial_solns.size() -1][0], n, MPI_UNSIGNED, stat.MPI_SOURCE, SEND_TAG, MPI_COMM_WORLD);
                partial_solns.pop_back();
            }
        }
    }
 
    for(int i = 0; i < num_procs; i ++) {
        MPI_Send(&dummy[0], n, MPI_UNSIGNED, i, TERMINATE, MPI_COMM_WORLD);
    }

    printf("%d\n", solns.size());
    print_2D_vector(solns);



    

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

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //MPI_Request req;
    MPI_Status stat;
    std::vector <unsigned int> dummy(n, n);
    unsigned int dummy_size = dummy.size();

    unsigned int sol_size;

    while(true) {
        std::vector <std::vector <unsigned int> > completed_solns;
        std::vector <unsigned int> partial_sol;
        partial_sol.resize(n);

        // receive partial solution from master
        MPI_Recv(&partial_sol[0], n, MPI_UNSIGNED, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);

        if (stat.MPI_TAG == SEND_TAG) {

            // solve the partial solution
            solve_nqueens(partial_sol, completed_solns, exit_on_first, k);

            if (!completed_solns.empty()) {
                // if valid solutions found

                // send the size of the valid solutions
                sol_size = completed_solns.size();
                MPI_Send(&sol_size, 1, MPI_UNSIGNED, 0, YES_SOLUTION, MPI_COMM_WORLD);

                // iteratively send the individual solutions
                for(int i = 0; i < sol_size; i ++) {
                    MPI_Send(&completed_solns[i][0], n, MPI_UNSIGNED, 0, YES_SOLUTION, MPI_COMM_WORLD);
                }

            } else {
                // send 1 as the size of the solution
                MPI_Send(&dummy_size, 1, MPI_UNSIGNED, 0, NO_SOLUTION, MPI_COMM_WORLD);

                // send the dummy solution
                MPI_Send(&dummy[0], n, MPI_UNSIGNED, 0, NO_SOLUTION, MPI_COMM_WORLD);
            }
        } else {
            // printf("terminating\n");
            break;
        }

     
    }
}



/*************************** DEFINE YOUR HELPER FUNCTIONS HERE ************************/