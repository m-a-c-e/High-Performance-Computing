#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <vector>


void print_2D_vector(std::vector <std::vector <int>> solns) {
    for(int i = 0; i < solns.size(); i ++) {
        printf("{");
        for (int j = 0; j < solns[i].size(); j ++) {
            printf("%d ", solns[i][j]);
        }
        printf("}\n");
    }
}


bool is_valid(std::vector <int> arr) {
    // Find the first occurance of -1 in arr
    int idx = arr.size();
    for(int i = 0; i < arr.size(); i ++) {
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


void nqueen_master( unsigned int n,
                    unsigned int k,
                    unsigned int exit_on_first,
                    std::vector<std::vector<int> >* solns) {
    
    std::vector <int> arr (n, -1);
    int start_value = 0;
    bool flag       = false;
    int i           = 0;
    while(i < k && i >= 0) {
        flag = false;
        while(start_value < n) {
            arr[i] = start_value;
            if(is_valid(arr)) {
                start_value = 0;
                flag        = true;
                if (i == k - 1) {
                    // a partial solution of size k is created here
                    // send it to an idle worker
                    solns->push_back(arr);
                    printf("entered\n");
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


int main(int argc, char* argv[]) {
    std::vector <unsigned int> a{1, 2, 3};
    std::vector <unsigned int> b = {};

    while (!a.empty())
    {
        printf("%u\n", a[a.size() - 1]);
        b.push_back(a[a.size()-1]);
        printf("%u\n", b[b.size()-1]);
        a.pop_back();
    }

    while(!b.empty()) {
        printf("%u\n", b[b.size()-1]);
        b.pop_back();
    }
    

}

// int main(int argc, char *argv[]) {
//     // t up MPI
//     MPI_Init(&argc, &argv);
//     // get communicator size and my rank
//     MPI_Comm comm = MPI_COMM_WORLD;
//     int p, rank;
//     MPI_Comm_size(comm, &p);
//     MPI_Comm_rank(comm, &rank);
//     /* code */
//     int n = 8;
//     int k = 4;
//     std::vector<std::vector<unsigned int> > ans;

//     std::vector <int> a = {1, 3, 0, 2};
//     if (is_valid(a)) {
//         printf("works\n");
//     } else {
//         printf(":'(");
//     }

//     std::vector <std::vector <int> > solns;
//     nqueen_master(5, 4, true, &solns);
//     print_2D_vector(solns);

//     // finalize MPI
//     MPI_Finalize();

//     return 0;
// }