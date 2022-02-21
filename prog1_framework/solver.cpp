#include "solver.h"



/*************************** DECLARE YOUR HELPER FUNCTIONS HERE ************************/

void print_1D_vector(std::vector <unsigned int> vec) {

    /*

        Prints a 1D vector

    */

    for(int i = 0; i < (int)vec.size(); i ++) {

        printf("%d ", vec[i]);

    }

    printf("\n");

}



void print_2D_vector(std::vector <std::vector <unsigned int>> vec2D) {

    /*

        Prints all the vectors inside the passed in vector

    */

    for(int i = 0; i < (int) vec2D.size(); i ++) {

        for (int j = 0; j < (int) vec2D[i].size(); j ++) {

            printf("%d ", vec2D[i][j]);

        }

        printf("\n");

    }

}



bool is_valid(std::vector <unsigned int> arr, int idx) {

    /*

        Description:

            Checks the last assginment and returns true or false

        Args:

            arr - contains the assignment to n queens where n is the size of the vector

            idx - contains the last assignemnt idx

        

        Returns:

            true - if last assignment valid

            false - if not valid

    */

    for(int i = 0; i < idx; i ++) {

        if (arr[i] == arr[idx] || abs(i - idx) == abs((int)(arr[i] - arr[idx]))) {

            return false;

        }

    }

    return true;

}



/*************************** solver.h functions ************************/

void seq_solver(unsigned int n, 

                unsigned int exit_on_first, 

                std::vector<std::vector<unsigned int> >& solns) {

    /*

        Description:

            Solves the nqueens problem of size n

        Args:

            n            - size of board

            exit_onfirst - if true:  return after getting one solution

                              false: return all solutions

            solns        - Stores the solutions

        

        Returns:

            N/A

    */

    std::vector <unsigned int> arr (n, n);

    int start_value = 0;

    bool flag       = false;

    int i           = 0;



    // for all indices

    while(i < (int)n && i >= 0) {       


        // sets the backtrack condition

        flag = false;


        // for all values of assignment

        while(start_value < (int)n) {


            // make an assignment at i

            arr[i] = start_value;


            // check if assignment is valid

            if(is_valid(arr, i)) {


                // if last index is reached

                if (i == (int) n - 1) {


                    // append the solution

                    solns.push_back(arr);

                    start_value ++;


                    // if only one solution is required

                    if (exit_on_first) {

                        return;

                    }


                } else {

                    // break out

                    start_value = 0;

                    flag        = true;

                    break;

                }


            } else {

                start_value ++;

            }


        }

        if (flag) {

            // move on to the next index

            i ++;


        } else {

            // backtrack

            i      = i - 1;

            start_value = arr[i] + 1;

        }

    }

}



void partial_seq_solver(unsigned int n, 

                        unsigned int k, 

                        unsigned int exit_on_first, 

                        std::vector<std::vector<unsigned int> >& solns) {

    /*

        Description:

            Solves the nqueens problem upto a depth k

        Args:

            n            - size of board

            k            - depth to which to solve

            exit_onfirst - if true:  return after getting one solution

                              false: return all solutions

            solns        - Stores the solutions

        

        Returns:

            N/A

    */

    std::vector <unsigned int> arr (n, n);

    int start_value = 0;

    bool flag       = false;

    int i           = 0;

    if (k == 0) {
        solns.push_back(arr);
        return;
    }


    // for all indices less than depth

    while(i < (int)k && i >= 0) {


        // set the backtrack condition

        flag = false;


        // for all possible assignments

        while(start_value < (int)n) {


            // make an assignment

            arr[i] = start_value;


            // if assignment is valid

            if(is_valid(arr, i)) {


                // if it is a solution

                if (i == (int)k - 1) {


                    // a solutions is found here

                    solns.push_back(arr);

                    start_value ++;

                    

                    // return if only one solution is required

                    if (exit_on_first) {

                        return;

                    }


                } else {

                    // move to next index

                    start_value = 0;

                    flag = true;

                    break;

                }


            } else {

                start_value ++;

            }

        }

        if (flag) {

            // move to next index

            i ++;

        } else {

            // no valid assignment available for current index

            // backtrack

            i      = i - 1;

            start_value = arr[i] + 1;

        }

    }

}


void solve_nqueens( std::vector <unsigned int> arr,

                    std::vector<std::vector<unsigned int> >& solns,

                    bool exit_on_first, int idx) {

   /*

        Description:

            Solves the nqueens problem starting from index idx

        Args:

            arr          - partial solution

            solns        - Stores the solutions

            exit_on_first - if true:  return after getting one solution

                              false: return all solutions

            idx          - start solving from this index

        

        Returns:

            N/A

    */

    bool flag       = false;

    int i           = idx;

    int n           = (int) arr.size();

    int start_value = 0;

    if (idx == n) {
        solns.push_back(arr);
        return;
    }


    while(i < (int)n && i >= idx) {


        // sets the backtrack condition

        flag = false;


        // for all possible assignments

        while(start_value < (int)n) {


            // make an assignment

            arr[i] = start_value;


            // if the assignment is valid

            if(is_valid(arr, i)) {


                // if it is the last assignment to be made

                if (i == (int)n - 1) {


                    // append to solutions

                    solns.push_back(arr);

                    start_value ++;

                    if (exit_on_first) {

                        return;

                    }


                } else {

                    // move on to the next index by break

                    start_value = 0;

                    flag = true;

                    break;

                }


            } else {

                start_value ++;

            }

        }

        if (flag) {

            // move to the next index

            i ++;

        } else {

            // backtrac

            i      = i - 1;

            start_value = arr[i] + 1;

        }

    }

}


