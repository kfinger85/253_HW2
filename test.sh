#! /bin/bash
{
    echo ____________________________________________DATA RUN____________________________________________
    ./hw2 < data 
    echo ____________________________________________ALPHA RUN____________________________________________
    echo -e ' \n\r\techo\f(\talpha\fbeta\v333480) ' | ./hw2
    echo ____________________________________________CLOSING PARAN____________________________________________
    echo -e ' echo()) ' | ./hw2 
    echo ____________________________________________OPENING PARAN____________________________________________
    echo  'echo (\(\))' | ./hw2
    echo ____________________________________________BACKSLASH____________________________________________
    echo 'echo(hello)\' | ./hw2
    echo ____________________________________________TEST5____________________________________________
    echo  -e 'echo(\\)' | ./hw2
    echo ____________________________________________FINAL____________________________________________
    ./hw2 data
    ./hw2 data command
    echo ____________________________________________END FINAL____________________________________________
    ./hw2 fgh
}