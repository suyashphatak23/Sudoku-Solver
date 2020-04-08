#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>


#define SIZE 9

static GtkWidget *window,*wid[9][9], *hbox, *vbox, *sudoku_lbl, *gtkbox;
static GtkBuilder *builder ;
static int sol = 0;

int a[9][9], b[9][9], check[9][9];

//function to print sudoku
void print_sudoku()
{	
	for(int i = 0; i<9; ++i){
	  for(int j = 0; j<9; ++j){
			gtk_entry_set_alignment (GTK_ENTRY(wid[i][j]), 0.5);
			if(a[i][j] != 0){
				char c[2];
                sprintf(c,"%d",a[i][j]);
				gtk_entry_set_text(GTK_ENTRY(wid[i][j]),c);	
                gtk_editable_set_editable(GTK_EDITABLE(wid[i][j]), FALSE);
				}
		  }
	  }
  
}

//function to check if all cells are assigned or not
//if there is any unassigned cell
//then this function will change the values of
//row and col accordingly
int number_unassigned(int *row, int *col)
{
    int num_unassign = 0;
    int i,j;
    for(i=0;i<SIZE;i++)
    {
        for(j=0;j<SIZE;j++)
        {
            //cell is unassigned
            if(a[i][j] == 0)
            {
                //changing the values of row and col
                *row = i;
                *col = j;
                //there is one or more unassigned cells
                num_unassign = 1;
                return num_unassign;
            }
        }
    }
    return num_unassign;
}

//function to check if we can put a
//value in a paticular cell or not
int is_safe(int n, int r, int c)
{
    int i,j;
    //checking in row
    for(i=0;i<SIZE;i++)
    {
        //there is a cell with same value
        if(a[r][i] == n)
            return 0;
    }
    //checking column
    for(i=0;i<SIZE;i++)
    {
        //there is a cell with the value equal to i
        if(a[i][c] == n)
            return 0;
    }
    //checking sub a
    int row_start = (r/3)*3;
    int col_start = (c/3)*3;
    for(i=row_start;i<row_start+3;i++)
    {
        for(j=col_start;j<col_start+3;j++)
        {
            if(a[i][j]==n)
                return 0;
        }
    }
    return 1;
}

//function to solve sudoku
//using backtracking
int solve_sudoku()
{
    int row;
    int col;
    //if all cells are assigned then the sudoku is already solved
    //pass by reference because number_unassigned will change the values of row and col
    if(number_unassigned(&row, &col) == 0)
        return 1;
    int i;
    //number between 1 to 9
    for(i=1;i<=SIZE;i++)
    {
        //if we can assign i to the cell or not
        //the cell is a[row][col]
        if(is_safe(i, row, col))
        {
            a[row][col] = i;
           
            //backtracking
            if(solve_sudoku())
                return 1;
            //if we can't proceed with this solution
            //reassign the cell
            a[row][col]=0;
           
        }
    }
    return 0;
}

void get_elements(GtkWidget* widget,gpointer data)
{	
	
	if(sol == 0) return;
      if (solve_sudoku())
        print_sudoku();
    else
        printf("No solution\n");
    return;
     
}

void check_elements(GtkWidget* widget,gpointer data){
	int val, temp = 0;
	sol = 0;
	solve_sudoku();
	for(int i = 0; i<9; ++i){
		if(temp != 0) break;
	  for(int j = 0; j<9; ++j){
		  const gchar *c;
		  c = gtk_entry_get_text (GTK_ENTRY(wid[i][j]));
		  val = atoi(c);
		  check[i][j] = val;
		  
		  if(check[i][j] != a[i][j]){
			  ++temp;
			  
			  break;
			
				}
		  
		  }
		 
	}
	if(temp == 0){
			   GtkWidget *dlg = gtk_dialog_new_with_buttons ("You Won",GTK_WINDOW(window), GTK_DIALOG_MODAL,("_OK"),GTK_RESPONSE_ACCEPT,("_Cancel"),GTK_RESPONSE_REJECT,NULL);
			   guint resp;
			   resp = gtk_dialog_run (GTK_DIALOG (dlg));
			   if (resp == GTK_RESPONSE_ACCEPT)
				{
				
				}
			   //gint gtk_dialog_run (GtkDialog dlg);
			   gtk_widget_destroy (dlg);
			   
			  }
			  
	else{
		GtkWidget *dlg = gtk_dialog_new_with_buttons ("You Lost",GTK_WINDOW(window), GTK_DIALOG_MODAL,("_OK"),GTK_RESPONSE_ACCEPT,("_Cancel"),GTK_RESPONSE_REJECT,NULL);
		guint resp;
		resp = gtk_dialog_run (GTK_DIALOG (dlg));
		if (resp == GTK_RESPONSE_ACCEPT)
		{
		
    	}
		//gint gtk_dialog_run (GtkDialog dlg);
		
	    gtk_widget_destroy (dlg);
		}
	sol++;
}


