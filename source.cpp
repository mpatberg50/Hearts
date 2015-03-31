#include <iostream>
#include <time.h>
#include <string>

using namespace::std;

void createDeck(int (&playerHands)[4][13])
{
	int suitCounter=100,cardCounter=0;

	for(int x=0; x<4; x++)
	{
		for(int y=0; y<13; y++)
		{
			playerHands[x][y]=suitCounter+cardCounter;
			cardCounter++;
		}
		suitCounter+=100;
		cardCounter=0;
	}
}

void shuffle(int (&playerHands)[4][13])
{
	srand(time(NULL));

	for(int x=0; x<500;x++)
		swap(playerHands[rand()%4][rand()%13],playerHands[rand()%4][rand()%13]);
}

void orderHands(int (&hands)[4][13])
{
	int min=0;
	for(int z=0; z<4; z++)
		for(int x=0; x<13;x++)
		{
			min=x;
			for(int y=x;y<13;y++)
				if(hands[z][y]<hands[z][min])
					min=y;
			swap(hands[z][x],hands[z][min]);
		}
}

void printUserHand(int playerHands[4][13], string suits[4], string numbers[13])
{
	int counter=0;

	for(int x=0; x<13 && playerHands[0][x]!=0 ; x++)
	{
		if(counter!=playerHands[0][x]/100)
		{
			counter=playerHands[0][x]/100;
			cout<<"\n"<<suits[playerHands[0][x]/100-1];
		}

			cout<<"  "<<x<<"--"<<numbers[playerHands[0][x]%100];
	}
}

bool containsLeadSuit(int playerHands[4][13],int playerNumber, int leadCard)
{
	for(int x=0; x<13 && playerHands[playerNumber-1][x]<leadCard+20; x++)
		if(playerHands[playerNumber-1][x]/100==leadCard/100)
			return true;
	return false;
}

 bool pointsOnTable(int cardsPlayed[4])
 {
	 for(int x=0; x<4; x++)
		 if(cardsPlayed[x]==310 ||cardsPlayed[x]/100==1)
			 return true;
		 return false;
 }

 //returns the card to play if the player has a card in the lead suit
 int highCardSameSuit(int playerHands[4][13], int playerNumber, int cardsPlayed[4])
 {
	 int suit=cardsPlayed[0]/100;
	 int highCard=0;
	int max=0;

	if(pointsOnTable(cardsPlayed))
	{
	for(int x=0; x<4; x++)
		if(cardsPlayed[x]/100==suit && cardsPlayed[x]>cardsPlayed[max])
			max=x;

	int maxCard=cardsPlayed[max];

	for(int x=0; x<13 && playerHands[playerNumber-1][x]<maxCard && playerHands[playerNumber-1][x];x++)
		highCard=playerHands[playerNumber-1][x];

	if(highCard/100==maxCard/100)
		return highCard;
	
	for(int x=0; x<13 && playerHands[playerNumber-1][x]<maxCard+20 && playerHands[playerNumber-1][x];x++)
		highCard=playerHands[playerNumber-1][x];
	return highCard;
	}
	for(int x=0; x<13 && playerHands[playerNumber-1][x]<suit*100+20 && playerHands[playerNumber-1][x]!=0; x++)
		highCard=playerHands[playerNumber-1][x];

	return highCard;	
 }

bool contains(int playerHands[4][13], int playerNumber, int numberSearched)
{
	for(int x=0; x<13; x++)
		if(playerHands[playerNumber-1][x]==numberSearched)
			return true;
	return false;
}

int maxSuitMaxCard(int playerHand[4][13], int playerNumber, int maxSuits[4])
{
	int max=0;
	for(int x=0; x<13; x++)
		if((playerHand[playerNumber-1][x]/100==maxSuits[0] || playerHand[playerNumber-1][x]/100==maxSuits[1] || playerHand[playerNumber-1][x]/100==maxSuits[2] || playerHand[playerNumber-1][x]/100==maxSuits[3]) && playerHand[playerNumber-1][x]%100>playerHand[playerNumber-1][max] )
			max=x;
	return playerHand[playerNumber-1][max];
}

