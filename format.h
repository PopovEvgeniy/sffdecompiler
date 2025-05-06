#define PALETTE_LENGTH 768

// The format description is based on code from Sff Extract

typedef struct
{
 char signature[12];
 unsigned char version[4];
 unsigned long int group_amount:32;
 unsigned long int image_amount:32;
 unsigned long int subfile_offset:32;
 unsigned long int subfile_size:32;
 char palette_type;
 char blank[3];
 char comments[476];
} sff_head;

typedef struct
{
 unsigned long int next_offset:32;
 unsigned long int length:32;
 short int x_coordinate:16;
 short int y_coordinate:16;
 unsigned short int group:16;
 unsigned short int image:16;
 unsigned short int preversion:16;
 unsigned char same_palette:8;
 char comments [13];
} sff_subhead;