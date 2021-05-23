#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct word
{
    char main_word[10];
    char trans_1[20];
    char trans_2[20];
    char type;
    char example[50];
    char date[15];
    int rate;
};
typedef struct word word;

struct List
{
    word node;
    struct List *next;
} ;
typedef struct List List;

List *new_word()
{
    List *result=NULL;
    result=(List*)malloc(sizeof(List));
    if(result==NULL){
        perror("Error allocating memory 1");
        exit(1);
    }
    printf("word: ");
    gets(result->node.main_word);
    printf("translate 1: ");
    gets(result->node.trans_1);
    printf("translate 2: ");
    gets(result->node.trans_2);
    printf("type: ");
    scanf("%c",&result->node.type);
    fflush(stdin);
    printf("example with main word: ");
    gets(result->node.example);
    printf("current date: ");
    gets(result->node.date);
    printf("rate translate: ");
    scanf("%d",&result->node.rate);

    result->next=NULL;
    return result;
}

void print_node(List *duma)
{
    printf("\nWord: %s\n",duma->node.main_word);
    printf("Translation language 1: %s\n",duma->node.trans_1);
    printf("Translation language 2: %s\n",duma->node.trans_2);
    printf("Type: %c\n",duma->node.type);
    printf("Example: %s\n",duma->node.example);
    printf("Date of entry: %s\n",duma->node.date);
    printf("Rate of translation: %d\n",duma->node.rate);
    printf("\n-----------------------\n\n");
}

void create_word_english()
{
    FILE *fp;
    fp=fopen("eng.lang","ab");
    if(fp==NULL){
        perror("error creating word entry English");
        exit(1);
    }
    List *word=new_word();
    if(fwrite(word,sizeof(List),1,fp)!=1){
        perror("Error writing in eng.lang");
        exit(1);
    }
    printf("\nWord successfully writen in eng.lang\n\n");
    free(word);
    fclose(fp);
}

void create_word_german()
{
    FILE *fp;
    fp=fopen("ger.lang","ab");
    if(fp==NULL){
        perror("error creating word entry German");
        exit(1);
    }
    List *word=new_word();
    if(fwrite(word,sizeof(List),1,fp)!=1){
        perror("Error writing in ger.lang");
        exit(1);
    }
    printf("\nWord successfully writen in ger.lang\n\n");
    free(word);
    fclose(fp);
}

void create_word_bulgarian()
{
    FILE *fp;
    fp=fopen("bg.lang","ab");
    if(fp==NULL){
        perror("error creating word entry Bulgarian");
        exit(1);
    }
    List *word=new_word();
    if(fwrite(word,sizeof(List),1,fp)!=1){
        perror("Error writing in bg.lang");
        exit(1);
    }
    printf("\nWord successfully writen in bg.lang\n\n");
    free(word);
    fclose(fp);
}

void read_binary()
{
   FILE *fp;
    fp=fopen("eng.lang","rb");
    if(fp==NULL){
        perror("error reading word entry English");
        exit(1);
    }
    List *word=NULL;
    word=(List*)malloc(sizeof(List));
    if(word==NULL){
        perror("Error allocating memory 1");
        exit(1);
    }

    if(fread(word,sizeof(List),1,fp)!=1){
        perror("error reading from binary english");
        exit(1);
    }
    print_node(word);
    fclose(fp);
    free(word);
}

List * CreateStackList(List *root, FILE *fp)
{
 if(fp==NULL){perror("there is no file to read from");return root;}
 while(feof(fp)!=EOF)
 {
     List * newItem = (List *)malloc(sizeof(List));
      if(newItem==NULL)
        {
            perror("can't create element for stack list");
            return root;
        }
     if( fread(newItem,sizeof(List),1,fp)!=1)
        {
            return root;
        }
     newItem->next=root;
     root=newItem;
 }
 return root;
}

void Print_List(List *root)
{
    List *curr_item;
    curr_item=root;
    while(curr_item!=NULL)
    {
        print_node(curr_item);
        curr_item=curr_item->next;
    }
}

