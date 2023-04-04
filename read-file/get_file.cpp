#define HAVE_MPI
#define FULL_TIME_CHECK

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
const std::string filename("e40r5000.mtx"); //e40r5000.mtx s3dkq4m2.mtx
std::cout << std::fixed << std::setprecision(6);

// 1st method

  // put everything in a string 
  timer.reset();
#ifdef FULL_TIME_CHECK
  timer_full.reset();
#endif

  std::string content;
  if( isMaster ) {
    //get_file_c     (filename.c_str(), content);
    get_file_cpp   (filename.c_str(), content);
    //get_file_assign(filename.c_str(), content);
    //get_file_copy  (filename.c_str(), content);
    //get_file_rdbuf (filename.c_str(), content);
  }

#ifdef FULL_TIME_CHECK
  CHECK_TIME("load");
#endif

  
  // parse content
#ifdef HAVE_MPI
  int recvcount;
  std::vector<int> displaces, sendcount;

  if( isMaster ) {
    const int count = (int) content.length();
    displaces.push_back( 0 ); 
    sendcount.push_back( count / size + ((0 < count % size) ? 1 : 0) );
    while( content[ sendcount.back() - 1 ] != '\n' ) sendcount.back()++;
    for(int i = 1; i < size - 1; ++i) {
      displaces.push_back( displaces[ i - 1 ] + sendcount[ i - 1 ] );
      sendcount.push_back( count / size + ((i < count % size) ? 1 : 0) );
      while( content[ sendcount.back() + displaces.back() - 1 ] != '\n' ) {
        sendcount.back()++;
      } 
    }
    displaces.push_back( displaces.back() + sendcount.back() );
    sendcount.push_back( count - displaces.back() );
    
    for( int i = 1; i < size; ++i )
      MPI_Send( &sendcount[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
  } else {
    MPI_Recv( &recvcount, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    content.resize( recvcount );
  }

#ifdef FULL_TIME_CHECK
  CHECK_TIME("split");
#endif

  MPI_Scatterv( content.c_str(), sendcount.data(), displaces.data(),
                MPI_CHAR, &content[0], recvcount,
                MPI_CHAR, 0, MPI_COMM_WORLD);

  if( isMaster ) content.resize( sendcount[0] );

#ifdef FULL_TIME_CHECK
  CHECK_TIME("scatter");
#endif

#endif

  // trim the string
  int useless_char_end_length = 0;
  while( (int) content[content.length() - useless_char_end_length] < 44 )
    ++useless_char_end_length;
  content.resize(content.length() - useless_char_end_length);

  // parse the string
  char* begin = &content[0];
  char* temp = nullptr;
  const char* end = &content[content.length() - 1];
  std::cout << rank << " " << content.length() << std::endl;
  std::vector<int> rows, cols;
  std::vector<double> vals;
  while( (uint64_t) temp < (uint64_t) end ){
    rows.push_back(strtol( begin, &temp, 10 )); begin = temp;
    cols.push_back(strtol( begin, &temp, 10 )); begin = temp;
    vals.push_back(strtod( begin, &temp ));     begin = temp;
  }

#ifdef FULL_TIME_CHECK
  CHECK_TIME("parse");
#endif

#ifdef HAVE_MPI
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
    rows.resize(global_size); cols.resize(global_size); vals.resize(global_size);
  }

#ifdef FULL_TIME_CHECK
  CHECK_TIME("alloc");
#endif

  if( isMaster ) {
  MPI_Gatherv(MPI_IN_PLACE, (int) rows.size(), MPI_INT,
              rows.data(), sendcount.data(), displaces.data(),
              MPI_INT, 0, MPI_COMM_WORLD);

  MPI_Gatherv(MPI_IN_PLACE, (int) cols.size(), MPI_INT,
              cols.data(), sendcount.data(), displaces.data(),
              MPI_INT, 0, MPI_COMM_WORLD);

  MPI_Gatherv(MPI_IN_PLACE, (int) vals.size(), MPI_DOUBLE,
              vals.data(), sendcount.data(), displaces.data(),
              MPI_DOUBLE, 0, MPI_COMM_WORLD);
  } else {
  MPI_Gatherv(rows.data(), (int) rows.size(), MPI_INT,
              rows.data(), sendcount.data(), displaces.data(),
              MPI_INT, 0, MPI_COMM_WORLD);

  MPI_Gatherv(cols.data(), (int) cols.size(), MPI_INT,
              cols.data(), sendcount.data(), displaces.data(),
              MPI_INT, 0, MPI_COMM_WORLD);

  MPI_Gatherv(vals.data(), (int) vals.size(), MPI_DOUBLE,
              vals.data(), sendcount.data(), displaces.data(),
              MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

#ifdef FULL_TIME_CHECK
  CHECK_TIME("gather");
#endif
  
#endif

  if( isMaster ) std::cout<<"fast\ttime:"<<timer.split()<<std::endl;

// 2nd method
  // if( isMaster ) {
  //   timer.reset();
  //   readMatlabMatrix(filename);
  //   std::cout<<"naive\ttime:"<<timer.split()<<std::endl;
  // }

#ifdef HAVE_MPI
  MPI_Finalize();
#endif

  return 0;
}