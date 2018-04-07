#include <iostream>
#include <regex>
#include "ImportData.hpp"

using std::cout;
using std::endl;
using std::ifstream;
using std::ostringstream;
using std::regex;
ImportData::ImportData(string nome)
{
    string input;
    //cout << "Importando Arquivo \n";
    myfile.open(nome);
    if (!myfile.is_open()) {
        cout << "Falha na leitura do arquivo" << endl;
        exit(EXIT_FAILURE);
    }
    myfile >> input;
    regexManager(input);
    while (myfile >> input) {
        regexManager(input);
        if (findIgnoredWords(input)) {
            reader(flag, input);
        }
    }
}

void ImportData::regexManager(string input)
{
    //|[[:digit:]]+?=[\\.]?=[[:digit:]]+
    regex number("[[:digit:]]+");
    regex name("NAME(.*)");
    regex type("TYPE(.*)");
    regex comment("COMMENT(.*)");
    regex dimension("DIMENSION(.*)");
    regex edge_type("EDGE(.*)");
    regex node_comment("NODE(.*)");
    regex capacity("CAPACITY(.*)");
    regex demand("DEMAND_SECTION(.*)");
    regex depot("DEPOT_SECTION(.*)");
    if (regex_match(input, name)) {
        flag = "name";
        //cout << "Nome\n";
    } else if (regex_match(input, type)) {
        flag = "type";
        //cout << "Tipo\n";
    } else if (regex_match(input, comment)) {
        flag = "comment";
    } else if (regex_match(input, dimension)) {
        flag = "dimension";
    } else if (regex_match(input, edge_type)) {
        flag = "edge_type";
    } else if (regex_match(input, node_comment)) {
        flag = "node_comment";
    } else if (regex_match(input, number) && (flagaux == "wait for coord" || flagaux == "wait for demand" || flagaux == "wait for depot")) {
        flag = "number";
    } else if (regex_match(input, capacity)) {
        // cout<<"entrou aqui no capacity"<<endl;
        flag = "capacity";
    } else if (regex_match(input, demand)) {
        flag = "demand";
        // cout << "DEMAND_SECTION" << endl;
        flagaux = "wait for demand";
        vecpos = 0;
    } else if (regex_match(input, depot)) {
        flag = "depot";
        //cout << "DEPOT_SECTION" << endl;
        flagaux = "wait for depot";
    }
}

bool ImportData::findIgnoredWords(string input)
{
    regex ignore("-1|:|NAME(.*)|TYPE(.*)|COMMENT(.*)|DIMENSION(.*)|EDGE(.*)|EOF|CAPACITY(.*)|DEMAND_SECTION|DEPOT_SECTION");
    if (regex_match(input, ignore)) {
        return false;
    }
    //  cout << input;
    return true;
}

void ImportData::reader(string flag, string input)
{
    if (flag == "name") {
        tspName = input;
        //    cout << input << " is a name! \n";
    } else if (flag == "type") {
        type = input;
        //  cout << input << " is a type! \n";
    } else if (flag == "comment") {
        comment += (input + " ");
        //cout << input << " is a comment! \n";
    } else if (flag == "dimension") {
        //cout << input << " is a dimension! \n";
        //citiescoord.reserve(std::stoi(input));
        // flagaux = "wait for coord";
        dimension = stoi(input);
    } else if (flag == "edge_type") {
        edge_type = input;
        //cout << input << " is an edge_type! \n";
    } else if (flag == "node_comment") {
        node_comment = input;
        //cout << input << " is an node_comment! \n";
    }

    else if (flag == "capacity") {
        //cout<< input<< " is a capacity"<<endl;
        capacity = stoi(input);
        flagaux = "wait for coord";
    } else if (flag == "demand") {
        //cout << "starting the reading of the demand section" << endl;
        flagaux = "wait for demand";
    } else if (flag == "depot") {
        // cout<< "starting the reading of the demand section"<<endl;
        flagaux = "wait for depot";
    } else if (flag == "number" && flagaux == "wait for depot") {
        //cout << "I: " << input << endl;
        customerList[(std::stoi(input) - 1)].setDepot(true);
        myfile >> input;
    } else if (flag == "number" && flagaux == "wait for demand") {
        //cout << "I: " << input;
        myfile >> input;
        double demand = std::stod(input);
        ///cout << "\t Demand: " << input << endl;
        vecpos++;
        customerList[vecpos - 1].setDemand(demand);
        //APENAS PARA TESTE RETIRAR DEPOIS
        if (demand == 0) {
            customerList[vecpos - 1].setDepot(true);
        }
        //APENAS PARA TESTE RETIRAR DEPOIS
        //cout << "vecpos: " << vecpos << endl;
    }

    else if (flag == "number" && flagaux == "wait for coord") {
        int id = std::stoi(input);
        //cout << "I: " << input; //debug
        myfile >> input;
        double x = std::stod(input);
        // cout << "\t X: " << input; //debug
        myfile >> input;
        double y = std::stod(input);
        // cout << "\t Y: " << input << "\n"; //debug
        vecpos++;
        //  cout << "vecpos: " << vecpos << endl; //debug
        Customer caux;
        caux.setId(id);
        caux.setX(x);
        caux.setY(y);
        customerList.push_back(caux);
    }
}
void ImportData::printInfos()
{
    cout << "Nome: " << gettspName() << endl;
    cout << "Tipo: " << getType() << endl;
    cout << "Comentário: " << getcomment() << endl;
    cout << "Tipo do Vértice: " << getedge_type() << endl;
    cout << "Dimensão: " << dimension << endl;
    cout << "Capacidade: " << capacity << endl;
    cout << "Comentário do tipo de node: " << getnode_comment() << endl;
}
string ImportData::getInfos()
{
    string input;
    input = "Nome: " + gettspName() + "\n";
    input = input + "Tipo: " + getType() + "\n";
    input = input + "Comentário: " + getcomment() + "\n";
    input = input + "Tipo do Vértice: " + getedge_type() + "\n";
    input = input + "Comentário do tipo de node: " + getnode_comment() + "\n";
    cout << input;
    return input;
}

vector<Customer> ImportData::getCustomerList()
{
    return customerList;
}
string ImportData::gettspName()
{
    return tspName;
}
string ImportData::getType()
{
    return type;
}
string ImportData::getedge_type()
{
    return edge_type;
}
string ImportData::getnode_comment()
{
    return node_comment;
}
int ImportData::getCapacity()
{
    return capacity;
}
string ImportData::getcomment()
{
    if (comment == "") {
        return "N/A";
    } else {
        return comment;
    }
}