void free_list(List *root)
{
    List *curr_item=root;
    while(root!=NULL)
    {
        root=root->next;
        free(curr_item);
        curr_item=root;
    }
}

void Search(List *root,char word[10])
{
    List *curr_item=root;
    int count=0;
    while(curr_item!=NULL)
    {
        if(strcmp(curr_item->node.main_word,word)==0){
            print_node(curr_item);
            count++;
        }
        curr_item=curr_item->next;
    }
    if(count==0) printf("\nNo entries found\n");
}

void Filter()
{
    FILE *a,*b,*c;

    a=fopen("eng.lang","rb");
    if(a==NULL){
        perror("Error opening binary file in filter");
        exit(1);
    }
    b=fopen("ger.lang","rb");
    if(b==NULL){
        perror("Error opening binary file in filter");
        exit(1);
    }
    c=fopen("bg.lang","rb");
    if(c==NULL){
        perror("Error opening binary file in filter");
        exit(1);
    }

//----------------------------------------------------------------

    List *root_eng=NULL;
    root_eng=CreateStackList(root_eng,a);
    List *root_ger=NULL;

    root_ger=CreateStackList(root_ger,b);
    List *root_bg=NULL;
    root_bg=CreateStackList(root_bg,c);

    List *curr_eng=root_eng;
    List *curr_ger=root_ger;
    List *curr_bg=root_bg;

    int flag_a=0, flag_b=0;

    printf("\nWords that arent pressent in all 3 dictionaries:\n\n");
    while(curr_eng!=NULL)
    {
        while(curr_ger!=NULL)
        {
            if(strcmp(curr_eng->node.main_word,curr_ger->node.trans_1)==0) flag_a=1;
            curr_ger=curr_ger->next;
        }
        while(curr_bg!=NULL)
        {
            if(strcmp(curr_eng->node.main_word,curr_bg->node.trans_1)==0) flag_b=1;
            curr_bg=curr_bg->next;
        }
        if(flag_a==0 || flag_b==0) printf("%s\n",curr_eng->node.main_word);
        curr_eng=curr_eng->next;
        curr_ger=root_ger;
        curr_bg=root_bg;
        flag_a=0;
        flag_b=0;
    }
    curr_eng=root_eng;
    curr_ger=root_ger;
    curr_bg=root_bg;

    while(curr_ger!=NULL)
    {
        while(curr_eng!=NULL)
        {
            if(strcmp(curr_ger->node.main_word,curr_eng->node.trans_1)==0) flag_a=1;
            curr_eng=curr_eng->next;
        }
        while(curr_bg!=NULL)
        {
            if(strcmp(curr_ger->node.main_word,curr_bg->node.trans_2)==0) flag_b=1;
            curr_bg=curr_bg->next;
        }
        if(flag_a==0 || flag_b==0) printf("%s\n",curr_ger->node.main_word);
        curr_ger=curr_ger->next;
        curr_eng=root_eng;
        curr_bg=root_bg;
        flag_a=0;
        flag_b=0;
    }
    curr_eng=root_eng;
    curr_ger=root_ger;
    curr_bg=root_bg;

    while(curr_bg!=NULL)
    {
        while(curr_eng!=NULL)
        {
            if(strcmp(curr_bg->node.main_word,curr_eng->node.trans_2)==0) flag_a=1;
            curr_eng=curr_eng->next;
        }
        while(curr_ger!=NULL)
        {
            if(strcmp(curr_bg->node.main_word,curr_ger->node.trans_2)==0) flag_b=1;
            curr_ger=curr_ger->next;
        }
        if(flag_a==0 || flag_b==0) printf("%s\n",curr_bg->node.main_word);
        curr_bg=curr_bg->next;
        curr_eng=root_eng;
        curr_ger=root_ger;
        flag_a=0;
        flag_b=0;
    }


//-----------------------------------------------------------------------
    printf("\n");
    fclose(a);
    fclose(b);
    fclose(c);
    free_list(root_eng);
    free_list(root_ger);
    free_list(root_bg);
}

