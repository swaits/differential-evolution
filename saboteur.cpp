#include <cstdio>
#include <vector>
#include <algorithm>

#include "de.h"

#define MAX(a,b)          ((a)>(b)?(a):(b))
#define MIN(a,b)          ((a)<(b)?(a):(b))
#define CLAMP(min,max,x)  (MAX(min,MIN(max,x)))

class Saboteur: public DE::Engine<16>
{
	public:
		double TestCardMix(int players, int saboteur_cards, int digger_cards)
		{
			int total_games = 0;
			int total_saboteurs = 0;
			int total_diggers = 0;
			double saboteur_frequency = 0.0;

			// make deck
			std::vector<int> deck;
			for (int i=0;i<saboteur_cards;++i)
			{
				deck.push_back(1);
			}
			for (int i=0;i<digger_cards;++i)
			{
				deck.push_back(0);
			}

			const int NUM_ITER = 5000;
			for (int i=0;i<NUM_ITER;++i)
			{
				// shuffle deck
				std::random_shuffle(deck.begin(),deck.end());

				// deal first N cards
				for (int j=0;j<players;++j)
				{
					if ( deck[j] == 1 )
						++total_saboteurs;
				}
			}

			return ((double)total_saboteurs/(double)NUM_ITER) / (double)players;
		}

		double TestIndividualFitness(int players, int saboteur_cards, int digger_cards, bool verbose=false)
		{
			double error_bonus = 0.0;
			if ( saboteur_cards < 1 || saboteur_cards > 4 )
				error_bonus += 1000.0;
			if ( digger_cards < 1 || digger_cards > 7 )
				error_bonus += 1000.0;
			if ( saboteur_cards >= players )
				error_bonus += 500.0;
			if ( (saboteur_cards+digger_cards) < (players+1) )
				error_bonus += 500.0;
			if ( (saboteur_cards+digger_cards) > (players+3) )
				error_bonus += 500.0;

			if ( verbose )
			{
				printf("\n %2d players (%d, %d): ",players,saboteur_cards,digger_cards);
			}

			if ( error_bonus > 0.0 )
			{
				if ( verbose )
				{
					printf(" ERROR!  (%f)\n",error_bonus);
				}
				return error_bonus;
			}
			else
			{
				double actual = TestCardMix(players,saboteur_cards,digger_cards);
				if ( verbose )
				{
					printf(" %0.5f\n",actual);
				}
				return actual;
			}
		}

		double TestFitness(const double testcase[16], bool& stop)
		{
			ErrorAccumulator e;

			const int num_players[8] = { 3, 4, 5, 6, 7, 8, 9, 10 };
			const double expected = 0.363;
			//const double expected = 1.0/3.0; // saboteurs are 1:3

			for (int i=0;i<8;++i)
			{
				int i_saboteur_cards = i * 2;
				int i_digger_cards   = i_saboteur_cards + 1;
				int players          = num_players[i];

				int saboteur_cards = (int)testcase[i_saboteur_cards];
				int digger_cards   = (int)testcase[i_digger_cards];

				double actual = TestIndividualFitness(players,saboteur_cards,digger_cards);

				e.AddTestCase(expected,actual);
			}

			return e.GetRMSE();
		}
};

int main(int argc, char** argv)
{
	// init
	Saboteur s;
	for (int i=0;i<8;++i)
	{
		s.SetRange(  i*2, 1.0, 4.0);
		s.SetRange(i*2+1, 1.0, 7.0);
	}
	s.Reset();

	// print original
	const double original[16] = { 1,3,1,4,2,4,2,5,3,5,3,6,3,7,4,7 };
	for (int i=0;i<8;++i)
	{
		const int num_players[8] = { 3, 4, 5, 6, 7, 8, 9, 10 };
		int i_saboteur_cards = i * 2;
		int i_digger_cards   = i_saboteur_cards + 1;
		int players          = num_players[i];

		int saboteur_cards = (int)original[i_saboteur_cards];
		int digger_cards   = (int)original[i_digger_cards];

		s.TestIndividualFitness(players,saboteur_cards,digger_cards,true);
	}

	// run
	s.Solve(1000);

	// output
	const double* best = s.GetBest();
	for (int i=0;i<8;++i)
	{
		const int num_players[8] = { 3, 4, 5, 6, 7, 8, 9, 10 };
		int i_saboteur_cards = i * 2;
		int i_digger_cards   = i_saboteur_cards + 1;
		int players          = num_players[i];

		int saboteur_cards = (int)best[i_saboteur_cards];
		int digger_cards   = (int)best[i_digger_cards];

		s.TestIndividualFitness(players,saboteur_cards,digger_cards,true);
	}

	return 0;
}
