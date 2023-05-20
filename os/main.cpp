//
//  main.cpp
//  multiThread_multiProcess
//
//  Created by 陳柏宇 on 2022/3/23.
//

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <unistd.h>
#include <thread>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <mutex>

using namespace std ;

# define more_less( basic, remain ) ( remain > 0 ) ? ( basic + 1 ) : basic
# define odd_even( num ) ( num % 2 == 0 ) ? num : ( num + 1 )

void Swap( int& a, int& b ) {
  
  int temp ;
  temp = a ;
  a = b ;
  b = temp ;
  
} // Swap()
/*
void Bubble_sort( vector<int>& list ) {
  
  for ( int i = 0 ; i < list.size() ; i ++ ) {
    
    for ( int j = i + 1 ; j < list.size() ; j ++ ) {
      
      if ( list.at( i ) > list.at( j ) ) {
        
        Swap( list.at( i ), list.at( j ) ) ;
        
      } // if
      
    } // for
    
  } // for
  
} // Bubble_sort()
*/
void Real_bubble( vector<int>& list ) {
  
  int len = (int)list.size() - 1 ;
  bool flag = false ;
  
  for ( int i = 0 ; i < len ; i ++ ) {
    
    flag = false ;
    
    for ( int j = 0 ; j < len - i ; j ++ ) {
      
      if ( list.at( j ) > list.at( j + 1 ) ) {
        
        Swap( list.at( j ), list.at( j + 1 ) ) ;
        flag = true ;
        
      } // if
      
    } // for
    
    if ( !flag ) break ;
    
  } // for
  
} // Real_bubble

void Merge( vector<int> left, vector<int> right, vector<int>& result ) {
  
  while( left.size() > 0 && right.size() > 0 ) {
    
    if ( left.at( 0 ) < right.at( 0 ) ) {
      
      result.push_back( left.at( 0 ) ) ;
      left.erase( left.begin() ) ;
      
    } // if
    else {
      
      result.push_back( right.at( 0 ) ) ;
      right.erase( right.begin() ) ;
      
    } // else
    
  } // while
  
  if ( left.size() > 0 ) {
    
    while ( left.size() > 0 ) {
      
      result.push_back( left.at( 0 ) ) ;
      left.erase( left.begin() ) ;
      
    } // for
    
  } // if
  
  else {
    
    while ( right.size() > 0 ) {
      
      result.push_back( right.at( 0 ) ) ;
      right.erase( right.begin() ) ;
      
    } // for
    
  } // else
  
} // Merge()

void Num_of_each_part( int num, int part, vector<int>& index ) {
  
  int remain = 0, basic = 0 ;
  
  basic = num / part ;
  remain = num % part ;
  
  for ( int i = 0 ; i < part ; i ++ ) {
    
    index.push_back( more_less( basic, remain ) ) ;
    remain -- ;
    
  } // for
  
} // Num_of_each_part()

class Mission1 {
  
public:
  void sort( vector<int> all_numbers, string name ) {
    
    double duration ;
    fstream output ;
    clock_t  Begin, End;
    
    Begin = clock() ;
    Real_bubble( all_numbers ) ;
    End = clock() ;
    duration = double(End - Begin) / CLOCKS_PER_SEC ;
    
    output.open( name + "_output1.txt" , ios::out ) ;
    
    output << "Sort : " << endl ;
    
    if ( !output.is_open() ) cout << "Failed to open file" << endl ;
    else {
      
      for ( int i = 0 ; i < all_numbers.size() ; i ++ )
        output << all_numbers.at(i) << endl ;
      
    } // else
    
    output << "CPU Time : " << duration << endl ;
    cout << duration << endl ;
    
    time_t now = time(0);
    tm *ltm = localtime(&now);
    output << "Output Time : " << 1900 + ltm->tm_year << "-" ;
    output << 1 + ltm->tm_mon << "-" ;
    output << ltm->tm_mday << " " << ltm->tm_hour << ":";
    output << ltm->tm_min << ":";
    output << ltm->tm_sec << "+08:00" ;
    
    output.close() ;
    
  } // sort()
  
} ;

class Mission2 {
  
private:
  
  vector< vector<int> > my_list ;
  
public:
  
