#include "fs.h"
#include <math.h> 
#include <vector>
#include <iostream>
#include <signal.h>

#define DEBUG 1

using namespace std;

typedef struct idxNode idxNode;

myFileSystem::myFileSystem(char diskName[16]) {
  // open the file with the above name
  // this file will act as the "disk" for your file system
  this->disk.open(diskName, ios::out | ios::in);
}

int myFileSystem::create_file(char name[8], int size) {
  if (!this->disk.is_open())
    return -1; //error disk not open

  if (size > 8 || size < 1)
    return -1; //size too large

  if (strlen(name) > 8 || strlen(name) == 0)
    return -1; // invalid name

  //create a file with this name and this size

  // high level pseudo code for creating a new file

  #if DEBUG

  cout << "Attempting to create file: " << name << endl;

  #endif

  // Step 1: Check to see if we have sufficient free space on disk by
  //   reading in the free block list. To do this:
  // Move the file pointer to the start of the disk file.
  // Read the first 128 bytes (the free/in-use block information)
  // Scan the list to make sure you have sufficient free blocks to
  //   allocate a new file of this size

  this->disk.sync();

  this->disk.seekg(0, disk.beg); //move file ptr
  char buf_free[128];

  this->disk.read(buf_free, 128); //read first 128 bytes, free block list

  vector<int> blocks_free; // to be filled with free block numbers

  for (int i = 1; i < 128; i ++){
    // scan list
    if (buf_free[i] == 0)
      blocks_free.push_back(i);
    if (blocks_free.size() >= (size_t)size)
      break;
  }

  if (blocks_free.size() == 0 || blocks_free.size() < (size_t)size) {
    #if DEBUG
    cout << "Error insufficient free blocks available for creation\n";
    #endif
    
    return -1; //error, scanned entire free list and insufficient blocks
  }

  // Step 2: we look for a free inode on disk
  // Read in an inode
  // Check the "used" field to see if it is free
  // If not, repeat the above two steps until you find a free inode
  // Set the "used" field to 1
  // Copy the filename to the "name" field
  // Copy the file size (in units of blocks) to the "size" field

  idxNode found_inode;
  int inode_pos = -1;
  // i'th inode is located at 128 + 48 * i
  for(int i = 0; i < 16; i ++){ //look for free inode, there are 16

    // begin read in inode
    char tmp_buf[48];
    this->disk.read(tmp_buf, 48);
    //deserialize struct
    memcpy(&found_inode, tmp_buf, sizeof(found_inode));

    // end read in inode

    if (found_inode.used == 0){ // check "used" field
      inode_pos = i;
      break;
    } /*else if(strncmp(name, found_inode.name, 8) == 0){
      #if DEBUG
      cout << "Error in create, file with the same name exists\n";
      #endif

      return -1;
    }  */
    else if (i == 15){
      #if DEBUG
      cout << "Error in create, no free inodes\n";
      #endif

      return -1; //error, no free inodes
    }
  }

  if (inode_pos == -1)
    return -1;

  found_inode.used = 1; //set the used field to 1
  strncpy(found_inode.name, name, 8); //copy the filename to the name field
  found_inode.size = size; //copy the file size

  // Step 3: Allocate data blocks to the file
  // for(i=0;i<size;i++)
  // Scan the block list that you read in Step 1 for a free block
  // Once you find a free block, mark it as in-use (Set it to 1)
  // Set the blockPointer[i] field in the inode to this block number.
  // end for

  for (int i = 0; i < size; i ++){
    found_inode.blockPointers[i] = blocks_free[i];
    buf_free[blocks_free[i]] = 1;
  }

  // Step 4: Write out the inode and free block list to disk
  // Move the file pointer to the start of the file
  // Write out the 128 byte free block list
  // Move the file pointer to the position on disk where this inode was stored
  // Write out the inode

  this->disk.seekp(0, disk.beg); //move file ptr to start
  this->disk.write(buf_free, 128); // write out free block list
  this->disk.seekp(48 * inode_pos, disk.cur); // seek to inode

  char tmp_buf[48];
  memcpy(tmp_buf, &found_inode, sizeof(found_inode)); //serialize inode

  this->disk.write(tmp_buf, 48); // write out inode
  this->disk.flush();

  return 1;
} // End Create

