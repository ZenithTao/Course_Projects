/*************************************************************************
 *  THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
 *  CODE WRITTEN BY OTHERS. LIANG TAO
 *  DATE SUBMIITED: 10/26/2013
 *  Compilation:  javac -cp :./stdlib.jar CommentStripper.java
 *  Execution:    java -cp :./stdlib.jar CommentStripper < source.java
 *  Dependencies: StdIn.java
 *  
 *  Reads in a Java program and removes all of the comments using a 5
 *  state finite state automaton.
 *************************************************************************/

public class CommentStripper { 
    public static void main(String[] args) {
        final int CODE  = 0;   // parsing normal code
        final int SLASH = 1;   // found a leading '/'
        final int BLOCK = 2;   // in a block comment
        final int LINE  = 3;   // in a line comment
        final int STAR  = 4;   // found a trailing * in a block comment

        int state = CODE;      // current state

        while(!StdIn.isEmpty()) {
            char c = StdIn.readChar();
            switch(state) {
                case CODE:  if      (c == '/')  { state = SLASH;                           }
                            else if (c == '"')  { state = CODE; System.out.print(c);       }
                            else                { System.out.print(c);                     }
                            break;

                case SLASH: if      (c == '*')  { state = BLOCK;                           }
                            else if (c == '/')  { state = LINE;                            }
                            else                { state = CODE; System.out.print("/" + c); }
                            break;

                case BLOCK: if      (c == '*')  { state = STAR;                            }
                            else if (c == '"')  { state = BLOCK;                           }
                            break;

                case STAR:  if      (c == '/')  { state = CODE; System.out.print(" ");     }
                            else if (c == '*')  { state = STAR;                            }
                            else                { state = BLOCK;                           }
                            break;

                case LINE:  if      (c == '\n') { state = CODE; System.out.println();      }
                            break;
             }
        }
    }

}