int voidSuitCard(int playerHands[4][13], int playerNumber)
{

	int suitCounters[4]={0};

	for(int x=0; x<13; x++)
		if(playerHands[playerNumber-1][x]!=0)
			suitCounters[playerHands[playerNumber-1][x]/100-1]++;
	
	int max=0;
	int counter=1;
	int maxSuits[4]={1,0,0,0};

	for(int x=1; x<4;x++)
		if(suitCounters[x]<suitCounters[max] || suitCounters[max]==0)
		{
			maxSuits[1]=0; maxSuits[2]=0; maxSuits[3]=0;	maxSuits[0]=x+1;
			counter=1;
			max=x;
		}
		else if(suitCounters[x]==suitCounters[max])
		{
			maxSuits[counter]=x+1;
			counter++;
		}

	if(counter>1)
		return maxSuitMaxCard(playerHands,playerNumber,maxSuits);
	max=0;
	for(int x=0; x<13 && playerHands[playerNumber-1][x]<(maxSuits[0])*100+20 && playerHands[playerNumber-1][x]!=0;x++)
		max=playerHands[playerNumber-1][x];
	return max;
}

int playCardNotLeading(int playerHands[4][13],int playerNumber, int cardsPlayed[4], bool &heartsPlayed)
{
	//check for suit
	if(containsLeadSuit(playerHands, playerNumber,cardsPlayed[0]))
		return highCardSameSuit(playerHands,playerNumber,cardsPlayed);
	
	//check for queen of spades
	if(contains(playerHands,playerNumber,310))
		return 310;
	
	//check for hearts
	for(int x=12; x>=0; x--)
		if(contains(playerHands,playerNumber,100+x))
		{
			heartsPlayed=true;
			return 100+x;
		}
	//return king or ace of spades
	if(contains(playerHands,playerNumber,312))
		return 312;
	else if(contains(playerHands, playerNumber,311))
		return 311;


	//return card of suit you are trying to void
	return voidSuitCard(playerHands,playerNumber);

}

int findMinCardToLead(int playerHands[4][13], int playerNumber, bool heartsPlayed)
{
	int min=99;

	for(int x=0; x<13; x++)
		if(min%100>playerHands[playerNumber-1][x]%100 && playerHands[playerNumber-1][x]!=0)
			if(heartsPlayed || playerHands[playerNumber-1][x]>120)
				min=playerHands[playerNumber-1][x];

	if(min==99)
		return playerHands[playerNumber-1][0];
	return min;


}

int playCardLeading(int playerHands[4][13], int playerNumber, int (&cardsPlayed)[4], bool heartsPlayed)
{
	if(contains(playerHands,playerNumber, 400))
	{
		cardsPlayed[0]=400;
		return 400;
	}
	int minCard=findMinCardToLead(playerHands,playerNumber,heartsPlayed);
	cardsPlayed[0]=minCard;
	return minCard;
}



void modifyCards(int (&playerHands)[4][13], int (&cardsPlayed)[4], int cardPlayed)
{
	for(int x=0; x<4; x++)
		if(cardsPlayed[x]==0)
		{
			cardsPlayed[x]=cardPlayed;
			x=4;
		}
}



int wonHand(int cardsPlayed[4], int &leadPlayer)
{
	int winningSpot=0;
	for(int x=0; x<4; x++)
		if(cardsPlayed[winningSpot]<cardsPlayed[x] && cardsPlayed[x]/100==cardsPlayed[0]/100)
			winningSpot=x;

	int winningPlayer=leadPlayer+winningSpot;
	if(winningPlayer>4)
		winningPlayer-=4;
	leadPlayer=winningPlayer;
	return winningPlayer;
}

void printCardPlayed(int cardPlayed, bool leadCard, string suits[4], string numbers[13])
{
	if(leadCard)
		cout<<"Computer leads with ";
	else
		cout<<"Computer plays ";
	cout<<numbers[cardPlayed%100]<<" of "<<suits[cardPlayed/100-1];
}

