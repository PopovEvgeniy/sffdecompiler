#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "format.h"

void show_start_message();
void show_end_message();
void show_intro();
void command_line_help();
void show_progress(unsigned long int start, unsigned long int stop);
void go_offset(FILE *file, unsigned long int offset);
unsigned long int get_file_size(FILE *file);
void check_memory(void *memory);
char *get_string_memory(unsigned long int length);
unsigned char *get_memory(unsigned long int length);
FILE *open_input_file(char *name);
FILE *create_output_file(char *name);
void data_dump(FILE *input, FILE *output, unsigned long int length);
unsigned long int get_extension_position(char *source);
char *get_short_name(char *name);
char *get_name(unsigned long int index,char *short_name, char *extension);
unsigned long int read_sff_head(FILE *input);
sff_subhead read_sff_subhead(FILE *input);
void write_palette(FILE *output,unsigned char *palette);
unsigned char *get_palette_memory();
void read_palette(FILE *input,unsigned char *palette);
void extract_palette(char *name,unsigned char *palette);
void set_palette(FILE *output, sff_subhead subhead, unsigned char *palette, unsigned char *shared);
unsigned char *extract_first(FILE *input, char *short_name);
void extract_normal_sprite(FILE *input,char *name,sff_subhead subhead,unsigned char *palette, unsigned char *shared);
void extract_linked_sprite(char *name,char *short_name,unsigned short int index);
void extract_last(FILE *input,char *name,unsigned long int sff_size,sff_subhead subhead,unsigned char *palette, unsigned char *shared);
void extract(FILE *input,char *short_name);
void work(char *sff_name);

int main(int argc, char *argv[])
{
 show_intro();
 if (argc<2)
 {
  command_line_help();
 }
 else
 {
  show_start_message();
  work(argv[1]);
  show_end_message();
 }
 return 0;
}

void show_start_message()
{
 puts(" ");
 puts("Extracting a graphics... Please wait");
}

void show_end_message()
{
 puts(" ");
 puts("Work finish");
}

void show_progress(unsigned long int start, unsigned long int stop)
{
 unsigned long int progress;
 progress=start+1;
 progress*=100;
 progress/=stop;
 printf("\r");
 printf("Amount of extracted files: %ld from %ld.Progress:%ld%%",start+1,stop,progress);
}

void show_intro()
{
 puts(" ");
 puts("SFF DECOMPILER");
 puts("Version 1.6.9");
 puts("Mugen graphics extractor by Popov Evgeniy Alekseyevich, 2009-2016 year");
 puts("This program distributed under GNU GENERAL PUBLIC LICENSE");
 puts("Some was code taken from Sff extract");
 puts("Sff extract is created by Osuna Richert Christophe");
}

void command_line_help()
{
 puts(" ");
 puts("You must give a target file name as command line argument!");
}

void go_offset(FILE *file, unsigned long int offset)
{
 fseek(file,offset,SEEK_SET);
}

unsigned long int get_file_size(FILE *file)
{
 unsigned long int length;
 fseek(file,0,SEEK_END);
 length=ftell(file);
 rewind(file);
 return length;
}

void check_memory(void *memory)
{
 if(memory==NULL)
 {
  puts(" ");
  puts("Can't allocate memory");
  exit(1);
 }

}

char *get_string_memory(unsigned long int length)
{
 char *memory=NULL;
 memory=(char*)calloc(length+1,1);
 check_memory(memory);
 return memory;
}

unsigned char *get_memory(unsigned long int length)
{
 return (unsigned char*)calloc(length,1);
}

FILE *open_input_file(char *name)
{
 FILE *file;
 file=fopen(name,"rb");
 if (file==NULL)
 {
  puts(" ");
  puts("File operation error");
  exit(2);
 }
 return file;
}

FILE *create_output_file(char *name)
{
 FILE *file;
 file=fopen(name,"wb");
 if (file==NULL)
 {
  puts(" ");
  puts("File operation error");
  exit(2);
 }
 return file;
}

void data_dump(FILE *input, FILE *output, unsigned long int length)
{
 unsigned char single_byte;
 unsigned long int index;
 unsigned char *buffer=NULL;
 buffer=(unsigned char*)calloc(length,1);
 if (buffer==NULL)
 {
  for(index=0;index<length;index++)
  {
   fread(&single_byte,1,1,input);
   fwrite(&single_byte,1,1,output);
  }

 }
 else
 {
  fread(buffer,length,1,input);
  fwrite(buffer,length,1,output);
  free(buffer);
 }

}

unsigned long int get_extension_position(char *source)
{
 unsigned long int index;
 for(index=strlen(source);index>0;index--)
 {
  if(source[index]=='.')
  {
   break;
  }

 }
 if (index==0) index=strlen(source);
 return index;
}

char *get_short_name(char *name)
{
unsigned long int length;
char *result=NULL;
length=get_extension_position(name);
result=get_string_memory(length);
strncpy(result,name,length);
return result;
}

