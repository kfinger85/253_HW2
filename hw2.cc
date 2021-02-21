#include <iostream>
#include <fstream>
#include <sys/types.h>	// fork() & wait()
#include <unistd.h>	// fork(), execvp()
#include <cstdlib>	// exit()
#include <wait.h>	// wait()
#include <vector>	// vector<>
#include <string>	// string
#include <cstring>
using namespace std;

bool execute(vector<string> command) {
    // Convert vector of C++ strings to an array of C strings for exectp():
    vector<char *> argv;		// vector of C string pointers
    for (auto &v : command)		// for each C++ string:
	argv.push_back(v.data());	//    add address of C-string inside it
    argv.push_back(nullptr);	// argv ends with a null pointer

    // Duplicate ourself: -1:error, 0:child, >0:parent
    const auto pid = fork();		// step #1: copy ourself
    if (pid < 0)
	return false;			// fork failure?
    if (pid == 0) {			// child?
	execvp(argv[0], argv.data());	// step #2: replace child with new prog
	exit(0xff);			// only got here if execvp() failed.
    }
    int status;
    wait(&status);			// step #3: wait for child to finish
    return status != 0xff00;		// true iff we succeeded
}
void printError(const vector<string> &arguments, char *argv[])
{
/*
    Produce an error message and stop the program if:
    - second word is not an open paren
    - last word is not a closing paren
    - unescaped parentheses anywhere but a left paren as the second word and 
      a right paren as the last word
    - a filename argument can’t be opened for reading 
      (the error message must contain the offending filename)
      execute() indicates a problem (the error message must also contain the entire original bad line).
*/
 cerr << argv[0] << ": can’t run: " << arguments[0] << " ";  
                for(size_t i = 1 ; i < arguments.size() ; i++)
                {
                    cerr << arguments[i] << " "; 
                }
                cerr << "\n";
                exit(1);     

}
void evalbackslashflagged(char &ch, string &command)
{  
//Evalulate with -e flag (interpert backslashes) std input 
  if (ch == '\\')
        {              
            cin.get(ch);

                if (cin.peek() < 0){
                ch = -1; 
                return; 
                }
            
            switch (ch)
            {
            case ' ':
                cin.get(ch);
                command += ' ';
                break;
            case ')':
                cin.get(ch);
                command += ')';
                break; 
            case '(':
                cin.get(ch);
                command += '(';
                break; 
            default:
                break;
            }        
        }
}
void evalbackslash(char &ch, string &command, vector<string> &argumetns)
{
  if (ch == '\\')
        {         
            
            if ((cin.peek() == 10 ||cin.peek() < 0) && ch == '\\'){
                argumetns.push_back("\\");           
            }
            cin.get(ch);
            if(cin.peek() < 0){
                cin.get(ch); 
                ch = -1; 
            }
            else if (ch == ' ')
            {
                command += ' ';
                cin.get(ch); 
            }    
                else if (ch == '(')
            {
                command += '\\';
                command += '(';
                cin.get(ch); 
            }
            else if (ch == ')')
            {
                command += '\\';
                command += ')';
                cin.get(ch); 
            }       
         
    }
}
void evalbackslashfile(ifstream &file,char &ch, string &command, vector<string> &argumetns)
{
  if (ch == '\\')
        {         
            
            if ((file.peek() == 10 ||file.peek() < 0) && ch == '\\'){
                argumetns.push_back("\\");           
            }
            file.get(ch);
            if(file.peek() < 0){
                file.get(ch); 
                ch = -1; 

            }
            else if (ch == ' ')
            {
                command += ' ';
                file.get(ch); 
            }    
                else if (ch == '(')
            {
                command += '\\';
                command += '(';
                file.get(ch); 
            }
            else if (ch == ')')
            {
                command += '\\';
                command += ')';
                file.get(ch); 
            }       
         
    }
}
void evalbackslashflaggedfile(ifstream &file, char &ch, string &command)
{  
//Evalulate with -e flag (interpert backslashes) file input 
  if (ch == '\\')
        {              
            file.get(ch);

                if (file.peek() < 0){
                ch = -1; 
                return; 
                }
            
            switch (ch)
            {
            case ' ':
                file.get(ch);
                command += ' ';
                break;
            case ')':
                file.get(ch);
                command += ')';
                break; 
            case '(':
                file.get(ch);
                command += '(';
                break; 
            default:
                break;
            }        
        }
}
void parseArguments(char *argv[])