int myFileSystem::delete_file(char name[8]){
  if (!this->disk.is_open())
    return -1; //error disk not open
  // Delete the file with this name

  // Step 1: Locate the inode for this file
  // Move the file pointer to the 1st inode (129th byte)
  // Read in an inode
  // If the inode is free, repeat above step.
  // If the inode is in use, check if the "name" field in the
  //   inode matches the file we want to delete. If not, read the next
  //   inode and repeat

  #if DEBUG

  cout << "Deleting file : " << name << endl;

  #endif

  this->disk.sync();

  this->disk.seekg(128, disk.beg);

  idxNode found_inode;
  int inode_pos = -1;

  for(int i = 0; i < 16; i ++){

    // begin read in inode
    
    char tmp_buf[48];
    this->disk.read(tmp_buf, 48);

    //deserialize struct, copy buf to struct
    memcpy(&found_inode, tmp_buf, sizeof(found_inode));

    // end read in inode

    if (found_inode.used == 1 && strncmp(found_inode.name, name, 8) == 0 && i != 15){ // check "used" field
      inode_pos = i; //i < 15, found matching inode
      break;
    } else if (i == 15){
      if (found_inode.used && strncmp(found_inode.name, name, 8) == 0){
        inode_pos = i; //reached end, found matching inode
        break;
      } else {
        #if DEBUG
        cout << "Error in delete_file, no matching inode, all nodes scanned\n";
        #endif
        return -1;
      } // error, searched all files, non existant
    }
  }

  // Step 2: free blocks of the file being deleted
  // Read in the 128 byte free block list (move file pointer to start
  //   of the disk and read in 128 bytes)
  // Free each block listed in the blockPointer fields as follows:
  // for(i=0;i< inode.size; i++)
  // freeblockList[ inode.blockPointer[i] ] = 0;

  this->disk.seekg(0, disk.beg); //move file ptr
  char buf_free[128];

  this->disk.read(buf_free, 128); //read first 128 bytes, free block list

  for (int i = 0; i < found_inode.size; i ++){
    buf_free[found_inode.blockPointers[i]] = 0; //free blocks of found file
  }

  // Step 3: mark inode as free
  // Set the "used" field to 0.

  found_inode.used = 0;

  // Step 4: Write out the inode and free block list to disk
  // Move the file pointer to the start of the file
  // Write out the 128 byte free block list
  // Move the file pointer to the position on disk where this inode was stored
  // Write out the inode

  this->disk.seekp(0, disk.beg); //move file ptr to start
  this->disk.write(buf_free, 128); // write out free block list
  this->disk.seekp(48 * inode_pos, disk.cur); // seek to inode

  char tmp_buf[48];
  memcpy(tmp_buf, &found_inode, sizeof(found_inode)); //serialize inode

  this->disk.write(tmp_buf, 48); // write out inode
  this->disk.flush();
  return 1;
} // End Delete


int myFileSystem::ls(){
  if (!this->disk.is_open())
    return -1; //error disk not open
  // List names of all files on disk

  // Step 1: read in each inode and print
  // Move file pointer to the position of the 1st inode (129th byte)
  // for(i=0;i<16;i++)
  // Read in an inode
  // If the inode is in-use
  // print the "name" and "size" fields from the inode
  // end for
  this->disk.sync();

  this->disk.seekg(128, disk.beg);

  cout << endl << endl << "Listing INodes..." << endl;

  for(int i = 0; i < 16; i ++){

    // begin read in inode
    idxNode found_inode;
    char tmp_buf[48];
    this->disk.read(tmp_buf, 48);

    //deserialize struct, copy buf to struct
    memcpy(&found_inode, tmp_buf, sizeof(found_inode));

    // end read in inode

    if (found_inode.used == 1){ // check "used" field
      cout << "INode: " << found_inode.name << " size: " << found_inode.size << endl;
    }     
  }

  return 1;
}; // End ls