char *get_name(unsigned long int index,char *short_name, char *extension)
{
 char *name=NULL;
 char *result=NULL;
 unsigned long int length;
 length=strlen(short_name)+strlen(extension)+12;
 name=get_string_memory(length);
 sprintf(name,"%s%ld%s",short_name,index,extension);
 length=strlen(name);
 result=get_string_memory(length);
 strncpy(result,name,length);
 free(name);
 return result;
}

unsigned long int read_sff_head(FILE *input)
{
 sff_head head;
 fread(&head,sizeof(sff_head),1,input);
 if (strncmp(head.signature,"ElecbyteSpr",12)!=0)
 {
  puts(" ");
  puts("Bad signature of a mugen graphic pseudo-archive!");
  exit(3);
 }
 go_offset(input,head.sub_offset);
 return head.nb_imgs;
}

sff_subhead read_sff_subhead(FILE *input)
{
 sff_subhead subhead;
 fread(&subhead,sizeof(sff_subhead),1,input);
 return subhead;
}

void write_palette(FILE *output,unsigned char *palette)
{
 fwrite(palette,768,1,output);
}

unsigned char *get_palette_memory()
{
 unsigned char *memory=NULL;
 memory=(unsigned char*)calloc(768,1);
 check_memory(memory);
 return memory;
}

void read_palette(FILE *input,unsigned char *palette)
{
  fread(palette,768,1,input);
}

void extract_palette(char *name,unsigned char *palette)
{
 FILE *input;
 unsigned long int length;
 input=open_input_file(name);
 length=get_file_size(input);
 go_offset(input,length-768);
 read_palette(input,palette);
 fclose(input);
}

void set_palette(FILE *output, sff_subhead subhead, unsigned char *palette, unsigned char *shared)
{
 if (subhead.same_pal==1)
 {
  if (subhead.group>152)
  {
   write_palette(output,shared);
  }
  else
  {
   write_palette(output,palette);
  }

 }

}

unsigned char *extract_first(FILE *input, char *short_name)
{
 sff_subhead subhead;
 unsigned long int length;
 FILE *output;
 char *name=NULL;
 unsigned char *palette=NULL;
 subhead=read_sff_subhead(input);
 name=get_name(1,short_name,".pcx");
 output=create_output_file(name);
 length=subhead.next_offset-ftell(input);
 data_dump(input,output,length);
 fclose(output);
 free(name);
 go_offset(input,subhead.next_offset-768);
 palette=get_palette_memory();
 read_palette(input,palette);
 return palette;
}

void extract_normal_sprite(FILE *input,char *name,sff_subhead subhead,unsigned char *palette, unsigned char *shared)
{
 FILE *output;
 unsigned long int length;
 output=create_output_file(name);
 length=subhead.next_offset-ftell(input);
 data_dump(input,output,length);
 set_palette(output,subhead,palette,shared);
 fclose(output);
}

void extract_linked_sprite(char *name,char *short_name,unsigned short int index)
{
 FILE *output;
 FILE *input;
 char *linked_name=NULL;
 unsigned long int length;
 linked_name=get_name(index,short_name,".pcx");
 input=open_input_file(linked_name);
 output=create_output_file(name);
 length=get_file_size(input);
 data_dump(input,output,length);
 fclose(input);
 fclose(output);
 free(linked_name);
}

void extract_last(FILE *input,char *name,unsigned long int sff_size,sff_subhead subhead,unsigned char *palette, unsigned char *shared)
{
 FILE *output;
 unsigned long int length;
 output=create_output_file(name);
 length=sff_size-ftell(input);
 data_dump(input,output,length);
 set_palette(output,subhead,palette,shared);
 fclose(output);
}

void extract(FILE *input,char *short_name)
{
 char *name=NULL;
 unsigned char *palette=NULL;
 unsigned char *shared=NULL;
 unsigned long int index,sff_size,number;
 sff_subhead subhead;
 sff_size=get_file_size(input);
 shared=get_palette_memory();
 number=read_sff_head(input);
 show_progress(0,number);
 palette=extract_first(input,short_name);
 memmove(shared,palette,768);
 for(index=1;index<number;index++)
 {
  subhead=read_sff_subhead(input);
  show_progress(index,number);
  name=get_name(index+1,short_name,".pcx");
  if (subhead.next_offset!=0)
  {
   if (subhead.length>0)
   {
    extract_normal_sprite(input,name,subhead,palette,shared);
   }
   else
   {
    extract_linked_sprite(name,short_name,subhead.prev+1);
   }

  }
  else
  {
   extract_last(input,name,sff_size,subhead,palette,shared);
  }
  go_offset(input,subhead.next_offset);
  extract_palette(name,shared);
  free(name);
 }
 free(shared);
 free(palette);
}

void work(char *sff_name)
{
 FILE *input;
 char *short_name=NULL;
 input=open_input_file(sff_name);
 short_name=get_short_name(sff_name);
 extract(input,short_name);
 fclose(input);
 free(short_name);
}