  void Separate( vector<int> all_numbers, int part ) {
    
    string temp_str ;
    vector<int> temp, index ;
    
    Num_of_each_part( (int)all_numbers.size(), part, index ) ;
    
    for ( int i = 0 ; i < part ; i ++ ) {
    
      for ( int j = 0 ; j < index.at( i ) ; j ++ ) {
        
        temp.push_back( all_numbers.front() ) ;
        all_numbers.erase( all_numbers.begin() ) ;
        
      } // for
      
      my_list.push_back( temp ) ; // put numbers into vector ( vector in vector )
      temp.clear() ;
      
    } // for
    
    index.clear() ;
    
  } // Separate()
  
  void Bubble_part() {
    
    for ( int i = 0 ; i < (int)my_list.size() ; i ++ ) {
      
      Real_bubble( my_list.at( i ) ) ; // sort numbers
      
    } // for
    
  } // Bubble_part()
    
  void Merge_parts() {
    
    vector<int> result ;
    int times = ( int )my_list.size() - 1 ;
    
    for ( int i = 0 ; i < times ; i ++ ) {
      
      Merge( my_list.at(0), my_list.at(1), result ) ;
      my_list.erase( my_list.begin() ) ; // remove the first
      my_list.erase( my_list.begin() ) ; // remove the second
      my_list.push_back( result ) ; // add merge result to the end
      result.clear() ;
      
    } // while
    
  } // Merge()
    
  void Show( string name, double duration ) {
    
    fstream output ;
    
    output.open( name + "_output2.txt" , ios::out | ios::trunc ) ;
    
    output << "Sort : " << endl ;
    
    if ( !output.is_open() ) cout << "Failed to open file" << endl ;
    else {
      
      for ( int i = 0 ; i < my_list.front().size() ; i ++ )
        output << my_list.front().at(i) << endl ;
      
    } // else
    
    output << "CPU Time : " << duration << endl ;
    cout << duration << endl ;
    
    time_t now = time(0);
    tm *ltm = localtime(&now);
    output << "Output Time : " << 1900 + ltm->tm_year << "-" ;
    output << 1 + ltm->tm_mon << "-" ;
    output << ltm->tm_mday << " " << ltm->tm_hour << ":";
    output << ltm->tm_min << ":";
    output << ltm->tm_sec << "+08:00" ;
    
    output.close() ;
    
  } // Show()
  
  
} ; // Mission2

class Mission3 {
  
private:
  
  vector<int> position ;
  
public:
  
  void Separate( vector<int> all_numbers, int part, int *shm_array, string name ) {
    
    int pos = 0 ;
    string temp_str ;
    vector<int> temp, index ;
    clock_t Begin, End ;
    double duration = 0 ;
    
    Num_of_each_part( (int)all_numbers.size(), part, index ) ;
    
    for ( int i = 0 ; i < part ; i ++ ) { // mark the position
      
      position.push_back( pos ) ;
      pos = pos + ( int ) index.at( i ) - 1 ;
      position.push_back( pos ) ;
      pos ++ ;
      
    } // for
    
    for ( int i = 0 ; i < ( int )all_numbers.size() ; i ++ ) { // put every element into int pointer array
      
      shm_array[i] = all_numbers.at( i ) ;
      
    } // for
    
    Begin = clock() ;
    pid_t pid, pid2 ;
    
    for ( int i = 0 ; i < position.size() - 1 ; i += 2 ) {
      
      pid = fork() ;
      if ( pid < 0 ) {
        
        cout << "error1" << endl ;
        exit( -1 ) ;
        
      } // if
      else if ( pid == 0 ){
        
        Bubble_part( shm_array, position.at(i), position.at(i + 1) ) ;
        exit( 0 ) ;
        
      } // else
      else {
        
        waitpid( pid, NULL, 0 ) ;
        
      } // else
      
    } // for
    
    // MergeSort( shm_array, 0, ( int )all_numbers.size() - 1 ) ;
    
    for ( int i = 0 ; i < part - 1 ; i ++ ) {
      
      pid2 = fork() ;
      if ( pid2 < 0 ) {
        
        cout << "error2" << endl ;
        exit(-1) ;
        
      } // if
      else if ( pid2 == 0 ) {
        
        merge( shm_array, position.at(0), position.at(1), position.at(3) ) ;
        exit(0) ;
        
      } // else if
      else {
        
        waitpid( pid2, NULL, 0 ) ;
        position.erase( position.begin() + 1 ) ;
        position.erase( position.begin() + 1 ) ;
        
      } // else
      
    } // for
    
    End = clock() ;
    duration = double(End - Begin) / CLOCKS_PER_SEC ;
    
    Show( shm_array, ( int )all_numbers.size(), name, duration ) ;
    
  } // Separate()
  