{
    vector<string> arguments; 
    char ch; 
    string command;
    
    while (cin.peek() > 0)
    { 
        cin.get(ch);      
        if (arguments.size() > 2 && arguments[2] == "-e" )
        {
            evalbackslashflagged(ch,command);
        }else
            evalbackslash(ch,command,arguments);
        if(ch == '(' || ch == ')')
        {
             if (command.size() > 0){             
            arguments.push_back(command);
            command.clear(); 
             }
             
             if(ch == '(' ){
             arguments.push_back("(");

             }else {
             arguments.push_back(")");

            }            
        } 
        if(ch != ')' && ch != '(' &&  !isspace(ch) )
        {                                  
            command += ch;  
        }else
        {              
            if (command.size() > 0){             
            arguments.push_back(command);          
            }           
            command.clear();
            
             
        }
        /*
        end of run of line or end of file input. Run and pass command to execute(): 
        evaulate paras, ecspace chars, format vector<string> arguments for execute.
        on success end program or countiue to newline
        on fail printError()
        */

        if ((cin.peek() == 10 || cin.peek() < 0) && arguments.size() > 0)
        {       
            {
                vector<string> copy = arguments; 
                if ( arguments[1] != "(" || arguments[arguments.size()-1] != ")")
                {
                    printError(arguments,argv); 
                    return; 
                } 
                arguments.erase(arguments.begin()+1);  
                arguments.erase(arguments.end());
                if (arguments[1] != "-e")
                {             
                    for (string &s : arguments)
                    {
                        for (size_t i = 0; i < s.size(); i++)
                        {
                            if( s[i] == ')' || s[i] == '(' )
                            {
                                if (s[i-1] != '\\')
                                {
                                    
                                    printError(copy,argv);
                                    return; 
                                }else{
                                    s.erase(i-1,i);                                      
                                }
                            }
                        }
                        
                    }
        }
  
            if (execute(arguments))
            {               
                
            } else{
                printError(copy, argv);             
                return;               
            }
            arguments.clear(); 
        }
    }// if end peek == 0 or peek == 10        

    }//while end
}
void parseFile(ifstream &file,char *argv[])
{
    vector<string> arguments;
    char ch;
    string command; 
    while (file.peek() > 0)
    { 
        file.get(ch);      
        if (arguments.size() > 2 && arguments[2] == "-e" )
        {
            evalbackslashflaggedfile(file,ch,command);
        }else
            evalbackslashfile(file,ch,command,arguments);
        if(ch == '(' || ch == ')')
        {
             if (command.size() > 0){             
            arguments.push_back(command);
            command.clear(); 
             }
             
             if(ch == '(' ){
             arguments.push_back("(");

             }else {
             arguments.push_back(")");

            }            
        } 
        if(ch != ')' && ch != '(' &&  !isspace(ch) )
        {                                  
            command += ch;  
        }else
        {              
            if (command.size() > 0){             
            arguments.push_back(command);          
            }           
            command.clear();
            
             
        }
 
        /*
        end of run of line or file/std input. Run and pass command to execute(): 
        evaulate paras, ecspace chars, format vector<string> arguments for execute.
        on success end program or countiue to newline
        on fail printError()
        */
        if ((file.peek() == 10 || file.peek() < 0) && arguments.size() > 0)  
        {       
            {         
                vector<string> copy = arguments; 
            
                if ( arguments[1] != "(" || arguments[arguments.size()-1] != ")")
                {
                    printError(arguments,argv); 
                    return; 
                } 
                arguments.erase(arguments.begin()+1);  
                arguments.erase(arguments.end());
                if (arguments[1] != "-e")
                {             
                    for (string s : arguments)
                    {
                        for (size_t i = 0; i < s.size(); i++)
                        {
                            if( s[i] == ')' || s[i] == '(' )
                            {
                                if (s[i-1] != '\\')
                                {
                                    printError(copy,argv);
                                    return; 
                                }else{
                                    s.erase(i-1,i);  
                                }
                            }
                        }
                        
                    }
        }
 
            if (execute(arguments))
            {               
                
            } else{
                printError(copy, argv);             
                return;               
            }
            arguments.clear(); 
        }
    }// if end peek == 0 or peek == 10        

    }//while end
    return; 
}
int main (int argc, char *argv[]){
string s; 
if (argc > 1 ) //From file?
    {        
    for (auto i = 1; i < argc; i++)
    {            
         s = argv[i];   
         ifstream in(s);
         if (in)
         {
              parseFile(in,argv);
         }else{
             cerr << argv[0] <<": can't read from: " << s << "\n"; 
             return 1;
              
         }         
    }         
    }else
        parseArguments(argv); //std input

return 0;
}

