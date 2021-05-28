/*******************************************************************************/
/* Assignment: Food ordering system
/* What the program does: Customers can order food, restaurant managers can manage the system
/* Programmer name: Iris Yan Ning
/* Programmer Matric #: 153410
/******************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <chrono>

using namespace std;

struct UserDetails
{
    // manager login credentials
    string username; // to store the username of restaurant manager
    string password; // to store password of restaurant manager

    // customer 
    string customerName; // to store customer name
};

// general functions
char getUserType(); // get user type - manager/customer
void signup(UserDetails& ud); // sign up for manager
void login(UserDetails& ud); // login for manager
void getManagerAction(); // determine what task manager would like to perform
void getCustomerAction(UserDetails& ud); // determine what action customer would like to perform
//void trackManagerActions(int arrManagerChoices[]); // keep track of actions performed by manager
void continueProgram(); // determine whether user would like to continue using the program
int logout();

// manager functions
void createOrUpdateMenu(); // enables restaurant manager to create or update menu
void updatePrices(); // enables restaurant manager to update price of any item in menu
void viewStats(); // enables restaurant manager to view most popular dish, total number of customers, total sales, total orders etc
void managerHelpInfo(); // enables restaurant manager to understand better about navigating the program

// manager-related functions but not explicity called by manager
float calcTotalPaymentsPerOrder(); // calculates the total payment of the current order
int calcEstDeliveryTime(string& deliveryArea, int& totalPrepTime); // calculates delivery time based on location and preparation time
int displayMenu(); // displays menu to user
bool itemAlreadyExists(string itemName); // check whether the item input by restaurant manager when updating menu already exists
int acceptOrder(int**, int x, int totalMenuItems); // accepts or rejects an order by checking the stock quantity
void updateStocks(int menuIndex, int quantity, int totalMenuItems, int currentStock); // updates stock quantity after an order is accepted
string** readMenu(int&); // returns restaurant menu in dynamic 2D array

// customer functions
void orderOnline(UserDetails& ud); // enables customer to order food
void makePayments(UserDetails& ud); // enables customer to make payment for their order

// customer-related functions but not explicitly called by customer
bool isNewcomer(UserDetails& ud); // check whether customer is new or existing customer 

int main()
{
    UserDetails ud; // define structure variable of UserDetails

    // display welcome message
    cout << "===================================================================\n";
    cout << "====================== Welcome to NinjaFood! ======================\n";
    cout << "===================================================================\n\n";
		 
    char choice = 'Y'; // variable to store choice of user to start program

    cout << "\n=> Would you like to start using this program? [Y/N] "; // prompt user to start program
    cin >> choice; 

	// INPUT VALIDATION
    while (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n')
    {
        cout << "\n/// Sorry, that is not a valid selection. Please try again!";
        cout << "\n=> Do you wish to continue using this program? [Y/N] ";
        cin >> choice;
    }

    if (choice == 'Y' || choice == 'y') // if user chooses to start the program
    {

        // determine whether user is manager or customer
        char userTypeChoice = getUserType();

        // if user says that he is restaurant manager
        if (userTypeChoice == 'M' || userTypeChoice == 'm')
        {
            // call login function as confirmation
            login(ud);
            // manager proceed to carry out tasks
            getManagerAction();
        }
        else
        {
            // customer proceed to order online
            getCustomerAction(ud);
        }
    }

    else // exit program
    {

        // display thank you and goodbye message
        cout << "\n====== Thank you for using NinjaFood's Food Ordering System program! ======\n"
            << "===================== Have a nice day and take care :) ====================\n";
        exit(0);

    }
    return 0;
}

// function to obtain the identity of user, whether he/she is restaurant manager or customer
char getUserType()
{
    string choice; // variable to store choice (string to prevent error due to garbage input value)

    // prompt user to select his/her identity
    cout << "\n=> Please select your identity: \n"
        << "[M] Restaurant Manager\n"
        << "[C] Customer\n";
    cin >> choice; // read input as string but take only the first char

    // INPUT VALIDATION - check that the input value is only one character
    // then check whether the character is valid
    while (choice.length() > 1)
    {
        cout << "\n/// Sorry, that is not a valid selection. Please try again!\n";
        cout << "=> Please select your identity: \n"
            << "[M] Restaurant Manager\n"
            << "[C] Customer\n";
        cin >> choice;

        while (choice[0] != 'M' && choice[0] != 'm' && choice[0] != 'C' && choice[0] != 'c')
        {
            cout << "\n/// Sorry, that is not a valid selection. Please try again!\n";
            cout << "=> Please select your identity: \n"
                << "[M] Restaurant Manager\n"
                << "[C] Customer\n";

            cin >> choice;
        }
    }
    return choice[0];
}

// login module to ensure security of system so that ONLY restaurant managers have access to manage the system
void login(UserDetails& ud)
{
    bool matchFound = false; // variable to store bool of whether login credentials entered match database
    string choice; // variable to store choice of either login or sign up
    string data; // temporary variable to store anything not required from file

    // open login credentials file to read from it
    ifstream file;
    file.open("login_credentials_153410.txt");

    cout << "\n**************************** LOGIN PAGE ****************************\n";

	// prompt user to choose between login or sign up
    cout << "\n/// Welcome. You are accessing as RESTAURANT MANAGER.\n"
        << "=> Please select an option below to proceed:\n"
        << "[1] Login\n"
        << "[2] Sign up\n";
    cin >> choice;
    cin.ignore();

    // INPUT VALIDATION - check that the input value is only one character
    // then check whether the character is valid
    while (choice.length() > 1)
    {
        cout << "\n/// Sorry, that is not a valid selection. Please try again!\n";
        cout << "=> Please select your identity: \n"
            << "[M] Restaurant Manager\n"
            << "[C] Customer\n";
        cin >> choice;

        while (choice[0] != '1' && choice[0] != '2')
        {
            cout << "\n/// Sorry, that is not a valid selection. Please try again!\n"
                << "=> Please select an option below:\n"
                << "[1] Login\n"
                << "[2] Sign up\n";
            cin >> choice;
            cin.ignore();
        }
    }

    // if manager choose login
    if (choice[0] == '1')
    {
        // check whether login_credentials.txt file exists in the first place
        if (file.fail())
        {
            cout << "\n/// No login credentials of any restaurant managers found.\nPlease sign up first!\n"
                << "/// Redirecting to sign up page...\n\n";

            // if not, it means there are no login credentials record, proceed to sign up
            signup(ud);
        }

        // check whether login_credentials.txt is empty
        else if (file.seekg(0, ios::end), file.tellg() == 0)
        {
            cout << "\n/// No login credentials of any restaurant managers found.\nPlease sign up first!\n"
                << "/// Redirecting to sign up page...\n\n";

            // if not, it means there are no login credentials record, proceed to sign up
            signup(ud);
        }
        else
        {
            while (!matchFound) // loop to prompt user to enter credentials as long as either one is incorrect
            {
                cout << "\n=> Enter username: ";
                cin >> ud.username;
                cout << "=> Enter password: ";
                cin >> ud.password;

                file.clear(); // clear any badbit
                file.seekg(0, ios::beg); // set pointer to beginning of file

                while (getline(file, data, '\t')) // get username from file separated by tab delimiter
                {
                    // if username match found
                    if (ud.username == data)
                    {
                        // now retrieve password
                        getline(file, data, '\n');

                        // if password match found
                        if (ud.password == data)
                        {
                            matchFound = true; // access is given
                            break; // break out of loop, no need to check remaining usernames and passwords
                        }
                    }
                    else
                        // skip password checking if no username match found
                        getline(file, data);
                }
                if (!matchFound) // if match not found
                {
                    // re-enter username and password
                    cout << "\n/// Incorrect username or password."
                        << "\n/// Username and password are case-sensitive."
                        << "\n/// Please try again!\n";
                }
                else
                {
                    cout << "\n/// Login as RESTAURANT MANAGER successful!\n\n"; // successful login
                    break;
                }
            }

        }
        file.close();
    }
    else
    {
        signup(ud); // go to sign up function if user selects choice to sign up
    }
}

// function to log out restaurant manager and redirect to main page
int logout() 
{
    cout << "\n/// Redirecting to main page...\n\n";
    return main();
}

// function for new restaurant manager to sign up so that they can access the full system
void signup(UserDetails& ud)
{
    string data; // variable to temporarily store anything not required from file
    string newUsername; // variabe to store new username  for sign up
    bool valid = false; // variable to store whether the newly entered credentials are valid

    // open file containing login credentials
    fstream file;
    file.open("login_credentials_153410.txt", ios::out | ios::in | ios::app);

    cout << "\n*************************** SIGN UP PAGE ***************************\n";
    cout << "\n/// You are signing up as RESTAURANT MANAGER.\n";

    while (!valid) // loop as long as the credentials entered are invalid
    {
        file.seekp(0, ios::beg); // set pointer to beginning of file 
        
        // prompt user to enter a new username
        cout << "\n=> Enter new username: ";
        getline(cin, newUsername);

        while (file)
        {
            getline(file, data, '\t'); // check the file

            // if the new username is found to already exist
            if (newUsername == data)
            {
                valid = false;
                cout << "\n/// This username already exists. Please try again!\n";
                break;
                // break out of file-checking loop, then loop again to re-enter a new username
            }
            else // if no existing username match found on this line
            {
                valid = true;
                getline(file, data, '\n'); // skip password to check next username
            }
        }
    }

    file.clear(); // clear any badbit

    // if entered username found to be a completely new username hence valid
    if (valid)
    {
        file << newUsername << '\t'; // input username into file
    }
    cin.clear();

	// now prompt user to enter a new password, requirement is it must have at least 6 characters
    cout << "=> Enter new password (min 6 characters): ";
    getline(cin, ud.password);

    // check whether password has at least 6 characters
    while (ud.password.length() < 6)
    {
        cout << "\n/// Password must be at least 6 characters!\n";
        // if not, re-enter password
        cout << "=> Enter new password (min 6 characters): ";
        getline(cin, ud.password);
    }
    // if password valid, input password into file
    file << ud.password << "\n";

    cout << "\n/// Sign up successful!\n"
        << "/// Redirecting to login page...\n\n";

    file.close();
    
    // redirect to login after sign up
    login(ud);
}

// function to determine what action the restaurant manager would like to perform
void getManagerAction()
{
    string actionChoice; // variable to store int representation of action choice

	// prompt user to enter choice
    cout << "\n=> Please select an action as RESTAURANT MANAGER: \n"
        << "[1] Create/update menu\n"
        << "[2] Update prices\n"
        << "[3] View stats (most popular dish, total number of customers, total sales, total number of orders)\n"
        << "\n/// Do you need help navigating the system? Select the option below! \n"
        << "[4] View Manager Help info\n";
    cin >> actionChoice;


    // INPUT VALIDATION - check that the input value is only one character
    // then check whether the character is valid
    while (actionChoice.length() > 1)
    {
        cout << "\n/// Sorry, that is not a valid selection. Please try again!\n";
        cout << "=> Please select an action as RESTAURANT MANAGER: \n"
            << "[1] Create/update menu\n"
            << "[2] Update prices\n"
            << "[3] View stats (most popular dish, total number of customers, total sales, total number of orders)\n"
            << "\n/// Do you need help navigating the system? Select the option below! \n"
            << "[4] View Manager Help info\n";
        cin >> actionChoice;

        while (actionChoice[0] != '1' && actionChoice[0] != '2' && actionChoice[0] != '3' && actionChoice[0] != '4')
        {
            cout << "\n/// Sorry, that is not a valid selection. Please try again!\n";
            cout << "=> Please select an action as RESTAURANT MANAGER: \n"
                << "[1] Create/update menu\n"
                << "[2] Update prices\n"
                << "[3] View stats (most popular dish, total number of customers, total sales, total number of orders)\n"
                << "\n/// Do you need help navigating the system? Select the option below! \n"
                << "[4] View Manager Help info\n";
            cin >> actionChoice[0];
        }
    }

    // switch case based on action choice input by user
    switch (actionChoice[0])
    {
    case '1':
        createOrUpdateMenu(); // direct to function to create/update menu
        break;
    case '2':
        updatePrices(); // direct to function to update price of food item in menu
        break;
    case '3':
        viewStats(); // direct to function to view most popular dish, total number of customers, total sales, total orders etc
        break;
    case '4':
        managerHelpInfo(); // direct to function to view guidelines on using this program
        break;
    default:
        // do nothing
        cout << "Error!\n";
    }

}

// function to get customer action on whether they wish to make an order or make payment
void getCustomerAction(UserDetails& ud)
{
    string actionChoice; // variable to store int representation of action choice

    // prompt customer to select between order online or make payment
    cout << "\n=> Please select an action as CUSTOMER: \n"
        << "[1] Order online\n"
        << "[2] Make payment\n";
    cin >> actionChoice;

    // INPUT VALIDATION - check that the input value is only one character
    // then check whether the character is valid
    while (actionChoice.length() > 1)
    {
        cout << "\n/// Sorry, that is not a valid selection. Please try again!\n";
        cout << "\n=> Please select an action as CUSTOMER: \n"
            << "[1] Order online\n"
            << "[2] Make payment\n";
        cin >> actionChoice;

        while (actionChoice[0] != '1' && actionChoice[0] != '2')
        {
            cout << "\n/// Sorry, that is not a valid selection. Please try again!\n";
            cout << "\n=> Please select an action as CUSTOMER: \n"
                << "[1] Order online\n"
                << "[2] Make payment\n";
            cin >> actionChoice[0];
        }
    }

    switch (actionChoice[0])
    {
    case '1':
        orderOnline(ud); // direct to function to order online
        break;
    case '2':
        makePayments(ud); // direct to function to make payment
        break;
    default:
        // do nothing
        cout << "Error!\n";
    }
}

// As this is a user-friendly system,
// this function displays guidelines to help the restaurant manager navigate the program effectively
void managerHelpInfo()
{
    cout << "\n********************** MANAGER HELP INFO PAGE ***********************\n";
    cout << "\n/// You have selected the option to: View Manager Help Info\n";
    cout << "\nWelcome to NinjaFood Food Ordering System!\n"
        << "This system allows Restaurant Managers to oversee all restaurant details and statistics.\n";

    cout << "\n===> Options specifications"
        << "\n\n[1] Create/Update Menu"
        << "\n\t This option allows the Restaurant Manager to create a new menu."
        << "\n\t The menu should contain the following details: "
        << "\n\t\t 1. Item name"
        << "\n\t\t 2. Item price"
        << "\n\t\t 3. Preparation time (time taken to prepare this item)"
        << "\n\t\t 4. Stock (stock quantity of item available for order)"

        << "\n\n[2] Update Prices"
        << "\n\t This option allows the Restaurant Manager to update the price of any item."

        << "\n\n[3] View Stats"
        << "\n\t This option allows the Restaurant Manager to view the statistics and business analysis of the restaurant."
        << "\n\t The following details are calculated, analysed and displayed: "
        << "\n\t\t 1. Most popular dish"
        << "\n\t\t 2. Total number of orders today"
        << "\n\t\t 3. Total sales today"
        << "\n\t\t 4. Total number of customers today";

    cout << "\n=============================================================\n";

    // to ask user whether they want to continue using the program
    continueProgram();
}

// function to read the menu file and store all file content into a 2D dynamically allocated array
// this function takes variable totalNumItems (total number of menu items) passed by reference
string** readMenu(int& totalNumItems)
{
    string itemName; // string variable to store food item name
    string line; // string variable to store line in file temporarily
    float itemPrice = 0; // float variable to store item price
    string strItemPrice; // string variable to store item price in string
    int preparationTime = 0; // int variable to store preparation time in minutes
    string strPreparationTime; // string variable to store preparation time in string
    int stock = 0; // int variable to store stock quantity
    string strStock; // string variable to store stock quantity in string
    char toSkip; // char variable to store a character to skip in file
    string strIndex; // string to store index in string

    fstream file;
    file.open("menu_153410.txt", ios::in); // open menu file 

    int index = 0; // index the number of food items

    while (getline(file, line))  // check how many items in the menu by determining number of lines
        ++index;
    totalNumItems = index; // get total number of menu items by iterating through menu and count number of lines

    // declare and initialize a DYNAMICALLY ALLOCATED 2D ARRAY
    string** arrMenuContent = new string * [index];
    for (int j = 0; j < index; j++)
    {
        arrMenuContent[j] = new string[5];
    }

    if (index == 0) // check if menu is empty
        return arrMenuContent;
    else
    {
        // display menu
        // menu structure: index,itemName,itemPrice,preparationTime, stock
        file.clear(); // clear bad bit
        file.seekp(0, ios::beg); // set pointer to beginning of file

        int i = 0;
        while (file >> index)
        {
            file >> toSkip; // skip the comma between index and item name
            getline(file, itemName, ','); // retrieve item name, comma as delimiter
            file >> itemPrice; // retrieve item price
            file >> toSkip; // skip comma
            file >> preparationTime; // retrieve preparation time
            file >> toSkip; // skip comma
            file >> stock; // retrieve stock quantity

            // convert to string to store into string array
            strIndex = to_string(index); 
            strItemPrice = to_string(itemPrice);
            strPreparationTime = to_string(preparationTime);
            strStock = to_string(stock);

            // store each info into string array
            arrMenuContent[i][0] = strIndex;
            arrMenuContent[i][1] = itemName;
            arrMenuContent[i][2] = strItemPrice;
            arrMenuContent[i][3] = strPreparationTime;
            arrMenuContent[i][4] = strStock;

            ++i; // increment value of i to go to next element in array
        }
    }

    file.close(); // close file
    return arrMenuContent;
}

// manager functions here
// function to enable restaurant manager to create or update menu
void createOrUpdateMenu()
{
    int numbering = 1; // int variable to store numbering and will be incremented based on menu item sequence
    char choice = 'Y'; // char variable to store choice of whether user wishes to continue updating menu
    string itemName; // string variable to store food item name
    string line; // string variable to store line in file temporarily
    float itemPrice = 0; // float variable to store item price
    int preparationTime = 0; // int variablle to store preparation time in minutes
    int stock = 0; // int variable to store stock quantity

    fstream file;
    file.open("menu_153410.txt", ios::out | ios::app); // open file to output and append

    cout << "\n*********************** CREATE/UPDATE MENU PAGE ***********************\n";

    cout << "\n/// You have selected the option to: Update/Create Menu\n";

    // menu structure: index,itemName,itemPrice,preparationTime,stock
    int index = displayMenu();
    numbering = ++index;

    if (index == 0) // check if menu is empty
        cout << "/// Menu does not exist. Creating menu...\n";

    while (choice == 'Y' || choice == 'y')
    {
        cout << "\n=> Enter name of item #" << numbering << ": "; // enter new food item name
        cin.ignore(); // ignore enter key buffer
        getline(cin, itemName); // enter item name

        // input validation - check item name is not empty/length of name not more than 25 char/item doesnt already exist
        while (itemName.empty() || itemName == " " || itemName.length() >= 25 || itemAlreadyExists(itemName))
        {
            if (itemName.empty() || itemName == " ")
            {
                cout << "/// Item name cannot be empty!\n";
            }
            else if (itemName.length() >= 25)
            {
                cout << "/// Item name cannot be more than 25 characters! Please try again.\n";
            }
            else
            {
                cout << "\n/// This item already exists in the menu!";
                cout << "\n/// Please consider \"Update Prices\" option for this item instead.\n";
            }

            cout << "\n=> Enter name of item #" << numbering << ": "; // enter new food item name
            getline(cin, itemName); // enter item name
        }
        file << numbering << "," << itemName << ",";

        cout << "=> Enter price of item #" << numbering << ": $"; // enter item price
        cin >> itemPrice;
        // input validation - check item price is not equal to or less than 0
        while (itemPrice <= 0)
        {
            cout << "\n/// Item price cannot be equal to or less than zero! Please try again.\n";
            cout << "=> Enter price of item #" << numbering << ": $"; // enter item price
            cin >> itemPrice;
        }
        file << fixed << setprecision(2) << itemPrice << ","; // set to 2 decimal places

        cout << "=> Enter preparation time of item #" << numbering << " (in minutes): "; // enter preparation time
        cin >> preparationTime;
        // input validation - check preparation time is not equal to or less than 0
        while (preparationTime <= 0)
        {
            cout << "\n/// Preparation time cannot be less than or equal to zero! Please try again.\n";
            cout << "=> Enter preparation time of item #" << numbering << " (in minutes): "; // enter preparation time
            cin >> preparationTime;
        }
        file << preparationTime << ",";

        cout << "=> Enter stock quantity of item #" << numbering << " : "; // enter stock quantity
        cin >> stock;
        // input validation - check stock is not equal to or less than 0
        while (stock <= 0)
        {
            cout << "\n/// Stock quantity cannot be less than or equal to zero! Please try again.\n";
            cout << "=> Enter stock quantity of item #" << numbering << " : "; // enter stock quantity
            cin >> stock;
        }
        file << stock << "\n";


        cout << "=> Do you wish to continue? [Y/N] "; // ask user whether they wish to continue entering new food items
        cin >> choice;

        while (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n')
        {
            cout << "\n/// Sorry, that is not a valid selection. Please try again!";
            cout << "\n=> Do you wish to continue? [Y/N] "; // ask user whether they wish to continue entering new food items
            cin >> choice;
        }

        ++numbering; // increment index
    }

    file.seekp(0, ios::beg); // set pointer to beginning of file

    cout << "\n/// Displaying updated menu...\n";
    displayMenu();

    // to ask user whether they want to continue using the program
    continueProgram();
}

void updatePrices()
{
    int index = 0;
    int userIndex = 0;
    int totalNumItems = 0;
    string itemName;
    string strItemPrice; // item price in string form
    float itemPrice = 0;
    string strOldPrice;
    float oldPrice = 0;
    string strNewPrice;
    float newPrice = 0;
    char choice = 'Y';
    int preparationTime = 0;
    string strPreparationTime; // preparation time in string form
    int stock = 0;
    string strStock; // stock quantity in string form
    string line; // temporary variable to store anything not required from file

    string** arrMenuContent = readMenu(totalNumItems);

    cout << "\n**************************** UPDATE PRICES ****************************\n";
    cout << "\n/// You have selected the option to: Update Prices\n";

    displayMenu();
    fstream file;
    file.open("menu_153410.txt", ios::out); // open file for output to update price

    // if menu file is empty or not found, go to create menu first
    if (file.fail() || totalNumItems == 0)
    {
        cout << "/// Menu not found! Please create the menu first and try again.\n";
        cout << "/// Redirecting to Create/Update Menu...\n";
        createOrUpdateMenu();
    }
    else
    {
        // display menu
        // menu structure: index,itemName,itemPrice,preparationTime
        while (choice == 'Y' || choice == 'y')
        {
            cout << "\n=> Please enter index of item to update price: ";
            cin >> userIndex;

            while (userIndex < 1 || userIndex > totalNumItems) // input validation
            {
                cout << "/// Invalid index. Please try again!\n";
                cout << "\n=> Please enter index of item to update price: ";
                cin >> userIndex;
            }

            for (int i = 0; i < totalNumItems; i++)
            {
                index = stoi(arrMenuContent[i][0]);
                if (index == userIndex)
                {
                    itemName = arrMenuContent[i][1];
                    strOldPrice = arrMenuContent[i][2];
                    oldPrice = stof(strOldPrice);
                    cout << "\n/// Current price for " << itemName << ": $" << oldPrice;
                    cout << "\n=> Please enter the new price for " << itemName << ": $";
                    cin >> newPrice; // get new price

                    while (newPrice == oldPrice || newPrice <= 0)
                    {
                        if (newPrice == oldPrice)
                        {
                            cout << "\n/// New price cannot be equal to old price!\n";
                            cout << "=> Please enter the new price for " << itemName << ": $";
                            cin >> newPrice;
                        }
                        else if (newPrice <= 0)
                        {
                            cout << "\n/// New price cannot be less than or equal to zero!\n";
                            cout << "=> Please enter the new price for " << itemName << ": $";
                            cin >> newPrice;
                        }
                    }

                    strNewPrice = to_string(newPrice);
                    arrMenuContent[i][2] = strNewPrice;
                    break;
                }
            }

            for (int j = 0; j < totalNumItems; j++)
            {
                string strIndex = arrMenuContent[j][0]; // get index in string form from string array
                file << strIndex << ","; // output to file

                itemName = arrMenuContent[j][1]; // get item name
                file << itemName << ","; // output to file

                strItemPrice = arrMenuContent[j][2]; // get item price in string form from string array
                itemPrice = stof(strItemPrice); // convert price to float
                file << fixed << setprecision(2) << itemPrice << ","; // output to file

                strPreparationTime = arrMenuContent[j][3]; // get preparation time in string form
                preparationTime = stoi(strPreparationTime); // convert to int
                file << preparationTime << ","; // output to file

                strStock = arrMenuContent[j][4]; // get stock quantity in string form
                stock = stoi(strStock); // convert to int
                file << stock << "\n"; // output to file
            }

            file.clear();
            file.seekp(0, ios::beg); // set pointer to beginning of file

            cout << "\n/// Displaying updated menu...\n";
            displayMenu();

            cout << "\n=> Do you wish to continue updating prices? [Y/N] "; // ask user whether they wish to continue updating price
            cin >> choice;

            while (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n')
            {
                cout << "\n/// Sorry, that is not a valid selection. Please try again!";
                cout << "\n=> Do you wish to continue? [Y/N] "; // ask user whether they wish to continue updating price
                cin >> choice;
            }
        }
    }
    delete[] arrMenuContent;
    file.close();

    // to ask user whether they want to continue using the program
    continueProgram();
}

void viewStats()
{
    int index = 0;
    float line = 0; // represents each line of sales value in the file
    int numOfCustomers = 0;
    string data; // temporary variable to store anything not required from file
    int numbering = 0;
    int menuIndex = 0;
    string strMenuIndex;
    string itemName;
    string strItemPrice;
    float itemPrice = 0;
    int quantity = 0;
    int preparationTime = 0;
    char toSkip;
    bool alreadyExists = false;
    int topDishIndex = 0;
    int maxQuantity = 0;
    int totalNumItems;

    float totalSales = 0;
    fstream totalSalesFile;
    totalSalesFile.open("total_sales_153410.txt", ios::in | ios::out | ios::app);
    fstream topdishFile;
    topdishFile.open("topdish_153410.txt", ios::in | ios::out | ios::app);

    string** arrMenuContent = readMenu(totalNumItems);

    cout << "\n**************************** VIEW STATS ****************************\n";

    cout << "\n/// You have selected the option to: View Stats\n";

    while (getline(topdishFile, data))  // check if top dish file has content
        ++numbering;
    if (numbering == 0) // if file is empty
    {
        cout << "\n/// Stats not available. \n/// Reason: No orders have been made yet.\n"
            << "/// Please wait for a customer to order first, then try again!\n";
    }
    else
    {
        // dynamically allocated PARALLEL ARRAY
        int* arrDishes = nullptr;
        arrDishes = new int[numbering];

        int* arrQuantity = nullptr;
        arrQuantity = new int[numbering];

        topdishFile.clear();
        topdishFile.seekg(0, ios::beg);

        int x = 0;
        while (topdishFile >> menuIndex)
        {
            topdishFile >> toSkip;
            topdishFile >> quantity;

            for (int z = 0; z < x + 1; z++)
            {
                // if same dish found
                if (arrDishes[z] == menuIndex)
                {
                    arrQuantity[z] += quantity; // update quantity
                    alreadyExists = true;
                    break;
                }
            }
            if (!alreadyExists)
            {
                arrDishes[x] = menuIndex; // insert menu item to array if new
                arrQuantity[x] = quantity; // insert quantity to array
                ++x;
            }
        }

        // loop to look for item with highest quantity aka most popular dish
        for (int y = 0; y < x; y++)
        {
            if (arrQuantity[y] > maxQuantity)
            {
                maxQuantity = arrQuantity[y];
                topDishIndex = arrDishes[y];
            }
        }

        if (totalNumItems == 0)
        {
            cout << "/// Menu not found! Please create the menu first and try again.\n";
            cout << "/// Redirecting to Create/Update Menu...";
            createOrUpdateMenu();
        }
        else
        {
            // display menu
            // menu structure: index,itemName,itemPrice,preparationTime
            for (int i = 0; i < totalNumItems; i++)
            {
                strMenuIndex = arrMenuContent[i][0];
                menuIndex = stoi(strMenuIndex);
                if (menuIndex == topDishIndex)
                {
                    itemName = arrMenuContent[i][1];
                    strItemPrice = arrMenuContent[i][2];
                    itemPrice = stof(strItemPrice);

                    cout << "\n1. MOST POPULAR DISH OF NINJAFOOD: \n";
                    cout << "===> " << left << setw(20) << itemName
                        << "$" << setw(5) << fixed << setprecision(2) << itemPrice
                        << "\t\tTotal orders: " << maxQuantity
                        << "\tTotal profit: " << "$" << setw(5) << fixed << setprecision(2) << maxQuantity * itemPrice << "\n";
                    break;
                }
            }
        }

        while (getline(totalSalesFile, data))  // check if total sales file has content
            ++index;
        totalSalesFile.clear();
        totalSalesFile.seekg(0, ios::beg);
        while (totalSalesFile >> line)
        {
            totalSales += line;
            ++numOfCustomers;
        }
        cout << "\n2. TOTAL ORDERS TODAY: " << index;
        cout << fixed << setprecision(2) << "\n3. TOTAL SALES TODAY: $" << totalSales;
        cout << "\n4. TOTAL NUMBER OF CUSTOMERS TODAY: " << numOfCustomers << "\n";
    }

    totalSalesFile.close();
    topdishFile.close();

    // to ask user whether they want to continue using the program
    continueProgram();
}

// not a manager choice (called by other function)
int displayMenu()
{
    string itemName;
    string line;
    float itemPrice = 0;
    string strItemPrice;
    int preparationTime = 0; // in minutes
    string strPreparationTime;
    int stock = 0;
    string strStock;
    string strIndex;
    int index = 0;
    int totalNumItems = 0;

    string** arrMenuContent = readMenu(totalNumItems);

    if (totalNumItems == 0) // check if menu is empty
    {
        return 0;
    }
    else
    {
        // display menu
        // menu structure: index,itemName,itemPrice,preparationTime,stock
        cout << "\n/// There are currently " << totalNumItems << " item(s) on the menu.\n"; // display number of items on mennu
        cout << "\n============================================================================\n";
        cout << "=================================== MENU ===================================\n";
        cout << "============================================================================\n\n";
        cout << "NO.  ITEM NAME\t\t      ITEM PRICE\tPREPARATION TIME\t STOCK";
        cout << "\n-----------------------------------------------------------------------------\n";
        for (int i = 0; i < totalNumItems; i++)
        {
            strIndex = arrMenuContent[i][0];
            index = stoi(strIndex);

            itemName = arrMenuContent[i][1];

            strItemPrice = arrMenuContent[i][2];
            itemPrice = stof(strItemPrice);

            strPreparationTime = arrMenuContent[i][3];
            preparationTime = stoi(strPreparationTime);

            strStock = arrMenuContent[i][4];
            stock = stoi(strStock);

            cout << index << ")   " << left << setw(25) << itemName
                << "\t$" << fixed << setprecision(2) << itemPrice;
            cout << "\t\t" << preparationTime << " minutes";
            cout << "\t\t" << stock << "\n";
        }
    }
    return totalNumItems;
}


// not a manager choice (called by other function)
    // order taken into 2d dynamic array [menu index][quantity]
    // acceptOrder() takes pointer 2d dynamic array, reads whole array and checks each item whether it is valid by comparing stock quantity
    // if that item is found to be not valid, set that menu index and quantity to 0
int acceptOrder(int** arrOrder, int x, int totalMenuItems)
{
    int numbering = 0;
    int menuIndex = 0;
    string line;
    string itemName;
    int preparationTime = 0;
    int stock = 0;
    int invalidItemIndex = 0;
    int totalNumItems = 0;
    string strStock;

    fstream topdishFile;
    topdishFile.open("topdish_153410.txt", ios::out | ios::app);

    // menu structure: index,itemName,itemPrice,preparationTime,stock
    // dynamically allocated memory
    string** arrMenuContent = readMenu(totalNumItems);

    for (int i = 0; i < totalMenuItems; i++)
    {
        menuIndex = stoi(arrMenuContent[i][0]);
        if (menuIndex = arrOrder[x][0])
        {
            strStock = arrMenuContent[i][4];
            stock = stoi(strStock);
            break;
        }
    }

    if (arrOrder[x][1] > stock) // if quantity of the particular item more than stock (stock insufficient)
    {
        invalidItemIndex = arrOrder[x][0]; // store index of invalid item first
        arrOrder[x][0] = 0; // set menu index to 0
        arrOrder[x][1] = 0; // set quantity to 0
    }
    else // valid order!
    {
        // write (append) menu index and quantity to top dish file
        topdishFile << arrOrder[x][0] << "," << arrOrder[x][1] << "\n";
        updateStocks(menuIndex, arrOrder[x][1], totalMenuItems, stock);
    }
    topdishFile.close();
    return invalidItemIndex;
}

// not a manager choice (called by other function)
void updateStocks(int menuIndex, int quantity, int totalMenuItems, int currentStock)
{
    int index = 0;
    string itemName;
    string strItemPrice; // item price in string form
    float itemPrice = 0;
    char choice = 'Y';
    int preparationTime = 0;
    string strPreparationTime; // preparation time in string form
    int stock = 0;
    string strStock; // stock quantity in string form
    string line; // temporary variable to store anything not required from file
    int totalNumItems = 0;

    // menu structure: index,itemName,itemPrice,preparationTime,stock

    // dynamically allocated memory
    string** arrMenuContent = readMenu(totalNumItems);

    for(int i = 0; i < totalMenuItems; i++)
    {
        index = stoi(arrMenuContent[i][0]);
        if (index == menuIndex)
        {
            stock = currentStock - quantity;
            string strStock = to_string(stock); // convert new stock to string
            arrMenuContent[i][4] = strStock; // store new stock into array
            break;
        }
    }

    fstream file;
    file.open("menu_153410.txt", ios::out); // open file for output to update stock
    for (int j = 0; j < totalNumItems; j++)
    {
        string strIndex = arrMenuContent[j][0]; // get index in string form from string array
        file << strIndex << ","; // output to file

        itemName = arrMenuContent[j][1]; // get item name
        file << itemName << ","; // output to file

        strItemPrice = arrMenuContent[j][2]; // get item price in string form from string array
        itemPrice = stof(strItemPrice); // convert price to float
        file << fixed << setprecision(2) << itemPrice << ","; // output to file

        strPreparationTime = arrMenuContent[j][3]; // get preparation time in string form
        preparationTime = stoi(strPreparationTime); // convert to int
        file << preparationTime << ","; // output to file

        strStock = arrMenuContent[j][4]; // get stock quantity in string form
        stock = stoi(strStock); // convert to int
        file << stock << "\n"; // output to file
    }

    delete[] arrMenuContent;
    file.close();
}

// not a manager choice (called by other function)
float calcTotalPaymentsPerOrder()
{
    ifstream receipt;
    receipt.open("receipt_153410.txt");

    fstream totalSalesFile;
    totalSalesFile.open("total_sales_153410.txt", ios::in | ios::out | ios::app);

    string datetime;
    int numbering = 0;
    int menuIndex = 0;
    string itemName;
    float itemPrice = 0;
    int quantity = 0;
    int preparationTime = 0;
    char toSkip;

    float totalPayment = 0;

    // receipt structure: 
    //      date & time
    //      numbering,menuIndex,itemName,itemPrice,preparationTime,quantity
    getline(receipt, datetime);
    while (receipt >> numbering)
    {
        receipt >> toSkip;
        receipt >> menuIndex;
        receipt >> toSkip;
        getline(receipt, itemName, ',');
        receipt >> itemPrice;
        receipt >> toSkip;
        receipt >> preparationTime;
        receipt >> toSkip;
        receipt >> quantity;

        totalPayment += itemPrice * quantity;
    }

    // add total payment of this order to total sales file
    totalSalesFile << totalPayment << "\n";

    receipt.close();
    totalSalesFile.close();

    return totalPayment;
}

// not a manager choice (called by other function)
int calcEstDeliveryTime(string& deliveryArea, int& totalPrepTime)
{
    ifstream file;
    file.open("receipt_153410.txt");

    string datetime;
    int numbering = 0;
    int menuIndex = 0;
    string itemName;
    float itemPrice = 0;
    int quantity = 0;
    char toSkip;

    int deliveryTravelTime = 0;
    int preparationTime = 0;
    int deliveryTime = 0; // in minutes

    // file(receipt) structure: 
    //      date & time
    //      numbering,menuIndex,itemName,itemPrice,preparationTime,quantity
    getline(file, datetime);
    while (file >> numbering)
    {
        file >> toSkip;
        file >> menuIndex;
        file >> toSkip;
        getline(file, itemName, ',');
        file >> itemPrice;
        file >> toSkip;
        file >> preparationTime;
        file >> toSkip;
        file >> quantity;

        totalPrepTime += quantity * preparationTime;
    }

    cout << "\n=> Please enter your delivery area (1 - 6): "
        << "\n[1] Cahaya Gemilang"
        << "\n[2] Aman Damai"
        << "\n[3] Indah Kembara"
        << "\n[4] Restu"
        << "\n[5] Saujana"
        << "\n[6] Tekun\n";
    cin >> deliveryArea;

    // INPUT VALIDATION - check that the input value is only one character
 // then check whether the character is valid
    while (deliveryArea.length() > 1)
    {
        cout << "\n/// Invalid delivery area! Please try again.";
        cout << "\n=> Please enter your delivery area (1 - 6): "
            << "\n[1] Cahaya Gemilang"
            << "\n[2] Aman Damai"
            << "\n[3] Indah Kembara"
            << "\n[4] Restu"
            << "\n[5] Saujana"
            << "\n[6] Tekun\n";
        cin >> deliveryArea;

        while (deliveryArea[0] != '1' && deliveryArea[0] != '2' && deliveryArea[0] != '3' &&
            deliveryArea[0] != '4' && deliveryArea[0] != '5' && deliveryArea[0] != '6')
        {
            cout << "\n/// Invalid delivery area! Please try again.";
            cout << "\n=> Please enter your delivery area (1 - 6): "
                << "\n[1] Cahaya Gemilang"
                << "\n[2] Aman Damai"
                << "\n[3] Indah Kembara"
                << "\n[4] Restu"
                << "\n[5] Saujana"
                << "\n[6] Tekun\n";
            cin >> deliveryArea;
        }
    }

    switch (deliveryArea[0])
    {
    case '1':
        deliveryTravelTime = 5;
        break;
    case '2':
        deliveryTravelTime = 5;
        break;
    case '3':
        deliveryTravelTime = 6;
        break;
    case '4':
        deliveryTravelTime = 10;
        break;
    case '5':
        deliveryTravelTime = 9;
        break;
    default:
        deliveryTravelTime = 8;
        break;
    }

    deliveryTime = totalPrepTime + deliveryTravelTime;

    return deliveryTime;
}

// EXTRA FEATURE:
// to check whether newly input menu item name by user is an existing item
// not a manager choice (called by other function)
bool itemAlreadyExists(string itemName)
{
    char toSkip;
    int numbering;
    string line;
    float itemPrice;
    int preparationTime;
    fstream file;
    file.open("menu_153410.txt", ios::in); // open file
    while (file >> numbering)
    {
        file >> toSkip;
        getline(file, line, ',');
        if (line == itemName)
        {
            return true;
        }
        file >> itemPrice; // skip item price
        file >> toSkip; // skip comma
        file >> preparationTime; // skip preparation tiime
    }
    file.close();
    return false;
}

void continueProgram()
{
    // to ask user whether they want to continue using the program

    char choice = 'Y';

    cout << "\n=> Do you wish to continue using this program? [Y/N] ";
    cin >> choice;

    while (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n')
    {
        cout << "\n/// Sorry, that is not a valid selection. Please try again!";
        cout << "\n=> Do you wish to continue using this program? [Y/N] ";
        cin >> choice;
    }

    if (choice == 'Y' || choice == 'y')
        getManagerAction();
    else
    {
        cout << "\n/// Redirecting to logout...\n";
        logout();
    }
}

// customer functions here
void orderOnline(UserDetails& ud)
{
    int index = 0;
    int menuIndex = 0;
    string itemName;
    float itemPrice = 0;
    string strItemPrice;
    string strPreparationTime;
    int preparationTime = 0;
    string strStock;
    int stock = 0;
    int menuChoice = 0;
    int quantity = 0;
    float totalPrice = 0;
    char choice = 'Y';
    int invalidItemIndex = 0;
    int proceedChoice;
    int totalNumItems = 0;
    string line; // temporary variable to store anything not required from file
    bool itemAlreadyExists = false;

    time_t now = time(0); // get current date and time based on computer system
    char* datetime = ctime(&now); // convert now to string

    fstream receipt;
    receipt.open("receipt_153410.txt", ios::out);

    cout << "\n**************************** ORDER PAGE ****************************\n";

    string** arrMenuContent = readMenu(totalNumItems);

    if (totalNumItems == 0)
    {
        cout << "\n/// Sorry, menu does not exist. Please contact a RESTAURANT MANAGER for help to CREATE MENU.";
        logout();
    }
    else
    {
        cout << "\n/// You are now ordering online as customer.\n";

        // display menu to customer to refer to order food
        displayMenu();

        // dynamically allocated 2D ARRAY to get order from customer to store menu index and quantity
        int** arrMenuChoices = new int* [totalNumItems];
        for (int i = 0; i < totalNumItems; i++)
        {
            arrMenuChoices[i] = new int[2];
        }

        int x = 0;
        while (choice == 'Y' || choice == 'y')
        {
            cout << "\n=> Please enter the index number of your choice of food (1 - " << totalNumItems << "): ";
            cin >> menuChoice;

            while (menuChoice < 1 || menuChoice > totalNumItems)
            {
                cout << "/// Sorry, that is not a valid selection. Please try again!\n";
                cout << "\n=> Please enter the index number of your choice of food (1 - " << totalNumItems << "): ";
                cin >> menuChoice;
            }

            cout << "=> Please enter the quantity: ";
            cin >> quantity;

            while (quantity < 1)
            {
                cout << "/// Sorry, the quantity cannot be less than 1. Please try again!\n";
                cout << "\n=> Please enter the quantity: ";
                cin >> quantity;
            }

            // check whether that menu item has been ordered before
            for (int k = 0; k < x + 1; k++)
            {
                if (menuChoice == arrMenuChoices[k][0] && k != x)
                {
                    // if yes, add new quantity to existing quantity
                    arrMenuChoices[k][1] += quantity;
                    itemAlreadyExists = true;
                    cout << "\n/// This item has been ordered before. Updating quantity...\n";
                    break;
                }

                else
                {
                    itemAlreadyExists = false;
                }
            }

            if (itemAlreadyExists == false)
            {
                // else just add to array normally
                arrMenuChoices[x][0] = menuChoice;
                arrMenuChoices[x][1] = quantity;

                // x is accumulator of total number of items ordered
                ++x;
            }

            cout << "\n/// Processing order...\n";

            // check whether each item is valid
            invalidItemIndex = acceptOrder(arrMenuChoices, x - 1, totalNumItems);

            if (invalidItemIndex != 0)
            {
                for (int i = 0; i < totalNumItems; i++)
                {
                    index = stoi(arrMenuContent[i][0]);
                    if (index == invalidItemIndex)
                    {
                        itemName = arrMenuContent[i][1];
                    }
                }

                cout << "/// Apologies! Order of " << itemName << " is rejected due to insufficient stock.\n";
            }

            cout << "\n=> Do you wish to continue ordering? [Y/N] ";
            cin >> choice;

            while (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n')
            {
                cout << "\n/// Sorry, that is not a valid selection. Please try again!";
                cout << "\n=> Do you wish to continue ordering? [Y/N] "; // ask user whether they wish to continue ordering food
                cin >> choice;
            }
        }

        int numLine = 1;

        receipt << datetime; // write date and time of order to receipt

        for (int k = 0; k < x; k++)
        {
            if (arrMenuChoices[k][0] == 0)
                continue;
            // write order results to receipt
            // menu structure: index,itemName,itemPrice,preparationTime
            // receipt structure: 
            //      date & time
            //      numbering,menuIndex,itemName,itemPrice,preparationTime,quantity

            // iterate through menu array to find matching index
            for (int j = 0; j < totalNumItems; j++)
            {
                menuIndex = stoi(arrMenuContent[j][0]);
                if (arrMenuChoices[k][0] == menuIndex)
                {
                    receipt << numLine << ","; // write index of ordered food to receipt

                    receipt << menuIndex << ",";// write menu index of ordered food to receipt

                    itemName = arrMenuContent[j][1];
                    receipt << itemName << ","; // write name of ordered food to receipt

                    itemPrice = stof(arrMenuContent[j][2]);
                    receipt << itemPrice << ","; // write price of ordered food to receipt

                    preparationTime = stoi(arrMenuContent[j][3]);
                    receipt << preparationTime << ","; // write preparation time to receipt

                    receipt << arrMenuChoices[k][1] << "\n"; // write quantity to receipt
                    numLine++;
                    break;
                }               
            }  
        }


        delete[] arrMenuChoices;
        delete[] arrMenuContent;
        receipt.close();

        cout << "\n/// Would you like to proceed to make payment?\n";
        cout << "[1] Make payment\n"
            << "[2] Re-order\n";
        cin >> proceedChoice;

        while (proceedChoice != 1 && proceedChoice != 2)
        {
            cout << "\n/// Sorry, that is not a valid selection. Please try again!";
            cout << "\n/// Would you like to proceed to make payment?\n";
            cout << "[1] Make payment\n"
                << "[2] Re-order\n";
            cin >> proceedChoice;
        }

        if (proceedChoice == 1)
        {
            cout << "\n/// Redirecting to Make Payment...\n";
            makePayments(ud);
        }
        else
        {
            cout << "\n/// Redirecting to Order Online again...\n";
            orderOnline(ud);
        }
    }
}

void makePayments(UserDetails& ud)
{
    string line;
    fstream receipt;
    receipt.open("receipt_153410.txt", ios::in);

    int index = 0; // accumulator index of number of food items
    while (getline(receipt, line))  // check how many items in the menu by determining number of lines
        ++index;

    if (index == 0)
    {
        cout << "\n/// You have not made any orders yet!.\n";
        cout << "/// Redirecting to Order Online...\n";
        orderOnline(ud);
    }
    else
    {
        receipt.clear();
        receipt.seekg(0, ios::beg);

        bool eligibleNewcomerDiscount = isNewcomer(ud);

        string datetime;
        int numbering;
        int menuIndex = 0;
        string itemName;
        float itemPrice = 0;
        int quantity = 0;
        int preparationTime = 0;
        char toSkip;

        float totalPayment = 0;
        int totalPrepTime = 0;
        int deliveryTime = 0;
        string deliveryAreaNum;
        string deliveryArea;

        float userPayment = 0;
        float change = 0;
        bool hasChange = false;

        deliveryTime = calcEstDeliveryTime(deliveryAreaNum, totalPrepTime);
        switch (deliveryAreaNum[0])
        {
        case '1':
            deliveryArea = "Cahaya Gemilang";
            break;
        case '2':
            deliveryArea = "Aman Damai";
            break;
        case '3':
            deliveryArea = "Indah Kembara";
            break;
        case '4':
            deliveryArea = "Restu";
            break;
        case '5':
            deliveryArea = "Saujana";
            break;
        default:
            deliveryArea = "Tekun";
            break;
        }

        // receipt structure: 
        //      date & time
        //      numbering,menuIndex,itemName,itemPrice,preparationTime,quantity
        cout << "\n*************************** PAYMENT PAGE ***************************\n";
        cout << "\n/// You are now Making Payment.\n";

        cout << "\n=================================================================\n";
        cout << "============================ RECEIPT ============================\n";
        cout << "=================================================================\n\n";
        cout << "NO.  ITEM NAME\t\t      ITEM PRICE\tQUANTITY\tPREPARATION TIME";
        cout << "\n---------------------------------------------------------------------------------\n";
        receipt.clear();
        receipt.seekp(0, ios::beg);

        getline(receipt, datetime);
        while (receipt >> numbering)
        {
            receipt >> toSkip;
            receipt >> menuIndex;
            receipt >> toSkip;
            getline(receipt, itemName, ',');
            receipt >> itemPrice;
            receipt >> toSkip;
            receipt >> preparationTime;
            receipt >> toSkip;
            receipt >> quantity;
            cout << numbering << ") " << left << setw(25) << itemName;
            cout << "\t$" << fixed << setprecision(2) << itemPrice;
            cout << "\t\t" << quantity;
            cout << "\t\t" << preparationTime << " minutes\n";
        }

        totalPayment = calcTotalPaymentsPerOrder();

        if (eligibleNewcomerDiscount)
        {
            cout << "\nCongratulations! As a first-time user, you are entitled to 10% Newcomer Discount :)\n";
            float discount = totalPayment * 10 / 100;
            cout << fixed << setprecision(2) << "You save: $" << discount << "\n";
            totalPayment -= discount;
        }
        else
        {
            cout << "\nThanks for dining with us again, " << ud.customerName << "! :) \n";
        }

        cout << "\n==================== ORDER DETAILS ====================\n";
        cout << setw(30) << "\nTOTAL PAYMENT: " << fixed << setprecision(2) << "$" << totalPayment;
        cout << setw(30) << "\nFOOD PREPARATION TIME: " << totalPrepTime << " minutes";
        cout << setw(30) << "\nDELIVERY AREA: " << deliveryArea;
        cout << setw(30) << "\nTOTAL DELIVERY TIME: " << deliveryTime << " minutes. \nThank you for your patience!\t";
        cout << setw(30) << "\n\nDATE & TIME OF ORDER: " << datetime;
        cout << "\n=================================================================\n";

        cout << "\n=> Please enter the amount to pay: $";
        cin >> userPayment;

        while (userPayment < totalPayment)
        {
            cout << "/// Insufficient payment! Please try again.\n";
            cout << "\n=> Please enter the amount to pay: $";
            cin >> userPayment;
        }

        if (userPayment > totalPayment)
        {
            hasChange = true;
            change = userPayment - totalPayment;
        }

        cout << "\n/// PAID AMOUNT: $" << fixed << setprecision(2) << userPayment;

        if (hasChange)
        {
            cout << "\n/// Dispensing change...";
            cout << "\n/// CHANGE: $" << fixed << setprecision(2) << change;
        }

        cout << "\n/// Thank you for choosing NinjaFood! Enjoy your meal and see you again!\n";

        receipt.close();
        remove("receipt_153410.txt"); // remove receipt file after payment process is complete
        logout();
    }
}

bool isNewcomer(UserDetails& ud)
{
    string line;
    string phoneNumber;

    string phoneNumberFromFile; // to be matched with input of phone number
    bool eligible = true;

    fstream file;
    file.open("customer_record_153410.txt", ios::in | ios::out | ios::app);

    cout << "\n********************** CUSTOMER DETAILS PAGE **********************\n";
    cout << "\n/// We require your details to ensure we deliver the correct order to the right person and address!\n";

    cin.ignore();
    ;    cout << "\n=> Please enter your details to proceed.\n"
        << "\n=> Name: ";
    getline(cin, ud.customerName);

    cout << "\n=> Phone number (eg 0123456789): ";
    getline(cin, phoneNumber);
    while (phoneNumber.length() < 10 || phoneNumber.length() > 11)
    {
        cout << "\nInvalid phone number! Please try again.\n";
        cout << "=> Phone number (eg 0123456789): ";
        getline(cin, phoneNumber);
    }

    // check whether this phone number already exists in customer database
    while (getline(file, line, ','))
    {
        getline(file, phoneNumberFromFile);
        // if not, means this customer is new customer and entitled to newcomer discount
        if (phoneNumber == phoneNumberFromFile)
        {
            eligible = false;
            break;
        }
    }
    file.clear();

    file << ud.customerName << ",";
    file << phoneNumber << "\n";

    file.close();
    return eligible;
}


