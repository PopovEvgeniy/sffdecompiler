#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "format.h"

void show_intro();
void show_message(const char *message);
void show_progress(const unsigned long int start,const unsigned long int stop);
FILE *open_input_file(const char *name);
FILE *create_output_file(const char *name);
void go_offset(FILE *file,const unsigned long int offset);
void data_dump(FILE *input,FILE *output,const size_t length);
void fast_data_dump(FILE *input,FILE *output,const size_t length);
unsigned long int get_file_size(FILE *file);
void data_dump(FILE *input,FILE *output,const size_t length);
void check_memory(const void *memory);
char *get_memory(const size_t length);
size_t get_extension_position(const char *source);
char *get_short_name(const char *name);
char *get_name(const unsigned long int index,const char *short_name,const char *extension);
void check_signature(const char *signature);
unsigned long int read_sff_head(FILE *input);
sff_subhead read_sff_subhead(FILE *input);
void extract_palette(const char *name,char *palette);
void set_palette(FILE *output,const sff_subhead *subhead,const char *palette,const char *shared);
char *extract_first(FILE *input,const char *short_name);
void extract_normal_sprite(FILE *input,const char *name,const sff_subhead *subhead,const char *palette,const char *shared);
void extract_linked_sprite(const char *name,const char *short_name,const unsigned short int index);
void extract_last(FILE *input,const char *name,const unsigned long int sff_size,const sff_subhead *subhead,const char *palette,const char *shared);
void extract(FILE *input,const char *short_name);
void work(const char *sff_name);

int main(int argc, char *argv[])
{
 show_intro();
 if (argc<2)
 {
  show_message("You must give a target file name as command line argument");
 }
 else
 {
  show_message("Extracting a graphics... Please wait");
  work(argv[1]);
  show_message("Work finish");
 }
 return 0;
}

void show_progress(const unsigned long int start,const unsigned long int stop)
{
 unsigned long int progress;
 progress=start+1;
 progress*=100;
 progress/=stop;
 putchar('\r');
 printf("Amount of extracted files: %lu from %lu.Progress:%lu%%",start+1,stop,progress);
}

void show_intro()
{
 putchar('\n');
 puts("SFF DECOMPILER");
 puts("Version 1.9");
 puts("Mugen graphics extractor by Popov Evgeniy Alekseyevich, 2009-2022 years");
 puts("This program distributed under GNU GENERAL PUBLIC LICENSE");
 puts("Some code taken from Sff extract");
 puts("Sff extract made by Osuna Richert Christophe");
}

void show_message(const char *message)
{
 putchar('\n');
 puts(message);
}

FILE *open_input_file(const char *name)
{
 FILE *target;
 target=fopen(name,"rb");
 if (target==NULL)
 {
  show_message("Can't open input file");
  exit(1);
 }
 return target;
}

FILE *create_output_file(const char *name)
{
 FILE *target;
 target=fopen(name,"wb");
 if (target==NULL)
 {
  show_message("Can't create ouput file");
  exit(2);
 }
 return target;
}

void go_offset(FILE *file,const unsigned long int offset)
{
 if (fseek(file,offset,SEEK_SET)!=0)
 {
  show_message("Can't jump to target offset");
  exit(3);
 }

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
 unsigned char data;
 size_t index;
 data=0;
 for (index=0;index<length;++index)
 {
  fread(&data,sizeof(unsigned char),1,input);
  fwrite(&data,sizeof(unsigned char),1,output);
 }

}

void fast_data_dump(FILE *input,FILE *output,const size_t length)
{
 unsigned char *buffer=NULL;
 buffer=(unsigned char*)calloc(length,sizeof(unsigned char));
 if (buffer==NULL)
 {
  data_dump(input,output,length);
 }
 else
 {
  fread(buffer,sizeof(unsigned char),length,input);
  fwrite(buffer,sizeof(unsigned char),length,output);
  free(buffer);
 }

}

void check_memory(const void *memory)
{
 if(memory==NULL)
 {
  show_message("Can't allocate memory");
  exit(4);
 }

}

char *get_memory(const size_t length)
{
 char *memory=NULL;
 memory=(char*)calloc(length,sizeof(char));
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
 result=get_memory(length+1);
 strncpy(result,name,length);
 return result;
}

char *get_name(const unsigned long int index,const char *short_name,const char *extension)
{
 char *name=NULL;
 size_t length;
 length=strlen(short_name)+strlen(extension)+12;
 name=get_memory(length+1);
 sprintf(name,"%s%lu%s",short_name,index,extension);
 return name;
}

void check_signature(const char *signature)
{
  if (strncmp(signature,"ElecbyteSpr",12)!=0)
 {
  show_message("Bad signature of a mugen graphic pseudo-archive!");
  exit(5);
 }

}

unsigned long int read_sff_head(FILE *input)
{
 sff_head head;
 fread(&head,sizeof(sff_head),1,input);
 check_signature(head.signature);
 go_offset(input,head.sub_offset);
 return head.nb_imgs;
}

sff_subhead read_sff_subhead(FILE *input)
{
 sff_subhead subhead;
 fread(&subhead,sizeof(sff_subhead),1,input);
 return subhead;
}

void extract_palette(const char *name,char *palette)
{
 FILE *input;
 input=open_input_file(name);
 go_offset(input,get_file_size(input)-768);
 fread(palette,sizeof(char),768,input);
 fclose(input);
}

void set_palette(FILE *output,const sff_subhead *subhead,const char *palette,const char *shared)
{
 if (subhead->same_pal==1)
 {
  if (subhead->group>152)
  {
   fwrite(shared,sizeof(char),768,output);
  }
  else
  {
   fwrite(palette,sizeof(char),768,output);
  }

 }

}

char *extract_first(FILE *input,const char *short_name)
{
 sff_subhead subhead;
 unsigned long int length;
 FILE *output;
 char *name=NULL;
 char *palette=NULL;
 subhead=read_sff_subhead(input);
 name=get_name(1,short_name,".pcx");
 output=create_output_file(name);
 length=subhead.next_offset-ftell(input);
 fast_data_dump(input,output,(size_t)length);
 fclose(output);
 free(name);
 go_offset(input,subhead.next_offset-768);
 palette=get_memory(768);
 fread(palette,sizeof(char),768,input);
 return palette;
}

void extract_normal_sprite(FILE *input,const char *name,const sff_subhead *subhead,const char *palette,const char *shared)
{
 FILE *output;
 unsigned long int length;
 output=create_output_file(name);
 length=subhead->next_offset-ftell(input);
 fast_data_dump(input,output,(size_t)length);
 set_palette(output,subhead,palette,shared);
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
 fast_data_dump(input,output,(size_t)length);
 fclose(input);
 fclose(output);
 free(linked_name);
}

void extract_last(FILE *input,const char *name,const unsigned long int sff_size,const sff_subhead *subhead,const char *palette,const char *shared)
{
 FILE *output;
 unsigned long int length;
 output=create_output_file(name);
 length=sff_size-ftell(input);
 fast_data_dump(input,output,(size_t)length);
 set_palette(output,subhead,palette,shared);
 fclose(output);
}

void extract(FILE *input,const char *short_name)
{
 char *name=NULL;
 char *palette=NULL;
 char *shared=NULL;
 unsigned long int index,sff_size,number;
 sff_subhead subhead;
 sff_size=get_file_size(input);
 shared=get_memory(768);
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
    extract_normal_sprite(input,name,&subhead,palette,shared);
   }
   else
   {
    extract_linked_sprite(name,short_name,subhead.prev+1);
   }

  }
  else
  {
   extract_last(input,name,sff_size,&subhead,palette,shared);
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