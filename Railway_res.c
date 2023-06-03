#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

typedef struct NODE
{
    int reg_no;
    int age;
    char name[20];
    struct NODE *next;
} node;

typedef struct WAITLIST_NODE
{
    int reg_no;
    int age;
    char name[20];
    struct WAITLIST_NODE *next;
} waitlist_node;

node *start;
node *front;
node *rear;
int count = 0;
int num = 0;

void enq(waitlist_node *new_node)
{
    if (rear == NULL)
    {
        rear = new_node;
        rear->next = NULL;
        front = rear;
    }
    else
    {
        waitlist_node *temp;
        temp = new_node;
        rear->next = temp;
        temp->next = NULL;
        rear = temp;
    }
    count++;
}

waitlist_node *deq()
{
    waitlist_node *front1;
    front1 = front;
    if (front == NULL)
        return NULL;
    else
    {
        count--;
        if (front->next != NULL)
        {
            front = front->next;
            front1->next = NULL;
            return front1;
        }
        else
        {
            front = NULL;
            rear = NULL;
            return front1;
        }
    }
}

void saveToFile()
{
    FILE *file;
    file = fopen("reservation.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }

    node *temp = start;
    while (temp != NULL)
    {
        fprintf(file, "%d %s %d\n", temp->reg_no, temp->name, temp->age);
        temp = temp->next;
    }

    fclose(file);
    printf("Reservation data saved to file.\n");
}

void loadFromFile()
{
    FILE *file;
    file = fopen("reservation.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file for reading.\n");
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), file))
    {
        node *new_node = (node *)malloc(sizeof(node));
        sscanf(line, "%d %s %d", &(new_node->reg_no), new_node->name, &(new_node->age));
        new_node->next = NULL;

        if (start == NULL)
            start = new_node;
        else
        {
            node *temp = start;
            while (temp->next != NULL)
                temp = temp->next;
            temp->next = new_node;
        }

        num++;
    }

    fclose(file);
    printf("Reservation data loaded from file.\n");
}

int create()
{
    node *new_node;
    new_node = (node *)malloc(sizeof(node));
    new_node->reg_no = 1;
    printf("Name: ");
    scanf("%s", new_node->name);
    printf("Age : ");
    scanf("%d", &(new_node->age));

    if (new_node->age >= 70 || new_node->age <= 5)
    {
        free(new_node);
        return -1;
    }

    start = new_node;
    new_node->next = NULL;
    num++;
    return 1;
}

int reserve()
{
    int temp;
    if (start == NULL)
    {
        temp = create();
        return temp;
    }
    else
    {
        node *temp, *new_node;
        temp = start;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        new_node = (node *)malloc(sizeof(node));

        printf("Name: ");
        scanf("%s", new_node->name);
        printf("Age : ");
        scanf("%d", &(new_node->age));

        if (new_node->age >= 110 || new_node->age <= 0)
        {
            free(new_node);
            return -1;
        }

        new_node->next = NULL;
        if (num <= MAX_SIZE)
        {
            num++;
            new_node->reg_no = num;
            temp->next = new_node;
            return 1;
        }
        else
        {
            waitlist_node *wait_node = (waitlist_node *)malloc(sizeof(waitlist_node));
            wait_node->reg_no = num + 1;
            strcpy(wait_node->name, new_node->name);
            wait_node->age = new_node->age;
            enq(wait_node);
            return 0;
        }
    }
}

int cancel(int reg)
{
    node *ptr, *preptr, *new_node;
    ptr = start;
    preptr = NULL;
    if (start == NULL)
        return -1;
    if (ptr->next == NULL && ptr->reg_no == reg)
    {
        start = NULL;
        num--;
        free(ptr);
        return 1;
    }
    else
    {
        while (ptr->reg_no != reg && ptr->next != NULL)
        {
            preptr = ptr;
            ptr = ptr->next;
        }
        if (ptr == NULL && ptr->reg_no != reg)
            return -1;
        else
            preptr->next = ptr->next;
        free(ptr);
        new_node = deq();
        while (preptr->next != NULL)
            preptr = preptr->next;
        preptr->next = new_node;
        num--;
        return 1;
    }
}

