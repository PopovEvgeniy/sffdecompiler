#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "format.h"

void show_start_message();
void show_end_message();
void show_intro();
void command_line_help();
void show_progress(const unsigned long int start,const unsigned long int stop);
FILE *open_input_file(const char *name);
FILE *create_output_file(const char *name);
void go_offset(FILE *file,const unsigned long int offset);
void data_dump(FILE *input,FILE *output,const size_t length);
unsigned long int get_file_size(FILE *file);
void data_dump(FILE *input,FILE *output,const size_t length);
void check_memory(const void *memory);
char *get_string_memory(const size_t length);
unsigned char *get_memory(const size_t length);
size_t get_extension_position(const char *source);
char *get_short_name(const char *name);
char *get_name(const unsigned long int index,const char *short_name,const char *extension);
unsigned long int read_sff_head(FILE *input);
sff_subhead read_sff_subhead(FILE *input);
void write_palette(FILE *output,const unsigned char *palette);
unsigned char *get_palette_memory();
void read_palette(FILE *input,unsigned char *palette);
void extract_palette(const char *name,unsigned char *palette);
void set_palette(FILE *output,const unsigned char palette_type,const unsigned short int group,const unsigned char *palette,const unsigned char *shared);
unsigned char *extract_first(FILE *input,const char *short_name);
void extract_normal_sprite(FILE *input,const char *name,const unsigned long int next,const unsigned char palette_type,const unsigned short int group,const unsigned char *palette,const unsigned char *shared);
void extract_linked_sprite(const char *name,const char *short_name,const unsigned short int index);
void extract_last(FILE *input,const char *name,const unsigned long int sff_size,const unsigned char palette_type,const unsigned short int group,const unsigned char *palette,const unsigned char *shared);
void extract(FILE *input,const char *short_name);
void work(const char *sff_name);

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

void show_progress(const unsigned long int start,const unsigned long int stop)
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
 puts("Version 1.7.5");
 puts("Mugen graphics extractor by Popov Evgeniy Alekseyevich, 2009-2018 year");
 puts("This program distributed under GNU GENERAL PUBLIC LICENSE");
 puts("Some was code taken from Sff extract");
 puts("Sff extract is created by Osuna Richert Christophe");
}

void command_line_help()
{
 puts(" ");
 puts("You must give a target file name as command line argument!");
}

FILE *open_input_file(const char *name)
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

FILE *create_output_file(const char *name)
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

void go_offset(FILE *file,const unsigned long int offset)
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

void data_dump(FILE *input,FILE *output,const size_t length)
{
 unsigned char single_byte;
 size_t index;
 unsigned char *buffer=NULL;
 buffer=(unsigned char*)calloc(length,sizeof(unsigned char));
 if (buffer==NULL)
 {
  for(index=0;index<length;++index)
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

void check_memory(const void *memory)
{
 if(memory==NULL)
 {
  puts(" ");
  puts("Can't allocate memory");
  exit(1);
 }

}

char *get_string_memory(const size_t length)
{
 char *memory=NULL;
 memory=(char*)calloc(length+1,sizeof(char));
 check_memory(memory);
 return memory;
}

unsigned char *get_memory(const size_t length)
{
 unsigned char *memory=NULL;
 memory=(unsigned char*)calloc(length,sizeof(unsigned char));
 check_memory(memory);
 return memory;
}

size_t get_extension_position(const char *source)
{
 size_t index;
 for(index=strlen(source);index>0;--index)
 {
  if(source[index]=='.')
  {
   break;
  }

 }
 if (index==0) index=strlen(source);
 return index;
}

char *get_short_name(const char *name)
{
 size_t length;
 char *result=NULL;
 length=get_extension_position(name);
 result=get_string_memory(length);
 strncpy(result,name,length);
 return result;
}

char *get_name(const unsigned long int index,const char *short_name,const char *extension)
{
 char *name=NULL;
 size_t length;
 length=strlen(short_name)+strlen(extension)+12;
 name=get_string_memory(length);
 sprintf(name,"%s%ld%s",short_name,index,extension);
 return name;
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

void write_palette(FILE *output,const unsigned char *palette)
{
 fwrite(palette,768,1,output);
}

unsigned char *get_palette_memory()
{
 return get_memory(768);
}

void read_palette(FILE *input,unsigned char *palette)
{
 fread(palette,768,1,input);
}

void extract_palette(const char *name,unsigned char *palette)
{
 FILE *input;
 unsigned long int length;
 input=open_input_file(name);
 length=get_file_size(input);
 go_offset(input,length-768);
 read_palette(input,palette);
 fclose(input);
}

void set_palette(FILE *output,const unsigned char palette_type,const unsigned short int group,const unsigned char *palette,const unsigned char *shared)
{
 if(palette_type==1)
 {
  if(group>152) write_palette(output,shared);
 }
 else
 {
  write_palette(output,palette);
 }

}

unsigned char *extract_first(FILE *input,const char *short_name)
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
 data_dump(input,output,(size_t)length);
 fclose(output);
 free(name);
 go_offset(input,subhead.next_offset-768);
 palette=get_palette_memory();
 read_palette(input,palette);
 return palette;
}

void extract_normal_sprite(FILE *input,const char *name,const unsigned long int next,const unsigned char palette_type,const unsigned short int group,const unsigned char *palette,const unsigned char *shared)
{
 FILE *output;
 unsigned long int length;
 output=create_output_file(name);
 length=next-ftell(input);
 data_dump(input,output,(size_t)length);
 set_palette(output,palette_type,group,palette,shared);
 fclose(output);
}

void extract_linked_sprite(const char *name,const char *short_name,const unsigned short int index)
{
 FILE *output;
 FILE *input;
 char *linked_name=NULL;
 unsigned long int length;
 linked_name=get_name(index,short_name,".pcx");
 input=open_input_file(linked_name);
 output=create_output_file(name);
 length=get_file_size(input);
 data_dump(input,output,(size_t)length);
 fclose(input);
 fclose(output);
 free(linked_name);
}

void extract_last(FILE *input,const char *name,const unsigned long int sff_size,const unsigned char palette_type,const unsigned short int group,const unsigned char *palette,const unsigned char *shared)
{
 FILE *output;
 unsigned long int length;
 output=create_output_file(name);
 length=sff_size-ftell(input);
 data_dump(input,output,(size_t)length);
 set_palette(output,palette_type,group,palette,shared);
 fclose(output);
}

void extract(FILE *input,const char *short_name)
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
 for(index=1;index<number;++index)
 {
  subhead=read_sff_subhead(input);
  show_progress(index,number);
  name=get_name(index+1,short_name,".pcx");
  if (subhead.next_offset!=0)
  {
   if (subhead.length>0)
   {
    extract_normal_sprite(input,name,subhead.next_offset,subhead.same_pal,subhead.group,palette,shared);
   }
   else
   {
    extract_linked_sprite(name,short_name,subhead.prev+1);
   }

  }
  else
  {
   extract_last(input,name,sff_size,subhead.same_pal,subhead.group,palette,shared);
  }
  go_offset(input,subhead.next_offset);
  extract_palette(name,shared);
  free(name);
 }
 free(shared);
 free(palette);
}

void work(const char *sff_name)
{
 FILE *input;
 char *short_name=NULL;
 input=open_input_file(sff_name);
 short_name=get_short_name(sff_name);
 extract(input,short_name);
 fclose(input);
 free(short_name);
}