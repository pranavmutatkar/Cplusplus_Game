//
//  main.cpp
//  Cplusplus_Game
//
//  Created by Pranav Mutatkar on 8/3/15.
//  Copyright (c) 2015 Pranav Mutatkar. All rights reserved.
//

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <vector>
using namespace std;


struct character
{
    // pc profile
    int life;
    string name;
    char sex;
    string prime;
    bool ace;
    int gold;
    
    // pc's pet info
    string petType;
    string petName;
    int petLife;
    string petPrime;
    bool petAid;
};


// declare functions
int random(int num);
void report(string text);
void createCharacter(character& pc);
void createPet(character& pc);
int setChallenge(character pc, int challenge);
void showMenu(character pc);
void applyChoice(char choice, int& dc, int& clock);
bool resolveChallenge(character& pc, int& dc);
void applyHealing(character& pc, int& clock);
void petComment(character pc, int dc, int tries);
void getGold(character& pc, int dc, int& clock);



int main()
{
    // seed random number generator
    srand(time(0));
    
    // create character
    character pc;
    createCharacter(pc);
    createPet(pc);
    
    
    // declare variables
    int clock = 60;         // 60 minutes
    int challenge = 1;      // start at first challenge
    int maxChallenges = 10; // go for this number of challenges
    int dc;                 // each challenge has a difficulty check
    char choice;            // user's choice
    bool success;           // does character pass or fail challenge
    int tries;              // count player's attempts at challenge
    int specialChallenge = random(6)+5;	// which challenge has the one-time nemisis/love interest and special reward?
    string text;
    
    
    // welcome
    text = "Can " + pc.name + " overcome " + to_string(maxChallenges) + " challenges in " + to_string(clock) + " minutes and find the quest item?";
    report(text);
    
    
    // outer loop takes user through all the challenges
    do{
        cout << "-----------" << endl;
        cout << "Challenge #" << challenge << endl;
        cout << "Time remaining = " << clock << " minutes" << endl;
        cout << "Health points = " << pc.life << endl;
        
        // get and report challenge and its difficulty
        dc = setChallenge(pc,challenge);
        
        // inner loop tasks user to try multiple times against a single challenge
        tries=0;
        do{
            
            // offer user choices and apply some results
            showMenu(pc);
            cin >> choice;
            applyChoice(choice, dc, clock);
            
            // resove one-time ace action
            if(choice == 'a' && pc.ace == true) pc.ace = false;
            
            // determine if pass or fail challenge
            success = resolveChallenge(pc, dc);
            tries++;
            
        }while(success==false && clock > 0 && pc.life > 0);
        
        
        // handle out of time or out of life
        if(clock <= 0 || pc.life <= 0) break;
        
        
        // handle when pc finds special challenge reward
        if(challenge == specialChallenge) {
            pc.gold += 10000;
            text = "You FOUND the quest item worth 10,000 gold!!!";
            report(text);
        }
        
        
        // pet may make comment
        if(pc.petLife > 0) petComment(pc,dc,tries);
        
        
        // handle character's search for treasure
        text = "Take time to find treasure (y/n)?";
        report(text);
        cin >> choice;
        if(choice=='y') getGold(pc,dc,clock);
        
        
        // handle character's need for healing
        if(pc.life < 10) {
            text = "Take time to stop and rest (y/n)?";
            report(text);
            cin >> choice;
            if(choice=='y') { applyHealing(pc, clock); }
        }
        
        challenge++;
        
    }while(clock > 0 && pc.life > 0 && challenge <= maxChallenges);
    
    
    // rate player's overall performance
    report("Game over!");
    if(clock >= 0 && pc.life >= 0 && challenge == maxChallenges) text = "Fantastic! " + pc.name + " wins!!!";
    else if(pc.life <= 0) text = "Oh," + pc.name + " died! So sorry! Better luck next time.";
    else text = "Oops, " + pc.name + " ran out of time. Perhaps try again?";
    text += " At the end, " + pc.name + " has " + to_string(pc.gold) + " gold.";
    report(text);
    
    return 0;
}


// generate random number from 1 to N
int random(int num) {
    int x = rand()%num + 1;
    return x;
}


// push output message to the user
void report(string text) {
    cout << text << endl;
    return;
}


// set character's traits
// includes: life, name, sex, prime attribute, and whether ace is available
void createCharacter(character& pc)
{
    pc.life = 10;
    cout << "What's your character's name?";
    cin >> pc.name;
    cout << "Is " << pc.name << " male(m) or female(f)?";
    cin >> pc.sex;
    cout << "What is " << pc.name << "'s prime attribute? Options are (brawn), (endurance), (agility), (intellect), (faith), or (charm).";
    cin >> pc.prime;
    pc.ace = true;
    pc.gold = random(10);
    return;
}


// create and describe the character's assistant
// these traits typically vary according to the genre, location, etc
void createPet(character& pc)
{
    // randomly generate pet's traits
    vector<string> option(7);
    option = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    pc.petName = option[random(7)-1];
    pc.petType = "an apprentice";
    pc.petLife = 3;
    vector<string> attribute(6);
    attribute = {"brawn", "endurance", "agility", "intellect", "faith", "charm"};
    pc.petPrime = attribute[random(6)-1];
    pc.petAid = false;
    
    // tell user about pet
    string text;
    text = pc.name + " has " + pc.petType + " named " + pc.petName + " who helps when " + pc.petPrime + " is needed.";
    text += pc.petName + " only has " + to_string(pc.petLife) + " life points so take care!";
    report(text);
    return;
}