  void Bubble_part( int *shm_array, int start, int end ) {
    
    bool flag = false ;
    int len = end - start  ;
    
    for ( int i = 0 ; i < len ; i ++ ) {
      
      flag = false ;
      
      for ( int j = start ; j <= end - 1 - i ; j ++ ) {
        
        if ( shm_array[j] > shm_array[j+1] ) {
          
          int temp = shm_array[j] ;
          shm_array[j] = shm_array[j+1] ;
          shm_array[j+1] = temp ;
          flag = true ;
          
        } // if
        
      } // for
      
      if ( !flag ) break ;
      
    } // for
    
  } // Real_bubble
  /*
  void Bubble_part( int *shm_array, int start, int end ) {
    
    for ( int i = start ; i <= end ; i ++ ) {
      
      for ( int j = i + 1 ; j <= end ; j ++ ) {
        
        if ( shm_array[i] > shm_array[j] ) {
          
          int temp = shm_array[i] ;
          shm_array[i] = shm_array[j] ;
          shm_array[j] = temp ;
          
        } // if
        
      } // for
      
    } // for
    
  } // Bubble_part()
  */
  
  void MergeSort( int arr[], int start, int end ) {
    
    if ( start < end ) {
      
      int mid = ( start + end ) / 2 ;
      MergeSort( arr, start, mid ) ;
      MergeSort( arr, mid + 1, end ) ;
      merge( arr, start, mid, end ) ;
      
    } // if
    
  } // MergeSort()
  
  void merge( int shm_array[], int start, int mid, int end ) {
    
    int left = start, right = mid + 1 ;
    vector<int> temp ;
    
    while( left <= mid && right <= end ) {
      
      if ( shm_array[left] < shm_array[right] ) temp.push_back( shm_array[left++] ) ;
      else temp.push_back( shm_array[right++] ) ;
      
    } // while
    
    if ( left <= mid )
      while ( left <= mid ) temp.push_back( shm_array[left++] ) ;
    else
      while ( right <= end ) temp.push_back( shm_array[right++] ) ;
    
    for ( int i = 0 ; i < temp.size() ; i ++ ) shm_array[start++] = temp.at(i) ;
    
    temp.clear() ;
    
  } // merge
  
  void Show( int *shm_array, int num, string name, double duration ) {
    
    fstream output ;
    
    output.open( name + "_output3.txt" , ios::out ) ;
    
    output << "Sort : " << endl ;
    
    if ( !output.is_open() ) cout << "Failed to open file" << endl ;
    else {
      
      for ( int i = 0 ; i < num ; i ++ )
        output << shm_array[i] << endl ;
      
    } // else
    
    output << "CPU Time : " << duration << endl ;
    cout << duration << endl ;
    
    time_t now = time(0);
    tm *ltm = localtime(&now);
    output << "Output Time : " << 1900 + ltm->tm_year << "-" ;
    output << 1 + ltm->tm_mon << "-" ;
    output << ltm->tm_mday << " " << ltm->tm_hour << ":";
    output << ltm->tm_min << ":";
    output << ltm->tm_sec << "+08:00" ;
    
    output.close() ;
    
  } // Show()
  
} ; // Mission3

class Mission4 {
  
private:
  
  mutex mu ;
  vector< vector<int> > my_list ;
  
public:
  
  void Separate( vector<int> all_numbers, int part, string name ) {
    
    string temp_str ;
    vector<int> temp, index ;
    clock_t Begin, End ;
    double duration = 0 ;
    
    Num_of_each_part( (int)all_numbers.size(), part, index ) ;
    
    for ( int i = 0 ; i < part ; i ++ ) {
    
      for ( int j = 0 ; j < index.at( i ) ; j ++ ) {
        
        temp.push_back( all_numbers.front() ) ;
        all_numbers.erase( all_numbers.begin() ) ;
        
      } // for
      
      my_list.push_back( temp ) ; // put numbers into vector ( vector in vector )
      temp.clear() ;
      
    } // for
    
    Begin = clock() ;
    
    vector<thread> threads, threads2;
    for(int i = 0; i < part; i++){
      threads.emplace_back(thread( &Mission4::Bubble_part, this, i ));
    } // for
    for(int i = 0; i < threads.size(); i++){
      threads[i].join();
    } // for
    
    threads.clear() ;
    
    for(int i = 0; i < part - 1 ; i++ ){
      threads2.emplace_back(thread( &Mission4::Merge_parts, this ));
    } // for
    for(int i = 0; i < threads2.size(); i++){
      threads2[i].join();
    } // for
    
    End = clock() ;
    duration = double(End - Begin) / CLOCKS_PER_SEC ;
    
    threads2.clear() ;
    
    Show( name, duration ) ;
    
  } // Separate()
  
