#include <stdio.h>
#include <stdlib.h>


int calcstatus = 1;
int poldegree[2][6];
int orderarray[6];
char firstnchar[20];

void copystring(char * source, char * destination);
void shiftbuffer(char * sbuffer, int count);
int isequal(char * stringone, char * stringtwo);
void clearstring(char * clearbuffer);
char * firstncharacters(char * stringsource, int ncharacters);
int specialcharindex(char * stringbuffer, char specialchar);
int getdegreevalue(char character);
void parseCommand(char * commandphrase);
int parseterms(char * buffer);
void calculatedegrees(char * polynom, int arrayselection);
int checkorder(int * array);
void addpolinoms();
void derpolinom();
void mulpolinoms();
void clearbuffers();
void writeresult(int * array, int count);

int main()
{
    char command[3];
    char firstpol[50];
    char secondpol[50];
    char readline[100];

    while(calcstatus)
     {
        printf("\n>> ");
        gets(readline);

        sscanf(readline, "%s %s %s", command, firstpol, secondpol);
        fflush(stdin);
        if(isequal(command, "QUIT"))
            calcstatus = 0;

        if(calcstatus)
        {
            clearbuffers();
            calculatedegrees(firstpol, 0);
            calculatedegrees(secondpol, 1);
            parseCommand(command);
        }
     }

    return 0;
}

void parseCommand(char * commandphrase)
{
    if(isequal(commandphrase, "ADD"))
        addpolinoms();
    else
    if(isequal(commandphrase, "SUB"))      //for sub operation, multiply second pol with -1 then addpolinoms
        {
            int k;
            for(k = 0; k < 6; k++)
            {
                poldegree[1][k] = -1 * poldegree[1][k];
            }
        addpolinoms();
        }
    else
    if(isequal(commandphrase, "DER"))
        derpolinom();
    else
    if(isequal(commandphrase, "MUL"))
        mulpolinoms();
}

void addpolinoms()
{
    int tempmultiple[6];
    int looper;

    for(looper = 4; looper >= 0; looper--)
    {
        tempmultiple[looper] = 0;
    }
    int total = 0;

    for(looper = 5; looper >= 0; looper--)
    {
        tempmultiple[looper] = poldegree[0][looper] + poldegree[1][looper];

        if(tempmultiple[looper] == 0)
        {
            total++;
        }
    }

    writeresult(tempmultiple, 5);
}

void mulpolinoms()
{
        int tempmultiple[11];
        int firstcursor;
        int secondcursor;
        //int total = 0;

    for(firstcursor = 10; firstcursor >= 0; firstcursor--)
    {
        tempmultiple[firstcursor] = 0;
    }

        for(firstcursor = 5; firstcursor >= 0; firstcursor--)
         {
            for(secondcursor = 5; secondcursor >= 0; secondcursor--)
            {
                if(poldegree[0][firstcursor] != 0)
                {
                    if(poldegree[1][secondcursor] != 0)
                    {
                        tempmultiple[firstcursor + secondcursor] += poldegree[0][firstcursor] * poldegree[1][secondcursor];
                    }

                }
            }
        }

    writeresult(tempmultiple, 10);
}

void derpolinom()
{
    int tempmultiple[5];
    int looper;

    for(looper = 4; looper >= 0; looper--)
    {
        tempmultiple[looper] = 0;
    }

    for(looper = 5; looper >= 1; looper--)
    {
        tempmultiple[looper -1] = looper * poldegree[0][looper];
    }

    writeresult(tempmultiple, 4);
}

int stringlength(char * string)
{
    int length = 0;

    while(string[length] != '\0')
    {
        length++;
    }

    return length;
}