void doUserTurn(int playerHands[4][13], string suits[4], string numbers[13], int (&cardsPlayed)[4], bool &heartsPlayed)
{
	int input=0;

	if(contains(playerHands,1,400))
	{
		cout<<"\nYou lead with the two of clubs...\n";
		cardsPlayed[0]=400;
	}
	else
	{
		printUserHand(playerHands,suits,numbers);
		cout<<"\n";
		//determine output
		if(cardsPlayed[0]==0)
			cout<<"What card would you like to lead? ";
		else
			cout<<"What card would you like to play? ";
		cin>>input;

		//picks card that has been displayed
		int cardPicked= playerHands[0][input];
	
		while(cardsPlayed[0]==0 && !heartsPlayed && cardPicked/100==1)
		{
			cout<<"No hearts have been played. You cannot lead with a heart. Pick again: ";
			cin>>input;
			cardPicked= playerHands[0][input];
		}

		bool containsSuit=false;

		for(int x=0; x<13; x++)
			if(contains(playerHands,1,(cardsPlayed[0]/100)*100+x))
				containsSuit=true;

		while(cardsPlayed[0]!=0 && cardPicked/100!=cardsPlayed[0]/100 && containsSuit)
		{
			cout<<"Play the suit that has been led: ";
			cin>>input;
			cardPicked=playerHands[0][input];
		}


		if(cardPicked/100==1)
			heartsPlayed=true;
		//adds user card to array of the cards played
		for(int x=0; x<4;x++)
			if(cardsPlayed[x]==0)
			{
				cardsPlayed[x]=cardPicked;
				x=5;
			}
	}
}

void addPoints( int (&pointsForRound)[4], int cardsPlayed[4], int leadPlayer)
{
	int winner=wonHand(cardsPlayed,leadPlayer);
	int points=0;

	for(int x=0; x<4; x++)
		if(cardsPlayed[x]==310)
			points+=13;
		else if(cardsPlayed[x]/100==1)
			points++;
	pointsForRound[winner-1]+=points;
}

void removeCards(int (&playerHand)[4][13], int (&cardsPlayed)[4])
{
	for(int x=0; x<4; x++)
		for(int r=0; r<13; r++)
			for(int c=0; c<4; c++)
				if(cardsPlayed[x]==playerHand[c][r])
				{
					playerHand[c][r]=0;
					for(int y=r;y<12;y++)
						swap(playerHand[c][y+1],playerHand[c][y]);
				}
}

void printWinner(int playerHands[4][13], int cardsPlayed[4], int leadPlayer)
{
	int winner = wonHand(cardsPlayed, leadPlayer);

	if(winner==1)
		cout<<"The user has won the trick.\n";
	else
		cout<<"Computer #"<<winner-1<<" has won the trick.";
}

void doFullTurn(int (&playerHands)[4][13], int (&pointsForRound)[4], string suits[4], string numbers[13], int &leadPlayer, int (&cardsPlayed)[4], bool &heartsPlayed)
{
	int currentPlayer=0;	//1=user 2=comp1 3=comp2 4=comp3
	int cardPlayed=0;

	//searches for the 2 of clubs to start off the game
	if(playerHands[3][12]!=0)
	{
		for(int x=1; x<5; x++)
			if(contains(playerHands,x,400))
				currentPlayer=x;
		leadPlayer=currentPlayer;
	}
	else
	{
		currentPlayer=wonHand(cardsPlayed, leadPlayer);
	}

	cardsPlayed[0]=0;	cardsPlayed[1]=0;	cardsPlayed[2]=0;	cardsPlayed[3]=0;

	//does the lead card for the computer
	if(currentPlayer!=1)
	{
		cardPlayed=playCardLeading(playerHands,currentPlayer,cardsPlayed,heartsPlayed);
		cout<<"The computer has led with " <<numbers[cardPlayed%100]<<" of "<<suits[cardPlayed/100-1]<<"\n";
	}
	//allows the user to do their turn when they lead
	else
		doUserTurn(playerHands,suits,numbers,cardsPlayed,heartsPlayed);

	//takes care of the three following the lead
	for(int x=0; x<3; x++)
	{
		currentPlayer++; //moves on to next player
		if(currentPlayer==5)	//goes from computer 3 to the user player 
		{
			doUserTurn(playerHands,suits,numbers,cardsPlayed,heartsPlayed);
			currentPlayer=1;
		}
		else		//does the computer turns
		{
			cardPlayed=playCardNotLeading(playerHands, currentPlayer,cardsPlayed, heartsPlayed);
			cout<<"The computer has played "<<numbers[cardPlayed%100]<<" of "<<suits[cardPlayed/100-1]<<"\n";
			modifyCards(playerHands,cardsPlayed,cardPlayed);
		}
	}
	if(currentPlayer==4)
		addPoints(pointsForRound,cardsPlayed,1);
	else
		addPoints(pointsForRound, cardsPlayed, currentPlayer+1);

	removeCards(playerHands,cardsPlayed);
	cout<<"\n";
	printWinner(playerHands, cardsPlayed,leadPlayer);


	cout<<"\n\nCURRENT SCORES FOR THE ROUND: \nUSER: "<<pointsForRound[0]<<"\nCOMPUTER #1: "<<pointsForRound[1]<<"\nCOMPUTER #2: "<<pointsForRound[2]<<"\nCOMPUTER #3: "<<pointsForRound[3];
	cout<<"\n\n-----------------------------------------------\n\n";
}

