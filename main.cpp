#include <iostream>
#include "OSFS.hpp"
#include <fstream>
#include <string.h>
#include "OSFS.cpp"
using namespace std;


void parse( char* input, char* args[] ) {
    input[strlen(input)-1] = '\0';
    args[0] = strtok(input, " ");
    args[1] = strtok(NULL, " ");
    args[2] = strtok(NULL, "\0");
}


int main() {

    char arg [55000];
    char* arglist[3];
    Disk* disk1;
    disk1 = new Disk();
    while (strcmp(arg, "exit\n") != 0) {
        printf("File System > ");
        if ( fgets(arg, 55000, stdin) == NULL) {
            perror( "read input failed" );
            exit(1);
        }
        else {
            parse(arg, arglist);
            if ((strcmp(arglist[0], "create") == 0 || strcmp(arglist[0], "CREATE") == 0) && arglist[1]!= nullptr) {
                disk1->create(arglist[1]);
                disk1->display();
            }
            else if ((strcmp(arglist[0], "open") == 0 || strcmp(arglist[0], "OPEN") == 0) && arglist[1] != nullptr) {
                disk1->open(arglist[1]);
                disk1->display();
            }
            else if (strcmp(arglist[0], "close") == 0 || strcmp(arglist[0], "CLOSE") == 0) {
                disk1->close();
                disk1->display();
            }
            else if ((strcmp(arglist[0], "write") == 0 || strcmp(arglist[0], "WRITE") == 0) && arglist[2]!= nullptr && arglist[1]!= nullptr) {
                if (arglist[2][0] == '\'' && arglist[2][strlen(arglist[2])-1] == '\'') {
                    char trim[strlen(arglist[2])-2];
                    for (int i = 0; i < strlen(arglist[2])-2; i++) {
                        trim[i] = arglist[2][i+1];
                    }
                    disk1->write(atoi(arglist[1]), trim);
                    disk1->display();
                }
                else {
                    cout << "Error: Write failed. Please put single quote \' \' outside the writing input" << endl;
                    disk1->display();
                }
            }
            else if ((strcmp(arglist[0], "read") == 0 || strcmp(arglist[0], "READ") == 0) && arglist[1]!= nullptr) {
                disk1->read(atoi(arglist[1]));
                disk1->display();
            }
            else if ((strcmp(arglist[0], "seek") == 0 || strcmp(arglist[0], "SEEK") == 0) && arglist[1]!= nullptr && arglist[2]!= nullptr) {
                disk1->seek(atoi(arglist[1]), atoi(arglist[2]));
                disk1->display();
            }
            else if (strcmp(arglist[0], "exit") == 0){
                break;
            }
            else {
                cout << "Error: please enter a valid input" << endl;
                disk1->display();
            }
        }
    }
}


//    string = '311278579694851778956985832020972961098285632306108810593139486104523640001081408853089008967948560258106541787330017862951078282004979843264050105814863362891098101047618336010084095921007106172171352796756810---200---8231866011015729310109715658191059510841463101433485082410812772210358887753310109369430216571190551779627441091648146073603721006107067747474100105872590108678631969718265104307004640453896948199108451071951199010555---400---127886983308117987709310943755561461019108174103073862481161643770880527310394101087060949432941108884904599571056872491070179061005433938086905487157553206445870133005450652107005579721011102480151919800210455375---600---35122021021710639552919708547237338677592476526881003593810905138775577249795421265180091071281002977710801002219555062794810004234246310236146008261089110672546310162596067567824107310298392504301083444385611034462---800---31893857005102374761305461944081049298027857258592473923669628610378837904000806482631027741054854610169103332763610658358418880102846633104211840799928691081542729235738310051447106553101328310736041110211257923350---1000---78618567456102294526410572848627608101691821210771034054817061754410108106108810106385104181951373024841866981391169216219951006592562730187332110385374908557316680341841099889115699280557404810790810143062102791004614---1200---51319181566240910204163110271066910312644769191588107108477326102395263109941049251167376102839199493101561028'
//    string length = 1400