void calculatedegrees(char * polynom, int arrayselection)
{
    int charloop = 1;
    int counterm = 0;
    int location = -2;
    char tempbuffer[stringlength(polynom)];

    int xyz;
    for(xyz = 0; xyz < 6; xyz++)
    {
        orderarray[xyz] = -1;
    }

    while(charloop != -3)
    {
       charloop = parseterms(polynom);
       if(charloop == -3) //end of pol
       {
            clearstring(tempbuffer);
            copystring(polynom, tempbuffer);
       }
       else
       {
            clearstring(tempbuffer);
            copystring(firstncharacters(polynom, charloop), tempbuffer);
            shiftbuffer(polynom, charloop);
       }

       location = specialcharindex(tempbuffer, 'X');

        if(location == 0)
        {   if(tempbuffer[location + 1] == '^')
            {
                poldegree[arrayselection][getdegreevalue(tempbuffer[location + 2])] = 1;
                orderarray[getdegreevalue(tempbuffer[location + 2])] = counterm;
            }
            else
            {
                poldegree[arrayselection][1] = 1;
                orderarray[1] = counterm;
            }
        }
        else if(location == 1)
        {
            if(tempbuffer[location + 1] == '^')
            {
                if(tempbuffer[location - 1] == '-')
                {
                    poldegree[arrayselection][getdegreevalue(tempbuffer[location + 2])] = -1;
                    orderarray[getdegreevalue(tempbuffer[location + 2])] = counterm;
                }
                else
                {
                    poldegree[arrayselection][getdegreevalue(tempbuffer[location + 2])] = atoi(firstncharacters(tempbuffer, location));
                    orderarray[getdegreevalue(tempbuffer[location + 2])] = counterm;
                }
            }

            if(tempbuffer[location + 1] != '^')
            {
                if( tempbuffer[location - 1] == '-')
                {
                    poldegree[arrayselection][1] = -1;
                    orderarray[1] = counterm;
                }
                else
                {
                    poldegree[arrayselection][1] = atoi(firstncharacters(tempbuffer, location));
                    orderarray[1] = counterm;
                }
            }
        }
        else if(location > 1) //there is a mult
        {
            if(tempbuffer[location + 1] == '^')
            {
                poldegree[arrayselection][getdegreevalue(tempbuffer[location + 2])] = atoi(firstncharacters(tempbuffer, location));
                orderarray[getdegreevalue(tempbuffer[location + 2])] = counterm;
            }

            else
            {
                poldegree[arrayselection][1] = atoi(firstncharacters(tempbuffer, location));
                orderarray[1] = counterm;
            }
        }

        else if(location == -1)
        {
                poldegree[arrayselection][0] = atoi(tempbuffer);
                orderarray[0] = counterm;
        }
        counterm++;
    }

    if(checkorder(orderarray) == 0)
    {
           printf("\nUNORDERED INPUT FOR %d.POLYNOM!\n", arrayselection + 1);
    }

}

int checkorder(int * array)
{
    int index = 0;
    int loop = 0;
    int ordered = 1;
    int temparray[6];

    for(index = 0; index < 6; index++)
    {
        if(array[index] != -1)
        {
            temparray[loop++] = array[index];
        }
    }
    index = 0;

    while(index < loop -1 )
    {
        if(temparray[index] < temparray[index +1])
        {
            ordered = 0;
            break;
        }

        index++;
    }

    return ordered;

}

void copystring(char * source, char * destination)
{
        int copycursor = 0;

            while(copycursor < stringlength(source))
            {
                destination[copycursor] = source[copycursor];

                copycursor++;
            }
}

void clearstring(char * clearbuffer)
{
        int clearcursor = 0;

        while(clearcursor < stringlength(clearbuffer))
        {
            clearbuffer[clearcursor] = ' ';
            clearcursor++;
        }
}

int parseterms(char * buffer)
{
    int indexvalue = 0;
    int xcursor;
    int found = 0;

    for(xcursor = 1; xcursor <= stringlength(buffer); xcursor++)
    {
        if(buffer[xcursor] == '+')
        {
            found = 1;
            indexvalue = xcursor + 1; //ignore + signed before the number
            break;
        }
        if(buffer[xcursor] == '-')
        {
            found = 1;
            indexvalue = xcursor;
            break;
        }
        if(buffer[xcursor] == '\0')
        {
            found = 1;
            indexvalue = -3;
            break;
        }

    }

    if(found == 1)
    {
        return indexvalue;
    }
    else
    {
        return 0;
    }
}

