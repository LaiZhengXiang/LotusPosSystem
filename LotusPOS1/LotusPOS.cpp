#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <conio.h> //for password input _getch()
using namespace std;

//---GLOBAL VARIABLE & CONSTANT
//********************************************************************************************************************************************
//User management
const int MAXSTAFF = 200; //maximum staff

//cart system
const int MAXCART = 100; //maximum cart size
const int MAXITEM = 300; //how many items can store

//member system
const int MAX = 300; // Maximum number of members



//---STRUCTURE LIST
//********************************************************************************************************************************************
//User management
struct Staff
{
    string staffID;
    string name;
    string password;
    string role = "Staff"; // e.g., "Admin", "Staff"
};

//Category and Item in stock
struct Category
{
    string item_name[MAXITEM];
    double item_price[MAXITEM];
    int item_instock[MAXITEM];
    int num_item;
};

//cart
struct Cart
{
    string cart_item[MAXCART] = {};
    int cart_item_quantity[MAXCART] = {};
    double cart_item_price[MAXCART] = {};
    double cart_total = 0.0;
    int cart_count = 0;
};

//member
struct Member
{
    string id, name, phone;
    double points = 0.0;
};



//---FILE LIST
//********************************************************************************************************************************************
//staff data
ifstream inData;
ofstream outData;

//stock data
ifstream inCData;
ofstream outCData;

//member data
ifstream inMData;
ofstream outMData;



//---GLOBAL LIST
//********************************************************************************************************************************************
Staff staffList[MAXSTAFF];
int staffCount = 0;
Staff* currentUser = nullptr; // Pointer to the currently logged-in user

Cart cart_one;

Member list[MAX];
int total = 0;



//---STOCK DATA INITIALIZATION
//********************************************************************************************************************************************
Category food =
{
    {"Rice", "Noodles", "Nasi Lemak"},
    {2.50, 3.00, 4.00},
    {500, 500, 500},
    3
};

Category drinks =
{
    {"Coke", "Sprite", "Juice"},
    {3.00, 2.50, 1.50},
    {500, 500, 500},
    3
};

Category stationary =
{
    {"Pencil", "Eraser", "Highlighter"},
    {1.00, 0.50, 5.00},
    {500, 500, 500},
    3
};

Category electronics =
{
    {"Phone", "Tablet", "Laptop"},
    {500, 600, 1000},
    {500, 500, 500},
    3
};

Category toys =
{
    {"Aeroplane", "Cars", "Figure"},
    {30, 50, 100},
    {500, 500, 500},
    3
};

string categorylist[5] = { "Food", "Drinks", "Stationary", "Electronics", "Toys" }; //*Inventory systems nid FIXED  CATEGORY,still trying find solution*
int category_size = sizeof(categorylist) / sizeof(categorylist[0]);
Category category_data[20] = { food, drinks, stationary, electronics, toys }; //*nid to Further research*



//---FUNCTION LIST
//********************************************************************************************************************************************
//User management
void loadstaff();
void savestaff();
void ASLImenu();
void loginStaff();
void registerStaff();
void viewStaff();
void logout();
string getPasswordInput();
string newStaffID();

//stock
void loadCategory(Category& cat, ifstream& inCData);
void saveCategory(const Category& cat, ofstream& outCData);
void saveAllCategories();
void loadAllCategories();
void stockList();
void viewUpdateStock();
void addNewStock();

//sales & cart
void Cmenu();
void SSmenu();
void selectItem(Category& selected);
int selectCategory(int selected_category);
void CSmenu();
void cart(Category& selected, int choice, int quantity);
void cartremove();
void clearcart();

//payment system
void Pmenu(double total);
void PScard(double total);
void PSewallet(double total);
void PScash(double total);
void PSbank(double total);

//member system
void MSmenu();
void registerMember();
void loadMemberData();
void saveMemberData();
string newID();
Member* findMember(const string& phone);
void searchMember();
void addPoints();
void redeemPoints();
double memberdiscount(double total); //added by payment system



/*=============================================================MAIN MENU==================================================================*/
int main() 
{
    //load all data
    loadstaff();
    loadMemberData();
    loadAllCategories();
    
    //declare variable
    int option = 0;
    bool LImenu_running = false;

    do
    {
        //menu start to run
        LImenu_running = true;

        //main menu header
        cout << "\n====================================================\n"
            << "|          L O T U S   P O S   S Y S T E M         |\n"
            << "----------------------------------------------------\n"
            << "|               [ M A I N   M E N U ]              |\n"
            << "====================================================\n";

        //function Select
        cout << endl
            << "[1]    I'm a Customer" << endl 
            << "[2]    Admin/Staff Management" << endl 
            << "[0]    Exit Program" << endl; 

        cout << endl
            //get user input
            << "Select Option:  ";
        //check validation
        while (!(cin >> option) || !(option >= 0 && option <= 2)) //*option <= (number of option)*
        {
            cout << "Invalid Option, Please Re-Enter:   "; //if invalid tell the user to reenter
            cin.clear();
            cin.ignore(1000, '\n');
        }

        //process user input
        switch (option)
        {
        case 1: //locate to sales,cart,payment,member menu
            Cmenu();
            break;
        case 2: //locate register,login,manage stock menu
            ASLImenu();
            break;
        case 0: //save and exit program
            cout << "\nProgram Exited\n";
            LImenu_running = false; //menu stop running to exit loop
            savestaff();
            saveMemberData(); 
            saveAllCategories();
            break;
        default:
            //shouldnt be reach since validation checked earlier
            cout << "ERROR";
        }
    } while (LImenu_running == true);

    return 0; //exit program
}



