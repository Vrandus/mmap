/***********************************************************************/
/**      Author: Prateek Tailor                                       **/
/**        Date: Jul. 2019                                            **/
/** Description: Assgn. III                                           **/
/***********************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "array.h"
#include "functions.h"

/**********************************************************************/
/********************       O P E N  A R R A Y     ********************/
/**********************************************************************/
void open_array(char *filename, array_t *arrayp, int *sizep)
{
  /* Opens array from file filename and */
  /* returns its pointer and size */
  /* size is the size of the file divided by the size of the array struct */
  /* pointer array is obtained with mmap */

  //declare necessary variables
  struct stat sb;
  int fd;
  char *s;

  //open filename with read/write flag
  if(fd = open(filename, O_RDWR) < 0)
    fatalerr(filename, fd, "error accessing file");


  //use fstat to get file status information (size of file)
  if(fstat(fd, &sb) < 0)
    fatalerr("open_array", 0, "fstat fail");
  //truncate the size of the array onto the 
  if(sb.st_size==0){
    if(ftruncate(fd, ((**arrayp).index * sizeof(**arrayp))) < 0)
      fatalerr("ftruncate", errno, "error with trunc in open_array");
  }
  else{
    sizep = sb.st_size / sizeof(*arrayp);
  }
  //map fd to array pointer with read / write
  *arrayp = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  
  close(fd);


}

/**********************************************************************/
/********************     C L O S E  A R R A Y     ********************/
/**********************************************************************/
void close_array(array_t *arrayp, int size)
{				/* Unmaps array and sets it to NULL */
  munmap(arrayp, size);
  arrayp = NULL;
}

/**********************************************************************/
/********************    C R E A T E  A R R A Y    ********************/
/**********************************************************************/
void create_array(char *filename, int index, array_t *arrayp)
{
   int fd;
   char *s;
   //create file with name of filename variable
   if(fd = open(filename, O_RDWR | O_CREAT | O_EXCL, 777)  < 0)
     fatalerr(filename, fd, "error making file (might already exist)");

  //ftruncate to the size of the array struct
  if(ftruncate(fd, (index * sizeof(**arrayp))) < 0){
     perror("Could not ftruncate");
   }
  //map virtual memory to array pointer struct
  int strSize = sizeof(**arrayp);
  int mmapSize = index * strSize;
  *arrayp = mmap(NULL, mmapSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  close(fd);
  munmap(s, mmapSize);
    				/* Creates a file with an array with index */
                                /* elements, all with the valid member false. */
                                /* It is fatal error if file exists */
  /* size is the size of the file divided by the size of the array struct */
  /* pointer array is obtained with mmap */
}


/**********************************************************************/
/********************       S E T  E N T R Y       ********************/
/**********************************************************************/
void set_entry(array_t array, char *name, int index, float age)
{				/* Sets the elements of the index'ed */
                                /* entry to name and age and the valid */
                                /* member to true */
  *array[index].name = malloc(sizeof(name));
  strcpy(array[index].name, name);
  array[index].age = age; 
  array[index].index = index;
  array[index].valid = 1;
}

/**********************************************************************/
/********************       G E T  E N T R Y       ********************/
/**********************************************************************/
void get_entry(array_t array, char **name, int index, float *age)
{				/* Returns the index'ed entry's name and age */
                                /* if valid, fatal error o/w */
if(array[index].valid != 1){
  fatalerr("array", 0, "not a valid entry");       
}
  strcpy(*name, array[index].name);
  age = &array[index].age;
}

/**********************************************************************/
/********************   D E L E T E   E N T R Y    ********************/
/**********************************************************************/
void delete_entry(array_t array, int index)
{				/* Sets the valid element of the index'ed */
                                /* entry to false */
  array[index].valid = 0;
}

/**********************************************************************/
/********************     P R I N T   A R R A Y    ********************/
/**********************************************************************/
void print_array(array_t array, int size)
{				/* Prints all entries with valid member true */
                                /* using the same format as in the main */
    for(int i = 0; i < size; i++){
      if(array[i].valid == 1){
        printf("index: %d name: %s age: %f", array[i].index, array[i].name, array[i].age);
      }
    }
 
}