bool endGame(int points[4])
{
	for(int x=0; x<4; x++)
		if(points[x]>=100)
			return true;
	return false;
}

void addPointsToTotal(int (&totalPoints)[4], int (&roundPoints)[4])
{
	int shootingTheMoon=5;
	
	for(int x=0; x<4; x++)
		if(roundPoints[x]==26)
			shootingTheMoon=x;

	if(shootingTheMoon==5)
		for(int x=0; x<4; x++)
			totalPoints[x]+=roundPoints[x];
	else
		for(int x=0; x<4; x++)
			if(x!=shootingTheMoon)
				totalPoints[x]+=26;
}

void printEndGame(int points[4])
{
	cout<<"Final Scores: \n User: "<<points[0];
	for(int x=1;x<4;x++)
		cout<<"  Computer #"<<x<<": "<<points[x];

	int min=0;

	for(int x=1; x<4; x++)
		if(points[min]>points[x])
			min=x;

	cout<<"\n";

	if(min==0)
		cout<<"You win!";
	else
		cout<<"Computer #"<<min<<" is the winner";
}

///Need To Finish
void pickCardsToSwap(int playerCards[4][13], int (&cardsSwapped)[4][3])
{
		cout<<"What cards would you like to swap?";
		cin>>cardsSwapped[0][0]>>cardsSwapped[0][1]>>cardsSwapped[0][2];
		int counter=0;		
		for(int x=1; x<4; x++)
		{
			counter=0;
				if(contains(playerCards,x+1,301))
				{
					cardsSwapped[x][counter]=301;
					counter++;
				}
				if(contains(playerCards,x+1,300))
				{
					cardsSwapped[x][counter]=300;
					counter++;
				}

				int heartCounterMax=0;

				for(int y=0;y<13 && playerCards[x][y]<103;y++)
					heartCounterMax=y;

				for(int y=heartCounterMax; heartCounterMax<0 && counter!=3; y--)
				{
					cardsSwapped[x][counter]=playerCards[x][y];
					counter++;
				}

				
			
		}

}

int main()
{
	int playerHands[4][13]={0};
	string suits[4]={"Hearts", "Diamonds", "Spades", "Clubs"};
	string numbers[13]={"Two","Three","Four","Five","Six","Seven","Eight","Nine","Ten","Jack", "Queen","King","Ace"};
	int points[4]={0};
	int pointsInHand[4]={0};

	int leadPlayer=0;
	int cardsPlayed[4]={0};
	bool heartsPlayed=false;


	while(!endGame(points))
	{
		createDeck(playerHands);
		shuffle(playerHands);
		orderHands(playerHands);
		for(int x=0; x<13; x++)
		{
			doFullTurn(playerHands, pointsInHand,suits,numbers,leadPlayer,cardsPlayed,heartsPlayed);			
		}
		leadPlayer=0;
		heartsPlayed=0;
			
		addPointsToTotal(points,pointsInHand);
		cout<<"\nNEW HAND!";
		cout<<"\n\nCURRENT SCORES FOR THE GAME: \nUSER: "<<points[0]<<"\nCOMPUTER #1: "<<points[1]<<"\nCOMPUTER #2: "<<points[2]<<"\nCOMPUTER #3: "<<points[3];
		cout<<"\n\n-----------------------------------------------\n\n";
		for(int x=0; x<4; x++)
			pointsInHand[x]=0;
	}	
	printEndGame(points);

	int x=0;
	cin>>x;
	return x;
}