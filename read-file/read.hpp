#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
#include <cassert>
#include <string>
#include <cstdio>
#include <cerrno>
#include <fstream>
#include <streambuf>
#include <algorithm>
#include <iterator>
#include <sstream>

// http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html

void get_file_c(const char *filename, std::string &contents)
{
  std::FILE *fp = std::fopen(filename, "rb");
  if (fp)
  {
  std::fseek(fp, 0, SEEK_END);
  contents.resize(std::ftell(fp));
  std::rewind(fp);
  std::fread(&contents[0], 1, contents.size(), fp);
  std::fclose(fp);
  return;
  }
  throw(errno);
}

void get_file_cpp(const char *filename, std::string &contents)
{
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
  in.seekg(0, std::ios::end);
  contents.resize(in.tellg());
  in.seekg(0, std::ios::beg);
  in.read(&contents[0], contents.size());
  in.close();
  return;
  }
  throw(errno);
}

void get_file_assign(const char *filename, std::string &contents)
{
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
  in.seekg(0, std::ios::end);
  contents.reserve(in.tellg());
  in.seekg(0, std::ios::beg);
  contents.assign((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
  in.close();
  return;
  }
  throw(errno);
}

void get_file_copy(const char *filename, std::string &contents)
{
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
  in.seekg(0, std::ios::end);
  contents.reserve(in.tellg());
  in.seekg(0, std::ios::beg);
  std::copy((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>(), std::back_inserter(contents));
  in.close();
  return;
  }
  throw(errno);
}

void get_file_rdbuf(const char *filename, std::string &contents)
{
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
  std::ostringstream sscontents;
  sscontents << in.rdbuf();
  in.close();
  contents.assign(sscontents.str());
  return;
  }
  throw(errno);
}