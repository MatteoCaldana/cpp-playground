#include<vector>
#include<iostream>
#include<string>

void readMatlabMatrix(const std::string &filename){
  std::string temp;
  std::vector<int> rows, cols;
  std::vector<double> vals;
  //open file
  std::ifstream in(filename.c_str(), std::ios::in);
  if( !in ) { std::cerr << "Can't open the file: "<< filename <<std::endl; exit(1);}
  // support variables
  int i, j, nnz; double x;  
  // read file
  in >> i >> j >> nnz;
  rows.resize(nnz); cols.resize(nnz); vals.resize(nnz);
  nnz = 0;
  while(in >> i >> j >> x) {
    rows[nnz] = i - 1;
    cols[nnz] = j - 1;
    vals[nnz] = x;
    ++nnz;
  }
  in.close();

  return;
}