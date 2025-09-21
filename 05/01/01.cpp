#include <iostream>
#include <pqxx/pqxx>
#include <regex>
#include <string>
#include <vector>

struct PSQLPublicConnSpec {
    std::string host;
    std::string port;
    std::string dbName;
    std::string user;
    std::string password;
};

const std::string clientsTable = "clients";
const std::string phoneNumbersTable = "phone_numbers";
const std::string clientPhoneNumbersTable = "client_phone_numbers";

const std::vector<std::string> tableCreationOrder{
    clientsTable,
    phoneNumbersTable,
    clientPhoneNumbersTable
};

const std::vector<std::string> tableDeletionOrder{
    clientPhoneNumbersTable,
    clientsTable,
    phoneNumbersTable
};

const std::vector<int64_t> validMainUserMenuOptions{
    1,
    2,
    3,
    4,
    5,
    6,
    10
};

const std::vector<int64_t> validSearchUserMenuOptions{
    1,
    2,
    3,
    4,
    5
};

const std::vector<int64_t> validEditUserMenuOptions{
    1,
    2,
    3
};

using TableSchemaSpec = std::map<std::string, std::string>;

const TableSchemaSpec tableSchemaSpec{
        {clientsTable, "CREATE TABLE " + clientsTable +" ("
                                "id_client serial PRIMARY KEY CHECK (id_client > 0),"
                                "forename varchar(255) NOT NULL,"
                                "surname varchar(255) NOT NULL,"
                                "email varchar(255) NOT NULL"
                                ");"

        },
        {phoneNumbersTable, "CREATE TABLE " + phoneNumbersTable + " ("
                                "id_phone_number serial PRIMARY KEY CHECK (id_phone_number > 0),"
                                "phone_number bigint NOT NULL"
                                ");"

        },
        {clientPhoneNumbersTable, "CREATE TABLE " + clientPhoneNumbersTable + " ("
                                "id_client int,"
                                "id_phone_number int,"
                                "PRIMARY KEY (id_client, id_phone_number),"
                                "FOREIGN KEY (id_client) REFERENCES clients(id_client) ON DELETE CASCADE,"
                                "FOREIGN KEY (id_phone_number) REFERENCES phone_numbers(id_phone_number) ON DELETE CASCADE"
                                ");"
        }
};

std::string getInputStrVar(const std::string& questionText) {
    std::string userInputString{};
    std::string inputCorrectnessCheck{};
    std::string negativeAnswers = "Nn";
    while (userInputString.empty()) {
        std::cout << questionText;
        std::getline(std::cin, userInputString);
        std::cout << "You have entered '" + userInputString +
                         "'. Is that correct [Y/n]?";
        std::getline(std::cin, inputCorrectnessCheck);
        if (negativeAnswers.find(inputCorrectnessCheck) != std::string::npos &&
            !inputCorrectnessCheck.empty()) {
            userInputString = "";
            }
    }
    return userInputString;
}