// determine a challenge's difficulty
// DC follows a bell curve from very easy to very hard
// it gets harder with each challenge, and the
// challenge is easier if it matches pc's prime attribute

int setChallenge(character pc, int challenge) {
    string text;
    string focus;
    int dc;
    
    dc = random(6)+random(6)+random(6);     // difficulty check = bell curve from 3 to 18, average 10.5
    dc += challenge;                        // check gets progressively harder with each challenge
    
    // match up challenge with character's and pet's prime attribute
    vector<string> attribute(6);
    attribute = {"brawn", "endurance", "agility", "intellect", "faith", "charm"};
    focus = attribute[random(6)-1];
    if(focus == pc.prime) dc -= 5;          // check is much easier if it aligns with character's prime attribute
    if(focus == pc.petPrime && pc.petLife > 0) { dc -= 5; pc.petAid = true; }       // the pet might help you too!
    else pc.petAid = false;
    
    // how easy or hard is this challenge?
    if(dc <= 5) text = "very easy";
    else if(dc >= 8) text = "easy";
    else if(dc >= 12) text = "moderate";
    else if(dc >= 15) text = "hard";
    else text = "very hard";
    
    // give text description
    text = "The challenge requires " + focus + " and looks " + text + ".";
    if(focus == pc.prime) text += "You've got lots of " + focus + ".";
    report(text);
    if(focus == pc.petPrime && pc.petLife > 0) {
        text = pc.petName + " has " + pc.petPrime + " and comes forward to help you.";
        report(text);
    }
    return(dc);
}


// show user what actions are possible
void showMenu(character pc) {
    string text;
    
    if(pc.ace == true) text = "Does " + pc.name + " rush(r) the challenge, study(s) it, try normally(n), or play one-time ace(a)? ";
    else text = "Does " + pc.name + " rush(r) the challenge, study(s) it, or try normally(n)?";
    
    report(text);
    return;
}


// adjust clock and chance of success based on user's approach to challenge
// r = rush, s = study, a = ace, and n = normal
void applyChoice(char choice, int& dc, int& clock) {
    
    if(choice == 'r') { dc += 2; clock -=1; }   		// a little harder but fast
    else if(choice == 's') { dc -= 5; clock -= 5; } 	// much easier but slow
    else if(choice == 'a') { dc -= 20; clock -= 1; } 	// one-time special life saver
    else { clock -= 3; }   // so so                     // everything else
    
    return;
}


// resolve whether pass or fail challenge
bool resolveChallenge(character& pc, int& dc) {
    
    // set variables
    bool success;
    int skillCheck = random(20);
    string text;
    int x;
    
    // set up wound options
    vector<string> damage(4);
    damage = {"minor scratches, no harm", "minor damage, a wee bit of blood", "moderate damage, oh it hurts", "major damage, a gaping wound"};
    
    // show success or failure
    if(skillCheck >= dc) { success = true; text = "Success!"; }
    else {
        success = false;
        dc--;   // each failure brings you slightly closer to success (learn from mistakes!)
        x = random(4)-1; 	// suffer 0 to 3 life damage
        pc.life -= x;
        text = "Failure! " + pc.name + " suffers " + damage[x] + ", and now has " + to_string(pc.life) + " life points. Try again!";
        if(pc.petAid == true) {
            pc.petLife -= x;
            text += pc.petName + " suffers too!";
            if(pc.petLife <= 0) { pc.petAid = false; text += pc.petName + " is knocked out."; }
        }
    }
    report(text);
    
    return(success);
}


// character heals 3 life points after 10 minutes
// pet also heals
void applyHealing(character& pc, int& clock) {
    clock -= 10;
    pc.life += 3;
    pc.petLife += 3;
    if(pc.life > 10) pc.life = 10;      // life cannot exceed 10
    if(pc.petLife > 3) pc.petLife = 3;  // life cannot exceed 10
    string text = pc.name + " stops to rest 10 minutes and now has " + to_string(pc.life) + " health points.";
    text += pc.petName + " is also now full of life.";
    report(text);
    return;
}


// pet congratulates character
void petComment(character pc, int dc, int tries) {
    
    string text;
    if(tries == 1 && dc > 14) text = pc.petName + " cheers.";
    report(text);
    return;
}


// successful encounter results in treasure if character takes time to search
void getGold(character& pc, int dc, int& clock) {
    int x, g;
    string text;
    
    // apply search time
    x = random(3);
    clock -= x;
    text = "Searching took " + to_string(x) + " minutes.";
    report(text);
    
    // set gold potential of challenge
    if(dc <= 5) x = 10;
    else if(dc >= 8) x = 30;
    else if(dc >= 12) x = 100;
    else if(dc >= 15) x = 300;
    else x = 1000;
    
    // transfer gold to the character
    g = random(x);
    pc.gold += g;
    text = "You found " + to_string(g) + " gold. You now have " + to_string(pc.gold) + " gold.";
    report(text);
    
    return;
}