/*=============================================================ADMIN/STAFF LOG IN MENU==================================================================*/
//Lai Zheng Xiang's module

void ASLImenu()
{ 
    int Slogin_option = 0;

    while (currentUser == nullptr) //not logged in
    {
        //log in menu header
        cout << "\n====================================================\n"
            << "|          L O T U S   P O S   S Y S T E M         |\n"
            << "----------------------------------------------------\n"
            << "|            [ S T A F F   L O G   I N ]           |\n"
            << "====================================================\n";

        //Function Select
        cout << endl
            << "[1]    Log In" << endl
            << "[2]    Register as admin" << endl
            << "[0]    Return" << endl;

        cout << endl
            << "Select Option:  ";
        //check validation
        while (!(cin >> Slogin_option) || !(Slogin_option >= 0 && Slogin_option <= 2))
        {
            cout << "Invalid Option, Please Re-Enter:   "; //invalid reenter
            cin.clear();
            cin.ignore(1000, '\n');
        }
        if (Slogin_option == 1) //log in
        {
            loginStaff();
        }
        else if (Slogin_option == 2) //register as admin
        {
            if (staffCount == 0) 
            {
                registerStaff();
            }
            else 
            {
                cout << "\nAdmin is registered.\n";
            }
        }
        else 
        {
            return; //pressed 0 and return
        }
    }

    if (currentUser->role == "Admin") //logged in as admin
    {
        do //Admin Menu
        {
            cout << "\n====================================================\n"
                << "|          L O T U S   P O S   S Y S T E M         |\n"
                << "----------------------------------------------------\n"
                << "|               [ A D M I N   M E N U ]            |\n"
                << "====================================================\n";
            cout << "Welcome, " << currentUser->name << "!" << endl;

            cout << endl
                << "[1] View And Add Stock" << endl
                << "[2] Update Stock" << endl
                << "[3] View All Staff" << endl
                << "[4] Register New Staff" << endl
                << "[5] Log Out" << endl
                << "[0] Return" << endl;

            cout << endl
                //get input
                << "Select Option: ";
            //check validaiton
            while (!(cin >> Slogin_option) || !(Slogin_option >= 0 && Slogin_option <= 5))
            {
                cout << "Invalid Option, Please Re-Enter:    "; //invalid reenter
                cin.clear();
                cin.ignore(1000, '\n');
            }
            //process input
            switch (Slogin_option)
            {
            case 1: 
                viewUpdateStock();
                break;
            case 2: 
                addNewStock();
                break;
            case 3: //view all staff
                viewStaff();
                break;
            case 4: //register new staff
                registerStaff();
                break;
            case 5:
                logout(); 
            case 0:; //return
                return;
            default:
                cout << "ERROR";
            }
        } while (true);
    }

        //LOGGED IN as staff
        do
        {
            // Display Current User Information
            cout << "\n====================================================\n"
                << "|          L O T U S   P O S   S Y S T E M         |\n"
                << "----------------------------------------------------\n"
                << "|               [ S T A F F   M E N U ]            |\n"
                << "====================================================\n";
            cout << "Welcome, " << currentUser->name << "!" << endl;

            // Function Select
            cout << endl
                << "[1]    View And Add Stock" << endl
                << "[2]    Update Stock" << endl
                << "[3]     Log Out" << endl
                << "[0]    Return" << endl;

            // Get User Input
            cout << endl
                << "Select Option:    ";

            // Process User Input
            while (!(cin >> Slogin_option) || !(Slogin_option >= 0 && Slogin_option <= 3))
            {
                cout << "Invalid Option, Please Re-Enter:    ";
                cin.clear();
                cin.ignore(1000, '\n');
            }
            switch (Slogin_option)
            {
            case 1: // Proceed to view and add stock
                viewUpdateStock();
                break;

            case 2: // Proceed to update stock
                addNewStock();
                break;

            case 3:
                logout();
                return;
            case 0: //return
                return;

            default:
                cout << "ERROR";
            }

        } while (true);
  return;
}



/*=================================================================STAFF SYSTEM ================================================================*/
//Lai Zheng Xiang's module

//password function,print * and get password
string getPasswordInput()
{
    string password;
    char ch;
    while ((ch = _getch()) != 13) // 13 is ASCII for Enter key
    {
        if (ch == 8) // 8 is ASCII for Backspace
        {
            if (password.length() > 0) {
                password.pop_back();
                cout << "\b \b"; // Move back, overwrite with space, move back again
            }
        }
        else if (ch >= 32) { // Standard printable characters
            password += ch;
            cout << "*";
        }
    }
    cout << endl;
    return password;
}

// function to generate new staff id S001 S002
string newStaffID()
{
    int n = staffCount + 1; //1
    string num = to_string(n); //int 1 to string "1"

    while (num.size() < 3)
    {
        num = "0" + num; //first loop 01,second loop 001
    }

    return "S" + num; //S001
}

