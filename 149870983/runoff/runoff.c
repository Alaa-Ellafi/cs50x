#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
} candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // TODO
    int test = 1;
    int s = 0;
    while (s < candidate_count && test == 1)
    {
        test = 0;
        if (strlen(name) != strlen(candidates[s].name))
        {
            test = 1;
        }
        else
        {
            for (int j = 0; j < strlen(name); j++)
            {

                if (name[j] != candidates[s].name[j])
                {
                    test = 1;
                }
            }
        }

        s++;
    }

    if (test == 0)
    {
        preferences[voter][rank] = s - 1;
        return true;
    }
    else
    {
        return false;
    }
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // TODO
    int c;
    int b;
    for (int a = 0; a < voter_count; a++)
    {
        b = 0;

        while (b < candidate_count)
        {

            if (!candidates[preferences[a][b]].eliminated)
            {
                break;
            }
            b++;
        }
        candidates[preferences[a][b]].votes++;
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // TODO
    int half = voter_count / 2;
    int k = 0;
    while (k < candidate_count && candidates[k].votes <= half)
    {
        k++;
    }
    if (k < candidate_count)
    {
        printf("%s\n", candidates[k].name);
        return true;
    }
    else
    {
        return false;
    }
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // TODO
    int l = 0;
    while (l < candidate_count && (candidates[l].eliminated))
    {
        l++;
    }
    int min = candidates[l].votes;
    for (int m = 0; m < candidate_count; m++)
    {
        if ((!candidates[m].eliminated) && candidates[m].votes < min)
        {
            min = candidates[m].votes;
        }
    }
    return min;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // TODO
    int s = 0;
    while ((s < candidate_count && candidates[s].votes == min) || (candidates[s].eliminated))
    {
        s++;
    }
    if (s == candidate_count)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // TODO
    for (int n = 0; n < candidate_count; n++)
    {
        if (candidates[n].votes == min)
        {
            candidates[n].eliminated = true;
        }
    }
    return;
}