  void Bubble_part( int i ) {
    
    mu.lock() ;
    
    try {
      Real_bubble( my_list.at( i ) ) ; // sort numbers
    } catch ( exception e ) {
      mu.unlock() ;
    }
    
    mu.unlock() ;
    
  } // Bubble_part()
    
  void Merge_parts() {
    
    vector<int> result ;
    
    mu.lock() ;
    try {
      Merge( my_list.at(0), my_list.at(1), result ) ;
      my_list.erase( my_list.begin() ) ; // remove the first
      my_list.erase( my_list.begin() ) ; // remove the second
      my_list.push_back( result ) ; // add merge result to the end
      result.clear() ;
    } catch( exception e ) {
      mu.unlock() ;
    }
    
    mu.unlock() ;
    
  } // Merge()
    
  void Show( string name, double duration ) {
    
    fstream output ;
    
    output.open( name + "_output4.txt" , ios::out ) ;
    
    output << "Sort : " << endl ;
    
    if ( !output.is_open() ) cout << "Failed to open file" << endl ;
    else {
      
      for ( int i = 0 ; i < my_list.front().size() ; i ++ )
        output << my_list.front().at(i) << endl ;
      
    } // else
    
    output << "CPU Time : " << duration << endl ;
    cout << duration << endl ;
    
    time_t now = time(0);
    tm *ltm = localtime(&now);
    output << "Output Time : " << 1900 + ltm->tm_year << "-" ;
    output << 1 + ltm->tm_mon << "-" ;
    output << ltm->tm_mday << " " << ltm->tm_hour << ":";
    output << ltm->tm_min << ":";
    output << ltm->tm_sec << "+08:00" ;
    
    output.close() ;
    
  } // Show()
  
  
} ; // Mission4

int main(int argc, const char * argv[]) {
  
  int part = 0, method = 0 ;
  double duration = 0.0 ;
  string file_name, name ;
  ifstream file ;
  vector<int> all_numbers ;
  Mission1 one ;
  Mission2 two ;
  Mission4 four ;
  clock_t Begin, End ;
  
  Mission3 three ;
  int shmid;
  size_t shm_size ;
  int *shm_array ;
  
  cout << "請輸入檔案名稱：\n" ;
  cin >> name ;
  cout << endl ;
  
  file_name = name + ".txt" ;
  file.open( file_name, ios::in ) ;
  
  if ( !file.is_open() ) {
    cout << "Failed to open file" << endl ;
    return 0 ;
  } // if
  else {
    
    string temp_str ;
    while( getline( file, temp_str ) ) all_numbers.push_back( stoi( temp_str ) ) ; // read all number into vector
    file.close() ;
    
  } // else
  
  cout << "請輸入方法編號：（方法1, 方法2, 方法3, 方法4）\n" ;
  cin >> method ;
  cout << endl ;
  
  if ( method != 1 ) {
    
    cout << "請輸入要切幾份\n" ;
    cin >> part ;
    
  } // if
  
  if ( method == 1 ) {
    
    one.sort( all_numbers, name ) ;
    
  }
  else if ( method == 2 ) {
    
    two.Separate( all_numbers, part ) ;
    Begin = clock() ;
    two.Bubble_part() ;
    two.Merge_parts() ;
    End = clock() ;
    duration = double(End - Begin) / CLOCKS_PER_SEC ;
    
    two.Show( name, duration ) ;
    
  } // else if
  else if ( method == 3 ) {
    
    shm_size = sizeof(int) * (int)all_numbers.size() ;
    shmid = shmget(IPC_PRIVATE, shm_size, IPC_CREAT | 0600) ;
    if (shmid < 0) {
      perror("shmget error");
      exit(-1);
    } // if
    
    shm_array = ( int * ) shmat(shmid, NULL, 0) ;
    if ( shm_array == ( int * ) -1 ) {
      perror("shmat error");
      exit(-1);
    } // if
    
    three.Separate( all_numbers, part, shm_array, name ) ;
    
    shmdt(shm_array) ;
    shmctl(shmid, IPC_RMID, NULL) ;
    
  } // else if
  else if ( method == 4 ) {
    
    four.Separate( all_numbers, part, name ) ;
    
  } // else if
  
  all_numbers.clear() ;
  
} // main()