int main()
{
   char lang[4],func[7], word1[10];
   FILE *fp,*f;
   List *root;

   printf("\tCourcework Alex Ivanov PIK2 gr41 fak.No:121220075\n\n");

   while(1)
{
    root=NULL;
    lang[0] = '\0';
    func[0] = '\0';
    word1[0] = '\0';

   printf("Choose a function:\nadd - Adds a new word\nread - Lists all words in a dictionary\nsearch - Searches a word in a dictionary\nexit - Closes the program\nfilter - Lists all words that arent present in all 3 dictionaries\n");
   printf("->");
   gets(func);
   if(strcmp(func,"add")==0)
   {
        printf("\nChoose language to insert new word (eng, ger, bg): ");
        gets(lang);

        if(strcmp(lang,"eng")==0){
        printf("Translation language 1: German\nTranslation language 2: Bulgarian\n");
        create_word_english();
        }
        else if(strcmp(lang,"ger")==0){
        printf("Translation language 1: English\nTranslation language 2: Bulgarian\n");
        create_word_german();
        }
        else if(strcmp(lang,"bg")==0){
        printf("Translation language 1: English\nTranslation language 2: German\n");
        create_word_bulgarian();
        }
        else
        {
            printf("No such language");
        }
        fflush(stdin);
   }
    else if(strcmp(func,"read")==0)
    {
       printf("\nChoose language to insert new word (eng, ger, bg): ");
       gets(lang);
       printf("---------------\n");
       if(strcmp(lang,"eng")==0){

        fp=fopen("eng.lang","rb");
        if(fp==NULL){
        perror("error reading word entry English");
        exit(1);
        }
        root=CreateStackList(root,fp);
        Print_List(root);
        free_list(root);
        fclose(fp);

        }
        else if(strcmp(lang,"ger")==0){

        fp=fopen("ger.lang","rb");
        if(fp==NULL){
        perror("error reading word entry German");
        exit(1);
        }
        root=CreateStackList(root,fp);
        Print_List(root);
        free_list(root);
        fclose(fp);

        }
        else if(strcmp(lang,"bg")==0){

        fp=fopen("bg.lang","rb");
        if(fp==NULL){
        perror("error reading word entry Bulgarian");
        exit(1);
        }
        root=CreateStackList(root,fp);
        Print_List(root);
        free_list(root);
        fclose(fp);
        }
        else
        {
            printf("No such language");
        }
    }

    else if(strcmp(func,"search")==0)
    {
          printf("\nChoose language to search for a word (eng, ger, bg): ");
          gets(lang);
          if(strcmp(lang,"eng")==0)
          {
            f=fopen("eng.lang","rb");
            if(f==NULL){
                perror("Error opening file for searching");
                exit(1);
            }
            root=CreateStackList(root,f);
            printf("Search: ");
            gets(word1);
            Search(root,word1);
            fclose(f);
            free_list(root);
          }

          else if(strcmp(lang,"ger")==0)
          {
            f=fopen("ger.lang","rb");
            if(f==NULL){
                perror("Error opening file for searching");
                exit(1);
            }
            root=CreateStackList(root,f);
            printf("Search: ");
            gets(word1);
            Search(root,word1);
            fclose(f);
            free_list(root);
          }

          else if(strcmp(lang,"bg")==0)
          {
            f=fopen("bg.lang","rb");
            if(f==NULL){
                perror("Error opening file for searching");
                exit(1);
            }
            root=CreateStackList(root,f);
            printf("Search: ");
            gets(word1);
            Search(root,word1);
            fclose(f);
            free_list(root);
          }
          else
        {
            printf("No such language");
        }
    }
    else if(strcmp(func,"exit")==0)
    {
        printf("Closing program...");
        break;
    }

    else if(strcmp(func,"filter")==0)
    {
        Filter();
    }
    else
        {
            printf("No such command\n");
        }
}
    return 0;
}
