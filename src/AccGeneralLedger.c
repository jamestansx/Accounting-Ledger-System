/**
 * @file AccGeneralLedger.c
 * @author Tan Shao Xiang <James Tan> (jamestansx@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-01-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

#define MAXSIZE 1024
#define MAXWORD 1024
#define FILEREQUIRED 10
#define MAXNUMLEN 4

typedef struct USERINFO
{
    char positionlist[MAXSIZE];
    char username[MAXSIZE];
    char password[MAXSIZE];
    char *jobposition;
    char companyName[MAXSIZE];
} signupInfo, loginInfo;

typedef struct ptr
{
    char *ptr;
} ptr;
typedef struct FILES
{
    char filename[MAXSIZE];

} filePath;

typedef struct WELCOME_SCREEN
{
    int resolutions[2];
    char *titles;
    char *text_1;
    char *text_2;
} welScreenInfo;

struct date
{
    int day[MAXSIZE];
    int month[MAXSIZE];
    int year[MAXSIZE];
};

typedef struct LEDGER
{
    char ledgerCode[MAXSIZE][MAXSIZE];
    char description[MAXSIZE][MAXSIZE];
} ledgerCode;
typedef struct LEDGER_INFO
{
    char accNumber[MAXSIZE];
    struct date dt;
    ledgerCode ledgerCode;
    char description[MAXSIZE * 2];
    int approval;

} ledgerInfo, nledgerCode, ledgerApproval;

typedef struct LEDGER_DETAILS
{
    char description[MAXSIZE][MAXSIZE * 2];
    char ledgerCode[MAXSIZE][MAXSIZE];
    struct date dt;
    double credit_debit[MAXSIZE];
} ledger_Details;

struct checkStatus
{
    int status;
    char date[MAXSIZE];
    int approved;
};

FILE *openFileR(char file_name[MAXSIZE])
{
    FILE *file;
    if (!(file = fopen(file_name, "r")))
    {
        printf("Error in locating the file. Please try again later.\n");
        Sleep(1000);
        exit(1);
    }
    return file;
}

FILE *openFileA(char file_name[MAXSIZE])
{
    FILE *file;
    if (!(file = fopen(file_name, "a+")))
    {
        printf("Error in locating the file. Please try again later.\n");
        Sleep(1000);
        exit(1);
    }
    return file;
}

filePath **signUpInfo(void)
{
    filePath **signUpInfo = (filePath **)malloc(FILEREQUIRED * sizeof(filePath *));
    for (int j = 0; j < FILEREQUIRED; ++j)
        signUpInfo[j] = (filePath *)malloc(FILEREQUIRED * sizeof(filePath));

    signUpInfo[1] = "userInfo.txt";
    signUpInfo[0] = "appdata.txt";
}

filePath **ledgerListFile()
{
    filePath **ledgerFileListName = (filePath **)malloc(FILEREQUIRED * sizeof(filePath *));
    for (int i = 0; i < FILEREQUIRED; ++i)
        ledgerFileListName[i] = (filePath *)malloc(FILEREQUIRED * sizeof(filePath));

    ledgerFileListName[0] = "ledgerList.txt";
}

filePath **ledgerCodeFile()
{
    filePath **ledgerCodeListName = (filePath **)malloc(FILEREQUIRED * sizeof(filePath *));
    for (int i = 0; i < FILEREQUIRED; ++i)
        ledgerCodeListName[i] = (filePath *)malloc(FILEREQUIRED * sizeof(filePath));

    ledgerCodeListName[0] = "ledgerCode.txt";
}

signupInfo **jobPositionList(void)
{
    signupInfo **jobPositionList = (signupInfo **)malloc(3 * sizeof(signupInfo *));
    for (int i = 0; i < 3; ++i)
        jobPositionList[i] = (signupInfo *)malloc(3 * sizeof(signupInfo));

    jobPositionList[0] = "Accounting Manager";
    jobPositionList[1] = "Senior Accountant";
    jobPositionList[2] = "Staff Accountant";

    return jobPositionList;
}

int getScreenSize(int resolutions[2])
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    resolutions[0] = rows;
    resolutions[1] = columns;
}

const char *genAccNumber(filePath **appdatafilename)
{
    ledgerInfo *accNumber = malloc(sizeof(*accNumber));
    FILE *appdatafile = openFileR("appdata.txt");
    FILE *tmpfile = fopen("temp.tmp", "w");
    static char accNumName[MAXNUMLEN];
    char ctmp[MAXSIZE], c[1024];
    int num[MAXNUMLEN], count = 0, temp, cur_line, tmpc; //xxxx (num1, num2, num3, num4)

    fscanf(appdatafile, "%d\n", &temp);
    fprintf(tmpfile, "%d", temp);
    do
    {
        fscanf(appdatafile, "%s", ctmp);
        if (ctmp[3] == '\0')
        {
            num[0] = 0;
            num[1] = 0;
            num[2] = 0;
            num[3] = 0;
            goto skip;
        }
        count++;
    } while (!feof(appdatafile));
    count = 0;
    for (int i = 0; i < MAXNUMLEN; i++)
    {
        num[i] = ctmp[i] - '0';
    }
skip:
    num[3]++;
    if (num[3] == 10)
    {
        num[2]++;
        num[3] = 0;
    }
    if (num[2] == 10)
    {
        num[1]++;
        num[2] = 0;
    }
    if (num[1] == 10)
    {
        num[0]++;
        num[1] = 0;
    }
    if (num[0] == 10)
    {
        printf("Oopss... Acc Number out of range...");
        _Exit(-1);
    }
    for (int i = 0; i < MAXNUMLEN; i++)
    {
        accNumName[i] = num[i] + '0';
    }

    strncpy(accNumber->accNumber, accNumName, strlen(accNumName));
    accNumber->accNumber[strlen(accNumName)] = '\0';
    fprintf(tmpfile, "\n");
    for (int i = 0; i < 4; i++)
        fprintf(tmpfile, "%c", accNumber->accNumber[i]);

    fclose(tmpfile);
    fclose(appdatafile);
    remove(appdatafilename[0]);
    //system("del /f appdata.txt");
    rename("temp.tmp", "appdata.txt");

    return accNumName;
}

void welcomeScreen(void)
{
    welScreenInfo welcome;
    system("cls");
    getScreenSize(welcome.resolutions);
    int sleeptime = 1;
    welcome.titles = "    Accounting Ledger System";
    welcome.text_1 = "                By";
    welcome.text_2 = "              Group 6";
    int dot_no = strlen(welcome.titles) + 6;
    int rows = (welcome.resolutions[0] - 5) / 2;
    int columns = (welcome.resolutions[1] - dot_no - 1) / 2;

    for (int i = 0; i < rows; i++)
        putchar('\n');
    for (int i = 0; i < columns; i++)
        printf(" ");
    for (int i = 0; i < dot_no; i++)
    {
        putchar('*');
        Sleep(sleeptime);
    }
    printf("\n");
    for (int i = 0; i < columns; i++)
        printf(" ");
    for (int i = 0; welcome.titles[i] != '\0'; i++)
    {
        printf("%c", welcome.titles[i]);
        Sleep(sleeptime);
    }
    printf("\n");
    for (int i = 0; i < columns; i++)
        printf(" ");
    for (int i = 0; welcome.text_1[i] != '\0'; i++)
    {
        printf("%c", welcome.text_1[i]);
        Sleep(sleeptime);
    }
    printf("\n");
    for (int i = 0; i < columns; i++)
        printf(" ");
    for (int i = 0; welcome.text_2[i] != '\0'; i++)
    {
        printf("%c", welcome.text_2[i]);
        Sleep(sleeptime);
    }
    printf("\n");
    for (int i = 0; i < columns; i++)
        printf(" ");
    for (int i = 0; i < dot_no; i++)
    {
        putchar('*');
        Sleep(sleeptime);
    }
    printf("\n");
    for (int i = 0; i < columns; i++)
        printf(" ");
    printf("    (Press any key to continue)");
    int ch;
    while (1)
    {

        if (kbhit())
        {
            ch = getch();
            system("cls");
            break;
        }
    }
}

int checkUserInfo(loginInfo *ptr, FILE *usrinfofile)
{
    char *userName = malloc(sizeof(char));
    char *Password = malloc(sizeof(char));
    char *jobPosition = malloc(sizeof(char));
    char *jobPosition1 = malloc(sizeof(char));
    char *Position = malloc(sizeof(char));
    char *userValid = malloc(sizeof(char));
    userValid = ptr->username;
    char *pwdValid = malloc(sizeof(char));
    pwdValid = ptr->password;
    char readString[MAXSIZE];
    int cur_line;
    int loginStatus = 0;

    while (fgets(readString, sizeof(readString), usrinfofile) != NULL)
    {
        if (cur_line >= 1 && !feof(usrinfofile))
        {
            sscanf(readString, "%s %s %[^\n]\n", userName, Password, jobPosition);
            if (strcmp(userName, userValid) == 0 && *Password == *pwdValid)
            {
                loginStatus = TRUE;
                if (strcmp(jobPosition, "Accounting Manager") == 0)
                    loginStatus = 2;
            }
            else
                continue;
        }
        cur_line++;
    }
    return loginStatus;
}

void loginScreen(signupInfo *signUp, FILE *filename, int signUpInt)
{
    loginInfo *login = malloc(sizeof(*login));
    welScreenInfo loginR;
    getScreenSize(loginR.resolutions);
    char *loginTitles = "      To Login To The System.";
    char *loginTitles2 = "              Sign In";
    int rows = (loginR.resolutions[0] - 5) / 2;
    int columns = (loginR.resolutions[1] - strlen(loginTitles) - 6) / 2;
    int dot_no = strlen(loginTitles) + 6;
    int status = 0;
    if (signUpInt == 1)
    {
        while (status <= 3)
        {
            for (int i = 0; i < rows; i++)
                putchar('\n');
            for (int i = 0; i < columns; i++)
                printf(" ");
            printf("%s\n", loginTitles2);
            for (int i = 0; i < columns; i++)
                printf(" ");
            printf("%s\n", loginTitles);
            for (int i = 0; i < columns; i++)
                printf(" ");
            for (int i = 0; i < dot_no; i++)
                putchar('=');
            putchar('\n');
            for (int i = 0; i < columns; i++)
                printf(" ");
            printf("  Username: ");
            scanf(" %[^\n]%*c", login->username);
            for (int i = 0; i < columns; i++)
                printf(" ");
            printf("  Password: ");
            for (int i = 0; i < 15; i++)
            {
                char pwd = getch();
                if (pwd != '\r' && (int)(pwd) != 8 && (int)(pwd) != '\000')
                {
                    if (pwd != '\r')
                        login->password[i] = pwd;
                }
                else if ((int)(pwd) == 8)
                {
                    if (i >= 0)
                    {
                        login->password[i - 1] = '\0';
                        --i;
                        --i;
                    }
                }
                if (pwd == 13)
                {
                    break;
                }
            }
            status = checkUserInfo(login, filename);
            int attempt = 0;
            rewind(filename);
            if (status == 1)
            {
                system("cls");
                staff_Menu(login);
            }
            else if (status == 2)
            {
                system("cls");
                admin_Portal(login);
            }
            else if (attempt <= 3)
            {
                system("cls");
                printf("Login Failed. Please Try Again.");
                attempt++;
            }
            else
            {
                printf("Login Failed.. Please try again later.");
                exit(1);
            }
        }
    }
    else if (signUpInt == 0)
    {
        if (strcmp(signUp->jobposition, "Accounting Manager") == 0)
            admin_Portal(signUp);
        else
            staff_Menu(signUp);
    }
    fclose(filename);
}

int admin_Portal(loginInfo *user)
{
    system("cls");
    welScreenInfo menu;
    char *welcome = "   welcome to Admin Portal";
    char *option = " Please select an option:\n";
    char *accMain = "<crtl+n>\tEmployee List\n";
    char *accUp = "<crtl+e>\tAccount Approval Process\n";
    char *GenReport = "<crtl+p>\tReport Generation Process\n";
    char *exit = "<crtl+x>\tExit\n";
    getScreenSize(menu.resolutions);
    int dot_no = strlen(GenReport) + 6;
    int rows = (menu.resolutions[0] - 5) / 2;
    int columns = (menu.resolutions[1] - dot_no - 1) / 2;

repeat:
    for (int i = 0; i < rows; i++)
        putchar('\n');
    for (int i = 0; i < columns; i++)
        printf(" ");
    for (int i = 0; i < dot_no; i++)
    {
        putchar('-');
    }
    printf("\n");
    for (int i = 0; i < columns; i++)
        printf(" ");
    printf("%s %s", welcome, user->username);
    printf("\n");
    for (int i = 0; i < columns; i++)
        printf(" ");
    for (int i = 0; i < dot_no; i++)
    {
        putchar('-');
    }
    printf("\n");
    for (int i = 0; i < columns; i++)
        printf(" ");
    printf("%s\n", option);
    for (int i = 0; i < columns; i++)
        printf(" ");
    printf("%s\n", accMain);
    for (int i = 0; i < columns; i++)
        printf(" ");
    printf("%s\n", accUp);
    for (int i = 0; i < columns; i++)
        printf(" ");
    printf("%s\n", GenReport);
    for (int i = 0; i < columns; i++)
        printf(" ");
    printf("%s\n", exit);
    char ch;
    int STATUS = FALSE;
    while (STATUS == FALSE)
    {
        if (kbhit())
        {
            ch = getch();
            switch ((int)(ch))
            {
            case 14: //crtl + n
                STATUS = employeeList(jobPositionList());
                system("cls");
                goto repeat;
                break;

            case 5: //crtl + e
                STATUS = approvalLedger(ledgerListFile());
                system("cls");
                goto repeat;
                break;

            case 16: //crtl + p
                STATUS = ReportGenerationMenu();
                system("cls");
                goto repeat;
                break;

            case 24: //crtl + x
                _Exit(0);
            }
        }
    }
}

void signUpScreen(signupInfo **post, filePath **filePath)
{
    FILE *usrinfofile = openFileA(filePath[1]);
    FILE *appdatafile = openFileA(filePath[0]);
    int status = 0;
    fscanf(appdatafile, "%d", &status);
    signupInfo *signUp = malloc(sizeof *signUp);

    if (status != 0)
    {
        fclose(appdatafile);
        loginScreen(signUp, usrinfofile, 1);
        fclose(usrinfofile);
    }
    else
    {
        printf("Signing up for the first time user.\n");
        printf("Please enter your company name, username, password and position.\n");
        printf("Company name: ");
        scanf(" %[^\n]%*c", signUp->companyName);
        printf("Username: ");
        scanf(" %[^\n]%*c", signUp->username);
        printf("Password: ");

        for (int i = 0; i < 15; i++)
        {
            char pwd = getch();
            if (pwd != '\r' && (int)(pwd) != 8 && (int)(pwd) != '\000')
            {
                signUp->password[i] = pwd;
            }
            else if ((int)(pwd) == 8)
            {
                if (i >= 0)
                {
                    signUp->password[i - 1] = '\0';
                    --i;
                    --i;
                }
            }
            else if (pwd == 13)
            {
                break;
            }
        }
        printf("\n");
        printf("(Please use arrow key to select)\n");
        printf("Postition: %s", post[0]);
        char ch;
        int i = 0;
        do
        {
            if (kbhit())
            {
                ch = getch();
                if ((int)(ch) == 80)
                {
                    if (i == 3)
                        i = 0;
                    else
                        i++;
                    if (i == 3)
                        i--;

                    fflush(stdout);
                    printf("\rPostition: %s  ", post[i]);
                }
                else if ((int)(ch) == 72)
                {
                    if (i == -1)
                        i = 2;
                    else if (i > 0)
                        i--;

                    fflush(stdout);
                    printf("\rPostition: %s  ", post[i]);
                }
            }
        } while ((int)(ch) != 13);

        signUp->jobposition = post[i];
        fprintf(usrinfofile, "%s\n", signUp->companyName);
        fprintf(usrinfofile, "%s\t%s\t%s\n\n", signUp->username, signUp->password, signUp->jobposition);
        fprintf(appdatafile, "1\n");
        fclose(usrinfofile);
        fclose(appdatafile);

        loginInfo *name = malloc(sizeof(*name));

        memcpy(name->username, signUp->username, sizeof(signUp->username));
        loginScreen(signUp, usrinfofile, 0);
        free(signUp);
    }
}

void viewledgerCode(filePath **viewledgerCode)
{
    system("cls");
    FILE *file = openFileR(viewledgerCode[0]);
    char readString[1024];
    char *ledgerCode = malloc(sizeof(*ledgerCode));
    char *ledgerDetails = malloc(sizeof(*ledgerDetails));
    printf("-----LEDGER CODE VIEW-----\n");
    printf("LEDGER CODE\t\tDESCRIPTION\n");

    while (fgets(readString, sizeof(readString), file) != NULL)
    {
        sscanf(readString, "%s %[^\n]\n", ledgerCode, ledgerDetails);
        printf("%s\t\t\t%s\n", ledgerCode, ledgerDetails);
    }
    fclose(file);
    printf("(Press any key to continue)");
    int ch;
    while (1)
    {

        if (kbhit())
        {
            ch = getch();
            system("cls");
            break;
        }
    }
}

void createNewLedgerCode(filePath **ledgerCodeFile)
{
    nledgerCode *newCode = malloc(sizeof(*newCode));
    char ch;
    int numCreated = 0;
    void saveFile()
    {
        FILE *lCodeFile = openFileA(ledgerCodeFile[0]);
        for (int i = 0; i < numCreated; i++)
        {
            fprintf(lCodeFile, "%s\t%s\n", newCode->ledgerCode.ledgerCode[i], newCode->ledgerCode.description[i]);
        }
        fclose(lCodeFile);
    }

    void summary()
    {
        printf("Summary:\n");
        for (int i = 0; i < numCreated; i++)
        {
            printf("Account Created: %s\n", newCode->ledgerCode.ledgerCode[i]);
            printf("Description: %s\n", newCode->ledgerCode.description[i]);
        }
    }

    void input()
    {
        int err = 0;
        int *code;
        char input;
        int x = 0;
        printf("Please enter the ledger code number(xxxxxx): ");

        while ((int)(input) != 13)
        {
            input = getch();
            if ((int)(input) != 13)
            {
                printf("%c", input);
                //memcpy(newCode->ledgerCode.ledgerCode[numCreated][x], input, sizeof(input));
                newCode->ledgerCode.ledgerCode[numCreated][x] = input;
                x++;
            }
            else
                break;
        }

        printf("\nPlease enter description: ");
        scanf_s(" %[^\n]%*c", newCode->ledgerCode.description[numCreated]);
        numCreated++;
    }
    int exitStatus = 0;
    while (exitStatus != TRUE)
    {
        system("cls");
        printf("***********************************\n");
        printf("Create new Ledger Code\n");
        printf("***********************************\n");
        printf("<crtl+n>:Add New\t<crtl+x>:Save\t<esc>:exit\n");

        if (numCreated == 0)
        {
            input();
            //system("cls");
            continue;
        }
        summary();

        do
        {
            if (kbhit())
            {
                ch = getch();

                if ((int)(ch) == 14) // crtl + n
                {
                    system("cls");
                    printf("***********************************\n");
                    printf("Create new Ledger Code(s)\n");
                    printf("***********************************\n");
                    printf("<crtl+n>:Add New\t<crtl+x>:Save\t<esc>:exit\n");
                    input();
                    break;
                }
                else if ((int)(ch) == 24) // crtl + x
                {
                    saveFile();
                    exitStatus = TRUE;
                    break;
                }
                else if ((int)(ch) == 27) // esc
                {
                    printf("Save your changes to the database? <y(yes)/n(no)/c(cancel)>");
                    ch = getch();
                    if (ch == 'y')
                    {
                        saveFile();
                        exitStatus = TRUE;
                        break;
                    }
                    else if (ch == 'n')
                    {
                        exitStatus = TRUE;
                        break;
                    }
                    else if (ch == 'c')
                    {
                        system("cls");
                        printf("***********************************\n");
                        printf("Create new Ledger Code(s)\n");
                        printf("***********************************\n");
                        printf("<crtl+n>:Add New\t<crtl+x>:Save\t<esc>:exit\n");
                        input();
                        break;
                    }
                }
            }
        } while (ch == '\0' || !((int)(ch) == 24) || !((int)(ch) == 27) || !((int)(ch) == 27 || exitStatus == FALSE));
    }
}

int getEmployeeList(filePath **usrinfofile, loginInfo *employeeInfo, char *ptr)
{
    FILE *file = openFileR(usrinfofile[1]);
    char *userName = malloc(sizeof(*userName));
    char *password = malloc(sizeof(char));
    char *jobPosition = malloc(sizeof(char));
    char readString[MAXSIZE];
    int cur_line;
    int listExist = FALSE;
    int i = 0;
    while (fgets(readString, sizeof(readString), file) != NULL && !feof(usrinfofile[1]))
    {
        if (cur_line >= 1 && !feof(usrinfofile[1]))
        {
            sscanf(readString, "%s %s %[^\n]\n", userName, password, jobPosition);

            if (strcmp(ptr, jobPosition) == 0)
            {
                if (i == 0)
                {
                    printf("%s\n", userName);
                    i++;
                }
                else if (i == 1)
                {
                    i--;
                }

                listExist = TRUE;
            }
            else
                continue;
        }
        cur_line++;
    }
    if (listExist != TRUE)
        printf("No employees found for this position\n");
    fclose(file);
}

void addNewUser(signupInfo **post, filePath **filePath)
{
    system("cls");
    signupInfo *signUp = malloc(sizeof *signUp);
    FILE *usrinfofile = openFileA(filePath[1]);
    printf("Please enter username, password and position.\n");
    printf("Username: ");
    scanf(" %[^\n]%*c", signUp->username);
    printf("Password: ");

    for (int i = 0; i < 15; i++)
    {
        char pwd = getch();
        if (pwd != '\r' && (int)(pwd) != 8 && (int)(pwd) != '\000')
        {
            signUp->password[i] = pwd;
        }
        else if ((int)(pwd) == 8)
        {
            if (i >= 0)
            {
                signUp->password[i - 1] = '\0';
                --i;
                --i;
            }
        }
        else if (pwd == 13)
        {
            break;
        }
    }
    printf("\n");
    printf("(Please use arrow key to select)\n");
    printf("Postition: %s", post[0]);
    char ch;
    int i = 0;
    do
    {
        if (kbhit())
        {
            ch = getch();
            if ((int)(ch) == 80)
            {
                if (i == 3)
                    i = 0;
                else
                    i++;
                if (i == 3)
                    i--;

                fflush(stdout);
                printf("\rPostition: %s  ", post[i]);
            }
            else if ((int)(ch) == 72)
            {
                if (i == -1)
                    i = 2;
                else if (i > 0)
                    i--;
                fflush(stdout);
                printf("\rPostition: %s  ", post[i]);
            }
        }
    } while ((int)(ch) != 13);

    signUp->jobposition = post[i];
    fprintf(usrinfofile, "%s\t%s\t%s\n\n", signUp->username, signUp->password, signUp->jobposition);
    fclose(usrinfofile);
    free(signUp);
}

int employeeList(signupInfo **jobPositionlist)
{

repeat:;
    loginInfo *employeeInfo = malloc(sizeof(*employeeInfo));
    char *position = malloc(sizeof(*position));
    int exitStatus = 0;
    int status = 0;
    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    system("cls");
    printf("***********************************\n");
    printf("Employee List\n");
    printf("***********************************\n");
    printf("<ctrl+r> Add New User || <esc> Exit\n");
    printf("------------------------------------------------------\n");
    printf("Please select the position of employees to show: ");
    printf("\n(Please use arrow key to select)");
    printf("\nPosition: %s\n", jobPositionlist[0]);
    char ch;
    int i = 0;
    do
    {
        if (kbhit())
        {
            ch = getch();
            if ((int)(ch) == 80) //down
            {
                if (i == 3)
                    i = 0;
                else
                    i++;
                if (i == 3)
                    i--;

                fflush(stdout);

                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                GetConsoleScreenBufferInfo(hConsole, &coninfo);
                coninfo.dwCursorPosition.Y -= 1; // move up one line
                coninfo.dwCursorPosition.X += 10;
                SetConsoleCursorPosition(hConsole, coninfo.dwCursorPosition);
                printf("%s     \n", jobPositionlist[i]);
            }
            else if ((int)(ch) == 72) //up
            {
                if (i == -1)
                    i = 2;
                else if (i > 0)
                    i--;

                fflush(stdout);
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                GetConsoleScreenBufferInfo(hConsole, &coninfo);
                coninfo.dwCursorPosition.Y -= 1; // move up one line
                coninfo.dwCursorPosition.X += 10;
                SetConsoleCursorPosition(hConsole, coninfo.dwCursorPosition);
                printf("%s     \n", jobPositionlist[i]);
            }
            else if ((int)(ch) == 18) //ctrl + r
            {
                addNewUser(jobPositionList(), signUpInfo());
                return status = 1;
                break;
            }
            else if ((int)(ch) == 27)
            {
                return status = 1;
                break;
            }
        }

    } while ((int)(ch) != 13 || exitStatus == 1);
    printf("LIST: %c\n", (char)(25));
    getEmployeeList(signUpInfo(), employeeInfo, jobPositionlist[i]);
    printf("(Press any key to continue)");
    char c;
    while (1)
    {

        if (kbhit())
        {
            c = getch();
            system("cls");
            break;
        }
    }
    return status = 1;
}

int accMaintainProc(void)
{
repeat:
    system("cls");
    printf("***********************************\n");
    printf("Account Maintain Process\n");
    printf("***********************************\n");
    printf("Please select a function to perform: \n");
    printf("<n+l>\tCreate New Leadger Code\n");
    printf("<v+l>\tView Leadger Code List\n");
    printf("<*>\tExit to Main Menu\n");
    int exitStatus = 0;
    char ch;
    while (exitStatus == 0)
    {

        if (kbhit())
        {
            ch = getch();
            if (ch == 'n')
            {
                Sleep(1);
                ch = getch();
                if (ch == 'l') // new ledger code
                {
                    createNewLedgerCode(ledgerCodeFile());
                    goto repeat;
                }
            }
            else if (ch == 'v')
            {
                Sleep(1);
                ch = getch();
                if (ch == 'l')
                {
                    viewledgerCode(ledgerCodeFile());
                    goto repeat;
                }
            }
            else if (ch == '*')
                exitStatus = 1;
        }
    }

    return TRUE;
}

void get_Date(int *dd, int *mm, int *yy)
{
    char ch;
    int status = 0;
    while (status == 0)
    {
        if (kbhit())
        {
            ch = getch();
            switch ((int)(ch))
            {
            case 110: //n
                printf("Please enter date (dd/MM/yyyy format): ");
                scanf_s("%d/%d/%d", dd, mm, yy);
                int status_c = FALSE;
                while (status_c == FALSE)
                {

                    if (*yy >= 1900 && *yy <= 9999)
                    {
                        //check month
                        if (*mm >= 1 && *mm <= 12)
                        {
                            //check days
                            if ((*dd >= 1 && *dd <= 31) && (*mm == 1 || *mm == 3 || *mm == 5 || *mm == 7 || *mm == 8 || *mm == 10 || *mm == 12))
                                status_c = TRUE;
                            else if ((*dd >= 1 && *dd <= 30) && (*mm == 4 || *mm == 6 || *mm == 9 || *mm == 11))
                                status_c = TRUE;
                            else if ((*dd >= 1 && *dd <= 28) && (*mm == 2))
                                status_c = TRUE;
                            else if (*dd == 29 && *mm == 2 && (*yy % 400 == 0 || (*yy % 4 == 0 && *yy % 100 != 0)))
                                status_c = TRUE;
                            else
                            {
                                printf("Day is invalid.\n");
                                printf("Please reenter a valid day: ");
                                scanf_s("%d", dd);
                            }
                        }
                        else
                        {
                            printf("Month is not valid.\n");
                            printf("Please reenter a valid month: ");
                            scanf_s("%d", mm);
                        }
                    }
                    else
                    {
                        printf("Year is not valid.\n");
                        printf("Please reenter a valid year: ");
                        scanf_s("%d", yy);
                    }
                }
                status++;
                break;

            case 116: //t
            {
                SYSTEMTIME t;
                GetLocalTime(&t);
                *dd = t.wDay;
                *mm = t.wMonth;
                *yy = t.wYear;
                printf("%d/%d/%d", *dd, *mm, *yy);
                status++;
                break;
            }
            }
        }
    }
}
void ledgerDetails(ledgerInfo *newLedger, filePath **ledgerCodeFile, filePath **ledgerListFileName)
{

    ledger_Details *details = malloc(sizeof(*details));
    int numOfLedgers = 0;
    int exitStatus = FALSE;
    int day, month, year;
    char *lCode = malloc(sizeof(*lCode));
    char *ledgerDetails = malloc(sizeof(*ledgerDetails));
    char c;
    int codeAmount = 0;
    ledgerCode *ptr = malloc(sizeof(*ptr));
    CONSOLE_SCREEN_BUFFER_INFO coninfo;

    void readfile()
    {
        FILE *codelist = openFileR(ledgerCodeFile[0]);
        char readString[1024];
        while (fgets(readString, sizeof(readString), codelist) != NULL)
        {
            sscanf(readString, "%s %[^\n]\n", lCode, ledgerDetails);
            sprintf(ptr->ledgerCode[codeAmount], "%s", lCode);
            //memcpy(ptr->ledgerCode[codeAmount], lCode, sizeof(lCode));
            sprintf(ptr->description[codeAmount], "%s", ledgerDetails);
            //memcpy(ptr->description[codeAmount], ledgerDetails, sizeof(ledgerDetails));
            codeAmount++;
        }
        fclose(codelist);
    }

    void input()
    {
        int i = 0;
        printf("(%d)\n", numOfLedgers + 1);
        printf("(Please use arrow key to select)\n");
        printf("\rLedger Code: %s    Description: %s    \n", ptr->ledgerCode[i], ptr->description[i]);
        char ch;

        do
        {
            if (kbhit())
            {
                ch = getch();
                if ((int)(ch) == 80)
                {
                    if (i == codeAmount)
                        i = 0;
                    else
                        i++;
                    if (i == codeAmount)
                        i--;

                    fflush(stdout);

                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                    GetConsoleScreenBufferInfo(hConsole, &coninfo);
                    coninfo.dwCursorPosition.Y -= 1; // move up one line
                    coninfo.dwCursorPosition.X = 0;
                    SetConsoleCursorPosition(hConsole, coninfo.dwCursorPosition);
                    printf("\rLedger Code: %s    Description: %s     \n", ptr->ledgerCode[i], ptr->description[i]);
                }
                else if ((int)(ch) == 72)
                {
                    if (i == 0)
                        i = codeAmount - 1;
                    else
                        i--;

                    fflush(stdout);
                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                    GetConsoleScreenBufferInfo(hConsole, &coninfo);
                    coninfo.dwCursorPosition.Y -= 1; // move up one line
                    coninfo.dwCursorPosition.X = 1;
                    SetConsoleCursorPosition(hConsole, coninfo.dwCursorPosition);
                    printf("\rLedger Code: %s    Description: %s     \n", ptr->ledgerCode[i], ptr->description[i]);
                }
            }
        } while ((int)(ch) != 13);
        //memcpy(details->ledgerCode[numOfLedgers], &lCode[i], 8);
        sprintf(details->ledgerCode[numOfLedgers], "%s", &lCode[i]);
        printf("\nDate: (dd/MM/yyyy format)\n");
        printf("<t>Today || <n> specific date\n");
        get_Date(&day, &month, &year);
        details->dt.day[numOfLedgers] = day;
        details->dt.month[numOfLedgers] = month;
        details->dt.year[numOfLedgers] = year;
        printf("\nDescription: ");
        scanf(" %[^\n]%*c", details->description[numOfLedgers]);
        printf("Amount (+/-): ");
        scanf(" %lf", &(details->credit_debit[numOfLedgers]));
    }

    void summary()
    {
        printf("Summary: \n");
        for (int i = 0; i < numOfLedgers; i++)
            printf("%d. \nDate: %d/%d/%d\nDescription: %s\tAmount: %.2lf\n", i + 1, details->dt.day[i], details->dt.month[i], details->dt.year[i], details->description[i], details->credit_debit[i]);
    }

    void saveFile()
    {
        FILE *ledgerFile = fopen(newLedger->accNumber, "a+");
        FILE *ledgerListFile = openFileA(ledgerListFileName[0]);
        for (int i = 0; i < numOfLedgers; i++)
            fprintf(ledgerFile, "%d/%d/%d\t%s\t%.2lf\t%s\n\n", details->dt.day[i], details->dt.month[i], details->dt.year[i], details->ledgerCode[i], details->credit_debit[i], details->description[i]);
        fprintf(ledgerListFile, "%d/%d/%d\t%s\t%d\t%s\n\n", newLedger->dt.day[0], newLedger->dt.month[0], newLedger->dt.year[0], newLedger->accNumber, 0, newLedger->description);
        fclose(ledgerListFile);
        fclose(ledgerFile);
    }

    while (exitStatus != TRUE)
    {
        system("cls");
        printf("Date: %d/%d/%d\tAcc No.: %s\nDescription: %s\n", newLedger->dt.day[0], newLedger->dt.month[0], newLedger->dt.year[0], newLedger->accNumber, newLedger->description);
        printf("========================================================================\n");
        printf("<crtl+n> Create New || <crtl+x> View Mode || <crtl+s> Save || <esc> Exit\n");
        printf("========================================================================\n");

        if (numOfLedgers == 0)
        {
            readfile();
            input();
            numOfLedgers++;
            continue;
        }
        summary();

        do
        {

            if (kbhit())
            {
                c = getch();

                switch ((int)(c))
                {
                case 14: //crtl + n
                    system("cls");
                    printf("Date: %d/%d/%d\tAcc No.: %s\nDescription: %s\n", newLedger->dt.day[0], newLedger->dt.month[0], newLedger->dt.year[0], newLedger->accNumber, newLedger->description);
                    printf("========================================================================\n");
                    printf("<crtl+n> Create New || <crtl+x> View Mode || <crtl+s> Save || <esc> Exit\n");
                    printf("========================================================================\n");
                    input();
                    numOfLedgers++;
                    if (numOfLedgers > 1)
                    {
                        system("cls");
                        printf("Date: %d/%d/%d\tAcc No.: %s\nDescription: %s\n", newLedger->dt.day[0], newLedger->dt.month[0], newLedger->dt.year[0], newLedger->accNumber, newLedger->description);
                        printf("========================================================================\n");
                        printf("<crtl+n> Create New || <crtl+x> View Mode || <crtl+s> Save || <esc> Exit\n");
                        printf("========================================================================\n");
                        summary();
                    }
                    break;

                case 24: //crtl + x
                    system("cls");
                    printf("Date: %d/%d/%d\tAcc No.: %s\nDescription: %s\n", newLedger->dt.day[0], newLedger->dt.month[0], newLedger->dt.year[0], newLedger->accNumber, newLedger->description);
                    printf("========================================================================\n");
                    printf("<crtl+n> Create New || <crtl+x> View Mode || <crtl+s> Save || <esc> Exit\n");
                    printf("========================================================================\n");
                    summary();
                    break;

                case 19: //crtl + s
                    saveFile();
                    exitStatus = TRUE;
                    break;
                case 27: //esc
                    printf("Save your changes to the database? <y(yes)/n(no)/c(cancel)>");
                    c = getch();

                    if (c == 'y')
                    {
                        saveFile();
                        exitStatus = TRUE;
                        break;
                    }
                    else if (c == 'n')
                    {
                        exitStatus = TRUE;
                        break;
                    }
                    else if (c == 'c')
                    {
                        system("cls");
                        printf("Date: %d/%d/%d\tAcc No.: %s\nDescription: %s\n", newLedger->dt.day[0], newLedger->dt.month[0], newLedger->dt.year[0], newLedger->accNumber, newLedger->description);
                        printf("========================================================================\n");
                        printf("<crtl+n> Create New || <crtl+x> View Mode || <crtl+s> Save || <esc> Exit\n");
                        printf("========================================================================\n");
                        summary();
                    }
                    break;
                }
            }
        } while (exitStatus == FALSE);
    }
}

ledgerInfo *newGLedger()
{
    ledgerInfo *newLedger = malloc(sizeof(*newLedger));
    memcpy(newLedger->accNumber, genAccNumber(signUpInfo()), 6);
    char *accType = malloc(sizeof(*accType));
    double initBal[MAXSIZE];
    int codeAmount = 0;

    printf("Account Number: %s", newLedger->accNumber);
    printf("\n--------------------------------------------------\n");
    printf("Date: (dd/MM/yyyy format)\n");
    printf("<t>Today || <n> specific date\n");
    get_Date(newLedger->dt.day, newLedger->dt.month, newLedger->dt.year);
    printf("\nDescription: ");
    scanf(" %[^\n]%*c", newLedger->description);
    return newLedger;
}

void viewLedgers(filePath **viewLedgers, int admin)
{
    system("cls");
    FILE *file = openFileR(viewLedgers[0]);
    char readString[1024];
    int day, month, year, approval;
    char *accNumber = malloc(sizeof(*accNumber));
    char *description = malloc(sizeof(*description));

    printf("-----CREATED LEDGER LIST-----\n");
    printf("DATE\t\tACCOUNT NO.\tDESCRIPTION\t\tAPPROVAL\n");
    int i = 0;
    while (fgets(readString, sizeof(readString), file) != NULL)
    {
        if (!feof(file))
        {
            sscanf(readString, "%d/%d/%d %s %d %[^\n]\n", &day, &month, &year, accNumber, &approval, description);

            if (i == 0)
            {
                printf("%d/%d/%d\t%s\t\t%s\t\t%d\n", day, month, year, accNumber, description, approval);
                i++;
            }
            else if (i == 1)
                i--;
        }
    }
    fclose(file);
    if (admin == 0)
    {

        printf("(Press any key to continue)");
        int ch;
        while (1)
        {

            if (kbhit())
            {
                ch = getch();
                system("cls");
                break;
            }
        }
    }
}

void createNewLedger()
{
    char ch;
    int exit_status = FALSE;
    int stat = 0;
    while (exit_status == FALSE)
    {
        if (stat == 0)
        {
            system("cls");
            printf("***********************************\n");
            printf("Create new Ledger\n");
            printf("***********************************\n");
            printf("<crtl+n> Create new\t<esc> Return to previous menu\n");
            stat++;
        }
        if (kbhit())
        {
            ch = getch();
            switch ((int)(ch))
            {
            case 14: //crtl + n
                system("cls");
                ledgerDetails(newGLedger(), ledgerCodeFile(), ledgerListFile());
                stat = 0;
                break;

            case 27: //esc
                exit_status = TRUE;
                break;
            }
        }
    }
}

int accUpdateProc(void)
{
repeat:
    system("cls");
    printf("***********************************\n");
    printf("Account Update Process\n");
    printf("***********************************\n");
    printf("Please select a function to perform: \n");
    printf("<n+l>\tEnter New Ledger\n");
    printf("<v+l>\tView Ledger History\n");
    printf("<*>\tExit to Main Menu\n");

    char ch;
    char temp;
    int STATUS = FALSE;
    while (STATUS == FALSE)
    {

        if (kbhit())
        {
            ch = getch();
            if (ch == '*')
            {
                STATUS = TRUE;
                break;
            }
            else
            {
                temp = ch;
                Sleep(1);
                ch = getch();
                if (ch == 'l')
                {
                    switch ((int)(temp))
                    {
                    case 110:
                        createNewLedger();
                        goto repeat;
                        break;

                    case 118:
                        viewLedgers(ledgerListFile(), 0);
                        goto repeat;
                        break;
                    }
                }
            }
        }
    }
    return TRUE;
}

struct checkStatus trackAccNumber(char accnumber[MAXSIZE], filePath **ledgerListFileName)
{
    struct checkStatus Status;
    FILE *checkAccNumFile = openFileR(ledgerListFileName[0]);
    int day, month, year;
    char readString[1024], validation[MAXSIZE], description[MAXSIZE];
    while (fgets(readString, sizeof(readString), checkAccNumFile) != NULL)
    {
        sscanf(readString, "%d/%d/%d %s %d %[^\n]\n", &day, &month, &year, &validation, &(Status.approved), &description);

        sprintf(Status.date, "%d/%d/%d", day, month, year);

        if (strcmp(accnumber, validation) == 0)
            Status.status = 1;
        else if (strcmp(accnumber, validation) == 1)
            continue;
    }
    if (Status.status == 0)
        printf("\nNo such file. Please try again.\n");
    if (Status.approved <= 0)
    {
        printf("\nThis ledger is not approved. Please get a approval.\n");
        Status.status = 0;
    }
    fclose(checkAccNumFile);
    return Status;
}

void writeFileHeader(filePath **companyNamefile, filePath **ledgerListFile, FILE *report, char *date, char *AccountNumber)
{

    char *header = "General Ledger", companyName[MAXSIZE], readString[1024];
    FILE *file = openFileR(companyNamefile[1]);
    FILE *f = openFileR(ledgerListFile[0]);
    char String[1024];
    char *accNumber = malloc(sizeof(char));
    char *description = malloc(sizeof(char));
    char *writeDescription = malloc(sizeof(char));
    int approval, day, month, year;
    int status = 1;
    while (fgets(String, sizeof(String), f) != NULL)
    {
        if (!feof(file))
        {
            sscanf(String, "%d/%d/%d %s %d %[^\n]\n", &day, &month, &year, accNumber, &approval, description);

            if (strcmp(AccountNumber, accNumber) == 0 && status == 1)
            {
                strcpy(writeDescription, description);
                status--;
                //sprintf(writeDescription, "%[^\n]\n", description);
            }
        }
    }
    fgets(readString, sizeof(readString), file);
    sscanf(readString, "%s\n", &companyName);
    fclose(file);
    fprintf(report, "\t\t\t\t%s\n\t\t\t\t%s\n\t\t\t\t%s\n\t\t\t\t%s\n", companyName, header, date, writeDescription);
    fprintf(report, "Type\t\tDate\t\t\tDescription\t\t\tAmount\t\tBalance\t\n\n");
}

void writeFileContent(FILE *file, char *date, char *ledgerCode, char *description, double *amount, double *balance, int *num)
{

    if (*num == 0)
    {
        fprintf(file, "%s\t\t%s\t\t%s\t\t\t\t\t%.2lf\n", ledgerCode, date, description, *balance);
    }
    else
        fprintf(file, "%s\t\t%s\t\t%s\t\t%.2lf\t\t%.2lf\n", ledgerCode, date, description, *amount, *balance);
}

void readFile(FILE *file, FILE *report)
{
    char readString[1024];
    int cur_line = 0;
    char patientName[1024];
    int day, month, year;
    char *ledgerCode = malloc(sizeof(*ledgerCode));
    char *description = malloc(sizeof(*description));
    double amount;
    char *date = malloc(sizeof(*date));
    double balance = 0.0;
    int num = 0;
    int i = 0;

    while (fgets(readString, sizeof(readString), file) != NULL)
    {
        if (cur_line >= 0)
        {
            sscanf(readString, "%d/%d/%d %s %lf %[^\n]\n\n", &day, &month, &year, ledgerCode, &amount, description);
            sprintf(date, "%d/%d/%d", day, month, year);
            if (i == 0)
            {
                balance += amount;
                writeFileContent(report, date, ledgerCode, description, &amount, &balance, &num);
                i++;
            }
            else if (i == 1)
            {
                i--;
                continue;
            }
        }
        cur_line++;
        num++;
    }
}

int ReportGenerationMenu(void)
{
    system("cls");
    printf("***********************************\n");
    printf("Report Generation Process\n");
    printf("***********************************\n");
    printf("Please enter the Account Number of the Ledger: ");
    char AccountNumber[MAXSIZE];
    int valid = 0, approved = 0;
    char filename[MAXSIZE], *ptr;
    char input;
    int x = 0;
    while (valid == 0 && approved >= 0)
    {
        int x = 0;
        while ((int)(input) != 13)
        {
            input = getch();
            if ((int)(input) != 13)
            {
                printf("%c", input);
                AccountNumber[x] = input;
                x++;
            }
        }
        struct checkStatus stat = trackAccNumber(AccountNumber, ledgerListFile());
        ptr = stat.date;
        valid = stat.status;
        approved = stat.approved;
        if (valid == 0 && approved >= 0)
        {
            printf("(Press any key to continue)");
            int ch;
            while (1)
            {

                if (kbhit())
                {
                    ch = getch();
                    system("cls");
                    break;
                }
            }
            return TRUE;
        }
    }

    FILE *file = openFileR(AccountNumber);
    char acc[MAXSIZE];
    sprintf(acc, "%s.txt", AccountNumber);
    char *cmd = malloc(sizeof(char));
    sprintf(cmd, "notepad %s", acc);
    FILE *report = fopen(acc, "w");
    writeFileHeader(signUpInfo(), ledgerListFile(), report, ptr, AccountNumber);
    readFile(file, report);
    fclose(file);
    fclose(report);
    system(cmd);
    return TRUE;
}

int staff_Menu(loginInfo *user)
{
    system("cls");
    welScreenInfo menu;
    char *welcome = "         welcome";
    char *option = " Please select an option:\n";
    char *accMain = "<crtl+n>\tAccount Maintain Process\n";
    char *accUp = "<crtl+e>\tAccount Update Process\n";
    char *GenReport = "<crtl+p>\tReport Generation Process\n";
    char *exit = "<crtl+x>\tExit\n";
    getScreenSize(menu.resolutions);
    int dot_no = strlen(GenReport) + 6;
    int rows = (menu.resolutions[0] - 5) / 2;
    int columns = (menu.resolutions[1] - dot_no - 1) / 2;

repeat:
    for (int i = 0; i < rows; i++)
        putchar('\n');
    for (int i = 0; i < columns; i++)
        printf(" ");
    for (int i = 0; i < dot_no; i++)
    {
        putchar('-');
    }
    printf("\n");
    for (int i = 0; i < columns; i++)
        printf(" ");
    printf("%s %s", welcome, user->username);
    printf("\n");
    for (int i = 0; i < columns; i++)
        printf(" ");
    for (int i = 0; i < dot_no; i++)
    {
        putchar('-');
    }
    printf("\n");
    for (int i = 0; i < columns; i++)
        printf(" ");
    printf("%s\n", option);
    for (int i = 0; i < columns; i++)
        printf(" ");
    printf("%s\n", accMain);
    for (int i = 0; i < columns; i++)
        printf(" ");
    printf("%s\n", accUp);
    for (int i = 0; i < columns; i++)
        printf(" ");
    printf("%s\n", GenReport);
    for (int i = 0; i < columns; i++)
        printf(" ");
    printf("%s\n", exit);
    char ch;
    int STATUS = FALSE;
    while (STATUS == FALSE)
    {
        if (kbhit())
        {
            ch = getch();
            switch ((int)(ch))
            {
            case 14: //crtl + n
                STATUS = accMaintainProc();
                system("cls");
                goto repeat;
                break;

            case 5: //crtl + e
                STATUS = accUpdateProc();
                system("cls");
                goto repeat;
                break;

            case 16: //crtl + p
                STATUS = ReportGenerationMenu();
                system("cls");
                goto repeat;
                break;

            case 24: //crtl + x
                _Exit(0);
            }
        }
    }
}

void viewDetails(FILE *file, char *name)
{
    char readString[1024];
    int day, month, year;
    double amount;
    char *ledgerCode = malloc(sizeof(*ledgerCode));
    char *description = malloc(sizeof(*description));
    char temp[1024];
    int i = 0;

    printf("-----LEDGER %s-----\n", name);
    printf("DATE\t\tLEDGER CODE\t\tDESCRIPTION\tAMOUNT\n");

    while (fgets(readString, sizeof(readString), file) != NULL)
    {
        sscanf(readString, "%d/%d/%d %s %lf %[^\n]\n\n", &day, &month, &year, ledgerCode, &amount, description);
        if (i == 0)
        {
            printf("%d/%d/%d\t%s\t\t\t%s\t\t%.2lf\n", day, month, year, ledgerCode, description, amount);
            i++;
        }
        else if (i == 1)
        {
            i--;
        }
    }
    fclose(file);
}

int approvalLedger(filePath **ledgerListFile)
{
    ledgerApproval *approve = malloc(sizeof(*approve));
    char input;
    int x = 0, stat = 0;
    viewLedgers(ledgerListFile, 1);
    printf("\nPlease enter the account number to be reviewed: ");
    while ((int)(input) != 13)
    {
        input = getch();
        if ((int)(input) != 13 && input != '\000')
        {
            printf("%c", input);
            approve->ledgerCode.ledgerCode[0][x] = input;
            x++;
        }
        else
            break;
    }
    system("cls");
    FILE *f = openFileR(approve->ledgerCode.ledgerCode[0]);
    viewDetails(f, approve->ledgerCode.ledgerCode[0]);
    printf("\nApprove?<y/n>");
    char ch;
    int status = 0;
    while (status != 1)
    {

        if (kbhit())
        {
            ch = getch();
            if (ch == 'y')
            {
                FILE *tmpfile = fopen("temp.txt", "w");
                FILE *file = openFileR(ledgerListFile[0]);
                char readString[1024];
                int day, month, year, approval;
                char *accNumber = malloc(sizeof(*accNumber));
                char *description = malloc(sizeof(*description));

                while (fgets(readString, sizeof(readString), file) != NULL)
                {
                    sscanf(readString, "%d/%d/%d %s %d %[^\n]\n\n\n", &day, &month, &year, accNumber, &approval, description);
                    if (strcmp(accNumber, (approve->ledgerCode.ledgerCode[0])) == 0)
                        fprintf(tmpfile, "%d/%d/%d\t%s\t\t%d\t%s\n", day, month, year, accNumber, 1, description);
                    else
                        fprintf(tmpfile, "%d/%d/%d\t%s\t\t%d\t%s\n", day, month, year, accNumber, -1, description);
                }
                fclose(file);
                fclose(tmpfile);
                remove(ledgerListFile[0]);
                rename("temp.txt", ledgerListFile[0]);
                system("cls");
                status = 1;
            }
            else if (ch == 'n')
            {
                printf("Ledger Account Number (%s) is not approved.", approve->ledgerCode.ledgerCode[0]);
                system("cls");
                FILE *tmpfile = fopen("temp.txt", "w");
                FILE *file = openFileR(ledgerListFile[0]);
                char readString[1024];
                int day, month, year, approval;
                char *accNumber = malloc(sizeof(*accNumber));
                char *description = malloc(sizeof(*description));

                while (fgets(readString, sizeof(readString), file) != NULL)
                {
                    sscanf(readString, "%d/%d/%d %s %d %[^\n]\n\n\n", &day, &month, &year, accNumber, &approval, description);
                    if (strcmp(accNumber, (approve->ledgerCode.ledgerCode[0])) == 0)
                        fprintf(tmpfile, "%d/%d/%d\t%s\t\t%d\t%s\n", day, month, year, accNumber, -1, description);
                }
                fclose(file);
                fclose(tmpfile);
                remove(ledgerListFile[0]);
                rename("temp.txt", ledgerListFile[0]);
                status = 1;
            }
            break;
        }
    }
    return status = 1;
}

int main(int argc, char **argv)
{

    welcomeScreen();
    signUpScreen(jobPositionList(), signUpInfo());
}