void shiftbuffer(char * sbuffer, int count)
{
    char tempbuffer[stringlength(sbuffer)];
    clearstring(tempbuffer);

    int loopcur = 0;

    while(loopcur < (stringlength(sbuffer) - count))
    {
        tempbuffer[loopcur] = sbuffer[loopcur + count];
        loopcur++;
    }

    clearstring(sbuffer);
    copystring(tempbuffer, sbuffer);
}

int isequal(char * stringone, char * stringtwo)
{
    if(stringlength(stringone) == 0 || stringlength(stringtwo) == 0 || stringlength(stringone) != stringlength(stringtwo))
        return 0;

    int compareloop = 0;

    while(compareloop < stringlength(stringone))
    {
        if(stringone[compareloop] != stringtwo[compareloop])
        break;
        compareloop++;
    }

    if(compareloop == stringlength(stringone))
        return 1;
    else
        return 0;
}

char * firstncharacters(char * stringsource, int ncharacters)
{
    clearstring(firstnchar);
    int nloop = 0;

    if(stringlength(stringsource) >= ncharacters)
    {
        while(nloop < ncharacters)
        {
            firstnchar[nloop] = stringsource[nloop];
            nloop++;
        }

    }

    return firstnchar;
}

int specialcharindex(char * stringbuffer, char specialchar)
{
    int index = -1;
    int xcursor = 0;

    for(xcursor = 0; xcursor < stringlength(stringbuffer); xcursor++)
    {
        if(stringbuffer[xcursor] == specialchar)
        {
            index = xcursor; //ignore + signed before the number
            break;
        }

    }
    return index;
}

int getdegreevalue(char character)
{
    int result = 0;

    result = character - 48; //48 equals char '0'

    return result;
}

void clearbuffers()
{
    int k, l;

    for(k = 0; k < 2; k++)
        for(l = 0; l < 6; l++)
            poldegree[k][l] = 0;

}

void writeresult(int * array, int count)
{
    int looper;
    int firstcharacter = 1;
    int total = 0;

    printf("\nResult is : ");

    for(looper = count; looper > 1; looper--)
    {
        if(array[looper] != 0)
        {
            if(firstcharacter)
            {
                if(array[looper] == -1)
                {
                    printf("-X^%d", looper);
                }
                else
                if(array[looper] == 1)
                {
                        printf("X^%d", looper);
                }
                else
                {
                    printf("%dX^%d", array[looper], looper);
                }
                firstcharacter = 0;
            }
            else
            {
                if(array[looper] == 1)
                     printf("+X^%d", looper);
                else
                if(array[looper] == -1)
                    printf("-X^%d", looper);
                else
                if(array[looper] > 0)
                    printf("+%dX^%d", array[looper], looper);
                else
                    printf("%dX^%d", array[looper], looper);
            }
        }
        else
        {
            total++;
        }
    }
    if(array[1] != 0)
    {
        if(array[1] == 1)
        {
            if(firstcharacter == 1)
                printf("X");
            else
                printf("+X");
        }
        else
        if(array[1] == -1)
            printf("-X");

        if(array[1] > 1)
        {
            if(firstcharacter == 1)
                printf("%dX", array[1]);
            else
                printf("+%dX", array[1]);
        }
        else
        if(array[1] < -1)
        {
            printf("%dX", array[1]);
        }
    }
    else
    {
        total++;
    }

    if(array[0] != 0)
    {
         if(array[0] > 0)
         {
             if(firstcharacter == 1)
                printf("%d", array[0]);
             else
                printf("+%d", array[0]);
         }
        else
        {
            printf("%d", array[0]);

        }
    }
    else
    {
        total++;
    }
    if(total == count + 1)
    {
        printf("0");
    }
    printf("\n");
}
