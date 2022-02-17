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
                    bool exit_on_first, int idx, int start_value) {
    /*
        idx -> solved till this index
    */

    // get all the solutions resulting from this partial solutions

    bool flag       = false;
    int i           = idx;
    int n           = (int)arr.size();

    while(i < (int)n && i >= idx) {
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

    printf("ultimate size = %lu\n", partial_solns.size());

//    MPI_Request req;
//    int flag;

    int num_procs;
    MPI_Status stat;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    std::vector <unsigned int> ans(n, n);

    // Send the partial solutions initially
    while(!partial_solns.empty()) {
        // check which processor is ready
        MPI_Send(&partial_solns[partial_solns.size() - 1][0], n, MPI_UNSIGNED, 1, SEND_TAG, MPI_COMM_WORLD);
        partial_solns.pop_back();

        // check if you can receive anything
        MPI_Recv(&ans[0], n, MPI_UNSIGNED, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);

        // depnding on the tag, send
        if (stat.MPI_TAG == READY) {
            printf("get all the answers send a new partial solution to this %d\n", stat.MPI_SOURCE);
        }
        else {
            printf("only send a partial solution\n");
        } 

    }

    // terminate all processes
    std::vector <unsigned int> dummy(n, n);
    for(int i = 1; i < num_procs; i ++) {
        MPI_Send(&dummy[0], n, MPI_UNSIGNED, i, TERMINATE, MPI_COMM_WORLD);
    }

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


    //MPI_Request req;
    MPI_Status stat;

    std::vector <unsigned int> partial_sol;
    std::vector <unsigned int> dummy(n, n);
    partial_sol.resize(n);
    while(true) {
        std::vector <std::vector <unsigned int> > completed_solns;
        MPI_Recv(&partial_sol[0], n, MPI_UNSIGNED, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
        if (stat.MPI_TAG == SEND_TAG) {
            solve_nqueens(partial_sol, completed_solns, exit_on_first, k, 0);
            if (!completed_solns.empty())
            {
                MPI_Send(&completed_solns[0][0], n, MPI_UNSIGNED, 0, READY, MPI_COMM_WORLD);
                print_2D_vector(completed_solns);
            } 
            else
            {
                MPI_Send(&dummy[0], n, MPI_UNSIGNED, 0, 444, MPI_COMM_WORLD);
            }
            

        } else {
            printf("terminating\n");
            break;
        }

     
    }
}



/*************************** DEFINE YOUR HELPER FUNCTIONS HERE ************************/