void nqueen_master( unsigned int n,

                    unsigned int k,

                    unsigned int exit_on_first,

                    std::vector<std::vector<unsigned int> >& solns) {

    /*

        Description:

        -   Creates partial solutions to n queens problem till depth

            k and distributes them to workers

        -   Collects solutions from the workers

        -   Terminates all processes depending on one solution is required

            or all the solutions are required

        -   Function to be run by the master thread in MPI. Here,

            the rank of the process will be 0.

        Args:

            n            - size of board

            k            - depth to which to solve

            exit_onfirst - if true:  return after getting one solution

                              false: return all solutions

            solns        - Stores the solutions

        Returns:

            N/A

    */


    // generate a list of all partial solutions

    std::vector <std::vector <unsigned int> > partial_solns;

    partial_seq_solver(n, k, 0, partial_solns);

    // get the MPI parameters

    int num_procs;

    MPI_Status stat;

    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);


    std::vector <unsigned int> dummy(n, n);


    // for all processes

    for(int i = 1; i < num_procs; i ++) {

        if (partial_solns.empty()) {

            break;

        }

        // send a partial solution to the worker at i

        MPI_Send(&partial_solns[partial_solns.size() - 1][0], n, MPI_UNSIGNED, i, SEND_TAG, MPI_COMM_WORLD);

        partial_solns.pop_back();

    }


    // initialization step

    int new_counter = 0;

    unsigned int sol_size;

    std::vector <unsigned int> ans;

    ans.resize(n);


    // while all the workers have not returned

    while(new_counter != (num_procs - 1)) {


        // first receive size of solution from any source

        MPI_Recv(&sol_size, 1, MPI_UNSIGNED, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);


        if (stat.MPI_TAG == NO_SOLUTION) {

            // recieve the dummy solution from the worker which sent the above size (will not be solved)

            MPI_Recv(&ans[0], n, MPI_UNSIGNED, stat.MPI_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);


            // give it a partial solution to work on if any left in partial_solns

            if (partial_solns.empty()) {

                // no more work left

                new_counter ++;

            } else {

                // provide a partial solution to this worker

                MPI_Send(&partial_solns[partial_solns.size() -1][0], n, MPI_UNSIGNED, stat.MPI_SOURCE, SEND_TAG, MPI_COMM_WORLD);

                partial_solns.pop_back();

            }

        } else {

            // iteratively get the solutions from the source from which size was acquired

            for(int i = 0; i < (int) sol_size; i ++) {

                MPI_Recv(&ans[0], n, MPI_UNSIGNED, stat.MPI_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);

                solns.push_back(ans);

            }


            if (partial_solns.empty()) {

                // no more work left

                new_counter ++;

            } else {

                // send a partial solution

                MPI_Send(&partial_solns[partial_solns.size() -1][0], n, MPI_UNSIGNED, stat.MPI_SOURCE, SEND_TAG, MPI_COMM_WORLD);

                partial_solns.pop_back();

            }

        }

    }

    if (exit_on_first) {
        // if only one solution is require
        solns.resize(1);
    }

    // send terminate signal via the tag terminate
    for(int i = 0; i < num_procs; i ++) {

        MPI_Send(&dummy[0], n, MPI_UNSIGNED, i, TERMINATE, MPI_COMM_WORLD);

    }

}

   


void nqueen_worker( unsigned int n,

                    unsigned int k,

                    unsigned int exit_on_first) {

    /*

        Description:

        -   Recives partial solution from master

        -   Gets all the solutions which might results from the partial solution

        -   Sends back the vector of completed solutions

        Args:

            n            - size of board

            k            - depth from which to start solving

            exit_onfirst - if true:  return after getting one solution

                              false: return all solutions

        Returns:

            N/A

    */



    // initialisation

    int rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

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

                for(int i = 0; i < (int) sol_size; i ++) {

                    MPI_Send(&completed_solns[i][0], n, MPI_UNSIGNED, 0, YES_SOLUTION, MPI_COMM_WORLD);

                }


            } else {

                // send 1 as the size of the solution

                MPI_Send(&dummy_size, 1, MPI_UNSIGNED, 0, NO_SOLUTION, MPI_COMM_WORLD);


                // send the dummy solution

                MPI_Send(&dummy[0], n, MPI_UNSIGNED, 0, NO_SOLUTION, MPI_COMM_WORLD);

            }

        } else {

            // terminate if any tag other than SEND_TAG is recieved

            break;

        }

    }

}
