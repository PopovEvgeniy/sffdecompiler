#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PALETTE_LENGTH 768

// The format description bases on code from Sff Extract

typedef struct
{
 char signature [12];
 unsigned char verhi:8;
 unsigned char verlo:8;
 unsigned char verlo2:8;
 unsigned char verlo3:8;
 unsigned long int nb_groups:32;
 unsigned long int nb_imgs:32;
 unsigned long int sub_offset:32;
 unsigned long int sub_size:32;
 char palette_type;
 char blank [3];
 char comments [476];
} sff_head;

typedef struct
{
 unsigned long int next_offset:32;
 unsigned long int length:32;
 short int xcoord:16;
 short int ycoord:16;
 unsigned short int group:16;
 unsigned short int img:16;
 unsigned short int prev:16;
 unsigned char same_pal:8;
 char comments [13];
} sff_subhead;