std::string getInputEmailVar(const std::string& questionText) {
    std::string userInputString{};
    std::string inputCorrectnessCheck{};
    std::string negativeAnswers = "Nn";
    while (userInputString.empty()) {
        std::cout << questionText;
        std::getline(std::cin, userInputString);
        std::cout << "You have entered '" + userInputString +
                         "'. Is that correct [Y/n]?";
        std::getline(std::cin, inputCorrectnessCheck);
        if (negativeAnswers.find(inputCorrectnessCheck) != std::string::npos &&
            !inputCorrectnessCheck.empty()) {
            userInputString = "";
            }
        const std::regex emailRegex ("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
        if (!std::regex_match(userInputString, emailRegex)) {
            std::cout << "Input isn't an email. Please try again." << std::endl;
            userInputString = "";
        }
    }
    return userInputString;
}

bool getInputBoolVar(const std::string& questionText) {
    std::string userInputString{};
    std::string inputCorrectnessCheck{};
    std::string negativeAnswers = "Nn";
    while (userInputString.empty()) {
        std::cout << questionText;
        std::getline(std::cin, userInputString);
        if (negativeAnswers.find(userInputString) != std::string::npos &&
            !userInputString.empty()) {
            std::cout << "Your choice is 'No'. Is that correct [Y/n]?";
        } else {
            userInputString = "Y";
            std::cout << "Your choice is 'Yes'. Is that correct [Y/n]?";
        }
        std::getline(std::cin, inputCorrectnessCheck);

        if (negativeAnswers.find(inputCorrectnessCheck) != std::string::npos &&
            !inputCorrectnessCheck.empty()) {
            userInputString = "";
        }
    }
    if (negativeAnswers.find(userInputString) != std::string::npos) {
        return false;
    } else {
        return true;
    }
}

int64_t getPositiveInputInt64Var(const std::string& questionText) {
    std::string userInputString{};
    int64_t userInputInt{};
    while (userInputString.empty()) {
        std::cout << questionText;
        std::getline(std::cin, userInputString);
        try {
            userInputInt = std::stoll(userInputString);
            if (userInputInt <= 0) {
                std::cout
                    << "The number must be a positive integer number. Please try again."
                    << std::endl;
                userInputString = "";
            }
        } catch (...) {
            std::cout << "The number must be a positive integer number. Please try again."
                      << std::endl;
            userInputString = "";
        }
    }
    return userInputInt;
}

int32_t getMenuOption() {
    std::cout << "-----------------------------------------------------------------" << std::endl;
    std::cout << "CLI-interactive user management system, please choose menu option:" << std::endl;
    std::cout << "1. Add a new client." << std::endl;
    std::cout << "2. Add one or more phone numbers for an existing client." << std::endl;
    std::cout << "3. Edit client info." << std::endl;
    std::cout << "4. Delete one or more client phone numbers." << std::endl;
    std::cout << "5. Delete client." << std::endl;
    std::cout << "6. Find client by forename, surname, email or phone number." << std::endl;
    std::cout << "10. Exit program." << std::endl;
    int64_t userMainMenuOption{100};
    while (std::find(validMainUserMenuOptions.begin(), validMainUserMenuOptions.end(), userMainMenuOption) == validMainUserMenuOptions.end()) {
        userMainMenuOption = getPositiveInputInt64Var("Choose a menu option to proceed: ");
    }
    return userMainMenuOption;
}


class PSQLDBHandler {
private:
    std::unique_ptr<pqxx::connection> psqlConn;
    PSQLPublicConnSpec psqlConnParams;
    std::shared_ptr<TableSchemaSpec> tableSchema;
public:
    PSQLDBHandler(std::string constrHost, std::string constrPort, std::string constrDbName, std::string constrUser, std::string constrPassword) :
        psqlConnParams{constrHost, constrPort, constrDbName, constrUser, constrPassword},
        psqlConn{nullptr},
        tableSchema{std::make_shared<TableSchemaSpec>(tableSchemaSpec)}
        {
        std::string connString{};
        connString.append("host=" + psqlConnParams.host + " ");
        connString.append("port=" + psqlConnParams.port + " ");
        connString.append("dbname=" + psqlConnParams.dbName + " ");
        connString.append("user=" + psqlConnParams.user + " ");
        connString.append("password=" + psqlConnParams.password + " ");
        try {
            psqlConn = std::make_unique<pqxx::connection>(connString);
            psqlConn->prepare("getPhoneNumberIDByPhone", "SELECT id_phone_number "
                                                    "FROM phone_numbers WHERE phone_number = $1");
            psqlConn->prepare("addPhoneNumber", "INSERT INTO phone_numbers (phone_number) "
                                                    "VALUES ($1)");
            psqlConn->prepare("getClientIDByFullnameEmail", "SELECT id_client "
                                                    "FROM clients "
                                                    "WHERE (forename = $1) AND (surname = $2) AND (email = $3)");
            psqlConn->prepare("getClientDetailsByClientID", "SELECT forename, surname, email "
                                                    "FROM clients "
                                                    "WHERE (id_client = $1)");
            psqlConn->prepare("addClient", "INSERT INTO clients (forename, surname, email) "
                                                    "VALUES ($1, $2, $3)");
            psqlConn->prepare("addClientPhoneNumber", "INSERT INTO client_phone_numbers (id_client, id_phone_number) "
                                                    "VALUES ($1, $2) ON CONFLICT DO NOTHING");
            psqlConn->prepare("getClientIDByForename", "SELECT id_client "
                                                    "FROM clients "
                                                    "WHERE (forename = $1)");
            psqlConn->prepare("getClientIDBySurname", "SELECT id_client "
                                                    "FROM clients "
                                                    "WHERE (surname = $1)");
            psqlConn->prepare("getClientIDByEmail", "SELECT id_client "
                                                    "FROM clients "
                                                    "WHERE (email = $1)");
            psqlConn->prepare("getClientIDByPhoneNumber", "SELECT clients.id_client "
                                                    "FROM clients "
                                                    "JOIN client_phone_numbers "
                                                    "ON clients.id_client = client_phone_numbers.id_client "
                                                    "JOIN phone_numbers "
                                                    "ON phone_numbers.id_phone_number = client_phone_numbers.id_phone_number "
                                                    "WHERE (phone_number = $1)");
            psqlConn->prepare("getPhoneNumbersByClientID", "SELECT phone_numbers.phone_number "
                                                    "FROM phone_numbers "
                                                    "JOIN client_phone_numbers "
                                                    "ON client_phone_numbers.id_phone_number = phone_numbers.id_phone_number "
                                                    "JOIN clients "
                                                    "ON client_phone_numbers.id_client = clients.id_client "
                                                    "WHERE (clients.id_client = $1)");
            psqlConn->prepare("updateClientForenameByClientID", "UPDATE clients "
                                                    "SET forename = $1 "
                                                    "WHERE (id_client = $2)");
            psqlConn->prepare("updateClientSurnameByClientID", "UPDATE clients "
                                                    "SET surname = $1 "
                                                    "WHERE (id_client = $2)");
            psqlConn->prepare("updateClientEmailByClientID", "UPDATE clients "
                                                    "SET email = $1 "
                                                    "WHERE (id_client = $2)");
            psqlConn->prepare("deletePhoneNumber", "DELETE FROM phone_numbers "
                                                    "WHERE (phone_number = $1)");
            psqlConn->prepare("deleteClientByID", "DELETE FROM clients "
                                                    "WHERE (id_client = $1)");
        }
        catch (const pqxx::sql_error& ex) {
            std::cout << ex.what() << std::endl;
        }
    }

    void recreateDBShema() {
        for (auto tableToDelete : tableDeletionOrder) {
            pqxx::work psqlTransaction{*psqlConn};
            auto psqlQueryRes = psqlTransaction.exec("DROP TABLE IF EXISTS " + psqlTransaction.esc(tableToDelete));
            psqlTransaction.commit();
        }
        for (auto tableToCreate : tableCreationOrder) {
            pqxx::work psqlTransaction{*psqlConn};
            auto psqlQueryRes = psqlTransaction.exec((*tableSchema).at(tableToCreate));
            psqlTransaction.commit();
        }
        std::cout << "[Re]created DB schema, tables are now empty" << std::endl;
    }

    std::vector<int32_t> addPhoneNumbersToDB(bool clientHasPhone) {
        if (!clientHasPhone) {
            clientHasPhone = getInputBoolVar("Will client have a phone number? [YyNn]: ");
        }
        std::vector<int64_t> phoneNumbers;
        while (clientHasPhone) {
            phoneNumbers.push_back(getPositiveInputInt64Var("Please enter client's new phone number: "));
            if (!getInputBoolVar("Will client have more phone numbers? [YyNn]: ")) {
                clientHasPhone = false;
            }
        }
        pqxx::result psqlQueryRes;
        pqxx::work psqlTransaction{*psqlConn};
        std::vector<int32_t> clientPhoneNumberIDs;
        for (auto phoneNumber : phoneNumbers) {
            psqlQueryRes = psqlTransaction.exec_prepared("getPhoneNumberIDByPhone", phoneNumber);
            psqlTransaction.commit();
            if (psqlQueryRes.size() == 0) {
                psqlQueryRes = psqlTransaction.exec_prepared("addPhoneNumber", phoneNumber);
                psqlTransaction.commit();
                psqlQueryRes = psqlTransaction.exec_prepared("getPhoneNumberIDByPhone", phoneNumber);
                psqlTransaction.commit();
            } else {
                std::cout << "Phone number " << phoneNumber << " is already in the DB" << std::endl;
            }
            clientPhoneNumberIDs.push_back(psqlQueryRes[0][0].as<int32_t>());
        }
        return clientPhoneNumberIDs;
    }

    void printClientDetailsByClientID(int64_t clientID) {
        pqxx::result psqlQueryRes;
        pqxx::work psqlTransaction{*psqlConn};
        psqlQueryRes = psqlTransaction.exec_prepared("getClientDetailsByClientID", clientID);
        auto clientForename = psqlQueryRes[0][0].as<std::string>();
        auto clientSurname = psqlQueryRes[0][1].as<std::string>();
        auto clientEmail = psqlQueryRes[0][2].as<std::string>();
        std::cout << "Name: " << clientForename << ", surname: " << clientSurname << ", email: " << clientEmail << std::endl;
    }

    bool printPhoneNumbersByClientIDReturnStatus(int64_t clientID, pqxx::work& psqlTransaction) {
        pqxx::result psqlQueryRes;
        try {
            psqlQueryRes = psqlTransaction.exec_prepared("getPhoneNumbersByClientID", clientID);
            psqlTransaction.commit();
            psqlQueryRes.for_each([](std::string_view phone_number) {
                std::cout << "Found client phone number: " << phone_number << std::endl;
            });
            return true;
        } catch (...) {
            std::cout << "Client doesn't have phone numbers" << std::endl;
            return false;
        }
    }

    void addClient() {
        auto forename = getInputStrVar("Please enter client's forename: ");
        auto surname = getInputStrVar("Please enter client's surname: ");
        auto email = getInputEmailVar("Please enter client's email address: ");
        auto clientPhoneNumberIDs = addPhoneNumbersToDB(false);
        pqxx::result psqlQueryRes;
        pqxx::work psqlTransaction{*psqlConn};
        psqlQueryRes = psqlTransaction.exec_prepared("getClientIDByFullnameEmail", forename, surname, email);
        psqlTransaction.commit();
        if (psqlQueryRes.size() == 0) {
            psqlQueryRes = psqlTransaction.exec_prepared("addClient", forename, surname, email);
            psqlTransaction.commit();
            psqlQueryRes = psqlTransaction.exec_prepared("getClientIDByFullnameEmail", forename, surname, email);
            psqlTransaction.commit();
        } else {
            std::cout << "User " << forename << " " << surname << " with " << email << " email is already in the DB";
        }
        auto clientID= psqlQueryRes[0][0].as<int32_t>();
        for (auto clientPhoneNumberID : clientPhoneNumberIDs) {
            psqlQueryRes = psqlTransaction.exec_prepared("addClientPhoneNumber", clientID, clientPhoneNumberID);
            psqlTransaction.commit();
        }
        std::cout << "User " << forename << " " << surname << " with " << email << " email has been successfully added/updated" << std::endl;
    }

    void addClientPhoneNumbers() {
        auto clientID = findClientIDByFullNameEmailPhone();
        if (clientID == 0) {
            std::cout << "Client is not found" << std::endl;
            return;
        }
        auto clientPhoneNumberIDs = addPhoneNumbersToDB(true);
        pqxx::result psqlQueryRes;
        pqxx::work psqlTransaction{*psqlConn};
        for (auto clientPhoneNumberID : clientPhoneNumberIDs) {
            psqlQueryRes = psqlTransaction.exec_prepared("addClientPhoneNumber", clientID, clientPhoneNumberID);
            psqlTransaction.commit();
        }
        std::cout << "User with ID " << clientID << " phone numbers have been successfully added/updated" << std::endl;
    }

    void editClientInfo() {
        auto clientID = findClientIDByFullNameEmailPhone();
        if (clientID == 0) {
            std::cout << "Client is not found" << std::endl;
            return;
        }
        std::cout << "Please choose field to update:" << std::endl;
        std::cout << "1. Forename." << std::endl;
        std::cout << "2. Surname." << std::endl;
        std::cout << "3. Email." << std::endl;
        int64_t userEditMenuOption{100};
        while (std::find(validEditUserMenuOptions.begin(), validEditUserMenuOptions.end(), userEditMenuOption) == validEditUserMenuOptions.end()) {
            userEditMenuOption = getPositiveInputInt64Var("Choose a menu option to proceed: ");
        }
        pqxx::result psqlQueryRes;
        pqxx::work psqlTransaction{*psqlConn};
        switch (userEditMenuOption) {
            case 1: {
                auto newForename = getInputStrVar("Please enter client's new forename: ");
                psqlQueryRes = psqlTransaction.exec_prepared("updateClientForenameByClientID", newForename, clientID);
                break;
            }
            case 2: {
                auto newSurname = getInputStrVar("Please enter client's new surname: ");
                psqlQueryRes = psqlTransaction.exec_prepared("updateClientSurnameByClientID", newSurname, clientID);
                break;
            }
            case 3: {
                auto newEmail = getInputStrVar("Please enter client's new email: ");
                psqlQueryRes = psqlTransaction.exec_prepared("updateClientEmailByClientID", newEmail, clientID);
                break;
            }
        }
        psqlTransaction.commit();
        std::cout << "User with ID " << clientID << " details have been successfully added/updated" << std::endl;
    }

    void deleteClientPhoneNumber() {
        auto clientID = findClientIDByFullNameEmailPhone();
        if (clientID == 0) {
            std::cout << "Client is not found" << std::endl;
            return;
        }
        std::cout << "Client is found, their details are: " << std::endl;
        printClientDetailsByClientID(clientID);
        pqxx::result psqlQueryRes;
        pqxx::work psqlTransaction{*psqlConn};
        while (getInputBoolVar("Delete client phone numbers? [YyNn]:")) {
            auto clientHasPhoneNumbers = printPhoneNumbersByClientIDReturnStatus(clientID, psqlTransaction);
            if (clientHasPhoneNumbers) {
                auto phoneNumberToDelete = getInputStrVar("Please enter phone number to delete: ");
                psqlQueryRes = psqlTransaction.exec_prepared("deletePhoneNumber", phoneNumberToDelete);
                psqlTransaction.commit();
                std::cout << "Deleted phone number " << phoneNumberToDelete << " if it existed" << std::endl;
            } else {
                break;
            }
        }
    }

    void deleteClient() {
        auto clientID = findClientIDByFullNameEmailPhone();
        if (clientID == 0) {
            std::cout << "Client is not found" << std::endl;
            return;
        }
        std::cout << "Client is found, their details are: " << std::endl;
        printClientDetailsByClientID(clientID);
        pqxx::result psqlQueryRes;
        pqxx::work psqlTransaction{*psqlConn};
        psqlQueryRes = psqlTransaction.exec_prepared("deleteClientByID", clientID);
        psqlTransaction.commit();
        std::cout << "Deleted client with ID " << clientID << std::endl;
    }

    int64_t findClientIDByFullNameEmailPhone() {
        std::cout << "Please choose search option:" << std::endl;
        std::cout << "1. By forename." << std::endl;
        std::cout << "2. By surname." << std::endl;
        std::cout << "3. By email." << std::endl;
        std::cout << "4. By forename, surname and email." << std::endl;
        std::cout << "5. By phone number." << std::endl;
        int64_t userSearchMenuOption{100};
        while (std::find(validSearchUserMenuOptions.begin(), validSearchUserMenuOptions.end(), userSearchMenuOption) == validSearchUserMenuOptions.end()) {
            userSearchMenuOption = getPositiveInputInt64Var("Choose a menu option to proceed: ");
        }
        pqxx::result psqlQueryRes;
        pqxx::work psqlTransaction{*psqlConn};
        switch (userSearchMenuOption) {
            case 1: {
                auto searchForename = getInputStrVar("Please enter client's forename: ");
                psqlQueryRes = psqlTransaction.exec_prepared("getClientIDByForename", searchForename);
                break;
            }
            case 2: {
                auto searchSurname = getInputStrVar("Please enter client's surname: ");
                psqlQueryRes = psqlTransaction.exec_prepared("getClientIDBySurname", searchSurname);
                break;
            }
            case 3: {
                auto searchEmail = getInputStrVar("Please enter client's email: ");
                psqlQueryRes = psqlTransaction.exec_prepared("getClientIDByEmail", searchEmail);
                break;
            }
            case 4: {
                auto searchForename = getInputStrVar("Please enter client's forename: ");
                auto searchSurname = getInputStrVar("Please enter client's surname: ");
                auto searchEmail = getInputEmailVar("Please enter client's email address: ");
                psqlQueryRes = psqlTransaction.exec_prepared("getClientIDByFullnameEmail",
                    searchForename,
                    searchSurname,
                    searchEmail);
                break;
            }
            case 5: {
                auto searchPhone = getPositiveInputInt64Var ("Please enter client's phone number: ");
                psqlQueryRes = psqlTransaction.exec_prepared("getClientIDByPhoneNumber", searchPhone);
                break;
            }

        }
        psqlTransaction.commit();
        int32_t clientID;
        try {
            clientID = psqlQueryRes[0][0].as<int32_t>();
        } catch (...) {
            clientID = 0;
        }
        return clientID;
    }
};

int main() {
    auto passForDB = getInputStrVar("Please enter password for DB user: ");
    auto userDBPSQLConn = std::make_unique<PSQLDBHandler>("127.0.0.1", "5432", "testuser2", "testuser2", passForDB);
    if (getInputBoolVar("[Re]create DB schema? [YyNn]: ")) {
        userDBPSQLConn->recreateDBShema();
    }
    auto userMenuOption = getMenuOption();
    while(userMenuOption != 0) {
        switch (userMenuOption) {
            case 1:
                userDBPSQLConn->addClient();
                break;
            case 2:
                userDBPSQLConn->addClientPhoneNumbers();
                break;
            case 3:
                userDBPSQLConn->editClientInfo();
                break;
            case 4:
                userDBPSQLConn->deleteClientPhoneNumber();
                break;
            case 5:
                userDBPSQLConn->deleteClient();
                break;
            case 6:
                auto clientID = userDBPSQLConn->findClientIDByFullNameEmailPhone();
                if (clientID != 0) {
                    std::cout << "Client found, their ID is: " << clientID << std::endl;
                    userDBPSQLConn->printClientDetailsByClientID(clientID);
                } else {
                    std::cout << "Client is not found" << std::endl;
                }
                break;
        }
        userMenuOption = getMenuOption();
    }
    return EXIT_SUCCESS;
}