void viewStaff()
{
    cout << "\n--- All Registered Staff ---\n";
    for (int i = 0; i < staffCount; ++i) 
    {
        cout << "ID: " << staffList[i].staffID << ", Name: " << staffList[i].name
            << ", Role: " << staffList[i].role << endl;
    }
}

void loadstaff()
{
    inData.open("staff_data.txt");
    staffCount = 0;
    Staff tempStaff; //create a temporary list prevent data lost

    if (inData.is_open()) //check file is open
    {
        // Read StaffID Name Password Role
        while (inData >> tempStaff.staffID >> tempStaff.name >> tempStaff.password >> tempStaff.role) //note
        {
            if (staffCount < MAXSTAFF)
            {
                staffList[staffCount] = tempStaff;
                staffCount++;
            }
        }
        inData.close(); //close file
        cout << "Staff data loaded... (" << staffCount << " users).\n";
    }
    else
    {
        cout << "No existing staff data file found. Starting with empty staff list.\n";
    }
}

void savestaff()
{
    outData.open("staff_data.txt"); //open staff file

    if (outData.is_open()) //check file is open
    {
        for (int i = 0; i < staffCount; i++) //output to file
        {
            outData << staffList[i].staffID << " "
                << staffList[i].name << " "
                << staffList[i].password << " "
                << staffList[i].role << "\n";
        }
        outData.close(); //close file
        cout << "Staff data saved.\n";
    }
    else //file cant open print ERROR
    {
        cout << "ERROR, UNABLE TO OPEN FILE";
    }
}

void registerStaff()
{
    if (staffCount >= MAXSTAFF)
    {
        cout << "Staff list is full!" << endl;
        return;
    }

    staffList[staffCount].staffID = newStaffID();

    // Set first user as Admin, others as Staff
    if (staffCount == 0) 
    {
        staffList[staffCount].role = "Admin";
        cout << "\n!!! Registering First User as ADMIN !!!" << endl;
    }
    else 
    {
        staffList[staffCount].role = "Staff";
    }

    cout << "Staff ID: " << staffList[staffCount].staffID << endl;

    cin.ignore();
    cout << "Full Name (No Spaces): ";
    // Use cin >> name to avoid issues if the user inputs spaces, breaking the file format
    cin >> staffList[staffCount].name;

    cout << "Password: ";
    staffList[staffCount].password = getPasswordInput();

    staffCount++;
    savestaff(); // Save immediately after registration
    cout << "\nStaff registered successfully! ID: " << staffList[staffCount - 1].staffID << endl;
}

void loginStaff()
{
    string id, password;
    cout << "\nEnter Staff ID: ";
    cin >> id;

    cout << "Enter Password: ";
    password = getPasswordInput();

    for (int i = 0; i < staffCount; i++)
    {
        if (staffList[i].staffID == id && staffList[i].password == password)
        {
            currentUser = &staffList[i];
            cout << "\nLogin successful! Welcome, " << currentUser->name << ".\n";
            return;
        }
    }
    cout << "\nInvalid Staff ID or Password. Please try again.\n";
}

void logout()
{
    currentUser = nullptr;
    cout << "\nLogged Out.\n";
    return;
}



/*=============================================================INVENTORY==================================================================*/
//Lai Zheng Xiang's module

void saveCategory(const Category& cat, ofstream& outCData) //save single category
{
    outCData << cat.num_item << endl;

    for (int i = 0; i < cat.num_item; i++)
    {
        outCData << cat.item_name[i] << endl;
        outCData << cat.item_price[i] << endl;
        outCData << cat.item_instock[i] << endl;
    }
}

void loadCategory(Category& cat, ifstream& inCData) //load single category
{
    inCData >> cat.num_item;
    inCData.ignore(); 

    for (int i = 0; i < cat.num_item; i++)
    {
        getline(inCData, cat.item_name[i]);
        inCData >> cat.item_price[i];
        inCData >> cat.item_instock[i];
        inCData.ignore(); 
    }
}

void saveAllCategories()
{
    outCData.open("categories.txt");
    if (outCData.is_open()) //check file is open
    {
        for (int i = 0; i < category_size; i++)
        {
            saveCategory(category_data[i], outCData);
        }

        cout << "All Categories saved!\n";
        outCData.close();
    }
    else //file cant open print ERROR
    {
        cout << "ERROR, UNABLE TO OPEN FILE";
    }
}

void loadAllCategories()
{
    inCData.open("categories.txt");
    if (inCData.is_open()) //check file is open
    {
        for (int i = 0; i < category_size; i++)
        {
            loadCategory(category_data[i], inCData);
        }

        cout << "All Categories loaded!\n";
        inCData.close();
    }
    else //file cant open print ERROR
    {
        cout << "categories.txt not found. Using default data.\n";
        return;
    }
}


void stockList()
{
    cout << "\n====================================================\n"
        << "|          L O T U S   P O S   S Y S T E M         |\n"
        << "----------------------------------------------------\n"
        << "|               [ S T O C K   L I S T ]            |\n"
        << "====================================================\n";
    
    cout << endl
        << "[1]    Foods" << endl
        << "[2]    Drinks" << endl
        << "[3]    Stationary" << endl
        << "[4]    Electronics" << endl
        << "[5]    Toys" << endl
        << "[0]    Return" << endl;

    cout << "\nSelect Option: ";
}

