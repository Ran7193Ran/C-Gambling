#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <vector>
#include <windows.h>
//test
using namespace std;

string suitSymbol(char suit);

struct Card{
    public:
    int value;
    char suit;
    bool isAce;
};

class Deck{
    public:
    vector<Card> cards;

    void createDeck(){
        cards.clear();//empty the vector;
        char suits[4]={'D','C','H','S'};
        for(int s=0;s<4;s++){
            for(int v=1;v<=13;v++){
                Card c;
                c.value = (v>10?10:v);
                c.isAce = (v==1);
                c.suit = suits[s];
                cards.push_back(c);
            }
        }
    }

    void showDeck(){
        for(int i=0;i<cards.size();i++){
            cout<<"["<<cards[i].value<<cards[i].suit<<"]";
        }
    }

    void shuffle(){
        static std::mt19937 rng(time(0));
        std::shuffle(cards.begin(), cards.end(), rng);
    }

    Card draw(){
        Card c = cards.back();
        cards.pop_back();
        //cout<<"["<<c.value<<c.suit<<"]";
        return c;
    }
};

class Player {
    private:
    int balance = 0;
    bool hide = 0;

    public:
    int betAmount = 0;
    int total = 0;
    int aceCount = 0;
    vector<Card> hand;

    Card drawHand(Deck& d){
        Card c = d.draw();
        hand.push_back(c);
        //total += c.value;

        if(c.isAce)
        aceCount++;

        while(aceCount > 0 && total + 10 <= 21){
            total += 10;
            aceCount--;
        }

        return c;
    }

    int getTotal(){
        int sum = 0;
        int aceCount = 0;

        for(int i = 0;i < hand.size();i++){
            sum += hand[i].value;
            if(hand[i].value == 11) aceCount++;
        }

        while (sum > 21 && aceCount>0)
        {
            sum -= 10;
            aceCount--;
        }
        return sum;
    }

    void showHand(bool hide){
        for(int i=0;i<hand.size();i++){
            if(hide && i== 0){
                cout<<"[??]";
            }else{
                cout<<"["<<hand[i].value<<suitSymbol(hand[i].suit)<<"]";
            }
        }
        cout<<endl;
    }

    void resetHand(){
        hand.clear();
        total = 0;
        aceCount = 0;
    }

    void loadBalance(){
        ifstream infile("bet.txt");
        infile >> balance;
        infile.close();
    }

    void saveBalance(){
        ofstream outfile("bet.txt");
        outfile << balance;
        outfile.close();
    }

    void placeBet(){
        cout << "Current Balance: $"<< balance << endl;
        cout << "Place a Bet: $";
        cin >> betAmount;

        if (betAmount>balance)
        {
            cout << "Not enough Balance!!"<<endl;
        }

        balance -= betAmount;
    }

    void updateBalnce(int win){
        if(win == 4){
            balance += betAmount;
        }
        else if(win==3){
            balance += betAmount*2.5;
        }else if(win==2){
            balance += betAmount*5;
        }else if(win==1){
            balance += betAmount*2;
        }else if(win==0){
            
        }
    }
};

void coinFlip(int *win);
void slotMachine(int *win);
void blackJack(int *win);

int main(){
    SetConsoleOutputCP(CP_UTF8);
    char choice;
    int ans;
    int win = 0;
    int balance = 0;
    int betAmount = 0;
    Player player;

    cout<<"\n<<<Welcome To My Casino.com>>>"<<endl;

    do{
        cout<<"Please pick a game: "<<endl;
        cout<<"(1. slotMachin)"<<endl;
        cout<<"(2. coinFLip)"<<endl;
        cout<<"(3. blackJack)"<<endl;
        cout<<"(0. Exit)"<<endl;
        cin>>ans;

        if(ans==1){
            do{
                player.loadBalance();
                player.placeBet();
                slotMachine(&win);
                player.updateBalnce(win);
                player.saveBalance();

                cout << "Play Again?(y/n): ";
                cin >> choice;
            }while(choice == 'y'||choice =='Y');
            
        }else if(ans==2){
            do{
                player.loadBalance();
                player.placeBet();
                coinFlip(&win);    //Coin FLip Game
                player.updateBalnce(win);
                player.saveBalance();

                cout << "Play Again?(y/n): ";
                cin >> choice;
            }while(choice == 'y'||choice =='Y');
        
        }else if(ans==3){
            do{
                player.loadBalance();
                player.placeBet();
                blackJack(&win);
                player.updateBalnce(win);
                player.saveBalance();

                cout << "Play Again?(y/n)";
                cin>> choice;
            }while(choice == 'y' || choice == 'Y');
            
        }else if(ans==0){
            break;
        }
    }   while(true);

    cout << "Thanks for playing!" << endl;
    return 0;
}