int myFileSystem::read(char name[8], int blockNum, char buf[1024]){
  if (!this->disk.is_open())
    return -1; //error disk not open
  // read this block from this file

  // Step 1: locate the inode for this file
  // Move file pointer to the position of the 1st inode (129th byte)
  // Read in an inode
  // If the inode is in use, compare the "name" field with the above file
  // If the file names don't match, repeat

  #if DEBUG

  cout << "Reading file : " << name << " blockNum: " << blockNum << endl;

  #endif

  this->disk.sync();

  this->disk.seekg(128, disk.beg);

  idxNode found_inode;
  int inode_pos = -1;

  for(int i = 0; i < 16; i ++){

    // begin read in inode
    
    char tmp_buf[48];
    this->disk.read(tmp_buf, 48);

    //deserialize struct, copy buf to struct
    memcpy(&found_inode, tmp_buf, sizeof(found_inode));

    // end read in inode

    if (found_inode.used == 1 && strncmp(found_inode.name, name, 8) == 0 && i != 15){ // check "used" field
      inode_pos = i; //i < 15, found matching inode
      break;
    } else if (i == 15){
      if (found_inode.used && strncmp(found_inode.name, name, 8) == 0){
        inode_pos = i; //reached end, found matching inode
        break;
      } else {
        #if DEBUG
        cout << "Error in read_file, no matching inode, all nodes scanned\n";
        #endif
        return -1;
      } // error, searched all files, non existant
    }
  }

  // Step 2: Read in the specified block
  // Check that blockNum < inode.size, else flag an error
  // Get the disk address of the specified block
  // That is, addr = inode.blockPointer[blockNum]
  // Move the file pointer to the block location (i.e., to byte #
  //   addr*1024 in the file)

  if (blockNum >= found_inode.size){
    #if DEBUG
    cout << "Error requested blocknum greater than size of inode.\n";
    #endif
    return -1;
  }

  int offset = found_inode.blockPointers[blockNum] * block_size;

  #if DEBUG
  cout << "Read pointer: " << found_inode.blockPointers[blockNum] << " offset: " 
        << offset << endl;
  #endif
  // Read in the block => Read in 1024 bytes from this location
  //   into the buffer "buf"

  this->disk.seekg(offset, disk.beg);
  this->disk.read(buf, block_size);

  return 1;
} // End read


int myFileSystem::write(char name[8], int blockNum, char buf[1024]){
  if (!this->disk.is_open())
    return -1; //error disk not open
  // write this block to this file

  // Step 1: locate the inode for this file
  // Move file pointer to the position of the 1st inode (129th byte)
  // Read in a inode
  // If the inode is in use, compare the "name" field with the above file
  // If the file names don't match, repeat

  #if DEBUG

  cout << "Reading file : " << name << " blockNum: " << blockNum << endl;

  #endif

  this->disk.sync();

  this->disk.seekg(128, disk.beg);

  idxNode found_inode;
  int inode_pos = -1;

  for(int i = 0; i < 16; i ++){

    // begin read in inode
    
    char tmp_buf[48];
    this->disk.read(tmp_buf, 48);

    //deserialize struct, copy buf to struct
    memcpy(&found_inode, tmp_buf, sizeof(found_inode));

    // end read in inode

    if (found_inode.used == 1 && strncmp(found_inode.name, name, 8) == 0 && i != 15){ // check "used" field
      inode_pos = i; //i < 15, found matching inode
      break;
    } else if (i == 15){
      if (found_inode.used && strncmp(found_inode.name, name, 8) == 0){
        inode_pos = i; //reached end, found matching inode
        break;
      } else {
        #if DEBUG
        cout << "Error in read_file, no matching inode, all nodes scanned\n";
        #endif
        return -1;
      } // error, searched all files, non existant
    }
  }

  // Step 2: Write to the specified block
  // Check that blockNum < inode.size, else flag an error
  // Get the disk address of the specified block
  // That is, addr = inode.blockPointer[blockNum]
  // Move the file pointer to the block location (i.e., byte # addr*1024)

  if (blockNum >= found_inode.size){
    #if DEBUG
    cout << "Error requested blocknum greater than size of inode.\n";
    #endif
    return -1;
  }

  int offset = found_inode.blockPointers[blockNum] * block_size;

  #if DEBUG
  cout << "Write pointer: " << found_inode.blockPointers[blockNum] << " offset: " 
        << offset << endl;
  #endif

  // Write the block! => Write 1024 bytes from the buffer "buff" to
  //   this location

  this->disk.seekp(offset, disk.beg);
  this->disk.write(buf, block_size);

  this->disk.flush();

  return 1;
} // end write

int myFileSystem::close_disk(){
    disk.close();
    return 1;
}
