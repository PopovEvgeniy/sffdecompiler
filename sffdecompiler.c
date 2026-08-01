#include "sffdecompiler.h"
#include "format.h"

void show_intro();
void show_message(const char *message);
void show_progress(const unsigned long int start,const unsigned long int stop);
FILE *open_input_file(const char *name);
FILE *create_output_file(const char *name);
void read_data(void *data,const size_t length,FILE *input);
void write_data(const void *data,const size_t length,FILE *output);
void go_offset(FILE *target,const unsigned long int offset);
char *get_memory(const size_t length);
void check_signature(const char *signature);
void data_dump(FILE *input,FILE *output,const size_t length);
void fast_data_dump(FILE *input,FILE *output,const size_t length);
unsigned long int get_file_size(FILE *target);
size_t get_name_without_extension_length(const char *source);
char *get_name_without_extension(const char *name);
char *get_name(const unsigned long int index,const char *name_without_extension,const char *extension);
unsigned long int read_sff_head(FILE *input);
sff_subhead read_sff_subhead(FILE *input);
void extract_palette(const char *name,char *palette);
void set_palette(FILE *output,const sff_subhead *subhead,const char *palette,const char *shared);
char *extract_first(FILE *input,const char *name_without_extension);
void extract_normal_sprite(FILE *input,const char *name,const sff_subhead *subhead,const char *palette,const char *shared);
void extract_linked_sprite(const char *name,const char *name_without_extension,const unsigned short int index);
void extract_last(FILE *input,const char *name,const unsigned long int sff_size,const sff_subhead *subhead,const char *palette,const char *shared);
void extract(FILE *input,const char *name_without_extension);
void work(const char *sff_name);

int main(int argc, char *argv[])
{
 show_intro();
 if (argc<2)
 {
  show_message("You must give a target file name as the command-line argument");
 }
 else
 {
  show_message("Working...");
  work(argv[1]);
  show_message("The work has been finished");
 }
 return 0;
}

void show_progress(const unsigned long int start,const unsigned long int stop)
{
 putchar('\r');
 printf("Amount of the extracted files: %lu from %lu.The progress:%lu%%",start,stop,(start*100)/stop);
}

void show_intro()
{
 putchar('\n');
 puts("SFF DECOMPILER");
 puts("Version 2.1.7");
 puts("Mugen image extractor by Popov Evgeniy Alekseyevich, 2009-2026 years");
 puts("This program is distributed under the GNU GENERAL PUBLIC LICENSE");
 puts("Some code taken from Sffextract by Osuna Richert Christophe");
}

void show_message(const char *message)
{
 putchar('\n');
 puts(message);
}

FILE *open_input_file(const char *name)
{
 FILE *target=NULL;
 if (name==NULL)
 {
  puts("Can't open the input file");
  exit(1);
 }
 target=fopen(name,"rb");
 if (target==NULL)
 {
  puts("Can't open the input file");
  exit(1);
 }
 return target;
}

FILE *create_output_file(const char *name)
{
 FILE *target=NULL;
 if (name==NULL)
 {
  show_message("Can't create the ouput file");
  exit(2);
 }
 target=fopen(name,"wb");
 if (target==NULL)
 {
  show_message("Can't create the ouput file");
  exit(2);
 }
 return target;
}

void read_data(void *data,const size_t length,FILE *input)
{
 fread(data,sizeof(char),length,input);
 if (ferror(input)!=0)
 {
  show_message("Can't read data!");
  exit(3);
 }

}

void write_data(const void *data,const size_t length,FILE *output)
{
 fwrite(data,sizeof(char),length,output);
 if (ferror(output)!=0)
 {
  show_message("Can't write data!");
  exit(4);
 }

}

void go_offset(FILE *target,const unsigned long int offset)
{
 if (fseek(target,offset,SEEK_SET)!=0)
 {
  show_message("Can't jump to the target offset");
  exit(5);
 }

}

char *get_memory(const size_t length)
{
 char *memory=NULL;
 memory=(char*)calloc(length,sizeof(char));
 if(memory==NULL)
 {
  puts("Can't allocate memory");
  exit(6);
 }
 return memory;
}

void check_signature(const char *signature)
{
 if (strncmp(signature,"ElecbyteSpr",12)!=0)
 {
  puts("The invalid format!");
  exit(7);
 }

}

unsigned long int get_file_size(FILE *target)
{
 unsigned long int length=0;
 if (fseek(target,0,SEEK_END)!=0)
 {
  show_message("Can't get the file size!");
  exit(8);
 }
 length=ftell(target);
 rewind(target);
 return length;
}

void data_dump(FILE *input,FILE *output,const size_t length)
{
 char *buffer=NULL;
 size_t current=0;
 size_t elapsed=0;
 size_t block=4096;
 buffer=get_memory(block);
 for (current=0;current<length;current+=block)
 {
  elapsed=length-current;
  if (elapsed<block)
  {
   block=elapsed;
  }
  read_data(buffer,block,input);
  write_data(buffer,block,output);
 }
 free(buffer);
}

void fast_data_dump(FILE *input,FILE *output,const size_t length)
{
 char *buffer=NULL;
 buffer=(char*)malloc(length);
 if (buffer==NULL)
 {
  data_dump(input,output,length);
 }
 else
 {
  read_data(buffer,length,input);
  write_data(buffer,length,output);
  free(buffer);
 }

}

