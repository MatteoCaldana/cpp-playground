#define HAVE_MPI
#define FULL_TIME_CHECK
#define MPI_GATHER

#include "read.hpp"
#include "timer.hpp"
#include "read_matrix.hpp"
#include "time_macro.hpp"

#include <string>
#include <sstream>
#include <iomanip>
#include <stdlib.h>
#include <mpi.h>


template<typename T>
void printvect(const std::vector<T> &v){
    for(const auto i : v)
        std::cout << i << " ";
    std::cout<<std::endl;
}

int main(int argc, char** argv) {

// MPI handle stuff
    bool isMaster(true);
    int rank(0), size(1);

#ifdef HAVE_MPI
    MPI_Init (&argc, &argv);
    MPI_Comm_rank( MPI_COMM_WORLD, &rank ); 
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    if ( rank != 0 )
        isMaster = false;
#endif

// Initialize timers
    Chrono timer; // other types: CPUTimer, WallClockTimer
    const double rescale = 1000000.0; // rescale to seconds factor
#ifdef FULL_TIME_CHECK
    Chrono timer_full;
    uint64_t time, time_dead;
    std::vector<uint64_t> time_v(size), time_dead_v(size);
#endif

// Set variables
const std::string filename("s3dkq4m2.mtx"); //e40r5000.mtx s3dkq4m2.mtx
std::cout << std::fixed << std::setprecision(6);

// 1st method 
    timer.reset();
#ifdef FULL_TIME_CHECK
    timer_full.reset();
#endif

    // put everything in a string
    std::string content;

#ifdef HAVE_MPI
    uint64_t length, fbegin, fend;
    std::ifstream in(filename.c_str(), std::ios::in | std::ios::binary);
    if (!in) { return -1; }

    if( isMaster ) {
        in.seekg(0, std::ios::end);
        length = in.tellg();
        fbegin = 0;
        fend = length / size;

        char c; in.seekg(fend - 1); in.get(c);
        while( c != '\n' ) { fend++; in.seekg(fend - 1); in.get(c);}

        MPI_Send( &fend, 1, MPI_UNSIGNED_LONG_LONG, rank+1, 0, MPI_COMM_WORLD);
    }

    MPI_Bcast(&length, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);

    for(int i = 1; i < size - 1; ++i) {
        if( rank == i ) {
            MPI_Recv( &fbegin, 1, MPI_UNSIGNED_LONG_LONG, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            fend = (length / size) * (rank + 1);

            char c; in.seekg(fend - 1); in.get(c);
            while( c != '\n' ) { fend++; in.seekg(fend - 1); in.get(c);}
            MPI_Send( &fend, 1, MPI_UNSIGNED_LONG_LONG, rank+1, 0, MPI_COMM_WORLD);
        }
    }
    
    if( rank == size - 1 ) {
        fend = length;
        MPI_Recv( &fbegin, 1, MPI_UNSIGNED_LONG_LONG, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    content.resize(fend - fbegin);
    in.seekg(fbegin);
    in.read(&content[0], content.size());
    in.close();
#else
    //get_file_c     (filename.c_str(), content);
    get_file_cpp   (filename.c_str(), content);
    //get_file_assign(filename.c_str(), content);
    //get_file_copy  (filename.c_str(), content);
    //get_file_rdbuf (filename.c_str(), content);
#endif

#ifdef FULL_TIME_CHECK
    CHECK_TIME("load");
#endif

    // parse the string
    char* begin = &content[0];
    char* temp = nullptr;
    //   we are assuming:
    //    1) last number in file is a floating point of length greater than 6
    //    2) there aren't more than 5 whitespaces at the end of the file, otherwise the loop doesn't brake
    const char* end = &content[content.length() - 6];
    std::vector<int> rows, cols;
    std::vector<double> vals;
    while( (uint64_t) temp < (uint64_t) end ) {
        rows.push_back(strtol( begin, &temp, 10 )); begin = temp;
        cols.push_back(strtol( begin, &temp, 10 )); begin = temp;
        vals.push_back(strtod( begin, &temp ));     begin = temp;
    }

#ifdef FULL_TIME_CHECK
    CHECK_TIME("parse");
#endif

#ifdef HAVE_MPI 
#ifdef MPI_GATHER
    std::vector<int> sendcount(size,0), displaces(size,0);
    if( isMaster ) {
        sendcount[0] = (int) rows.size();
        for( int i = 1; i < size; ++i)
            MPI_Recv( &sendcount[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        const int size = (int) rows.size();
        MPI_Send( &size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    std::vector<int> nrows, ncols;
    std::vector<double> nvals;
    if( isMaster ) {
        for(int i = 1; i < size; ++i)
            displaces[i] = sendcount[i - 1] + displaces[i - 1];
        const int global_size = displaces.back() + sendcount.back();
        nrows.resize(global_size); ncols.resize(global_size); nvals.resize(global_size);
    }

#ifdef FULL_TIME_CHECK
    CHECK_TIME("alloc");
#endif

    MPI_Gatherv(rows.data(), (int) rows.size(), MPI_INT,
                            nrows.data(), sendcount.data(), displaces.data(),
                            MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Gatherv(cols.data(), (int) cols.size(), MPI_INT,
                            ncols.data(), sendcount.data(), displaces.data(),
                            MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Gatherv(vals.data(), (int) vals.size(), MPI_DOUBLE,
                            nvals.data(), sendcount.data(), displaces.data(),
                            MPI_DOUBLE, 0, MPI_COMM_WORLD);

#ifdef FULL_TIME_CHECK
    CHECK_TIME("gather");
#endif
#endif   
#endif

    MPI_Barrier(MPI_COMM_WORLD);
    if( isMaster ) std::cout<<"fast\ttime: "<<((double)timer.split())/rescale<<std::endl;


// 2nd method
    if( isMaster ) {
        timer.reset();
        readMatlabMatrix(filename);
        std::cout<<"naive\ttime: "<<((double)timer.split())/rescale<<std::endl;
    }

#ifdef HAVE_MPI
    MPI_Finalize();
#endif

    return 0;
}