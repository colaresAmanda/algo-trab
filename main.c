#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <conio.h>
#define clrscr() system("cls")
#else
#include <stdio.h>
#define clrscr() printf("\e[1;1H\e[2J")
#endif

#define CANDITATES_NUMBER 2
#define CANDIDATES_FILE "candidates.txt"
#define VOTES_FILE "votes.txt"
#define REESULT_FILE "result.txt"
#define USERS_FILE "users.txt"

typedef struct
{
    char name[6];
    char password[10];
} User;

typedef struct
{
    char name[30];
    char code[5];
} Candidate;


char *removeSpacesFromStr(char *string);
char *removeNewLineFromStr(char *string);
char *treatString(char *string);
int verifyUserName(char *name, char *password);
int getRoleFromUser();
Candidate *getCandidates();
Candidate *sortCandidates(Candidate *candidates);
Candidate *sortCandidates(Candidate *candidates);
void listCandidates();
void addCandidate();
void storeVote(char vote[5]);
void getVotes();
void showAdminMenu();
void showUserMenu();


int main()
{

    int role = 0;
    role = getRoleFromUser();
    if (role == 0)
    {
        showUserMenu();
    }
    else if (role == 1)
    {
        showAdminMenu();

    }
    else
    {
        printf("Missing permission.");
        return 1;
    }
    return 0;
}

char *removeSpacesFromStr(char *string)
{
    int non_space_count = 0;

    for (int i = 0; string[i] != '\0'; i++)
    {
        if (string[i] != ' ')
        {
            string[non_space_count] = string[i];
            non_space_count++;
        }
    }

    string[non_space_count] = '\0';
    return string;
}

char *removeNewLineFromStr(char *string)
{
    int len;
    len = strlen(string);
    if (string[len - 1] == '\n')
        string[len - 1] = 0;

    return string;
}

char *treatString(char *string)
{

    string = removeSpacesFromStr(string);
    string = removeNewLineFromStr(string);

    return string;
}
int verifyUserName(char *name, char *password)
{

    FILE *file;
    char user_line[80];

    int comp_name = 1;
    int comp_password = 1;

    file = fopen(USERS_FILE, "r");

    if (file == NULL)
    {
        printf("Error while opening %s ", USERS_FILE);
        return (-1);
    }

    while (fgets(user_line, sizeof(user_line), file) != NULL && comp_name != 0)
    {
        char *name_file = strtok(user_line, "|");
        char *password_file = strtok(NULL, "|");

        name_file = treatString(name_file);
        name = treatString(name);

        password_file = treatString(password_file);
        password = treatString(password);

        comp_name = strcmp(name, name_file);
        comp_password = strcmp(password, password_file);
    }

    fclose(file);

    if (comp_name == 0 && comp_password == 0)
        return 0;
    else
        return 1;
}
int getRoleFromUser()
{

    User user;

    int verifier = 1;

    while (verifier != 0)
    {
        printf("user: ");
        gets(user.name);
        printf("password: ");
        gets(user.password);

        verifier = verifyUserName(user.name, user.password);

        if (verifier)
        {
            clrscr();
            printf("\nwrong credentials, try again.\n\n");
        }
    }

    clrscr();

    // The name is the same a the role (user, admin, etc) - I know this is not secure
    if (strcmp(user.name, "admin") == 0)
    {
        // role 1 = admin
        return 1;
    }
    else
    {
        // role 0 = user
        return 0;
    }
}




Candidate *getCandidates()
{

    FILE *file;
    char candidate_line[80];
    static Candidate candidates[CANDITATES_NUMBER];
    int counter = 0;

    file = fopen(CANDIDATES_FILE, "r");
    if (file == NULL)
    {
        printf("Error while opening %s ", CANDIDATES_FILE);
        return (-1);
    }

    while (fgets(candidate_line, sizeof(candidate_line), file) != NULL && counter < CANDITATES_NUMBER)
    {
        char *name_file = strtok(candidate_line, "|");
        char *code_file = strtok(NULL, "|");

        name_file = treatString(name_file);

        code_file = treatString(code_file);

        strcpy(candidates[counter].name, name_file);
        strcpy(candidates[counter].code, code_file);

        counter++;
    }

    fclose(file);

    return candidates;
}