void blackJack(int* win){
    int ans = 0;
    Deck deck;
    Player player1, player2, dealer;

    deck.createDeck();
    deck.shuffle();
    cout<<"Dealing";
    this_thread::sleep_for(chrono::seconds(1));
    cout<<".";
    this_thread::sleep_for(chrono::seconds(1));
    cout<<".";
    this_thread::sleep_for(chrono::seconds(1));
    cout<<".";
    cout<<"Dealer :\n";
    dealer.drawHand(deck);
    dealer.drawHand(deck);
    dealer.showHand(true);
    cout<<"Player :\n";
    player1.drawHand(deck);
    player1.drawHand(deck);
    player1.showHand(false);

    do{
        cout<<"(1 to Hit) or (0 to Stand)\n";
        cin>>ans;
        switch(ans){
            case 1:
            player1.drawHand(deck);
            player1.showHand(false);
            if(player1.getTotal() > 21){
                ans = 0;
            }
            break;
            case 0:
            break;
        }
    }while(ans==1);
    
    if(player1.getTotal() > 21){
        cout<<"BUSTED! You Lose!"<<endl;
        *win = 0;
        cout<<player1.getTotal();
        return;
    }

    while(dealer.getTotal()<17){
        cout<<"Dealer drewing";
        this_thread::sleep_for(chrono::seconds(1));
        cout<<".";
        this_thread::sleep_for(chrono::seconds(1));
        cout<<".";
        this_thread::sleep_for(chrono::seconds(1));
        cout<<".\n";
        cout<<"Dealer drew a card \n";
        dealer.drawHand(deck);
    }
    cout<<"Dealer Hand: \n";
    dealer.showHand(false);
    
    if(dealer.getTotal() > 21){
        cout<<"Dealer Busted, You WIN!!"<<endl;
        *win = 1;
    }else if(player1.getTotal() == 21){
        cout<<"BLACK JACK!!"<<endl;
        *win = 3;
    }else if(player1.getTotal() > dealer.getTotal() && player1.getTotal() <=21 ){
        cout<<"You WIN"<<endl;
        *win = 1;
    }else if(player1.getTotal() < dealer.getTotal()){
        cout<<"You Lose!!"<<endl;
        *win = 0;
    }else{
        cout<<"PUSH (Tie)"<<endl;
        *win = 4;
    }
    
}

void slotMachine(int *win){
    char start;
    int a,b,c;
    cout<<"Pull the lever (Y/N): ";
    cin>>start;

    if(start == 'y' || start == 'Y'){
        cout<<"spinning";
        this_thread::sleep_for(chrono::seconds(1));
        cout<<".";
        this_thread::sleep_for(chrono::seconds(1));
        cout<<".";
        this_thread::sleep_for(chrono::seconds(1));
        cout<<".";
        srand(time(0));
        a = rand()%7+1;
        b = rand()%7+1;
        c = rand()%7+1;

        this_thread::sleep_for(chrono::seconds(1));
        cout<<"\n["<<a<<"]["<<b<<"]["<<c<<"]/"<<endl;

        if(a==b && b==c){
            cout<<"ITS A BIG WIN!!"<<endl;
            *win = 2;
        }else if(a==b || a==c || b==c){
            cout<<"YOU WIN!!"<<endl;
            *win = 1;
        }else{
            cout<<"LOSER!!"<<endl;
            *win = 0;
        }
    }
}

void coinFlip(int *win){

    int ans;
    int userAns;

    cout<<"Pick a side (1 for head/ 0 for tail): ";
    cin>>userAns;
    srand(time(0));
    ans = rand()%2;

    if(userAns==ans){
        cout<<"You Win!!"<<endl;
        *win=1;
    }else{
        cout<<"You Lose!!"<<endl;
        *win=0;
    }

}

string suitSymbol(char suit){
    switch(suit){
        case 'H': return "\u2665";
        case 'D': return "\u2666";
        case 'C': return "\u2663";
        case 'S': return "\u2660";
        default: return "?";
    }
}