void on_clear_menu_item_activate(){
	for(int i = 0; i<9; ++i){
	  for(int j = 0; j<9; ++j){
		  if(b[i][j] == 0){
                gtk_entry_set_text(GTK_ENTRY(wid[i][j]),"");
                gtk_entry_set_alignment (GTK_ENTRY(wid[i][j]), 0.5);
                gtk_editable_set_editable(GTK_EDITABLE(wid[i][j]), TRUE);
				}
		  }
	  }	
}

void on_open_menu_item_activate(){
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
	guint res;
	GtkWidget *dialog;
	dialog = gtk_file_chooser_dialog_new ("Open File", GTK_WINDOW(window),action,("_Cancel"),GTK_RESPONSE_CANCEL,("_Open"),GTK_RESPONSE_ACCEPT,NULL);

				
	res = gtk_dialog_run (GTK_DIALOG (dialog));
	if (res == GTK_RESPONSE_ACCEPT)
		{
	
		FILE *f;
		
		GFile *file;
		gchar *path;
		file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(dialog));
		path = g_file_get_path(file);
		if(strcmp(path, "/home/lenovo/cproject/sudocu_project/sudoku1") && strcmp(path, "/home/lenovo/cproject/sudocu_project/sudoku2") && strcmp(path, "/home/lenovo/cproject/sudocu_project/sudoku3") && strcmp(path, "/home/lenovo/cproject/sudocu_project/sudoku4") && strcmp(path, "/home/lenovo/cproject/sudocu_project/sudoku5") && strcmp(path, "/home/lenovo/cproject/sudocu_project/sudoku6")){
			gtk_widget_destroy(dialog);
			GtkWidget *dlg = gtk_dialog_new_with_buttons ("Invalid File",GTK_WINDOW(window), GTK_DIALOG_MODAL,("_OK"),GTK_RESPONSE_ACCEPT,NULL);
			guint resp;
			resp = gtk_dialog_run (GTK_DIALOG (dlg));
			if (resp == GTK_RESPONSE_ACCEPT){}
			gtk_widget_destroy (dlg);
			
			return;
			}
			   
		f = fopen(path, "r");
		for(int i = 0; i < 9; i++){
			for(int j = 0; j < 9; j++){
				char c = fgetc(f);
				c = (int)c;
				c -= 48;
				b[i][j] = c;
				a[i][j] = c;
				
			
				}
			}
		sol++;
		print_sudoku();
		fclose(f);
		
		}
                gtk_widget_destroy(dialog);
	}

void on_quit_menu_item_activate(){
	gtk_main_quit();
	}



int main(int argc, char *argv[]) {

  gtk_init(&argc, &argv);
  GtkWidget  *button1, *separator, *button2, *buttonbox;
  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "/home/lenovo/cproject/sudocu_project/glade/ss1.glade", NULL);
  window = GTK_WIDGET(gtk_builder_get_object(builder, "window0"));
  
  gtkbox = GTK_WIDGET(gtk_builder_get_object(builder, "gtkbox0"));
  buttonbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  

  vbox=gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
 
  gtk_window_set_title(GTK_WINDOW(window), "Sudoku Solver");
   
  
  gtk_builder_connect_signals(builder, NULL);
  
 

  
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);  
      
  sudoku_lbl = gtk_label_new("Sudoku Puzzle - 1");
  gtk_box_pack_start(GTK_BOX(vbox),sudoku_lbl,TRUE,TRUE,10);
  
  for(int i=0;i<9;i++)
        {
                hbox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
                for(int j=0;j<9;j++)
                {
                        wid[i][j]=gtk_entry_new();
                        gtk_entry_set_width_chars(GTK_ENTRY(wid[i][j]),5);
                        gtk_entry_set_max_length(GTK_ENTRY(wid[i][j]),1);
                        gtk_widget_set_size_request(wid[i][j],50,50);
                        gtk_box_pack_start(GTK_BOX(hbox),wid[i][j],0,0,0);
                        if(((j+1)%3)==0)
                        {
                                separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
                                gtk_box_pack_start(GTK_BOX(hbox),separator,TRUE,TRUE,4);
                        }
                        
                }
                gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,0);
                 if(((i+1))%3==0)
                {		
                        separator=gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
                        gtk_box_pack_start(GTK_BOX(vbox),separator,TRUE,TRUE,3);
                }
               
        }
 
  button1=gtk_button_new_with_label("Check");
  button2=gtk_button_new_with_label("Sudoku Solution");
  g_signal_connect(button1,"clicked",G_CALLBACK(check_elements),NULL);
  g_signal_connect(button2,"clicked",G_CALLBACK(get_elements),NULL);
  gtk_box_pack_start(GTK_BOX(gtkbox),vbox,TRUE,TRUE,2);
  gtk_box_pack_start(GTK_BOX(gtkbox),buttonbox,TRUE,TRUE,2);
  gtk_box_pack_start(GTK_BOX(buttonbox),button1,TRUE,TRUE,2);
  gtk_box_pack_start(GTK_BOX(buttonbox),button2,TRUE,TRUE,3);
  
  gtk_widget_show_all(window);
  

  gtk_main();

  return 0;
}

void on_window0_destroy(){
	gtk_main_quit();
	}