Candidate *sortCandidates(Candidate *candidates)
{
    Candidate aux;

    for (int i = 0; i < CANDITATES_NUMBER - 1; i++)
    {
        for (int j = 0; j < CANDITATES_NUMBER - i - 1; j++)
        {
            if (strcmp(candidates[j].name, candidates[j + 1].name) > 0)
            {
                aux = candidates[j];
                candidates[j] = candidates[j + 1];
                candidates[j + 1] = aux;
            }
        }
    }

    return candidates;
}
void listCandidates()
{
    Candidate *candidates;

    candidates = getCandidates();
    candidates = sortCandidates(candidates);

    printf("\tName - Code\n");

    for (int i = 0; i < CANDITATES_NUMBER; i++)
    {
        printf("\t%s - %s\n", candidates[i].name, candidates[i].code);
    }
}
void addCandidate()
{

    Candidate candidate;

    printf("Enter the candidate name: ");
    gets(candidate.name);

    printf("Enter a uniuque candidate code: ");
    gets(candidate.code);
    FILE *file;
    file = fopen(CANDIDATES_FILE, "a");

    char aux[100] = "";

    strcpy(aux, candidate.name);
    strcat(aux, " | ");
    strcat(aux, candidate.code);
    strcat(aux, " \n");
    fputs(aux, file);

    fclose(file);
}

void storeVote(char vote[5])
{

    FILE *file;
    file = fopen(VOTES_FILE, "a");

    char aux[10] = "";

    strcpy(aux, vote);
    strcat(aux, "\n");

    fputs(aux, file);

    fclose(file);
}

void getVotes()
{

    FILE *file;
    char vote_line[80];
    int total_counter = 0;
    int total_cand_1 = 0;
    int total_cand_2 = 0;
    int total_blank = 0;
    int total_null = 0;
    Candidate *candidates;

    candidates = getCandidates();
    candidates = sortCandidates(candidates);
    file = fopen(VOTES_FILE, "r");
    if (file == NULL)
    {
        printf("Error while opening %s ", CANDIDATES_FILE);
    }

    while (fgets(vote_line, sizeof(vote_line), file) != NULL)
    {
        vote_line[strcspn(vote_line, "\n")] = '\0';
        if (strcmp(vote_line, "999") == 0) // Blank vote
        {
            total_blank++;
        }
        else if (strcmp(vote_line, candidates[0].code) == 0)
        {
            total_cand_1++;
        }
        else if (strcmp(vote_line, candidates[1].code) == 0)
        {
            total_cand_2++;
        }
        else if (strcmp(vote_line, "000") == 0)
        {
            total_null++;
        }
        total_counter++;
    }

    float percent_blank = (total_counter > 0) ? ((float)total_blank / total_counter) * 100 : 0;
    float percent_null = (total_counter > 0) ? ((float)total_null / total_counter) * 100 : 0;


    printf("Total Votes: %d\n", total_counter);
    printf("Votes for %s: %d\n", candidates[0].name, total_cand_1);
    printf("Votes for %s: %d\n", candidates[1].name, total_cand_2);
    printf("Blank Votes: %d (%.2f%%)\n", total_blank, percent_blank);
    printf("Null Votes: %d (%.2f%%)\n", total_null, percent_null);

    fclose(file);

    FILE *file_;
    file_ = fopen(REESULT_FILE, "w");

    fprintf(file_,
            "Total Votes: %d\n"
            "Votes for %s: %d\n"
            "Votes for %s: %d\n"
            "Blank Votes: %d (%.2f%%)\n"
            "Null Votes: %d (%.2f%%)\n",
            total_counter,
            candidates[0].name, total_cand_1,
            candidates[1].name, total_cand_2,
            total_blank, percent_blank,
            total_null, percent_null);
    fclose(file_);
}

void showAdminMenu()
{
    int op = 1;

    while (op != 0)
    {

        printf("\nAdmin \n1 - Add candidate\n2 - List candidates\n3 - show result\n0 - exit program\nSelect an option: ");

        scanf(" %d", &op);

        getchar();

        switch (op)
        {
        case 0:
            printf("Ending program.");
            exit(0);
            break;
        case 1:
            addCandidate();
            break;
        case 2:
            listCandidates();
            break;
        case 3:
            getVotes();
            break;
        default:
            printf("Invalid option");
            break;
        }
    }
}

void showUserMenu()
{
    char op[5];

    Candidate *candidates;

    candidates = getCandidates();
    candidates = sortCandidates(candidates);

    while (op != 0)
    {
        printf("\n\n");
        listCandidates();
        printf("\n\tBlank vote - 999\n\tExit program - 0\n");
        printf("\nEnter your vote: ");
        gets(op);

        if (strcmp(op, "0") == 0) 
        {
            clrscr();
            printf("Ending program.\n");
            break;
        }
        else if (strcmp(op, "999") == 0) 
        {
            clrscr();
            storeVote(op);
        }

        else if (strcmp(op, candidates[0].code) == 0)
        {
            clrscr();
            storeVote(op);
        }
        else if (strcmp(op, candidates[1].code) == 0)
        {
            clrscr();
            storeVote(op);
        }
        else
        {
            clrscr();
            storeVote("000");
        }

    }
}