void viewUpdateStock() //view and update stock
{
    int option = 0;
    stockList(); //display stocklist
    while (!(cin >> option) || !(option >= 0 && option <= 5))
    {
        cout << "Invalid Option, Please Re-Enter:    ";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    option -= 1;
    if (option == -1) //since 0-1 is -1 to return
    {
        return;
    }
    else
    {
        Category& selected = category_data[option];
        cout << "Category: " << categorylist[option] << endl
            << "=============================================" << endl;

        //display
        for (int i = 0; i < selected.num_item; i++)
        {
            cout << i << ". "
                << left << setw(10) << selected.item_name[i]
                << right << setw(15) << "	- "
                << "RM" << fixed << setprecision(2) << selected.item_price[i]
                << "	( " << fixed << setprecision(0) << selected.item_instock[i] << " in stock )" << endl;
        }

        cout << "Select Item: ";
        int item_option = 0;
        int add_stock_amt = 0;
        //check validation
        while (!(cin >> item_option) || !(item_option >= 0 && item_option <= selected.num_item))
        {
            cout << "Invalid Item, Please Re-Enter:    ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        cout << "Add Amount To Stock: ";
        //check validation
        while (!(cin >> add_stock_amt) || !(add_stock_amt > 0))
        {
            cout << "Invalid Amount, Please Re-Enter:    ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        //process
        selected.item_instock[item_option] += add_stock_amt;
        cout << "\nSucessfully added " << add_stock_amt << " to " << selected.item_name[item_option] << endl;
        saveAllCategories();
    }

}

void addNewStock()
{
    int option = 0;
    stockList();
    while (!(cin >> option) || !(option >= 0 && option <= 5))
    {
        cout << "Invalid Option, Please Re-Enter:    ";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    option -= 1;
    if (option == -1) //since 0-1 is -1
    {
        return;
    }
    else
    {
        //get item name
        Category& selected = category_data[option];
        
        // CHECK IF FULL FIRST
        if (selected.num_item >= MAXITEM) {
            cout << "Category is full! Cannot add more items.\n";
            return;
        }

        // Capture the correct index (Current count is the index for the new item)
        int new_index = selected.num_item;

        cout << "Enter Item Name: ";
        cin.ignore();
        getline(cin, selected.item_name[new_index]);
        cout << "Enter Price: ";
        //check validation
        while (!(cin >> selected.item_price[new_index]) || !(selected.item_price[new_index] > 0))
        {
            cout << "Invalid Option, Please Re-Enter:    ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        cout << "Enter Stock Amount: ";
        //check validation
        while (!(cin >> selected.item_instock[new_index]) || !(selected.item_instock[new_index] > 0))
        {
            cout << "Invalid Option, Please Re-Enter:    ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        
        selected.num_item++;

        cout << "\nSucessfully added " << selected.item_name[new_index] << " - RM " << fixed << setprecision(2) << selected.item_price[new_index]
            << " with " << selected.item_instock[new_index] << " stocks!\n";
        saveAllCategories();
    }
}


/*=============================================================CUSTOMER MENU==================================================================*/
//Lai Zheng Xiang's module

void Cmenu() //customer menu
{
    //Declare variable
    int option = 0;
    bool Cmenu_running = false;
    
    do
    {
        //Menu start to run
        Cmenu_running = true;

        //Customer Menu Header design
        cout << "\n====================================================\n"
            << "|          L O T U S   P O S   S Y S T E M         |\n"
            << "----------------------------------------------------\n"
            << "|           [ C U S T O M E R   M E N U ]          |\n"
            << "====================================================\n";

        //Function Select
        cout << endl
            << "[1]    Select Product" << endl
            << "[2]    View My Cart & Purchase" << endl
            << "[3]    Member System" << endl
            << "[0]    Return" << endl;

        //Get User Input
        cout << endl
            << "Select Option:   ";

        //check validation
        while (!(cin >> option) || !(option >= 0 && option <= 3)) //*option <= (number of option)*
        {
            cout << "Invalid Option, Please Re-Enter:   "; //invalid reenter
            cin.clear();
            cin.ignore(1000, '\n');
        }

        //process input
        switch (option)
        {
        case 1: //Proceed to purchase system
            SSmenu();
            break;

        case 2: //Proceed to cart system
            CSmenu();
            break;

        case 3: //Proceed to member system
            MSmenu();
            break;

        case 0: //return
            Cmenu_running = false;
            continue;

        default: //shouldnt be reach, since validation checked earlier
            cout << "ERROR";
        }

    } while (Cmenu_running == true);

    return;
}


/*=============================================================SALES SYSTEM================================================================*/
//Lai Zheng Xiang's module

void SSmenu()
{
    //declare variable
    int selected_category = 0;

    do 
    {
        //Sales system header
        cout << "\n====================================================\n"
            << "|          L O T U S   P O S   S Y S T E M         |\n"
            << "----------------------------------------------------\n"
            << "|                [ C A T E G O R Y ]               |\n"
            << "====================================================\n";

        //showing category food,drinks,toys etc
        for (int i = 0; i < category_size; i++)
        {
            cout << "[" << i + 1 << "]  " << categorylist[i] << endl;
        }

        //get user input of category
        cout << "[0] Return";
        cout << endl;
        cout << "\nPlease select your category: ";

        //check validation
        while (!(cin >> selected_category) || !((selected_category >= 0) && (selected_category <= category_size)))
        {
            cout << "Entered invalid number, Please enter a number: "; //invalid reenter
            cin.clear();
            cin.ignore(1000, '\n');
        }
        if (selected_category == 0) //return
        {
            break;
        }

        selectCategory(selected_category);

    } while (true); //menu done

}


//Function for processing selected category
int selectCategory(int selected_category)
{
    int select = selected_category - 1;

    if (select >= 0 && select < category_size)
    {
        selectItem(category_data[select]);
    }
    else //shouldnt be reached since variable checked earlier
    {
        cout << "Invalid, Error occured";
    }
    return 0;
}


//function for display item and choose item
void selectItem(Category& selected)
{
    int choice = 0;
    int quantity = 0;
    double price = 0.0;

    //header for items
    cout << "\n=========================================\n";
    cout << "---       ITEM LIST (Select Item)    --- \n";
    cout << "=========================================\n";
    cout << endl;

    //display items
    for (int i = 0; i < selected.num_item; i++)
    {
        cout << i + 1 << ". "
            << left << setw(10) << selected.item_name[i]
            << right << setw(15) << "	- "
            << "RM" << fixed << setprecision(2) << selected.item_price[i]
            << "	( " << fixed << setprecision(0) << selected.item_instock[i] << " in stock )" << endl;
    }

    //making a return button
    cout << "[0] Return\n" << endl;
    
    //get user input
    cout << "\nSelect Option: ";
    while (!(cin >> choice) || !((choice >= 0) && (choice <= selected.num_item + 1)))
    {
        cout << "Invalid number, Please re-enter: "; //invalid reenter
        cin.clear();
        cin.ignore(1000, '\n');
    }
    choice -= 1; //since 1-1 = 0 to select first item

    if (choice == -1) //return
    {
        return;
    }
    
    //get quantity
    cout << "Enter Quantity: ";
    //check validation
    while (!(cin >> quantity) || !(quantity > 0))
    {
        cout << "Invalid number, Please re-enter: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    //check stock availability
    if (quantity > selected.item_instock[choice])
    {
        cout << "\nNo enough stock, Please select another item.\n";
        return;
    }

    //proceed to cart system
    cart(selected, choice, quantity); //add item to cart
}

/*=============================================================CART SYSTEM================================================================*/
//Tan Jun Yuing's module

void cart(Category& selected, int choice, int quantity) //add to cart system
{
    double subtotal = 0.0;
    double total = 0.0;

    if (cart_one.cart_count >= MAXCART) //cart full
    {
        cout << "Cart is full!" << endl;
    }
    else
    {
        //add item to cart
        cart_one.cart_item[cart_one.cart_count] = selected.item_name[choice];
        //add item quantity to cart
        cart_one.cart_item_quantity[cart_one.cart_count] = quantity;
        //add item price to cart
        cart_one.cart_item_price[cart_one.cart_count] = selected.item_price[choice];
        //update stock
        selected.item_instock[choice] -= quantity;

        //cart count +1
        cart_one.cart_count++;

        //display output
        cout << "\n(Succesfully added " << quantity << "x " << selected.item_name[choice] << " to cart)\n";
        cout << "=========== Your Cart Summary ===========\n";

        for (int i = 0; i < cart_one.cart_count; i++) //display cart summary
        {
            subtotal = cart_one.cart_item_price[i] * cart_one.cart_item_quantity[i];

            cout << cart_one.cart_item[i] << " x" << cart_one.cart_item_quantity[i] << " = " << fixed << setprecision(2) << "RM" << subtotal
                << endl << "-------------------" << endl;

            total += subtotal; //calculate Total

        }
        cout << "Total = RM" << fixed << setprecision(2) << total << endl; //display total
    }

}

//cart system menu
void CSmenu()
{
    int cartsum_option = 0;
    double subtotal = 0.0;
    bool inCart = true;

    while (inCart)
    {
        double total = 0.0;
        
        //cart summary main header
        cout << "\n====================================================\n"
            << "|          L O T U S   P O S   S Y S T E M         |\n"
            << "----------------------------------------------------\n"
            << "|            [ C A R T   S U M M A R Y ]           |\n"
            << "====================================================\n";

        //display cart summary
        cout << "My Cart Summary:" << endl;
        if (cart_one.cart_count == 0)
        {
            cout << endl
                << "(No Selected Product)" << endl;

            cout << "====================================================" << endl;
            cout << "[0]    Return" << endl;
            cout << endl << "Select Option: ";
            //check validation
            while (!(cin >> cartsum_option) || cartsum_option != 0)
            {
                cout << "Invalid Number, Please Re-Enter:   "; //invalid reenter
                cin.clear();
                cin.ignore(1000, '\n');
            }

            return;
        }

        for (int i = 0; i < cart_one.cart_count; i++)
        {
            subtotal = cart_one.cart_item_price[i] * cart_one.cart_item_quantity[i];

            cout << "[" << i + 1 << "]  " << cart_one.cart_item[i] << " x" << cart_one.cart_item_quantity[i]
                << " = " << fixed << setprecision(2) << "RM" << subtotal << endl;

            total += subtotal;
        }
        cout << "----------------------------------------------------\n"
            << "Total: " << fixed << setprecision(2) << "RM" << total << endl;

        cout << "====================================================" << endl;
        cout << "[1]    Remove Item" << endl
            << "[2]    Proceed To Payment" << endl
            << "[0]    Return" << endl;
        cout << endl << "Select Option: ";
        while (!(cin >> cartsum_option) || !(cartsum_option >= 0) && (cartsum_option <= 2))
        {
            cout << "Invalid Number, Please Re-Enter:   ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        switch (cartsum_option)
        {
        case 1: //remove item function
            cartremove();
            break;
        case 2: //proceed to payment
            Pmenu(total);
            inCart = false;
            break;
        case 0: //return to main menu
            inCart = false;
            return;
        default: //shouldnt be reach since checked validation earlier
            cout << "ERROR";
        }
    }
    return;
}

//remove item function
void cartremove()
{
    int item_to_remove = 0;
    int array_to_remove = 0;
    
    cout << "[0] Return" << endl;
    cout << "Select Item To Remove: ";
    while (!(cin >> item_to_remove) || !(item_to_remove >= 0 && item_to_remove <= cart_one.cart_count))
    {
        cout << "Invalid selection. Please re-enter: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    if (item_to_remove == 0) //return menu
    {
        return;
    }

    array_to_remove = item_to_remove - 1; //convert to 0 base

    //find back original array
    string item_name = cart_one.cart_item[array_to_remove];
    int quantity_removed = cart_one.cart_item_quantity[array_to_remove];
    bool stock_updated = false;

    for (int c = 0; c < category_size; c++) //seacrh through main category
    {
        for (int i = 0; i < category_data[c].num_item; i++) //search each item in category
        {
            if (category_data[c].item_name[i] == item_name) //found item
            {
                category_data[c].item_instock[i] += quantity_removed; //add back how many item added
                stock_updated = true;
                break; //remove done end loop
            }
        }
        if (stock_updated)
        {
            break;
        }
    }

    //Perform the shift since array order disturpted
    for (int i = array_to_remove; i < cart_one.cart_count - 1; i++)
    {
        cart_one.cart_item[i] = cart_one.cart_item[i + 1];
        cart_one.cart_item_quantity[i] = cart_one.cart_item_quantity[i + 1];
        cart_one.cart_item_price[i] = cart_one.cart_item_price[i + 1];
    }

    //Decrease cart count
    cart_one.cart_count--;

    // Recalculate cart total after removed
    cart_one.cart_total = 0.0; //reset
    for (int i = 0; i < cart_one.cart_count; i++)
    {
        cart_one.cart_total += cart_one.cart_item_price[i] * cart_one.cart_item_quantity[i];
    }

    cout << "\nItem '" << item_name << "' removed successfully. Stock returned.\n";
}

//clear cart after purchase
void clearcart()
{
        cart_one.cart_count = 0;
        cart_one.cart_total = 0.0;

        //Resetting whole cart value
        for (int i = 0; i < MAXCART; i++)
        {
            cart_one.cart_item[i] = "";
            cart_one.cart_item_quantity[i] = 0;
            cart_one.cart_item_price[i] = 0.0;
        }

        cout << "\nCart cleared successfully. Ready for a new transaction.\n";
    
}




/*=============================================================PAYMENT SYSTEM================================================================*/
//Tan Jun Yuing's module

void Pmenu(double total)
{
    //declare variables
    int a = 0;
    int b = 0;
    int choice1 = 0;

    //get user input
    cout << "\n==========SELF CHECKOUT==========\n" << endl;

    //check whtether if she/he is a lotusplus member
    cout << "ARE YOU A LOTUSPLUS MEMBER ? \n";
    cout << "1.YES\n2.NO\n";
    while (!(cin >> a) || a < 1 || a > 2)
    {
        cout << "INVALID!\nPLEASE ENTER AGAIN : ";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    switch (a)
    {
    case 1:    //yes
        total = memberdiscount(total);
        break;

    default://no
        cout << "WOULD YOU LIKE TO REGISTER LOTUSPLUS ? \n";
        cout << "1.YES\n2.NO";
        while (!(cin >> b) || b < 1 || b>2)
        {
            cout << "INVALID\nPLEASE ENTER AGAIN : ";
            cin.clear();
            cin.ignore(1000, '\n');

        }

        switch (b)
        {
        case 1:
            registerMember();
            total *= 0.95;
            break;
        default:        //no
            break;
        }
        break;
    }

    cout << "YOUR TOTAL IS : RM" << fixed << setprecision(2) << total << "\n\n";

    cout << "======SELECT PAYMENT METHOD======\n";
    cout << "1.CARD\n";
    cout << "2.E-WALLET\n";    //select payment method
    cout << "3.CASH\n";
    cout << "4.ONLINE BANKING\n";
    cout << "ENTER YOUR CHOICE : ";
    while (!(cin >> choice1) || choice1 < 1 || choice1 > 4)
    {
        cout << "======INVALID======\nPLEASE ENTER AGAIN : ";
        cin.clear();                                               //to prevent user to type other to get bugged
        cin.ignore(1000, '\n');
    }

    //processing
    switch (choice1)
    {
    case 1: //card payment
    {
        PScard(total);
        break;
    }
    case 2: //e-wallet payment
    {
        PSewallet(total);
        break;
    }
    case 3:  //cash payment
    {
        PScash(total);
        break;
    }
    case 4:              //online banking
    {
        PSbank(total);
        break;
    }
    default:
    {
        cout << "ERROR"; //wont reach since validation checked earlier
    }
    }
    
    cout << "===================================================================\n";
    cout << "            THANK YOU FOR SHOPPING AT LOTUS'S (^_^)\n";                              //end 
    cout << "===================================================================\n";

    //clear cart
    clearcart();
}

//using card payment
void PScard(double total) 
{
    cout << "\nPROCESSING CREDIT CARD....\n";
    cout << "TOTAL PAYMENT OF RM " << fixed << setprecision(2) << total << " APPROVED\n";
}

//using ewallet payment
void PSewallet(double total)
{
    int choice = 0;
    int id = 0;

    cout << "\n=========SELECT PREFERED E-WALLET METHOD========\n";
    //types of ewallet payment to choose
    cout << "1.TOUCH&GO\n" 
        << "2.BOOST\n"
        << "3.GRABPAY\n"
        << "4.SHOPEE PAY\n"
        << "5.ALIPAY PLUS\n";
    cout << "ENTER YOUR CHOICE : ";
    while (!(cin >> choice) || choice < 1 || choice > 5)
    {
        cout << "INVALID! ENTER AGAIN: ";
        cin.clear();                                           //debug
        cin.ignore(1000, '\n');
    }

    cout << "ENTER YOUR E-WALLET ID : ";
    while (!(cin >> id))
    {
        cout << "INVALID! ENTER AGAIN: ";
        cin.clear();                                           //debug
        cin.ignore(1000, '\n');
    }
    cout << "PROCESSING E-WALLET ID......\n";
    cout << "\nPLEASE CONFIRM ON THE TNG APP ON YOUR PHONE\n";
    cout << "PROCESSING E-WALLET PAYMENT....\nPAYMENT SUCCESSFUL FOR ID : " << id << endl;
}

//using cash payment
void PScash(double total)
{
    double cash = 0.0;
    double change = 0.0;
    
    cout << "ENTER CASH AMOUNT (RM): ";                     //get cash amt
    while (!(cin >> cash))
    {
        cout << "INVALID!\nPLEASE ENTER AGAIN : ";
        cin.clear();                                        //debugg
        cin.ignore(1000, '\n');
    }
    while (cash < total)
    {
        cout << "\nINSUFFICIENT CASH! ENTER AGAIN: ";         //given cash is less than total
        while (!(cin >> cash))
        {
            cout << "INVALID!\nPLEASE ENTER AGAIN : ";
            cin.clear();                                        //debug
            cin.ignore(1000, '\n');
        }
    }

    change = (double)cash - total;
    cout << "PAYMENT SUCCESSFUL.\n";
    cout << "CHANGE: RM " << fixed << setprecision(2) << change << "\n";
}

//using online banking
void PSbank(double total)
{
    int choice = 0;
    int accno = 0;
    int pin = 0;
    string input;
    string input2;
    
    cout << "\n========SELECT A BANK TO MAKE PAYMENT========" << endl;           //types of bank to choose
    cout << "1.PUBLIC BANK\n";
    cout << "2.MAYBANK2U\n";
    cout << "3.CIMB CLICKS\n";
    cout << "4.RHB NOW\n";
    cout << "5.AMBANK\n";
    cout << "6.MYBSN\n";
    cout << "7.BANK RAKYAT\n8.UOB\n9.HONG LEONG CONNECT\n";
    cout << "ENTER YOUR PREFENRENCE : ";
    while (!(cin >> choice) || choice < 1 || choice > 9)
    {
        cout << "======INVALID======\nPLEASE ENTER AGAIN : ";
        cin.clear();                                               //to prevent user to type other to get bugged
        cin.ignore(1000, '\n');

    }
    cout << "PLEASE ENTER YOUR ACC NO. : \n";
    while (true)
    {
        accno = _getch();          //get account number without show
        if (accno == '\r')
        {
            cout << endl;
            break;
        }
        if (accno >= '0' && accno <= '9')        //make sure only digits are type
        {
            input2 += accno;
            cout << "*";           //show star
        }


    }
    cout << "PROCESSING.....\n";
    cout << "ENTER YOUR PIN \n";
    while (true) 
    {
        pin = _getch();  // get pin without showing it

        if (pin == '\r') 
        {  // Enter key
            cout << endl;
            break;
        }
        if (pin >= '0' && pin <= '9') { // only digits
            input += pin;
            cout << "*";  // show star
        }
    }

    cout << "PROCESSING ONLINE BANKING PAYMENT....\n";
    cout << "TOTAL PAYMENT OF RM " << fixed << setprecision(2) << total << " APPROVED\n\n\n\n";
}




/*=============================================================MEMBER SYSTEM================================================================*/
//Chew Wei Heng's module

// =============================
//   Save Member Data to File
// =============================
void saveMemberData()
{
    outMData.open("members.txt");

    if (outMData.is_open())
    {
        for (int i = 0; i < total; i++)
        {
            outMData << list[i].id << " "
                << list[i].name << " "
                << list[i].phone << " "
                << list[i].points << "\n";
        }
        outMData.close();
        cout << "Member data saved.\n";
    }
    else
    {
        cout << "ERROR, UNABLE TO OPEN FILE";
    }
}

// =============================
//   Load Member Data From File
// =============================
void loadMemberData()
{
    inMData.open("members.txt");
    total = 0;
    Member tempMember;

    if (inMData.is_open()) 
    {
        while (inMData >> tempMember.id >> tempMember.name >> tempMember.phone >> tempMember.points)
        {
            if (total < MAX)
            {
                list[total] = tempMember;
                total++;
            }
        }
        inMData.close();
        cout << "Member data loaded... (" << total << " users).\n";
    }
    else
    {
        cout << "No existing member data file found. Starting with empty member list.\n";
    }
}

// Generate new Member ID
string newID()
{
    int n = total + 1;
    string num = to_string(n);

    while (num.size() < 3)
        num = "0" + num;

    return "M" + num;
}

// Find member by phone
Member* findMember(const string& phone)
{
    for (int i = 0; i < total; i++)
    {
        if (list[i].phone == phone)
            return &list[i];
    }
    return nullptr;
}

// Register new member
void registerMember()
{
    if (total >= MAX)
    {
        cout << "Member list is full!" << endl;
        return;
    }

    list[total].id = newID();
    list[total].points = 0.0;

    cout << "\nMember ID: " << list[total].id << endl;

    cout << "Username(no space): ";
    cin >> list[total].name;

    cout << "Phone(no space and '-' ): ";
    cin >> list[total].phone;

    total++;
    saveMemberData(); // <<< SAVE AFTER REGISTER

    cout << "Member registered successfully!" << endl;
}

// Search member
void searchMember()
{
    string phone;
    cout << "Enter phone number: ";
    cin >> phone;

    Member* m = findMember(phone);
    if (!m)
    {
        cout << "Member not found!" << endl;
        return;
    }

    cout << "=== Member Found ===" << endl;
    cout << "ID: " << m->id << endl;
    cout << "Username: " << m->name << endl;
    cout << "Phone: " << m->phone << endl;
    cout << "Points: " << m->points << endl;
}

// Add points
void addPoints()
{
    string phone;
    cout << "Enter phone number: ";
    cin >> phone;

    Member* m = findMember(phone);
    if (!m)
    {
        cout << "Member not found!" << endl;
        return;
    }

    double amount;
    cout << "Purchase amount: ";
    cin >> amount;

    double earned = amount / 100.0;
    m->points += earned;

    saveMemberData(); // <<< SAVE AFTER ADD POINTS

    cout << "Added " << earned << " points." << endl;
    cout << "Total points: " << m->points << endl;
}

// Redeem points
void redeemPoints()
{
    string phone;
    cout << "Enter phone number: ";
    cin >> phone;

    Member* m = findMember(phone);
    if (!m)
    {
        cout << "Member not found!" << endl;
        return;
    }

    if (m->points < 10)
    {
        cout << "You need at least 10 points to redeem." << endl;
        return;
    }

    cout << "Available points: " << m->points << endl;
    cout << "Every 10 points = 1 FREE GIFT" << endl;
    cout << "How many times do you want to redeem 10 points? ";

    int r = 0;
    while (!(cin >> r))
    {
        cout << "Invalid input. Re-enter: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    double needed = r * 10;

    if (needed > m->points)
        cout << "Not enough points!" << endl;
    else
    {
        m->points -= needed;
        saveMemberData(); // <<< SAVE AFTER REDEEM

        cout << "Free Gift x" << r << endl;
        cout << "Remaining points: " << m->points << endl;
    }
}

double memberdiscount(double total) //added by payment system to get discount if member
{
    string phone;
    cout << "Enter phone number: ";
    cin >> phone;

    Member* m = findMember(phone);
    if (!m)
    {
        cout << "Member not found! No discount applied." << endl;
        return total;
    }
    cout << "=== Member Found: " << m->name << " ===" << endl;
    cout << "5% Discount Applied!" << endl;
    total *= 0.95;
    return total;
}

// Menu
void MSmenu()
{
    do
    {
        cout << "\n====================================================\n"
            << "|          L O T U S   P O S   S Y S T E M         |\n"
            << "----------------------------------------------------\n"
            << "|           [ M E M B E R   S Y S T E M ]          |\n"
            << "====================================================\n\n";

        cout << "[1]    Register New Member\n";
        cout << "[2]    Search Member\n";
        cout << "[3]    Add Points\n";
        cout << "[4]    Redeem Points\n";
        cout << "[0]    Return\n";
        cout << "Select Option: ";

        int MSchoice;
        while (!(cin >> MSchoice) || !(MSchoice >= 0 && MSchoice <= 4))
        {
            cout << "Invalid choice. Re-enter:  ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        switch (MSchoice)
        {
        case 1: registerMember(); 
            break;
        case 2: searchMember(); 
            break;
        case 3: addPoints(); 
            break;
        case 4: redeemPoints(); 
            break;
        case 0:
            cout << "Returned To Main Menu" << endl;
            return;
        }
    } while (true);
}