size_t get_name_without_extension_length(const char *source)
{
 size_t index=0;
 size_t position=0;
 size_t length=0;
 if (source!=NULL)
 {
  length=strlen(source);
 }
 for (index=length;index>0;--index)
 {
  position=index-1;
  if (source[position]==DIRECTORY_SEPARATOR)
  {
   break;
  }
  if (source[position]=='.')
  {
   if (position>0)
   {
    if ((source[position-1]!=DIRECTORY_SEPARATOR) && (source[position-1]!='.'))
    {
     length=position;
     break;
    }

   }

  }

 }
 return length;
}

char *get_name_without_extension(const char *name)
{
 char *result=NULL;
 size_t length=0;
 length=get_name_without_extension_length(name);
 if (length>0)
 {
  result=get_memory(length+1);
  strncpy(result,name,length);
 }
 return result;
}

char *get_name(const unsigned long int index,const char *name_without_extension,const char *extension)
{
 char *name=NULL;
 size_t length=0;
 if (name_without_extension!=NULL)
 {
  if (extension!=NULL)
  {
   length=strlen(name_without_extension)+strlen(extension)+12;
   name=get_memory(length+1);
   sprintf(name,"%s%lu%s",name_without_extension,index,extension);
  }

 }
 return name;
}

unsigned long int read_sff_head(FILE *input)
{
 sff_head head;
 read_data(&head,sizeof(sff_head),input);
 check_signature(head.signature);
 go_offset(input,head.subfile_offset);
 return head.image_amount;
}

sff_subhead read_sff_subhead(FILE *input)
{
 sff_subhead subhead;
 read_data(&subhead,sizeof(sff_subhead),input);
 return subhead;
}

void extract_palette(const char *name,char *palette)
{
 FILE *input=NULL;
 input=open_input_file(name);
 go_offset(input,get_file_size(input)-PALETTE_LENGTH);
 read_data(palette,PALETTE_LENGTH,input);
 fclose(input);
}

void set_palette(FILE *output,const sff_subhead *subhead,const char *palette,const char *shared)
{
 if (subhead->same_palette==1)
 {
  if (subhead->group>152)
  {
   write_data(shared,PALETTE_LENGTH,output);
  }
  else
  {
   write_data(palette,PALETTE_LENGTH,output);
  }

 }

}

char *extract_first(FILE *input,const char *name_without_extension)
{
 unsigned long int length=0;
 FILE *output=NULL;
 char *name=NULL;
 char *palette=NULL;
 sff_subhead subhead;
 subhead=read_sff_subhead(input);
 name=get_name(1,name_without_extension,".pcx");
 output=create_output_file(name);
 length=subhead.next_offset-ftell(input);
 fast_data_dump(input,output,(size_t)length);
 fclose(output);
 free(name);
 go_offset(input,subhead.next_offset-PALETTE_LENGTH);
 palette=get_memory(PALETTE_LENGTH);
 fread(palette,sizeof(char),PALETTE_LENGTH,input);
 return palette;
}

void extract_normal_sprite(FILE *input,const char *name,const sff_subhead *subhead,const char *palette,const char *shared)
{
 FILE *output=NULL;
 unsigned long int length=0;
 output=create_output_file(name);
 length=subhead->next_offset-ftell(input);
 fast_data_dump(input,output,(size_t)length);
 set_palette(output,subhead,palette,shared);
 fclose(output);
}

void extract_linked_sprite(const char *name,const char *name_without_extension,const unsigned short int index)
{
 FILE *output=NULL;
 FILE *input=NULL;
 char *linked_name=NULL;
 unsigned long int length=0;
 linked_name=get_name(index,name_without_extension,".pcx");
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
 FILE *output=NULL;
 unsigned long int length=0;
 output=create_output_file(name);
 length=sff_size-ftell(input);
 fast_data_dump(input,output,(size_t)length);
 set_palette(output,subhead,palette,shared);
 fclose(output);
}

void extract(FILE *input,const char *name_without_extension)
{
 char *name=NULL;
 char *palette=NULL;
 char *shared=NULL;
 unsigned long int index=0;
 unsigned long int sff_size=0;
 unsigned long int amount=0;
 unsigned long int stop=0;
 sff_subhead subhead;
 sff_size=get_file_size(input);
 shared=get_memory(PALETTE_LENGTH);
 amount=read_sff_head(input);
 show_progress(1,amount);
 palette=extract_first(input,name_without_extension);
 memcpy(shared,palette,PALETTE_LENGTH);
 subhead=read_sff_subhead(input);
 stop=amount-1;
 for(index=1;index<stop;++index)
 {
  show_progress(index+1,amount);
  name=get_name(index+1,name_without_extension,".pcx");
  if (subhead.length>0)
  {
   extract_normal_sprite(input,name,&subhead,palette,shared);
  }
  else
  {
   extract_linked_sprite(name,name_without_extension,subhead.preversion+1);
  }
  go_offset(input,subhead.next_offset);
  subhead=read_sff_subhead(input);
  extract_palette(name,shared);
  free(name);
 }
 show_progress(index+1,amount);
 name=get_name(index+1,name_without_extension,".pcx");
 extract_last(input,name,sff_size,&subhead,palette,shared);
 free(name);
 free(shared);
 free(palette);
}

void work(const char *sff_name)
{
 FILE *input=NULL;
 char *name_without_extension=NULL;
 input=open_input_file(sff_name);
 name_without_extension=get_name_without_extension(sff_name);
 extract(input,name_without_extension);
 fclose(input);
 free(name_without_extension);
}