void display()
{
    node *temp;
    temp = start;
    while (temp != NULL)
    {
        printf("\nRegistration Number: %d\n", temp->reg_no);
        printf("Name : %s\n", temp->name);
        printf("Age : %d\n", temp->age);
        temp = temp->next;
    }
}

void searchPassenger(int reg)
{
    node *temp = start;
    while (temp != NULL)
    {
        if (temp->reg_no == reg)
        {
            printf("\nPassenger Found:\n");
            printf("Registration Number: %d\n", temp->reg_no);
            printf("Name: %s\n", temp->name);
            printf("Age: %d\n", temp->age);
            return;
        }
        temp = temp->next;
    }
    printf("\nPassenger not found with Registration Number: %d\n", reg);
}

void printMenu()
{
    printf("\n==============================\n");
    printf("RailwayRes\n");
    printf("________________________A project by Group 3 of division B DESH Department____________________________\n");

    printf("1. Book a ticket\n");
    printf("2. Cancel Booking\n");
    printf("3. Display passenger details\n");
    printf("4. Search passenger\n");
    printf("5. Save reservation data to file\n");
    printf("6. Load reservation data from file\n");
    printf("7. Passenger Statistics\n");
    printf("8. Calculate Ticket Fare\n");
    printf("9. Exit\n");
    printf("==============================\n");
    printf("Option: ");
}

float calculateTicketFare(int age)
{
    float fare = 80.0;

    if (age <= 6)
        fare *= 0.5;
    else if (age >= 60)
        fare *= 0.75;

    return fare;
}

void passengerStatistics()
{
    int totalPassengers = 0;
    int childrenCount = 0;
    int adultCount = 0;
    int seniorCount = 0;

    node *temp = start;
    while (temp != NULL)
    {
        if (temp->age <= 12)
            childrenCount++;
        else if (temp->age <= 59)
            adultCount++;
        else
            seniorCount++;

        totalPassengers++;

        temp = temp->next;
    }

    printf("\nPassenger Statistics:\n");
    printf("Total Passengers: %d\n", totalPassengers);
    printf("Children (Age <= 12): %d\n", childrenCount);
    printf("Adults (Age 13 - 59): %d\n", adultCount);
    printf("Seniors (Age >= 60): %d\n", seniorCount);
}

int main()
{
    int choice, reg;
    start = NULL;
    front = NULL;
    rear = NULL;
    loadFromFile();

    while (1)
    {
        printMenu();
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            if (reserve() == 1)
                printf("\nTicket Booked Successfully!\n");
            else if (reserve() == 0)
                printf("\nReservation is Full! Placed in the waitlist.\n");
            else
                printf("\nInvalid age! Booking failed.\n");
            break;

        case 2:
            printf("\nEnter Registration Number to Cancel: ");
            scanf("%d", &reg);
            if (cancel(reg) == 1)
                printf("\nBooking Cancelled Successfully!\n");
            else
                printf("\nBooking not found with Registration Number: %d\n", reg);
            break;

        case 3:
            display();
            break;

        case 4:
            printf("\nEnter Registration Number to Search: ");
            scanf("%d", &reg);
            searchPassenger(reg);
            break;

        case 5:
            saveToFile();
            break;

        case 6:
            loadFromFile();
            break;

        case 7:
            passengerStatistics();
            break;

        case 8:
            printf("\nEnter passenger age: ");
            int age;
            scanf("%d", &age);
            float fare = calculateTicketFare(age);
            printf("Ticket Fare: Rs %.2f\n", fare);
            break;

        case 9:
            printf("\nExiting Railway Reservation System.\n");
            return 0;

        default:
            printf("\nInvalid choice! Please select a valid option.\n");
            break;
        }
    }

    return 0;
}
