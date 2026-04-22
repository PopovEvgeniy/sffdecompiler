#define PALETTE_LENGTH 768

// The format description is based on code from Sffextract

typedef struct
{
 char signature[12];
 unsigned char version[4];
 unsigned int group_amount;
 unsigned int image_amount;
 unsigned int subfile_offset;
 unsigned int subfile_size;
 char palette_type;
 char blank[3];
 char comments[476];
} sff_head;

typedef struct
{
 unsigned int next_offset;
 unsigned int length;
 short int x_coordinate;
 short int y_coordinate;
 unsigned short int group;
 unsigned short int image;
 unsigned short int preversion;
 unsigned char same_palette;
 char comments [13];
} sff_subhead;